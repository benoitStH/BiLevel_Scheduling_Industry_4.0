#ifndef BILEVEL_SCHEDULING_FLATETOEARLY_H
#define BILEVEL_SCHEDULING_FLATETOEARLY_H

#include "IFollowerSwapRule.h"

namespace swapRule {
	const unsigned int LATE2EARLY = 1;
	const unsigned int DUMB_METHOD = 2;
}

class FLateToEarly : public IFollowerSwapRule
{

public:
	FLateToEarly() : IFollowerSwapRule() {};

	FLateToEarly(unsigned int ruleNumber)
	{
		this->ruleNumber = ruleNumber;
		switch (ruleNumber)
		{
		case swapRule::LATE2EARLY:
			ruleName = "LateToEarly";
			break;
			
		case swapRule::DUMB_METHOD:
			ruleName = "Dumb";
			break;

		default:
			ruleName = "Unknown";
			break;
		}
	}

	/**
	 * Check between the k-th job of equal speed machine l1 and l2
	 * Returns a swap operator containing the gain we would obtain if this swap was to be performed
	 * @param s A 'Solution' object containing the current scheduling
	 * @param k the position of the k-th job
	 * @param li the machine's number. Must be between 0 and the total number of machines.
	 * @return a SwapOperation containing the parameters (l1, l2, k, gain)
	 */
	virtual SwapOperation swapVPossibleFor(const Solution& s, unsigned int k, unsigned int l1, unsigned int l2)
	{
		// Our answer
		SwapOperation swapOp;
		swapOp.machine1 = l1;
		swapOp.machine2 = l2;
		swapOp.bloc = k;
		swapOp.gain = -1;  // Negative gain means its not worth swapping

		const Machine m1 = s.getMachine(l1);
		const Machine m2 = s.getMachine(l2);
		const float speed = s.getMachine(l1).getSpeed();  // We assume l1's speed and l2's speed are the same

		// Using the rule associated with ruleNumber
		switch (ruleNumber)
		{
		case swapRule::LATE2EARLY:
			swapLateToEarly(swapOp, m1, m2, k, speed); //
			break;
			
		case swapRule::DUMB_METHOD:
			swapDumbly(swapOp, s); //
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
		// If one of the machine doesn't have a k-th job, return false
		if (m1.getAffectedJob().size() <= k) { return; }
		if (m2.getAffectedJob().size() <= k) { return; }

		const Job& job1 = m1.getAffectedJob()[k];
		const Job& job2 = m2.getAffectedJob()[k];

		bool swapIsPossible; // Is a swap possible ?

		// Are the jobs both late and swapping will make job1 early ?
		swapIsPossible = (job1.isLate() && job2.isLate() && (m2.startTimeOfJob(k) + (job1.getPi() / speed)) < job1.getDi());

		// Otherwise, Is job1 the only one late ?
		if (job1.isLate() && !job2.isLate() && swapIsPossible == false)
		{
			// Swapping will make job1 early and job2 will still be early
			swapIsPossible = ((m2.startTimeOfJob(k) + (job1.getPi() / speed) < job1.getDi()) &&
				m1.startTimeOfJob(k) + (job2.getPi() / speed) < job2.getDi());
			if (swapIsPossible) {
				swapOp.gain = job1.getWi();
			}

			// Or Swapping will make job1 early and job2 will be late, Swap is possible if w1 > w2
			swapIsPossible = (swapIsPossible || (job1.getWi() > job2.getWi() && (m2.startTimeOfJob(k) + (job1.getPi() / speed) < job1.getDi())));
			if (swapIsPossible)
			{
				swapOp.gain = job1.getWi() - job2.getWi();
			}

		}
		else if (job2.isLate() && !job1.isLate())  // Is job2 the only one late ?
		{
			// Swapping will make job2 early and job1 will still be early
			swapIsPossible = ((m2.startTimeOfJob(k) + (job1.getPi() / speed) < job1.getDi()) &&
				m1.startTimeOfJob(k) + (job2.getPi() / speed) < job2.getDi());
			if (swapIsPossible)
			{
				swapOp.gain = job2.getWi();
			}

			// Or Swapping will make job2 early and job1 will be late, Swap is possible if w2 > w1
			swapIsPossible = (swapIsPossible || (job2.getWi() > job1.getWi() && (m1.startTimeOfJob(k) + (job2.getPi() / speed) < job2.getDi())));
			if (swapIsPossible)
			{
				swapOp.gain = job2.getWi() - job1.getWi();
			}
		}
	}

	/**
	 * Perform the swap and check the difference of sum wjUj
	 * Update the swap operator's gain
	 * @param swapOp A 'SwapOperation' object representing the swapping of the k-th job between machine m1 and m2
	 * @param s A 'Solution' object containing the schedule
	 * @param speed the machine's speed. We assume m1's speed equals m2's speed.
	 */
	void swapDumbly(SwapOperation& swapOp, const Solution& s)
	{
		Solution test = Solution(s);
		std::vector<unsigned int> machines;
		machines.push_back(swapOp.machine1);
		machines.push_back(swapOp.machine2);

		// If one of the machine doesn't have a k-th job, return false
		if (s.getMachine(swapOp.machine1).getAffectedJob().size() <= swapOp.bloc) { return; }
		if (s.getMachine(swapOp.machine2).getAffectedJob().size() <= swapOp.bloc) { return; }

		test.swapV(machines, swapOp.bloc, false);

		swapOp.gain = s.getSumWjUj() - test.getSumWjUj();

	}


	/** 
	 * Method which return the swap with the highest gain as the best swap
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
			if (swapOp.gain > 0) { printOperator(swapOp); }

			if (swapOp.gain > best.gain)
			{
				best = swapOp;
			}
		}

		return best;
	}
};

#endif //BILEVEL_SCHEDULING_FLATETOEARLY_H