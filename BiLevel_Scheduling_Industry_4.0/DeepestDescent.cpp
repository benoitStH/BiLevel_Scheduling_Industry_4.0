#include "DeepestDescent.h"

void DeepestDescent::solve()
{
	std::vector<Job> jobs = instance->getListJobs();  // All available jobs
	std::vector<Job> chosenJobs; // Jobs selected by the Leader
	std::vector<Solution> follower_solutions;  // list of solutions provided by the follower for each set of jobs given by the Leader

	// Leader's selects the n among N jobs with the largest processing times
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

	std::cout << best_solution;
	follower_solutions.push_back(best_solution);

	// Leader selects the n among N jobs with the largest weighted processing time
	std::vector<Job> listLWPT_jobs = jobs;
	std::sort(listLWPT_jobs.begin(), listLWPT_jobs.end(), Job::LWPT);

	chosenJobs.clear();
	i = listLWPT_jobs.begin();
	while (i != listLWPT_jobs.end())
	{
		if (chosenJobs.size() >= 5)
		{
			break;
		}

		chosenJobs.push_back(*i);
		i++;
	}

	// Follower's Heuristic
	solution_init = generateur.generateInitialSolution(chosenJobs, *instance);
	best_solution = Solution(solution_init);

	std::cout << best_solution;
	follower_solutions.push_back(best_solution);

	// We save the best solution found which has the lowest score
	std::sort(follower_solutions.begin(), follower_solutions.end(), std::less<Solution>());
	setSolution(new Solution(follower_solutions[0]));
}