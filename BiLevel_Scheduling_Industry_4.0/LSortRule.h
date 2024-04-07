#ifndef BILEVEL_SCHEDULING_LLPTRULE_H
#define BILEVEL_SCHEDULING_LLPTRULE_H

#include "ILeaderSelectRule.h"

namespace sortRule {

	const unsigned int RandomRule = 1;
	const unsigned int LPTRULE = 2;
	const unsigned int LWPTRULE = 3;
	const unsigned int InvEDDRULE = 4; // UNUSED
	const unsigned int SPT_EDD_CONST_LATENESS = 5;
	const unsigned int SPT_EDD_VAR_LATENESS = 6;
}

class LSortRule : public ILeaderSelectRule
{

public:
	LSortRule() : ILeaderSelectRule() {};


	LSortRule(unsigned int ruleNumber) {

		// Setting the rule name according to the rule number
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
	 * @return a list of selected jobs or an empty list by default
	 */
	virtual std::vector<Job> selectJobsFrom(const Instance& instance)
	{
		verbose.setRequiredLevel(3);

		// If we need to select every jobs, no need to apply a selection rule
		if (instance.getNbToSelectJob() == instance.getNbJobs())
		{
			verbose << "N == n, selection rule not needed\n";
			return instance.getListJobs();
		}

		verbose << "Applying selection rule " << getRuleName() << "...\n";
		verbose.endRequiredLevel();

		// Calling the selection method according to the ruleNumber
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
	 * Private Method which will select a random subset of n jobs among the list of Jobs given
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

		// Selecting jobs until we reach n jobs or all jobs are selected  
		std::vector<Job>::iterator i = jobs.begin();
		while (i != jobs.end() && chosenJobs.size() < instance.getNbToSelectJob())
		{
			chosenJobs.push_back(*i);
			i++;
		}

		return chosenJobs;
	}

	/*
	 * Private Method which will select a subset of n jobs among the list of Jobs given
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
		while (i != jobs.end() && chosenJobs.size() < instance.getNbToSelectJob())
		{
			chosenJobs.push_back(*i);
			i++;
		}

		return chosenJobs;
	}

	/*
	 * Private Method which will select a subset of n jobs among the list of Jobs given
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
		while (i != jobs.end() && chosenJobs.size() < instance.getNbToSelectJob())
		{
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

		// Sorting by increasing due date
		std::sort(jobs.begin(), jobs.end(), Job::EDD);

		std::vector<Job>::iterator i = jobs.begin();
		while (i != jobs.end())
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

		// Sorting permanently late jobs according to increasing weight
		std::sort(permanentlyLateJobs.begin(), permanentlyLateJobs.end(), Job::SmallestWeight);

		// Selecting jobs until the number of jobs to select is reached or all is selected
		i = permanentlyLateJobs.begin();
		while (chosenJobs.size() < instance.getNbToSelectJob() && i != permanentlyLateJobs.end())
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

		// We select jobs with a non positive lateness
		// until we reach the number of jobs to select or either all jobs are selected or none can be selected
		unsigned int i = 0;
		while (i != jobs.size() && chosenJobs.size() < instance.getNbToSelectJob())
		{
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
			unsigned int min_i = -1;  // job's index with the minimal positive lateness
			float min_lateness = -1;  // minimal positive lateness

			// We compare the jobs
			for (i = 0; i < jobs.size(); i++)
			{
				// If one of the job has a smaller positive lateness, we save its index number and its lateness
				// We also save it if min_lateness is currently non positive
				if ((0 < lateness[i] && lateness[i] < min_lateness) || min_lateness <= 0)
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
	 * Private Method which will select a subset of n jobs among the list of Jobs given
	 * The Selection criterion is the Lateness of jobs ordered by increasing processing time.
	 * This criterion is recalculated when m jobs has been selected (m is the total number of machines)
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	std::vector<Job> SptEddVariableLateness(const Instance& instance)
	{
		std::vector<Job> jobs = filterOutPermanentLateJobs(instance);  // List of jobs with least amount of permanently late jobs
		std::vector<Job> chosenJobs; // Jobs selected by the Leader's Rule

		std::vector<float> lateness; // lateness of each job
		std::vector<float> endTimes; // end time of each job, used to compute each job's lateness for each iteration
		float cumulatedEndTime = 0;  // sum of all jobs' processing time

		// If the filtered list size is equal to n, return it
		if (jobs.size() == instance.getNbToSelectJob()) { return jobs; }


		// Number of total machines : represent the size m of a bloc
		unsigned int nbTotalMachines = instance.getNbOfLowSpeedMachines() + instance.getNbOfHighSpeedMachines();

		// Sorting jobs by increasing processing time and earliest due date
		std::sort(jobs.begin(), jobs.end(), std::less<Job>());

		// Calculating and storing each job's lateness and end time if they were to be scheduled on one high speed Machine 
		unsigned int nbClean = 1;
		for (const Job& job : jobs)
		{
			cumulatedEndTime += job.getPi();

			lateness.push_back(cumulatedEndTime - job.getDi());
			endTimes.push_back(cumulatedEndTime);
			

			// If the job's end time on the high speed machine is higher than a threshold, it is penalized
			// The thresold is the due date times the square root of the number of preceding jobs without penalization
			
			if(cumulatedEndTime/(std::sqrt(nbClean) * instance.getHighSpeed()) - job.getDi() > 0)
			{
				// The penalization is to set its end time and lateness to a high value so it won't be selected first
				endTimes.pop_back(); endTimes.push_back(1000 - job.getPi()*instance.getHighSpeed());
				lateness.pop_back(); lateness.push_back(1000 - job.getPi()* instance.getHighSpeed() - job.getDi());

				// Since it won't be selected first, we don't take into account its processing time
				cumulatedEndTime -= job.getPi();
				//std::cout << "--";
			}
			else { nbClean++; }  // Otherwise, it is a job without penalization


			//std::cout << job.getNum() << " p " << job.getPi() << " d " << job.getDi() << " lateness " << lateness[lateness.size() - 1]
			//	<< " e " << cumulatedEndTime << std::endl;
		}


		unsigned int nbToSelectJobs = instance.getNbToSelectJob();  // number n of jobs to select
		unsigned int lastNbSelectedJobs = 0;  // Last number of selected jobs
		unsigned int nbFails = 0;			  // Number of times where we didn't selected more jobs since last iteration
		std::vector<unsigned int> selected_i;   // List of forcely selected jobs' index


		// While the number of selected jobs is less than n
		while (chosenJobs.size() < nbToSelectJobs)
		{
			chosenJobs.clear();  // (Re)Starting Selection of jobs
			//std::cout << "(re)starting..." << std::endl;

			std::vector<float> PiPredecesseur;  // Processing time of the last bloc's jobs
			float sum_PiPredecesseur = 0;  // The sum of each selected job's processing time

			// Check each job until the number of selected jobs is met
			for (unsigned int i = 0; i < jobs.size(); i++)
			{
				if (chosenJobs.size() >= nbToSelectJobs)
				{ break; }

				//std::cout << jobs[i].getNum() << " lateness " << lateness[i] - sum_PiPredecesseur << (lateness[i] - sum_PiPredecesseur <= 0 ? " <-| " : "") << std::endl;

				// If a job is early, we select it, save its processing time and set its lateness to negative value
				if (lateness[i] <= sum_PiPredecesseur)
				{
					chosenJobs.push_back(jobs[i]);
					PiPredecesseur.push_back(jobs[i].getPi());
					lateness[i] = -1;  // The job isn't late
					// std::cout << "selected job " << jobs[i].getNum() << std::endl;

					// If the number of saved values can make a bloc, we add their processing time to sum_PiPredecesseur
					if (PiPredecesseur.size() == nbTotalMachines)
					{
						float sum_PiPredecesseurBloc = 0;
						for (std::vector<float>::iterator pi = PiPredecesseur.begin(); pi != PiPredecesseur.end(); pi++)
						{
							sum_PiPredecesseurBloc += *pi;
						}

						sum_PiPredecesseur += sum_PiPredecesseurBloc;
						PiPredecesseur.clear();
						//std::cout << "sumPiPredecesseur " << sum_PiPredecesseur << std::endl;
					}
				}
			}


			// If the number of selected jobs didn't increase
			if (chosenJobs.size() <= lastNbSelectedJobs && chosenJobs.size() < nbToSelectJobs)
			{

				nbFails++;
				float latenessWjThresold = 0;  // jobs whose score is highest will be selected

				// While their is still jobs to be forcely added
				while (chosenJobs.size()-lastNbSelectedJobs < nbTotalMachines * nbFails && chosenJobs.size() < nbToSelectJobs)
				{
					unsigned int min_i = -1; // index of the job with the minimal positive lateness
					float min_latenessWj = -1; // minimal positive lateness multiplied by its job's weight
					float latenessWj;

					// We compare each job
					for (unsigned int i = 0; i < jobs.size(); i++)
					{
						latenessWj = lateness[i] * jobs[i].getWi();  // the job's score

						// If one of the job has a smaller score and above our Thresold, we save its index number and its score
						// We also save it if min_latenessWj is currently non positive   
						//std::cout << jobs[i].getNum() << " score " << latenessWj << std::endl;
						if ((latenessWj < min_latenessWj || min_latenessWj <= 0) && latenessWj > latenessWjThresold)
						{
							min_i = i;
							min_latenessWj = latenessWj;
						}
					}

					// After that, if we found a job with the smallest score above our Thresold, We forcely select it
					if (min_latenessWj > latenessWjThresold)
					{
						latenessWjThresold = min_latenessWj;  // update the Threshold
						//std::cout << "forcely selected job " << jobs[min_i].getNum() << std::endl;

						// Forcely selecting the job and saving the job's index
						chosenJobs.push_back(jobs[min_i]);
						selected_i.push_back(min_i);
					}
					else  // No job can be selected
					{
						//std::cout << "No more selectable\n";
						break;
					}
				}

				// If no jobs were forcely selected, stop
				if (selected_i.size() == 0) { break; }

			}

			// We reset the lateness of each job
			for (unsigned int i = 0; i < jobs.size(); i++)
			{
				lateness[i] = endTimes[i] - jobs[i].getDi();
			}

			// Each forcely selected jobs will be considered early for the next iteration
			for (std::vector<unsigned int>::iterator selected_it = selected_i.begin(); selected_it != selected_i.end(); selected_it++)
			{
				lateness[*selected_it] = -1;
			}


			// We take note of the number of selected jobs
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

