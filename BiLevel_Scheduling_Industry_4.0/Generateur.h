#pragma once

#include <random>
#include <iostream>
#include <time.h>
#include "Instance.h"

class Generateur
{
private:
    // the seed use for generate instance
    unsigned int seed;
    std::mt19937 numGenerator;

public:
    Generateur();

    Generateur(unsigned int seed);

    Job generateJob(unsigned int infPi = 1, unsigned int supPi = 10, unsigned int infDi = 1, unsigned int supDi = 10, unsigned int infWi = 1, unsigned int supWi = 20);

    /**
     * Method that generate a instance from the given parameters
     * @param nbJobs the number of jobs to generate
     * @param nbOfHighSpeedMachines the number of high speed machines to create
     * @param nbOfHighLowMachines the number of low speed machines to create
     * @param highSpeed the high speed for machine
     * @param lowSpeed the low speed for machine
     */
    Instance generateInstance(std::string newInstancePath, unsigned int nbJobs, unsigned int nbOfHighSpeedMachines, unsigned int nbOfLowSpeedMachines, float highSpeed, float lowSpeed);

};

