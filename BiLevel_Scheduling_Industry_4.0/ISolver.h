#ifndef BILEVEL_SCHEDULING_ISOLVER_H
#define BILEVEL_SCHEDULING_ISOLVER_H

#include "Solution.h"
#include "ILeaderSelectRule.h"
#include "ISubSolver.h"
#include "Generateur.h"

class ISolver
{
protected:
	Solution* solution;
	const Instance* instance;
	bool verbose;
    std::vector<ILeaderSelectRule> listRules;
    ISubSolver* subSolver;

    Generateur generateur;

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/


	ISolver() : solution(new Solution()), instance(nullptr), verbose(false), listRules(std::vector<ILeaderSelectRule>()), subSolver(nullptr) {}

    /**
     * Constructor of a solver from a instance.
     * @param instance the instance to solve
     */
    ISolver(const Instance* instance) : solution(new Solution(instance)), instance(instance), verbose(false), subSolver(nullptr) {}


    /***********************/
    /*      DESTRUCTOR     */
    /***********************/

    virtual ~ISolver() { delete solution; }


    /********************/
    /*      METHODS     */
    /********************/

    /**
     * Pure Virtual Method that implement a algorithm to solve the current instance.
     */
    virtual void solve() = 0;

    /**
    * Method to display a message during verbose if its value is true
    */
    void printVerbose(std::string msg) { if (verbose) std::cout << msg << std::endl; }

    /********************/
    /*      GETTER      */
    /********************/

    const Solution* getSolution() const { return solution; }

    const Instance* getInstance() const { return instance; }

    bool isVerbose() const { return verbose; }
    /********************/
    /*      SETTER      */
    /********************/

    void setRules(std::vector<ILeaderSelectRule> listRule) {listRules = listRule;}

    void setSubSolver(ISubSolver* subSolver)
    {
        if (this->subSolver != nullptr)
        {
            delete this->subSolver;
        }
        this->subSolver = subSolver;

    }

    void setSolution(Solution* solution) 
    { 
        if (this->solution != nullptr)
        {
            delete this->solution;
        }
        this->solution = solution; 
    }

    void setInstance(const Instance* instance) { ISolver::instance = instance; }

    void setVerbose(bool verbose) { ISolver::verbose = verbose; }



};


#endif //  BILEVEL_SCHEDULING_ISOLVER_H

