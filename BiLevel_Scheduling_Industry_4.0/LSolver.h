#ifndef BILEVEL_SCHEDULING_LSOLVER_H
#define BILEVEL_SCHEDULING_LSOLVER_H


#include "ISolver.h"

class LSolver : public ISolver
{
public:

	virtual void solve();
};


#endif // BILEVEL_SCHEDULING_LSOLVER_H

