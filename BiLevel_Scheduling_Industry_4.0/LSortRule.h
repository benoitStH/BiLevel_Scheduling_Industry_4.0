#ifndef BILEVEL_SCHEDULING_LLPTRULE_H
#define BILEVEL_SCHEDULING_LLPTRULE_H

#include "ILeaderSelectRule.h"

namespace sortRule {

	const unsigned int RandomRule = 0;
	const unsigned int LPTRULE = 1;
	const unsigned int LWPTRULE = 2;
	const unsigned int InvEDDRULE = 3; // UNUSED
	const unsigned int SPT_EDD_CONST_LATENESS = 4;
	const unsigned int SPT_EDD_VAR_LATENESS = 5;
}

class LSortRule : public ILeaderSelectRule
{

public:
	LSortRule() : ILeaderSelectRule() {};
	LSortRule(unsigned int ruleNumber) {
		this->ruleNumber = ruleNumber;
		switch (ruleNumber)
		{

		case sortRule::RandomRule:
			ruleName = "RandomRule";
			break;

		case sortRule::LPTRULE:
			ruleName = "LPTRule";
			break;

		case sortRule::LWPTRULE:
			ruleName = "LWPTRule";
			break;

		case sortRule::InvEDDRULE:
			ruleName = "invEDDRule";
			break;
			
		case sortRule::SPT_EDD_CONST_LATENESS:
			ruleName = "SPT_EDD_WithConstantLateness";
			break;
			
		case sortRule::SPT_EDD_VAR_LATENESS:
			ruleName = "SPT_EDD_WithVariableLateness";
			break;

		default:
			ruleName = "Unknown";
			break;
		}
	}

