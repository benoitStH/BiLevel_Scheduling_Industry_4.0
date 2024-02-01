#include "LSolver.h"

void LSolver::solve()
{
	// List of best solution for each Leader's decision
	std::vector<Solution> follower_solutions;

	// The Leader applies each selection rule as its decision
	for (ILeaderSelectRule* selectRule : listRules)
	{
		// The subset of jobs chosen by the Leader's selection rule
		std::vector<Job> chosenJobs = selectRule->selectJobsFrom(*instance);

		// Follower's heuristic
		subSolver->generateInitialSolution(chosenJobs, *instance);
		subSolver->heuristic();

		// We save the solution found by the follower
		Solution best_solution = *(subSolver->getSolution());
		follower_solutions.push_back(best_solution);
	}

	// We save the best solution found by the Follower for each Leader's decision
	std::sort(follower_solutions.begin(), follower_solutions.end(), std::less<Solution>());
	setSolution(new Solution(follower_solutions[0]));
}