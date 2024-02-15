#include "LSolver.h"

void LSolver::solve()
{
	// List of best solution for each Leader's decision
	std::vector<Solution> follower_solutions;

	// The Leader applies each selection rule as its decision
	for (ILeaderSelectRule* selectRule : listRules)
	{
		std::cout << "Applying " << selectRule->getRuleName() << " as Leader's decision criterion\n\n";
		// The subset of jobs chosen by the Leader's selection rule
		std::vector<Job> chosenJobs = selectRule->selectJobsFrom(*instance);

		// Follower's heuristic
		subSolver->generateInitialSolution(chosenJobs, *instance);
		float init_score = subSolver->getSolution()->getSumWjUj();
		subSolver->heuristic();
		float final_score = subSolver->getSolution()->getSumWjUj();
		std::cout << "Taux d'amelio : " << (init_score == 0 ? 1 : (init_score - final_score) / init_score) << std::endl;

		// We save the solution found by the follower
		Solution best_solution = *(subSolver->getSolution());
		follower_solutions.push_back(best_solution);
	}

	// We save the best solution found by the Follower for each Leader's decision
	std::sort(follower_solutions.begin(), follower_solutions.end(), std::less<Solution>());
	setSolution(new Solution(follower_solutions[0]));
}