	/*
	 * Overriden Method which will select a subset of n jobs among the list of Jobs given
	 * This method will call a sorting rule according to its ruleNumber
	 * If the ruleNumber doesn't match, it return an empty list.
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	virtual std::vector<Job> selectJobsFrom(const Instance& instance)
	{
		// If we need to select all jobs, no need to apply a selection rule
		if (instance.getNbToSelectJob() == instance.getNbJobs())
		{
			return instance.getListJobs();
		}

		switch (ruleNumber)
		{

		case sortRule::RandomRule:
			return RandomRule(instance);
			break;
		case sortRule::LPTRULE:
			return LPTRule(instance);
			break;

		case sortRule::LWPTRULE:
			return LWPTRule(instance);
			break;

		case sortRule::InvEDDRULE:
			return std::vector<Job>();
			break;

		case sortRule::SPT_EDD_CONST_LATENESS:
			return SptEddConstLateness(instance);
			break;

		case sortRule::SPT_EDD_VAR_LATENESS:
			return SptEddVariableLateness(instance);
			break;

		default:
			return std::vector<Job>();
			break;
		}
	}

private:

	/*
	 * Method which will select a random subset of n jobs among the list of Jobs given
	 * No Selection criterion is used
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	std::vector<Job> RandomRule(const Instance& instance)
	{
		std::vector<Job> jobs = instance.getListJobs();  // List of jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule

		// Shuffling the list of jobs
		std::shuffle(jobs.begin(), jobs.end(), std::default_random_engine());

		std::vector<Job>::iterator i = jobs.begin();
		while (i != jobs.end())
		{
			if (chosenJobs.size() >= instance.getNbToSelectJob())
			{
				break;
			}

			chosenJobs.push_back(*i);
			i++;
		}

		return chosenJobs;
	}

	/*
	 * Method which will select a subset of n jobs among the list of Jobs given
	 * The Selection criterion is the Largest Processing Time.
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	std::vector<Job> LPTRule(const Instance& instance)
	{
		std::vector<Job> jobs = instance.getListJobs();  // List of jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule

		// Leader's selects the n among N jobs with the largest processing times
		std::sort(jobs.begin(), jobs.end(), std::greater<Job>());
		std::vector<Job>::iterator i = jobs.begin();

		// We select from largest to smallest job 
		// until we reach the number of jobs to select or all jobs are selected
		while (i != jobs.end())
		{
			if (chosenJobs.size() >= instance.getNbToSelectJob())
			{
				break;
			}

			chosenJobs.push_back(*i);
			i++;
		}

		return chosenJobs;
	}

	/*
	 * Method which will select a subset of n jobs among the list of Jobs given
	 * The Selection criterion is the Largest Weighted Processing Time.
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	std::vector<Job> LWPTRule(const Instance& instance)
	{
		std::vector<Job> jobs = instance.getListJobs();  // List of jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule

		// Leader's selects the n among N jobs with the largest weighted processing times
		std::sort(jobs.begin(), jobs.end(), Job::LWPT);
		std::vector<Job>::iterator i = jobs.begin();

		// We select from largest to smallest job 
		// until we reach the number of jobs to select or all jobs are selected
		while (i != jobs.end())
		{
			if (chosenJobs.size() >= instance.getNbToSelectJob())
			{
				break;
			}

			chosenJobs.push_back(*i);
			i++;
		}

		return chosenJobs;
	}

	/*
	 * Method which will rule out permanently late jobs from the given instance
	 * If a job's processing time on high speed machine will still make the job late, it is considered permanently late
	 * Permanently jobs will be kept if the number of jobs to select isn't reacheable without them
	 * If they are kept, the selection criterion is the Smallest Weight.
	 * @param instance An instance with the list of jobs
	 * @return a list of jobs
	 */
	std::vector<Job> filterOutPermanentLateJobs(const Instance& instance)
	{
		std::vector<Job> jobs = instance.getListJobs();  // List of jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule
		std::vector<Job> permanentlyLateJobs;  // Jobs which are permanently late (will be chosen as a last resort)

		std::sort(jobs.begin(), jobs.end(), Job::inv_EDD);

		std::vector<Job>::iterator i = jobs.begin();
		std::vector<Job>::iterator end = jobs.end();
		while (i != end)
		{

			Job& job = *i;
			//std::cout << job.getNum() << " p " << job.getPi() << " d " << job.getDi() << " w " << job.getWi();

			// if the due date is greater than the processing time on high speed machine, we keep the job
			if (job.getDi() * instance.getHighSpeed() >= job.getPi()) 
			{
				chosenJobs.push_back(*i); //std::cout << " <-|";
			}
			else {
				permanentlyLateJobs.push_back(*i);
			}
			i++;
			//std::cout << std::endl;
		}

		std::sort(permanentlyLateJobs.begin(), permanentlyLateJobs.end(), Job::SmallestWeight);

		i = permanentlyLateJobs.begin();
		end = permanentlyLateJobs.end();
		while (chosenJobs.size() < instance.getNbToSelectJob() && i != end)
		{
			chosenJobs.push_back(*i);
			i++;
		}


		return chosenJobs;

	}

