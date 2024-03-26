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


Job Generateur::generateJob(unsigned int num, unsigned int infPi, unsigned int supPi, unsigned int infDi, unsigned int supDi, unsigned int infWi, unsigned int supWi) {

    // Generate random values using an uniform distribution
    std::uniform_int_distribution<> piDistribution(infPi, supPi);
    std::uniform_int_distribution<> diDistribution(infDi, supDi);
    std::uniform_int_distribution<> wiDistribution(infWi, supWi);
    unsigned int pi = piDistribution(numGenerator);
    unsigned int di = diDistribution(numGenerator);
    unsigned int wi = wiDistribution(numGenerator);

    return Job(pi+di*4, di, wi, num);  // We can change this with a better formula
}

Instance Generateur::generateInstance(std::string newInstancePath, unsigned int nbJobs, unsigned int nbOfHighSpeedMachines,
    unsigned int nbOfLowSpeedMachines, unsigned int highSpeed, unsigned int lowSpeed) {

    verbose.setRequiredLevel(1);
    verbose << "Random generation of an instance (N: " << nbJobs << " m_Max: " << nbOfHighSpeedMachines << " m_0: "<< nbOfLowSpeedMachines
        << " V_Max: " << highSpeed << " V_0: " << lowSpeed << ")...";

    Instance instance = Instance(newInstancePath);

    // set attributes
    instance.setNbJobs(nbJobs);
    instance.setNbOfHighSpeedMachines(nbOfHighSpeedMachines);
    instance.setNbOfLowSpeedMachines(nbOfLowSpeedMachines);
    instance.setHighSpeed(highSpeed);
    instance.setLowSpeed(lowSpeed);

    std::vector<Job> listJobs;

    // generate Jobs from 1 to <nbJobs>
    listJobs.reserve(nbJobs);
    for (unsigned int i = 1; i <= nbJobs; i++) {
        Job newJob = generateJob(i);
        listJobs.push_back(newJob);
    }

    instance.setListJobs(listJobs);

    verbose.setRequiredLevel(1);
    verbose << "Done\n";

    return instance;

}

