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

    return Job(pi, di, wi, num);
}

Instance Generateur::generateInstance(std::string newInstancePath, unsigned int nbJobs, unsigned int nbOfHighSpeedMachines,
    unsigned int nbOfLowSpeedMachines, unsigned int highSpeed, unsigned int lowSpeed) {

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

    return instance;

}

Solution Generateur::generateInitialSolution(const Instance& instance)
{
    // Initialize the solution (with empty machines)
    Solution solution_initiale = Solution(&instance);

    // getting the sorted list of jobs
    instance.sort_by_LPT();
    std::vector<Job> listLPT_jobs = instance.getListJobs();

    // total number of machines
    unsigned int nbMachines = instance.getNbOfHighSpeedMachines() + instance.getNbOfLowSpeedMachines();

    // affectation weights of each machine which are each equal to (number of affected jobs + 1)/speed
    std::vector<float> affectationWeights = std::vector<float>(nbMachines, float(1));

    for (int i = 0; i < nbMachines; i++)
    {
        affectationWeights[i] = affectationWeights[i] / solution_initiale[i].getSpeed();
    }

    // machine whose affectation weight is the smallest (only the first occurence)
    unsigned int min_index;

    // for each job
    for (Job& job : listLPT_jobs)
    {
        // find the machine with smallest affectation weight
        min_index = std::distance(affectationWeights.begin(), std::min_element(affectationWeights.begin(), affectationWeights.end()));

        // inserting the job at the start of the machine's sequence
        solution_initiale[min_index].add_job(0, job);

        // updating the machine's affectation weight
        affectationWeights[min_index] = (solution_initiale[min_index].getAffectedJob().size() + 1) / solution_initiale[min_index].getSpeed();
    }

    // evaluate solution before returning it
    solution_initiale.evaluate();
    return solution_initiale;


}