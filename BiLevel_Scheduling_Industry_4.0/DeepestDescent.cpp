#include "DeepestDescent.h"

void DeepestDescent::solve()
{
	return;



	std::vector<Job> jobs = instance->getListJobs();  // All available jobs
	unsigned int nbJobsToSelect = instance->getNbToSelectJob();
	std::vector<Job> chosenJobs; // Jobs selected by the Leader
	std::vector<Solution> follower_solutions;  // list of solutions provided by the follower for each set of jobs given by the Leader

	// Leader's selects the n among N jobs with the largest processing times
	std::sort(jobs.begin(), jobs.end(), std::greater<Job>());
	std::vector<Job>::iterator i = jobs.begin();

	while (i != jobs.end())
	{
		if(chosenJobs.size() >= nbJobsToSelect)
		{
			break;
		}

		chosenJobs.push_back(*i);
		i++;
	}

	//// Follower's Heuristic
	//Solution solution_init = generateur.generateInitialSolution(chosenJobs, *instance);
	//Solution best_solution = Solution(solution_init);

	//std::cout << "LPT Rule :\n";
	//best_solution.print();
	//follower_solutions.push_back(best_solution);

	//// Leader selects the n among N jobs with the largest weighted processing time
	//std::vector<Job> listLWPT_jobs = jobs;
	//std::sort(listLWPT_jobs.begin(), listLWPT_jobs.end(), Job::LWPT);

	//chosenJobs.clear();
	//i = listLWPT_jobs.begin();
	//while (i != listLWPT_jobs.end())
	//{
	//	if (chosenJobs.size() >= nbJobsToSelect)
	//	{
	//		break;
	//	}

	//	chosenJobs.push_back(*i);
	//	i++;
	//}

	//// Follower's Heuristic
	//solution_init = Solution(generateur.generateInitialSolution(chosenJobs, *instance));
	//best_solution = Solution(solution_init);
	//std::cout << "Solution Initiale : " << std::endl;
	//best_solution.print();

	//// Swapping jobs between High speed machines
	//unsigned int nbHighSpeedBlocs = solution_init.getMaxNumberOfHighSpeedBlocs();

	///*
	//* We try to perform a swap between two machines for each bloc k
	//* If a swap happened at least once, we try again. Otherwise, we've reached an optimum.
	//*/
	//
	//bool end;
	//do
	//{
	//	end = true; // No swap has been performed

	//	// For each bloc k
	//	for (unsigned int k = 0; k < nbHighSpeedBlocs; k++)
	//	{
	//		// if a swap is possible, do it
	//		std::vector<unsigned int> swapableMachines = solution_init.possibleSwapVHighSpeed(k);
	//		if (swapableMachines.size() != 0)
	//		{
	//			solution_init.swapV(swapableMachines, k);
	//			end = false;
	//		}
	//	}

	//	// if the new solution is better, save it
	//	if (solution_init < best_solution)
	//	{
	//		best_solution = Solution(solution_init);
	//	}

	//} while (end == false);


	//// Swapping jobs between Low speed machines
	//unsigned int nbLowSpeedBlocs = solution_init.getMaxNumberOfLowSpeedBlocs();

	///*
	//* The same logic is applied to low speed machines
	//*/
	//do
	//{
	//	end = true; // No swap has been performed

	//	// For each bloc k
	//	for (unsigned int k = 0; k < nbLowSpeedBlocs; k++)
	//	{
	//		// if a swap is possible, do it
	//		std::vector<unsigned int> swapableMachines = solution_init.possibleSwapVLowSpeed(k);
	//		if (swapableMachines.size() != 0)
	//		{
	//			solution_init.swapV(swapableMachines, k);
	//			end = false;
	//		}
	//	}

	//	// if the new solution is better, save it
	//	if (solution_init < best_solution)
	//	{
	//		best_solution = Solution(solution_init);
	//	}

	//} while (end == false);
	//
	//// The best solution found is added to the list of best solutions
	//std::cout << "Meilleure Solution" << std::endl;
	//best_solution.print();
	//follower_solutions.push_back(best_solution);

	// We save the best solution found which has the lowest score
	std::sort(follower_solutions.begin(), follower_solutions.end(), std::less<Solution>());
	setSolution(new Solution(follower_solutions[0]));
}