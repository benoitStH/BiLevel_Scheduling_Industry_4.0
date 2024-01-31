#ifndef BILEVEL_SCHEDULING_ISUBSOLVER_H
#define BILEVEL_SCHEDULING_ISUBSOLVER_H

#include "Solution.h"
#include "IFollowerSwapRule.h"

class ISubSolver
{
protected:
	Solution* solution;
	bool verbose;
	std::vector<IFollowerSwapRule> listRules;

public:
	/************************/
	/*      CONSTRUCTOR     */
	/************************/
	ISubSolver() : solution(nullptr), verbose(false), listRules(std::vector<IFollowerSwapRule>()) {};

	ISubSolver(std::vector<IFollowerSwapRule> listRule) : solution(nullptr), verbose(false), listRules(listRule) {};

	~ISubSolver()
	{
		if (solution != nullptr)
		{
			delete solution;
			solution = nullptr;
		}
	}

	/********************/
	/*      GETTERS     */
	/********************/
	const Solution* getSolution() const { return solution; }

	bool isVerbose() const { return verbose; }

	/********************/
	/*      METHODS     */
	/********************/
	virtual void heuristic() = 0;

};

#endif // BILEVEL_SCHEDULING_ISUBSOLVER_H