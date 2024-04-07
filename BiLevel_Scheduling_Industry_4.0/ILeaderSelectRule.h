#ifndef BILEVEL_SCHEDULING_ILEADERSELECTRULE_H
#define BILEVEL_SCHEDULING_ILEADERSELECTRULE_H

#include "Rule.h"
#include <vector>
#include "Instance.h"

class ILeaderSelectRule : public Rule
{

public:
	/************************/
	/*      CONSTRUCTOR     */
	/************************/
	ILeaderSelectRule() : Rule() {};
	ILeaderSelectRule(std::string ruleName, unsigned int ruleNumber) : Rule(ruleName, ruleNumber) {};

	/********************/
	/*      GETTERS     */
	/********************/
	virtual std::string getRuleName() { return "L_"+ruleName; }

	/********************/
	/*      METHODS     */
	/********************/

	/* 
	 * Overridable Method which will select a subset of n jobs among the list of Jobs given
	 * @param instance An instance with the list of jobs and the number of jobs to select
	 * @return a list of selected jobs
	 */
	virtual std::vector<Job> selectJobsFrom(const Instance& instance) = 0;



};

#endif // BILEVEL_SCHEDULING_ILEADERSELECTRULE_H

