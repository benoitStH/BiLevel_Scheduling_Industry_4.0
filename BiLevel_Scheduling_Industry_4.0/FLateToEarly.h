#ifndef BILEVEL_SCHEDULING_FLATETOEARLY_H
#define BILEVEL_SCHEDULING_FLATETOEARLY_H

#include "IFollowerSwapRule.h"

class FLateToEarly : public IFollowerSwapRule
{

public:
	FLateToEarly() : IFollowerSwapRule("LateToEarly", 1) {};

	/**
	 * Check between the k-th job of equal speed machine l1 and l2
	 * If both jobs are late but the job of machine l1 will be early after swapping, return true. False otherwise.
	 * @param s A 'Solution' object containing the current scheduling
	 * @param k the position of the k-th job
	 * @param li the machine's number. Must be between 0 and the total number of machines.
	 * @return True if l1's job and l2's job are late and l1's job will be early after swapping
	 * False Otherwise.
	 */
	virtual bool swapVPossibleFor(const Solution& s, unsigned int k, unsigned int l1, unsigned int l2)
	{
		const Machine m1 = s.getMachine(l1);
		const Machine m2 = s.getMachine(l2);
		const float speed = s.getMachine(l1).getSpeed();  // We assume l1's speed and l2's speed are the same

		// If one of the machine doesn't have a k-th job, return false
		if (m1.getAffectedJob().size() <= k) { return false; }
		if (m2.getAffectedJob().size() <= k) { return false; }

		const Job& job1 = m1.getAffectedJob()[k];
		const Job& job2 = m2.getAffectedJob()[k];

		bool swapIsPossible = (job1.isLate() && job2.isLate() && (m2.startTimeOfJob(k) + (job1.getPi() / speed)) < job1.getDi());

		return swapIsPossible;
	}

	/** 
	 * Method which return the first possible swap as the best swap
	*/
	virtual std::vector<unsigned int> bestSwapV(std::vector<std::vector<unsigned int>> possibleSwaps, const Solution& s)
	{
		if (possibleSwaps.empty()) { return std::vector<unsigned int>(); }

		return possibleSwaps[0];
	}
};

#endif //BILEVEL_SCHEDULING_FLATETOEARLY_H