#ifndef BILEVEL_SCHEDULING_FLATETOEARLY_H
#define BILEVEL_SCHEDULING_FLATETOEARLY_H

#include "IFollowerSwapRule.h"

namespace swapRule {
	const unsigned int LATE2EARLY = 1;
	const unsigned int DUMB_METHOD = 2;
	const unsigned int LATENESS = 3;
	const unsigned int LATENESS_CAREFUL = 4;
}

class FSwapRule : public IFollowerSwapRule
{

public:
	FSwapRule() : IFollowerSwapRule() {};

	FSwapRule(unsigned int ruleNumber)
	{

		// Setting the name according to the ruleNumber
		this->ruleNumber = ruleNumber;
		switch (ruleNumber)
		{
		case swapRule::LATE2EARLY:
			ruleName = "LateToEarly";
			break;
			
		case swapRule::DUMB_METHOD:
			ruleName = "Dumb";
			break;
			
		case swapRule::LATENESS:
			ruleName = "Lateness";
			break;
			
		case swapRule::LATENESS_CAREFUL:
			ruleName = "LatenessCareful";
			break;

		default:
			ruleName = "Unknown";
			break;
		}
	}

	/**
	 * Check if a swap is possible between the k-th job of equal speed machine l1 and l2
	 * Returns a swap operator containing the gain we would obtain if this swap was to be performed
	 * @param s A 'Solution' object containing the current scheduling
	 * @param k the position of the k-th job
	 * @param li the machine's number. Must be between 0 and the total number of machines.
	 * @return a SwapOperation containing the parameters (l1, l2, k, gain)
	 */
	virtual SwapOperation swapVPossibleFor(const Solution& s, unsigned int k, unsigned int l1, unsigned int l2)
	{
		// Our default answer
		SwapOperation swapOp;
		swapOp.machine1 = l1;
		swapOp.machine2 = l2;
		swapOp.bloc = k;
		swapOp.gain = -1;  // Negative gain means its not worth swapping

		const Machine m1 = s.getMachine(l1);
		const Machine m2 = s.getMachine(l2);
		const float speed = s.getMachine(l1).getSpeed();  // We assume l1's speed and l2's speed are the same


		// If one of the machine doesn't have a k-th job, return default answer
		if (m1.getAffectedJob().size() <= swapOp.bloc) { return swapOp; }
		if (m2.getAffectedJob().size() <= swapOp.bloc) { return swapOp; }

		// Using the rule associated with ruleNumber
		switch (ruleNumber)
		{
		case swapRule::LATE2EARLY:
			swapLateToEarly(swapOp, m1, m2, k, speed); //
			break;
			
		case swapRule::DUMB_METHOD:
			swapDumbly(swapOp, s); //
			break;
			
		case swapRule::LATENESS:
			swapLateness(swapOp, s); //
			break;
			
		case swapRule::LATENESS_CAREFUL:
			swapLatenessCareful(swapOp, s); //
			break;

			default:
				break;
		}

		return swapOp;
	}

	/**
	 * Check between the k-th job of equal speed machine l1 and l2
	 * Update the swap operator's gain
	 * @param swapOp A 'SwapOperation' object representing the swapping of the k-th job between machine m1 and m2
	 * @param mi the machines (m1 and m2) 
	 * @param k the position of the k-th job
	 * @param speed the machine's speed. We assume m1's speed equals m2's speed.
	 */
	void swapLateToEarly(SwapOperation& swapOp, const Machine& m1, const Machine& m2, unsigned int k, float speed)
	{
		const Job& job1 = m1.getAffectedJob()[k];
		const Job& job2 = m2.getAffectedJob()[k];

		float p1 = job1.getPi() / speed;
		float p2 = job2.getPi() / speed;
		float w1 = job1.getWi();
		float w2 = job2.getWi();
		float d1 = job1.getDi();
		float d2 = job2.getDi();

		bool swapIsPossible; // Is a swap possible ?

		// Are the jobs both late and swapping will make job1 early ?
		swapIsPossible = (job1.isLate() && job2.isLate() && (m2.startTimeOfJob(k) + p1) < d1);

		// Otherwise, Is job1 the only one late ?
		if (job1.isLate() && !job2.isLate() && swapIsPossible == false)
		{
			// Swapping will make job1 early and job2 will still be early
			swapIsPossible = ((m2.startTimeOfJob(k) + p1 < d1) &&
				m1.startTimeOfJob(k) + p2 < d2);
			if (swapIsPossible) {
				swapOp.gain = w1;
			}

			// Or Swapping will make job1 early and job2 will be late, Swap is possible if w1 > w2
			swapIsPossible = (m2.startTimeOfJob(k) + p1 < d1);
			if (swapIsPossible && w1 > w2)
			{
				swapOp.gain = w1 - w2;
			}

		}
		else if (job2.isLate() && !job1.isLate())  // Is job2 the only one late ?
		{
			// Swapping will make job2 early and job1 will still be early
			swapIsPossible = ((m2.startTimeOfJob(k) + p1 < d1) &&
				m1.startTimeOfJob(k) + p2 < d2);
			if (swapIsPossible)
			{
				swapOp.gain = w2;
			}

			// Or Swapping will make job2 early and job1 will be late, Swap is possible if w2 > w1
			swapIsPossible = (m1.startTimeOfJob(k) + p2 < d2);
			if (swapIsPossible && w2 > w1)
			{
				swapOp.gain = w2 - w1;
			}
		}
	}