	/*
	 * Method which will select a subset of n jobs among the list of Jobs given
	 * The Selection criterion is the Lateness of jobs ordered by increasing processing time.
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	std::vector<Job> SptEddConstLateness(const Instance& instance)
	{
		std::vector<Job> jobs = instance.getListJobs();  // List of jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule

		std::vector<float> lateness; // lateness of all jobs
		float cumulatedEndTime = 0;

		// Sorting jobs by increasing processing time and earliest due date
		std::sort(jobs.begin(), jobs.end(), std::less<Job>());

		// Calculating each job's lateness
		for (const Job& job : jobs)
		{
			cumulatedEndTime += job.getPi();

			lateness.push_back(cumulatedEndTime - job.getDi());
			//std::cout << job.getNum() << " p " << job.getPi() << " d " << job.getDi() << " lateness " << lateness[lateness.size() - 1] << std::endl;
		}

		unsigned int i = 0;

		// We select jobs with a non positive lateness
		// until we reach the number of jobs to select or either all jobs are selected or none can be selected
		while (i != jobs.size())
		{
			if (chosenJobs.size() >= instance.getNbToSelectJob())
			{
				break;
			}

			if (lateness[i] <= 0)
			{
				//std::cout << "selected job " << jobs[i].getNum() << std::endl;
				chosenJobs.push_back(jobs[i]);
			}
			i++;
		}

		// While there is still an amount of jobs to select
		while (chosenJobs.size() < instance.getNbToSelectJob())
		{
			// We assume the first job has the smallest positive lateness
			unsigned int min_i = 0;
			float min_lateness = lateness[min_i];

			// We compare with the other jobs
			for (i = 1; i < jobs.size(); i++)
			{
				// If one of the other job has a smaller positive lateness, we save its index number and its lateness
				// We also save the job if min_lateness is currently non positive
				if ((lateness[i] < min_lateness && lateness[i] > 0) || min_lateness <= 0)
				{
					min_i = i;
					min_lateness = lateness[min_i];
				}
			}

			// After that, if we found a job with the smallest positive lateness, We select it
			if (min_lateness > 0)
			{
				// Setting its lateness to zero to ensure it won't be selected again
				lateness[min_i] = 0;
				//std::cout << "selected job " << jobs[min_i].getNum() << std::endl;
				chosenJobs.push_back(jobs[min_i]);
			}
		}

		return chosenJobs;


	}


	/*
	 * Method which will select a subset of n jobs among the list of Jobs given
	 * The Selection criterion is the Lateness of jobs ordered by increasing processing time.
	 * This criterion is recalculated when m jobs has been selected (m is the total number of machines)
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	std::vector<Job> SptEddVariableLateness(const Instance& instance)
	{
		std::vector<Job> jobs = filterOutPermanentLateJobs(instance);  // List of jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule

		std::vector<float> lateness; // lateness of each job
		std::vector<float> endTimes; // end time of each job
		float cumulatedEndTime = 0;  // sum of all jobs' processing time

		if (jobs.size() == instance.getNbToSelectJob()) { return jobs; }


		// Number of total machines : represent the size m of a bloc
		unsigned int nbTotalMachines = instance.getNbOfLowSpeedMachines() + instance.getNbOfHighSpeedMachines();

		// Sorting jobs by increasing processing time and earliest due date
		std::sort(jobs.begin(), jobs.end(), std::less<Job>());

		// Calculating each job's lateness and end time
		unsigned int nbClean = 1;
		for (const Job& job : jobs)
		{
			cumulatedEndTime += job.getPi();

			lateness.push_back(cumulatedEndTime - job.getDi());
			endTimes.push_back(cumulatedEndTime);
			//if (cumulatedEndTime >= job.getDi() * instance.getHighSpeed()) // Revoir la condition (ecart entre d et p ?) (retard au dessus d'un seuil ?)
			if(cumulatedEndTime/(std::sqrt(nbClean) * instance.getHighSpeed()) - job.getDi() > 0)
			{
				endTimes.pop_back(); endTimes.push_back(0 + 1000 - job.getPi()*instance.getHighSpeed());
				lateness.pop_back(); lateness.push_back(0 + 1000 - job.getPi()* instance.getHighSpeed() - job.getDi());
				cumulatedEndTime -= job.getPi();
				//std::cout << "--";
			}
			else { nbClean++; }
			//std::cout << job.getNum() << " p " << job.getPi() << " d " << job.getDi() << " lateness " << lateness[lateness.size() - 1]
			//	<< " e " << cumulatedEndTime << std::endl;
		}


		unsigned int nbToSelectJobs = instance.getNbToSelectJob();
		unsigned int lastNbSelectedJobs = 0;
		unsigned int nbFails = 0;
		unsigned int maxFails = nbToSelectJobs;
		std::vector<unsigned int> selected_i;


		// While the number of selected jobs is less than n
		while (chosenJobs.size() < nbToSelectJobs)
		{
			chosenJobs.clear();  // (Re)Starting Selection of jobs
			//std::cout << "(re)starting..." << std::endl;

			std::vector<float> PiPredecesseur;  // Processing time of the last bloc's jobs
			float sum_PiPredecesseur = 0;  // The sum of processing time per bloc

			// Check each job until the number of selected jobs is met
			for (unsigned int i = 0; i < jobs.size(); i++)
			{
				if (chosenJobs.size() >= nbToSelectJobs)
				{ break; }

				//std::cout << jobs[i].getNum() << " lateness " << lateness[i] - sum_PiPredecesseur << (lateness[i] - sum_PiPredecesseur <= 0 ? " <-| " : "") << std::endl;

				// If a job is early, we select it and save its processing time
				if (lateness[i] <= sum_PiPredecesseur)
				{
					chosenJobs.push_back(jobs[i]);
					PiPredecesseur.push_back(jobs[i].getPi());
					lateness[i] = -1;
					// std::cout << "selected job " << jobs[i].getNum() << std::endl;

					// If the number of saved values can make a bloc, we save the lowest value and clear our list
					if (PiPredecesseur.size() == nbTotalMachines)
					{
						float maxPi = *std::max_element(PiPredecesseur.begin(), PiPredecesseur.end());
						float total_minusMax = 0;// -maxPi;
						for (std::vector<float>::iterator pi = PiPredecesseur.begin(); pi != PiPredecesseur.end(); pi++)
						{
							total_minusMax += *pi;
						}

						sum_PiPredecesseur += total_minusMax;
						PiPredecesseur.clear();
						//std::cout << "sumPiPredecesseur " << sum_PiPredecesseur << std::endl;
					}
				}
			}


			// If no new bloc has been made
			if (lastNbSelectedJobs >= chosenJobs.size() && chosenJobs.size() < nbToSelectJobs)
			{
				// While their is still room in the last bloc
				//	// Check each job whose lateness is positive and save the one with lowest value
				// 
				// 
				nbFails++;
				if (nbFails == maxFails) { break; }
				float latenessThresold = 0;

				// While their is still jobs to be forcely added
				while (chosenJobs.size()-lastNbSelectedJobs < nbTotalMachines * nbFails && chosenJobs.size() <= nbToSelectJobs)
				{
					////// We assume the first job has the smallest positive lateness
					unsigned int min_i = -1;
					float min_lateness = -1; //lateness[min_i];

					// We compare with the other jobs
					for (unsigned int i = 0; i < jobs.size(); i++)
					{
						// If one of the other job has a smaller positive lateness, we save its index number and its lateness
						// We also save the job if min_lateness is currently non positive   
						//std::cout << jobs[i].getNum() << " score " << lateness[i] * jobs[i].getWi() << std::endl;
						if ((lateness[i]*jobs[i].getWi() < min_lateness || min_lateness <= 0) && lateness[i]*jobs[i].getWi() > latenessThresold)
						{
							min_i = i;
							min_lateness = lateness[min_i]*jobs[min_i].getWi();
						}
					}

					// After that, if we found a job with the smallest positive lateness, We select it
					if (min_lateness > latenessThresold)
					{
						latenessThresold = min_lateness;
						//std::cout << "forcely selected job " << jobs[min_i].getNum() << std::endl;
						chosenJobs.push_back(jobs[min_i]);
						PiPredecesseur.push_back(jobs[min_i].getPi());
						selected_i.push_back(min_i);


						// If the number of saved values can make a bloc, we save the lowest value and clear our list
						if (PiPredecesseur.size() == nbTotalMachines)
						{
							float maxPi = *std::max_element(PiPredecesseur.begin(), PiPredecesseur.end());
							float total_minusMax = 0;// -maxPi;
							for (std::vector<float>::iterator pi = PiPredecesseur.begin(); pi != PiPredecesseur.end(); pi++)
							{
								total_minusMax += *pi;
							}

							sum_PiPredecesseur += total_minusMax;
							PiPredecesseur.clear();
							//std::cout << "sumPiPredecesseur " << sum_PiPredecesseur << std::endl;
							//break;
						}
					}
					else
					{
						//std::cout << "No more selectable\n";
						break;
					}
				}

				if (selected_i.size() == 0) { break; }
				// For each job afterward
					// subtract sumPiPredecesseur from initial lateness


				
				//////
			}

			for (unsigned int i = 0; i < jobs.size(); i++)
			{
				lateness[i] = endTimes[i] - jobs[i].getDi();
			}

			for (std::vector<unsigned int>::iterator selected_it = selected_i.begin(); selected_it != selected_i.end(); selected_it++)
			{
				lateness[*selected_it] = -1;
			}



			lastNbSelectedJobs = chosenJobs.size();
			//std::cout << lastNbSelectedJobs << " jobs selected\n";
		}
		
		/////////
		//std::cout << "Selection Done !\n";
		while (chosenJobs.size() > nbToSelectJobs)
		{
			chosenJobs.pop_back();
			//std::cout << "Removing excess\n";
		}

		return chosenJobs;

	}
	
};

#endif // BILEVEL_SCHEDULING_LLPTRULE_H

