#ifndef BILEVEL_SCHEDULING_ILEADERSELECTRULE_H
#define BILEVEL_SCHEDULING_ILEADERSELECTRULE_H

#include "Rule.h"
#include <vector>
#include "Job.h"

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
	virtual std::vector<Job> selectJobsFrom(std::vector<Job> listJobs) = 0;



};

#endif // BILEVEL_SCHEDULING_ILEADERSELECTRULE_H

