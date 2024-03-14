#ifndef BILEVEL_SCHEDULING_LSOLVER_H
#define BILEVEL_SCHEDULING_LSOLVER_H


#include "ISolver.h"

class LSolver : public ISolver
{
public:

	virtual void solve();

	virtual std::string getHeuristicName() const;
	virtual std::string getHeuristicDescription() const;
};


#endif // BILEVEL_SCHEDULING_LSOLVER_H

