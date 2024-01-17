#include "Generateur.h"


Generateur::Generateur()
{
	seed = time(nullptr);
	numGenerator = std::mt19937(seed);
}

Generateur::Generateur(unsigned int seed)
{
	this->seed = seed;
	numGenerator = std::mt19937(this->seed);
}

/**
     * Method that generates an instance with random values for pi, di, and wi.
     * These values come from a uniform distribution where the
     * lower bound and upper bound are defined by infPi and supPi for pi,
     * infDi and supDi for di, and infWi and supWi for wi.
     * @param infPi lower bound of the uniform distribution to generate pi
     * @param supPi upper bound of the uniform distribution to generate pi
     * @param infDi lower bound of the uniform distribution to generate di
     * @param supDi upper bound of the uniform distribution to generate di
     * @param infWi lower bound of the uniform distribution to generate wi
     * @param supWi upper bound of the uniform distribution to generate wi
     */
Job Generateur::generateJob(unsigned int infPi, unsigned int supPi, unsigned int infDi, unsigned int supDi, unsigned int infWi, unsigned int supWi) {

    std::uniform_int_distribution<> piDistribution(infPi, supPi);
    std::uniform_int_distribution<> diDistribution(infDi, supDi);
    std::uniform_int_distribution<> wiDistribution(infWi, supWi);
    unsigned int pi = piDistribution(numGenerator);
    unsigned int di = diDistribution(numGenerator);
    unsigned int wi = wiDistribution(numGenerator);

    return Job(pi, di, wi);
}

Instance Generateur::generateInstance(std::string newInstancePath, unsigned int nbJobs, unsigned int nbOfHighSpeedMachines,
    unsigned int nbOfLowSpeedMachines, float highSpeed, float lowSpeed) {

    Instance instance = Instance(newInstancePath);

    // set attributes
    instance.setNbJobs(nbJobs);
    instance.setNbOfHighSpeedMachines(nbOfHighSpeedMachines);
    instance.setNbOfLowSpeedMachines(nbOfLowSpeedMachines);
    instance.setHighSpeed(highSpeed);
    instance.setLowSpeed(lowSpeed);

    std::vector<Job> listJobs;

    // generate Jobs
    listJobs.reserve(nbJobs);
    for (unsigned int i = 0; i < nbJobs; i++) {
        Job newJob = generateJob();
        listJobs.push_back(newJob);
    }

    instance.setListJobs(listJobs);

    return instance;

}