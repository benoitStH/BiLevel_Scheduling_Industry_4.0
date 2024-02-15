#ifndef BILEVEL_SCHEDULING_LLPTRULE_H
#define BILEVEL_SCHEDULING_LLPTRULE_H

#include "ILeaderSelectRule.h"

namespace sortRule {

	const unsigned int RandomRule = 0;
	const unsigned int LPTRULE = 1;
	const unsigned int LWPTRULE = 2;
	const unsigned int InvEDDRULE = 3;
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
			return std::vector<Job>();
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
				chosenJobs.push_back(jobs[min_i]);
			}
		}

		return chosenJobs;


	}
};

#endif // BILEVEL_SCHEDULING_LLPTRULE_H

