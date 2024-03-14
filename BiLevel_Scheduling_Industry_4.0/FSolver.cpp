#include "FSolver.h"

void FSolver::heuristic()
{
	if (solution == nullptr)
	{
		std::cout << "Error FSolver.heuristic() : Solution not initialized !\n";
		return;
	}
	//solution->print(); std::cout << std::endl;
	//solution->compactPrint(); std::cout << std::endl;

	Solution solution_init = Solution(*solution);
	std::vector<SwapOperation> possibleSwaps;
	SwapOperation swapOp;

	// HIGH SPEED MACHINES //

	unsigned int nbHighSpeedBlocs = solution_init.getMaxNumberOfHighSpeedBlocs();
	unsigned int nbMachines = solution_init.getListHighSpeedMachines().size();

	//std::cout << "HighSpeedMachines Swap\n";
	bool end;
	do
	{
		end = true; // No swap has been performed

		// For each bloc k
		for (unsigned int k = 0; k < nbHighSpeedBlocs; k++)
		{
			possibleSwaps.clear();
			// We compare between the machine l1
			for (unsigned int l1 = 0; l1 < nbMachines-1; l1++)
			{
				// And a following machine l2
				for (unsigned int l2 = l1 + 1; l2 < nbMachines; l2++)
				{
					// Using each rule to check if a swap is possible or not
					for (IFollowerSwapRule* swapRule : listRules)
					{
						swapOp = swapRule->swapVPossibleFor(solution_init, k, l1, l2);
						possibleSwaps.push_back(swapOp);
					}
				}
			}

			//unsigned int bloc = swapOp.bloc;
			//std::cin >> swapOp.bloc;
			//swapOp.bloc = bloc;

			// We extract the best possible swap according to a rule, and perform it if it exists
			swapOp = listRules[0]->bestSwapV(possibleSwaps, solution_init);
			if (swapOp.gain > 0)
			{
				std::vector<unsigned int> machines;
				machines.push_back(swapOp.machine1);
				machines.push_back(swapOp.machine2);

				solution_init.swapV(machines, k);
				solution_init.evaluate();
				//std::cout << "swap (" << swapOp.machine1 << ", " << swapOp.machine2 << ") at bloc " << k << std::endl;
				end = false;
			}
		}

		//solution_init.print(); std::cout << std::endl;

		// if the new solution is better, save it
		if (solution_init < *solution)
		{
			setSolution(new Solution(solution_init));
		}

	} while (end == false);

	//solution->compactPrint(); std::cout << std::endl;

	// LOW SPEED MACHINES //

	unsigned int nbLowSpeedBlocs = solution_init.getMaxNumberOfLowSpeedBlocs();
	unsigned int nbTotalMachines = solution_init.getListLowSpeedMachines().size() + nbMachines;

	//std::cout << "LowSpeedMachines Swap\n";
	do
	{
		end = true; // No swap has been performed

		// For each bloc k
		for (unsigned int k = 0; k < nbLowSpeedBlocs; k++)
		{
			possibleSwaps.clear();
			// We compare between the machine l1
			for (unsigned int l1 = nbMachines; l1 < nbTotalMachines-1; l1++)
			{
				// And a following machine l2
				for (unsigned int l2 = l1 + 1; l2 < nbTotalMachines; l2++)
				{
					// Using each rule to check if a swap is possible or not
					for (IFollowerSwapRule* swapRule : listRules)
					{
						swapOp = swapRule->swapVPossibleFor(solution_init, k, l1, l2);
						possibleSwaps.push_back(swapOp);
					}
				}
			}

			// We extract the best possible swap according to a rule, and perform it if it exists
			swapOp = listRules[0]->bestSwapV(possibleSwaps, solution_init);
			if (swapOp.gain > 0)
			{
				std::vector<unsigned int> machines;
				machines.push_back(swapOp.machine1);
				machines.push_back(swapOp.machine2);

				solution_init.swapV(machines, k);
				solution_init.evaluate();
				//std::cout << "swap (" << swapOp.machine1 << ", " << swapOp.machine2 << ") at bloc " << k << std::endl;
				end = false;
			}
		}

		//solution_init.print(); std::cout << std::endl;

		// if the new solution is better, save it
		if (solution_init < *solution)
		{
			setSolution(new Solution(solution_init));
		}

	} while (end == false);

	//solution->compactPrint(); std::cout << std::endl;

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