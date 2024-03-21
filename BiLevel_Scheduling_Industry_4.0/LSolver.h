#ifndef BILEVEL_SCHEDULING_LSOLVER_H
#define BILEVEL_SCHEDULING_LSOLVER_H


#include "ISolver.h"

class LSolver : public ISolver
{
public:

	/**
	* Overriden method which solve the instance using each selection rule
	* The best solution has the smallest sum of wjUj compared to the other selection rule
	* The resolution time is measured
	*/
	virtual void solve();

	virtual std::string getHeuristicName() const;
	virtual std::string getHeuristicDescription() const;
};


#endif // BILEVEL_SCHEDULING_LSOLVER_H

