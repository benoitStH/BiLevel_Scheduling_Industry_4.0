#ifndef BILEVEL_SCHEDULING_LLPTRULE_H
#define BILEVEL_SCHEDULING_LLPTRULE_H

#include "ILeaderSelectRule.h"

namespace sortRule {

	const unsigned int RandomRule = 0;
	const unsigned int LPTRULE = 1;
	const unsigned int LWPTRULE = 2;
	const unsigned int InvEDDRule = 3;
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

		case sortRule::InvEDDRule:
			ruleName = "invEDDRule";
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

		case sortRule::InvEDDRule:
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
};

#endif // BILEVEL_SCHEDULING_LLPTRULE_H

