#ifndef BILEVEL_SCHEDULING_ISOLVER_H
#define BILEVEL_SCHEDULING_ISOLVER_H

#include "Solution.h"
#include "Generateur.h"

class ISolver
{
protected:
	Solution* solution;
	const Instance* instance;
	bool verbose;

    Generateur generateur;

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/


	ISolver() : solution(new Solution()), instance(nullptr), verbose(false) {}

    /**
     * Constructor of a solver from a instance.
     * @param instance the instance to solve
     */
    ISolver(const Instance* instance) : solution(new Solution(instance)), instance(instance), verbose(false) {}


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

