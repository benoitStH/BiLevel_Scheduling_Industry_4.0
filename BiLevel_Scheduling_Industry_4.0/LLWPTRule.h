#ifndef BILEVEL_SCHEDULING_LLWPTRULE_H
#define BILEVEL_SCHEDULING_LLWPTRULE_H

#include "ILeaderSelectRule.h"

class LLWPTRule : public ILeaderSelectRule
{
	LLWPTRule() : ILeaderSelectRule("LWPTRule", 2) {};

	/*
	 * Overriden Method which will select a subset of n jobs among the list of Jobs given
	 * The Selection criterion is the Largest Weighted Processing Time.
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	virtual std::vector<Job> selectJobsFrom(const Instance& instance)
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

#endif // BILEVEL_SCHEDULING_LLWPTRULE_H
