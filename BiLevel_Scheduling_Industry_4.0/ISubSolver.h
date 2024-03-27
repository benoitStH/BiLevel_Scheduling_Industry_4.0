#ifndef BILEVEL_SCHEDULING_ISUBSOLVER_H
#define BILEVEL_SCHEDULING_ISUBSOLVER_H

#include "Solution.h"
#include "Instance.h"
#include "IFollowerSwapRule.h"

class ISubSolver
{
protected:
	Solution* solution;  // Dynamicaly allocated
	std::vector<IFollowerSwapRule*> listRules;

	Verbose verbose;

public:
	/************************/
	/*      CONSTRUCTOR     */
	/************************/
	ISubSolver() : solution(nullptr), listRules(std::vector<IFollowerSwapRule*>()) {};

	ISubSolver(std::vector<IFollowerSwapRule*> listRule) : solution(nullptr), listRules(listRule) {};

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

	virtual std::string getHeuristicName() const = 0;

	virtual std::string getHeuristicDescription() const = 0;

	/********************/
	/*      SETTERS     */
	/********************/
	void setSolution(Solution* solution)
	{
		if (this->solution != nullptr)
		{
			delete this->solution;
		}
		this->solution = solution;
	}

	/********************/
	/*      METHODS     */
	/********************/

	/**
	 * Method which solves the lexicographic problem of the Follower
	 * Lex(sum Cj, Sum wjUj)
	 * An initial solution must be generated before calling this method
	 * The solution attribute will contain the final solution found by the heuristic after calling this method
	 * @param None
	 */
	virtual void heuristic() = 0;

	/**
	 * Method that generate an initial solution from a given list of jobs of an instance
	 * This solution is evaluated after generation and minimize the sum Cj
	 * @param listJobs a list of selected jobs to schedule
	 * @param instance the instance to solve
	 */
	void generateInitialSolution(const std::vector<Job> listJobs, const Instance& instance)
	{
		verbose.setRequiredLevel(3);
		verbose << "Generating initial solution...\n";
		verbose.endRequiredLevel();

		// Initialize the solution (with empty machines)
		setSolution(new Solution(&instance));

		// getting the sorted list of jobs
		std::vector<Job> listLPT_jobs = listJobs;
		std::sort(listLPT_jobs.begin(), listLPT_jobs.end(), Job::LPT_inv_EDD);

		// total number of machines
		unsigned int nbMachines = instance.getNbOfHighSpeedMachines() + instance.getNbOfLowSpeedMachines();

		// affectation weights of each machine which are each equal to (number of affected jobs + 1)/speed
		std::vector<float> affectationWeights = std::vector<float>(nbMachines, float(1));

		for (int i = 0; i < nbMachines; i++)
		{
			affectationWeights[i] = affectationWeights[i] / (*solution)[i].getSpeed();
		}

		// machine whose affectation weight is the smallest (only the first occurence)
		unsigned int min_index;

		// for each job
		for (Job& job : listLPT_jobs)
		{
			// find the machine with smallest affectation weight
			min_index = std::distance(affectationWeights.begin(), std::min_element(affectationWeights.begin(), affectationWeights.end()));

			// inserting the job at the start of the machine's sequence
			(*solution)[min_index].add_job(0, job);

			// updating the machine's affectation weight
			affectationWeights[min_index] = ((*solution)[min_index].getAffectedJob().size() + 1) / (*solution)[min_index].getSpeed();
		}

		unsigned int nbJobsOnHighSpeedMachines = solution->getMaxNumberOfHighSpeedBlocs();
		unsigned int nbHighMachines = instance.getNbOfHighSpeedMachines();

		unsigned int nbJobsOnLowSpeedMachines = solution->getMaxNumberOfLowSpeedBlocs();
		unsigned int nbLowSpeedMachines = instance.getNbOfLowSpeedMachines();

		// Adding ghost jobs (whose number, processing time, due date and weight are 0)
		for (unsigned int m = 0; m < nbMachines; m++)
		{
			while ((*solution)[m].getAffectedJob().size() < ( m < nbHighMachines ? nbJobsOnHighSpeedMachines : nbJobsOnLowSpeedMachines))
			{
				// inserting a ghost job at the start of the machine's sequence
				(*solution)[m].add_job(0, Job());
				//std::cout << "added ghost job at machine " << m << std::endl;
			}
		}

		// evaluate solution
		solution->evaluate();
	}

	void addRule(IFollowerSwapRule* swapRule) { listRules.push_back(swapRule); }

};

#endif // BILEVEL_SCHEDULING_ISUBSOLVER_H