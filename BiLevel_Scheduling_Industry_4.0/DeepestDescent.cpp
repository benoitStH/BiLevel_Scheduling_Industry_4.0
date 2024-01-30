#include "DeepestDescent.h"

void DeepestDescent::solve()
{
	std::vector<Job> jobs = instance->getListJobs();
	std::vector<Job> chosenJobs;

	// Leader's Heuristic
	std::sort(jobs.begin(), jobs.end(), std::greater<Job>());
	std::vector<Job>::iterator i = jobs.begin();

	while (i != jobs.end())
	{
		if(chosenJobs.size() >= 5)
		{
			break;
		}

		chosenJobs.push_back(*i);
		i++;
	}


	// Follower's Heuristic
	Solution solution_init = generateur.generateInitialSolution(chosenJobs, *instance);
	Solution best_solution = Solution(solution_init);


	// We save the best solution found
	setSolution(new Solution(best_solution));
}