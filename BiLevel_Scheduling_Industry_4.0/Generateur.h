#ifndef BILEVEL_SCHEDULING_GENERATEUR_H
#define BILEVEL_SCHEDULING_GENERATEUR_H

#include <random>
#include <iostream>
#include <time.h>
#include "Instance.h"
#include "Solution.h"

class Generateur
{
private:
    // the seed use for generate instance
    unsigned int seed;
    std::mt19937 numGenerator;

public:
    Generateur();

    Generateur(unsigned int seed);

    /**
     * Method that generate a job from the given parameters
     * @param num the job's number
     * @param infPi the smallest processing time possible
     * @param supPi the largest processing time possible
     * @param infDi the earliest due date possible
     * @param supDi the latest due date possible
     * @param infWi the smallest weight possible
     * @param supWi the largest weight possible
     */
    Job generateJob(unsigned int num, unsigned int infPi = 1, unsigned int supPi = 10, unsigned int infDi = 1, unsigned int supDi = 10, unsigned int infWi = 1, unsigned int supWi = 20);

    /**
     * Method that generate a instance from the given parameters
     * @param nbJobs the number of jobs to generate
     * @param nbOfHighSpeedMachines the number of high speed machines to create
     * @param nbOfHighLowMachines the number of low speed machines to create
     * @param highSpeed the high speed for machine
     * @param lowSpeed the low speed for machine
     */
    Instance generateInstance(std::string newInstancePath, unsigned int nbJobs, unsigned int nbOfHighSpeedMachines, unsigned int nbOfLowSpeedMachines, unsigned int highSpeed, unsigned int lowSpeed);

    /**
     * Method that generate an initial solution from a given instance
     * @param nbJobs the number of jobs to generate
     * @return an initial solution which minimize the sum of completion times
     */
    Solution generateInitialSolution(const Instance& instance);

};

#endif //BILEVEL_SCHEDULING_GENERATEUR_H
