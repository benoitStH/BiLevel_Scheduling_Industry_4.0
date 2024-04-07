#include "FSolver.h"

void FSolver::heuristic()
{
	if (solution == nullptr)
	{
		std::cout << "Error FSolver.heuristic() : Solution not initialized !\n";
		return;
	}

	verbose.setRequiredLevel(4);
	solution->completeVerbose(); verbose << "\n";
	verbose.endRequiredLevel();

	verbose.setRequiredLevel(3);
	solution->compactVerbose(); verbose << "\n";
	verbose.endRequiredLevel();

	Solution current_solution = Solution(*solution);
	std::vector<SwapOperation> possibleSwaps;  // List of possible swaps on a bloc
	SwapOperation swapOp;

	// HIGH SPEED MACHINES //

	unsigned int nbHighSpeedBlocs = current_solution.getMaxNumberOfHighSpeedBlocs();
	unsigned int nbMachines = current_solution.getListHighSpeedMachines().size();

	verbose.setRequiredLevel(3);
	verbose << "HighSpeedMachines Swap\n";
	verbose.endRequiredLevel();

	bool NoSwapWasApplied;
	do
	{
		NoSwapWasApplied = true;

		// For each bloc k
		for (unsigned int k = 0; k < nbHighSpeedBlocs; k++)
		{
			possibleSwaps.clear();
			// We compare between the machine l1
			for (unsigned int l1 = 0; l1 < nbMachines-1; l1++)
			{
				// And an other machine l2
				for (unsigned int l2 = l1 + 1; l2 < nbMachines; l2++)
				{
					// Using each rule to check if a swap is possible or not
					for (IFollowerSwapRule* swapRule : listRules)
					{
						swapOp = swapRule->swapVPossibleFor(current_solution, k, l1, l2);
						possibleSwaps.push_back(swapOp);
					}
				}
			}

			// We extract the best possible swap, and perform it if it exists
			swapOp = listRules[0]->bestSwapV(possibleSwaps, current_solution);
			if (swapOp.gain > 0)
			{
				std::vector<unsigned int> machines;
				machines.push_back(swapOp.machine1);
				machines.push_back(swapOp.machine2);

				current_solution.swapV(machines, k);
				current_solution.evaluate();

				verbose.setRequiredLevel(4);
				listRules[0]->verboseOperator(swapOp);
				verbose.endRequiredLevel();

				NoSwapWasApplied = false;
			}
		}

		// if the new solution is better, save it
		if (current_solution < *solution)
		{
			verbose.setRequiredLevel(3);
			current_solution.compactVerbose(); verbose << "\n";
			verbose.endRequiredLevel();

			setSolution(new Solution(current_solution));
		}
		else
		{
			verbose.setRequiredLevel(4);
			current_solution.compactVerbose(); verbose << "\n";
			verbose << "This is not better\n";
			verbose.endRequiredLevel();
		}

	} while (NoSwapWasApplied == false);

	verbose.setRequiredLevel(3);
	solution->compactVerbose(); verbose << "\n";
	verbose.endRequiredLevel();

	// LOW SPEED MACHINES //

	unsigned int nbLowSpeedBlocs = current_solution.getMaxNumberOfLowSpeedBlocs();
	unsigned int nbTotalMachines = current_solution.getListLowSpeedMachines().size() + nbMachines;

	verbose.setRequiredLevel(3);
	verbose << "LowSpeedMachines Swap\n";
	verbose.endRequiredLevel();

	do
	{
		NoSwapWasApplied = true; // No swap has been performed

		// For each bloc k
		for (unsigned int k = 0; k < nbLowSpeedBlocs; k++)
		{
			possibleSwaps.clear();
			// We compare between the machine l1
			for (unsigned int l1 = nbMachines; l1 < nbTotalMachines-1; l1++)
			{
				// And an other machine l2
				for (unsigned int l2 = l1 + 1; l2 < nbTotalMachines; l2++)
				{
					// Using each rule to check if a swap is possible or not
					for (IFollowerSwapRule* swapRule : listRules)
					{
						swapOp = swapRule->swapVPossibleFor(current_solution, k, l1, l2);
						possibleSwaps.push_back(swapOp);
					}
				}
			}

			// We extract the best possible swap, and perform it if it exists
			swapOp = listRules[0]->bestSwapV(possibleSwaps, current_solution);
			if (swapOp.gain > 0)
			{
				std::vector<unsigned int> machines;
				machines.push_back(swapOp.machine1);
				machines.push_back(swapOp.machine2);

				current_solution.swapV(machines, k);
				current_solution.evaluate();

				verbose.setRequiredLevel(4);
				listRules[0]->verboseOperator(swapOp);
				verbose.endRequiredLevel();

				NoSwapWasApplied = false;
			}
		}


		// if the new solution is better, save it
		if (current_solution < *solution)
		{
			verbose.setRequiredLevel(3);
			current_solution.compactVerbose(); verbose << "\n";
			verbose.endRequiredLevel();

			setSolution(new Solution(current_solution));
		}
		else
		{
			verbose.setRequiredLevel(4);
			current_solution.compactVerbose(); verbose << "\n";
			verbose << "This is not better\n";
			verbose.endRequiredLevel();
		}

	} while (NoSwapWasApplied == false);

	verbose.setRequiredLevel(3);
	solution->compactVerbose(); verbose << "\n";
	verbose.endRequiredLevel();

}

std::string FSolver::getHeuristicName() const
{
	return "FSolver - Best 1-Swap Heuristic";
}

std::string FSolver::getHeuristicDescription() const
{
	std::string description = "Checking 1-swap possibility with : ";

	for (IFollowerSwapRule* swapRule : listRules)
	{
		description += swapRule->getRuleName() + " ";
	}
	description += "\n and choosing the 1-swap with highest improvement";

	return description;
}