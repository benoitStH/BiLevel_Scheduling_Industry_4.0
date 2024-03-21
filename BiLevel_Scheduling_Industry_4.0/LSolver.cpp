#include "LSolver.h"

void LSolver::solve()
{
	// List of best solution for each Leader's decision
	std::vector<Solution> follower_solutions;


	std::chrono::steady_clock::time_point timeStart = std::chrono::high_resolution_clock::now();

	// The Leader applies each selection rule as its decision
	for (ILeaderSelectRule* selectRule : listRules)
	{
		//std::cout << "Applying " << selectRule->getRuleName() << " as Leader's decision criterion\n\n";
		
		// The subset of jobs chosen by the Leader's selection rule
		std::vector<Job> chosenJobs = selectRule->selectJobsFrom(*instance);

		// Follower's heuristic
		subSolver->generateInitialSolution(chosenJobs, *instance);
		float init_score = subSolver->getSolution()->getSumWjUj();
		subSolver->heuristic();
		float final_score = subSolver->getSolution()->getSumWjUj();
		//std::cout << "Taux d'amelio : " << (init_score == 0 ? 1 : (init_score - final_score) / init_score) << std::endl;

		// We save the best solution found by the follower
		Solution follower_bestSolution = *(subSolver->getSolution());
		follower_solutions.push_back(follower_bestSolution);
	}

	// We save the best solution according to sum wjUj only
	std::sort(follower_solutions.begin(), follower_solutions.end(), Solution::smaller_wjUj);
	setSolution(new Solution(follower_solutions[0]));

	// Measuring resolution time
	std::chrono::steady_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
	timeResol = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart).count();
}

std::string LSolver::getHeuristicName() const
{
	return "\"LSolver - Best of All Decision && " + subSolver->getHeuristicName()+"\"";
}

std::string LSolver::getHeuristicDescription() const
{
	std::string description = "\"Applying the selection rules : ";

	for (ILeaderSelectRule* selectRule : listRules)
	{
		description += selectRule->getRuleName() + " ";
	}

	description += "\n and selecting the best solution from these selection rules";

	description += "\n" + subSolver->getHeuristicDescription()+ "\"";

	return description;
}