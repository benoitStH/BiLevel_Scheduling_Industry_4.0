#ifndef BILEVEL_SCHEDULING_IFOLLOWER_SWAPRULE_H
#define BILEVEL_SCHEDULING_IFOLLOWER_SWAPRULE_H

#include "Rule.h"
#include "Solution.h"

/*
Represent a swap operation to apply between two machines on a specific bloc
machine<i> is the index of the machine i
bloc is the job's position on each machine
gain is the improvement we can obtain if we apply this swap operation (The higher, the better)
*/
struct SwapOperation
{
	unsigned int machine1;
	unsigned int machine2;
	unsigned int bloc;
	float gain; // How much will this operation improve our solution ? The higher, the better. 
};

class IFollowerSwapRule : public Rule
{

public:
	/************************/
	/*      CONSTRUCTOR     */
	/************************/
	IFollowerSwapRule() : Rule() {};
	IFollowerSwapRule(std::string ruleName, unsigned int ruleNumber) : Rule(ruleName, ruleNumber) {};

	/********************/
	/*      GETTERS     */
	/********************/
	virtual std::string getRuleName() { return "F_" + ruleName; }

	/********************/
	/*      METHODS     */
	/********************/

	/**
	 * Method which return a swap operation if a swap between the k-th jobs of two machines l1 and l2 is possible
	 * This method needs too be overriden
	*/
	virtual SwapOperation swapVPossibleFor(const Solution& s, unsigned int k, unsigned int l1, unsigned int l2) = 0;

	/**
	 * Method which return the best swap according to a criterion among a list of possible swap
	 * This method must be overriden
	*/
	virtual SwapOperation bestSwapV(const std::vector<SwapOperation>& possibleSwaps, const Solution& s) = 0;

	void printOperator(const SwapOperation& swapOp)
	{
		//std::cout << "(" << swapOp.machine1 << ", " << swapOp.machine2 << ", " << swapOp.bloc << ") : " << swapOp.gain << std::endl;
	}

};
#endif // BILEVEL_SCHEDULING_IFOLLOWER_SWAPRULE_H
