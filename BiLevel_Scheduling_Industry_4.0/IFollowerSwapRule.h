#ifndef BILEVEL_SCHEDULING_IFOLLOWER_SWAPRULE_H
#define BILEVEL_SCHEDULING_IFOLLOWER_SWAPRULE_H

#include "Rule.h"
#include "Solution.h"

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
	virtual bool swapVPossibleFor(const Solution& s, unsigned int k, unsigned int l1, unsigned int l2) = 0;

	virtual std::vector<unsigned int> bestSwapV(std::vector<std::vector<unsigned int>> possibleSwaps, const Solution& s) = 0;

};
#endif // BILEVEL_SCHEDULING_IFOLLOWER_SWAPRULE_H
