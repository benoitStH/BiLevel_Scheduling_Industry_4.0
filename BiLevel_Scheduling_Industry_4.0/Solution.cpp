#include "Solution.h"


Solution::Solution()
{
	HighSpeedMachines = std::vector<Machine>();
	LowSpeedMachines = std::vector<Machine>();
	score = 0;
}

Solution::Solution(std::vector<Machine>& HighSpeedMachines, std::vector<Machine>& LowSpeedMachines, unsigned int score)
{
	this->HighSpeedMachines = HighSpeedMachines;
	this->LowSpeedMachines = LowSpeedMachines;
	this->score = score;
}

unsigned int Solution::getScore()
{
	return score;
}

void Solution::setScore(unsigned int score)
{
	this->score = score;
}

void Solution::updateScore(int delta)
{
	score = int(score) + delta;
}