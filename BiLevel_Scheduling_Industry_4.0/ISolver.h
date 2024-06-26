﻿#ifndef BILEVEL_SCHEDULING_ISOLVER_H
#define BILEVEL_SCHEDULING_ISOLVER_H

#include "Solution.h"
#include "ILeaderSelectRule.h"
#include "ISubSolver.h"

class ISolver
{
protected:
	Solution* solution;  // Dynamically allocated
	const Instance* instance;
    std::vector<ILeaderSelectRule*> listRules;
    long timeResol;
    ISubSolver* subSolver;

    Verbose verbose;

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/

    // Generate the solver with an empty solution
	ISolver() : solution(new Solution()), instance(nullptr), listRules(std::vector<ILeaderSelectRule*>()), subSolver(nullptr), timeResol(0) {}

    /**
     * Constructor of a solver from a instance.
     * @param instance the instance to solve
     */
    ISolver(const Instance* instance) : solution(new Solution(instance)), instance(instance), subSolver(nullptr), timeResol(0) {}


    /***********************/
    /*      DESTRUCTOR     */
    /***********************/

    virtual ~ISolver() {

        if (solution != nullptr)
        {
            delete solution;
        }
    }


    /********************/
    /*      METHODS     */
    /********************/

    /**
     * Pure Virtual Method that implement an algorithm to solve the current instance.
     */
    virtual void solve() = 0;

    void addRule(ILeaderSelectRule* selectRule) { listRules.push_back(selectRule); }

    /********************/
    /*      GETTER      */
    /********************/

    const Solution* getSolution() const { return solution; }

    const Instance* getInstance() const { return instance; }

    const long getTimeResol() const { return timeResol; }

    virtual std::string getHeuristicName() const = 0;

    virtual std::string getHeuristicDescription() const = 0;
    /********************/
    /*      SETTER      */
    /********************/

    void setRules(std::vector<ILeaderSelectRule*> listRule) {listRules = listRule;}

    // Free the old pointer if not nullptr
    void setSubSolver(ISubSolver* subSolver)
    {
        if (this->subSolver != nullptr)
        {
            delete this->subSolver;
        }
        this->subSolver = subSolver;

    }

    // Free the old pointer if not nullptr
    void setSolution(Solution* solution) 
    { 
        if (this->solution != nullptr)
        {
            delete this->solution;
        }
        this->solution = solution; 
    }

    void setInstance(const Instance* instance) { ISolver::instance = instance; }



};


#endif //  BILEVEL_SCHEDULING_ISOLVER_H

