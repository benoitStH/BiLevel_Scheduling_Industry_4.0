#ifndef BILEVEL_SCHEDULING_RULE_H
#define BILEVEL_SCHEDULING_RULE_H

#include <string>

class Rule
{
protected:
	std::string ruleName;  // rule's name
	unsigned int ruleNumber;  // rule's number id

public:

	/************************/
	/*      CONSTRUCTOR     */
	/************************/
	Rule() : ruleName("Unknown"), ruleNumber(0) {};

	Rule(std::string ruleName, unsigned int ruleNumber) : ruleName(ruleName), ruleNumber(ruleNumber) {};

	/********************/
	/*      GETTERS     */
	/********************/
	virtual std::string getRuleName() = 0;

	unsigned int getRuleNumber() { return ruleNumber; }

};
#endif //BILEVEL_SCHEDULING_RULE_H
