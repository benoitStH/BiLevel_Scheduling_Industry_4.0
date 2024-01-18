#pragma once

#include "Machine.h"

class Solution
{

private:
	std::vector<Machine> HighSpeedMachines;
	std::vector<Machine> LowSpeedMachines;

	unsigned int score;

public:
	Solution();

	Solution(std::vector<Machine>& HighSpeedMachines, std::vector<Machine>& LowSpeedMachines, unsigned int score);

	unsigned int getScore();

	void setScore(unsigned int);

	void updateScore(int);

};

