#include "DeepestDescent.h"

void DeepestDescent::solve()
{
	Solution solution_init = generateur.generateInitialSolution(*instance);
	Solution best_solution = Solution(solution_init);

	// Heuristic



	// We save the best solution found
	setSolution(new Solution(best_solution));
}