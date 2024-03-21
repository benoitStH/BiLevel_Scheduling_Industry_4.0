#ifndef BILEVEL_SCHEDULING_FSOLVER_H
#define BILEVEL_SCHEDULING_FSOLVER_H

#include "ISubSolver.h"

class FSolver : public ISubSolver
{

public:
	FSolver() : ISubSolver() {};

	FSolver(std::vector<IFollowerSwapRule*> listRule) : ISubSolver(listRule) {};

	/*********************/
	/*      METHODS      */
	/*********************/

	/**
	 * Method which solves the lexicographic problem of the Follower
	 * Lex(sum Cj, Sum wjUj)
	 * An initial solution must be generated before calling this method
	 * The solution attribute will contain the final solution found by the heuristic after calling this method
	 * @param None
	 */
	virtual void heuristic();

	/********************/
	/*      GETTER      */
	/********************/

	virtual std::string getHeuristicName() const;

	virtual std::string getHeuristicDescription() const;
};

#endif //BILEVEL_SCHEDULING_FSOLVER_H