	/**
	 * Perform the swap and check the difference of sum wjUj
	 * Update the swap operator's gain
	 * @param swapOp A 'SwapOperation' object representing the swapping of the k-th job between machine m1 and m2
	 * @param s A 'Solution' object containing the schedule
	 */
	void swapDumbly(SwapOperation& swapOp, const Solution& s)
	{
		Solution test = Solution(s);
		std::vector<unsigned int> machines;
		machines.push_back(swapOp.machine1);
		machines.push_back(swapOp.machine2);

		verbose.setRequiredLevel(5);
		test.swapV(machines, swapOp.bloc);
		test.evaluate();
		test.compactVerbose(); verbose << "\n";
		verbose.endRequiredLevel();

		swapOp.gain = s.getSumWjUj() - test.getSumWjUj();

	}

	/**
	 * Check if a swap will improve the solution by checking the lateness of the following jobs
	 * Update the swap operator's gain
	 * @param swapOp A 'SwapOperation' object representing the swapping of the k-th job between machine m1 and m2
	 * @param s A 'Solution' object containing the schedule
	 */
	void swapLateness(SwapOperation& swapOp, const Solution& s)
	{
		unsigned int k = swapOp.bloc;
		const Machine& m1 = s.getMachine(swapOp.machine1);
		const Machine& m2 = s.getMachine(swapOp.machine2);

		const Job& job1 = m1.getAffectedJob()[k];
		const Job& job2 = m2.getAffectedJob()[k];
		float speed = m1.getSpeed();

		float pi_diff = (job1.getPi() - job2.getPi()) / speed;

		// If it is the last bloc, use the dumb method
		if (swapOp.bloc == m1.getAffectedJob().size() - 1)
		{
			swapDumbly(swapOp, s);
			return;
		}


		float lateness1; // lateness of job1's following job
		do
		{
			const Job& nextJob1 = m1.getAffectedJob()[k + 1];  // job1's following job
			lateness1 = m1.startTimeOfJob(swapOp.bloc + 2) - float(nextJob1.getDi());  // computing its lateness

			// If it is late and the lateness is smaller than the difference of job1's and job2's processing time
			if (0 < lateness1 && lateness1 <= pi_diff)
			{
				// Initialize the swap's gain at 0 if that is not the case already
				if (swapOp.gain < 0) { swapOp.gain = 0; }

				// If we choose this swap, this following job will become early, we add its weight to our swap's gain
				swapOp.gain += nextJob1.getWi();
			}

			k++; // We check the next following job until we reach the one before last one
		} while (k < m1.getAffectedJob().size() - 1);
		

	}

	/**
	 * Check if a swap will improve the solution by checking the lateness of the following jobs and the swaped jobs
	 * Update the swap operator's gain
	 * @param swapOp A 'SwapOperation' object representing the swapping of the k-th job between machine m1 and m2
	 * @param s A 'Solution' object containing the schedule
	 */
	void swapLatenessCareful(SwapOperation& swapOp, const Solution& s)
	{
		unsigned int k = swapOp.bloc;
		const Machine& m1 = s.getMachine(swapOp.machine1);
		const Machine& m2 = s.getMachine(swapOp.machine2);

		const Job& job1 = m1.getAffectedJob()[k];
		const Job& job2 = m2.getAffectedJob()[k];
		float speed = m1.getSpeed();

		// If it is the last bloc, use the dumb method
		if (swapOp.bloc == m1.getAffectedJob().size() - 1)
		{
			swapDumbly(swapOp, s);
			return;
		}

		swapOp.gain = 0;

		// If job1 is late and will be early after swapping
		if (job1.isLate() && m2.startTimeOfJob(k) + (job1.getPi() / speed) <= job1.getDi())
		{
			swapOp.gain += job1.getWi();
		}

		// If job1 is early and will be late after swapping
		if (job1.isLate() == false && m2.startTimeOfJob(k) + (job1.getPi() / speed) > job1.getDi())
		{
			swapOp.gain -= job1.getWi();
		}
		
		// If job2 is late and will be early after swapping
		if (job2.isLate() && m2.startTimeOfJob(k) + (job2.getPi() / speed) <= job2.getDi())
		{
			swapOp.gain += job2.getWi();
		}

		// If job2 is early and will be late after swapping
		if (job2.isLate() == false && m2.startTimeOfJob(k) + (job2.getPi() / speed) > job2.getDi())
		{
			swapOp.gain -= job2.getWi();
		}

		// Check job1's following jobs
		swapLateness(swapOp, s);
	}


	/** 
	 * Method which return the swap with the highest gain as the best swap
	 * @param possibleSwaps is a vector of SwapOperation to compare
	 * @param s the current solution (unused in this implementation)
	 * @return the best swap operation
	 * If possibleSwaps is empty, a swap operation with all atributes equal to 0 is returned
	*/
	virtual SwapOperation bestSwapV(const std::vector<SwapOperation>& possibleSwaps, const Solution& s)
	{
		SwapOperation best;
		best.machine1 = 0;
		best.machine2 = 0;
		best.bloc = 0;
		best.gain = 0;

		if (possibleSwaps.empty()) { return best; }

		for (SwapOperation swapOp : possibleSwaps)
		{
			if (swapOp.gain > 0) { verboseOperator(swapOp); }

			if (swapOp.gain > best.gain)
			{
				best = swapOp;
			}
		}

		return best;
	}
};

#endif //BILEVEL_SCHEDULING_FLATETOEARLY_H