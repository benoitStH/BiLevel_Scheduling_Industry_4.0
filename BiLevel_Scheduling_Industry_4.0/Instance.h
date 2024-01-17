//  Copyright (C) 2024
//  EA 6300 ERL CNRS 7002 Laboratoire d'Informatique Fondamentale et Appliquée de Tours, Tours, France
//
//  DIGEP, Politecnico di Torino, Corso Duca degli Abruzzi 24, Torino, Italy
//  This file is part of bilevel-scheduling.
//
//  bilevel-scheduling is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation, either version 3 of the License,
//  or (at your option) any later version.
//
//  bilevel-scheduling is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with bilevel-scheduling. If not, see <https://www.gnu.org/licenses/>.
//
//

#ifndef BILEVEL_SCHEDULING_INSTANCE_H
#define BILEVEL_SCHEDULING_INSTANCE_H

#include <string>
#include <filesystem>
#include "Job.h"
#include <vector>
#include <ostream>
#include <stdexcept>

class Instance {

private:

    std::string instanceName;
    std::filesystem::path instancePath; // the path to the instance
    unsigned int nbJobs; // the nb of job
    unsigned int nbOfHighSpeedMachines; // the nb of high speed machine
    unsigned int nbOfLowSpeedMachines; // the nb of high low machine
    float highSpeed;
    float lowSpeed;
    std::vector<Job> listJobs; // the list of jobs

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/

    /**
     * Default Constructor
     */
    Instance() : listJobs(std::vector<Job>()) {
        nbJobs = nbOfHighSpeedMachines = nbOfLowSpeedMachines = 0;
        highSpeed = lowSpeed = 0;
        instanceName = "";
        instancePath = "";
    }

    /**
     * Constructor by instance's path. It construct an instance with setting the attribute path. Moreover,
     * the method check if the path have a corresponding file, otherwise an exception is throw
     * @param newInstancePath the path to set
     */
    Instance(std::string& newInstancePath) : listJobs(std::vector<Job>()) {
        std::filesystem::path newPath = std::filesystem::path(newInstancePath);
        std::filesystem::exists(newPath) ? instancePath = newPath : throw std::invalid_argument("The path of the file is empty");
        instanceName = instancePath.stem().string();
    }

    /********************/
    /*      METHODS     */
    /********************/

    /**
     * Method that generate a instance from the given parameters
     * @param nbJobs the number of jobs to generate
     * @param nbOfHighSpeedMachines the number of high speed machines to create
     * @param nbOfHighLowMachines the number of low speed machines to create
     * @param highSpeed the high speed for machine
     * @param lowSpeed the low speed for machine
     */
    void generateInstance(unsigned int nbJobs, unsigned int nbOfHighSpeedMachines, unsigned int nbOfLowSpeedMachines, float highSpeed, float lowSpeed);

    /**
     * Method that add a new job to the list of jobs of this instance
     * @param newJob the job to add
     */
    void add_job(const Job& newJob) { listJobs.push_back(newJob); }

    /********************/
    /*      GETTER      */
    /********************/


    const std::filesystem::path& getInstancePath() const { return instancePath; }

    const std::string& getInstanceName() const { return instanceName; }

    unsigned int getNbJobs() const { return nbJobs; }

    unsigned int getNbOfHighSpeedMachines() const { return nbOfHighSpeedMachines; }

    unsigned int getNbOfLowSpeedMachines() const { return nbOfLowSpeedMachines; }

    float getHighSpeed() const { return highSpeed; }

    float getLowSpeed() const { return lowSpeed; }

    const std::vector<Job>& getListJobs() const { return listJobs; }


    /********************/
    /*      SETTER      */
    /********************/


    void setInstanceName(const std::string& instanceName) { Instance::instanceName = instanceName; }

    void setInstancePath(const std::filesystem::path& newInstancePath) { instancePath = newInstancePath; }

    void setNbJobs(unsigned int nbJobs) { Instance::nbJobs = nbJobs; }

    void setNbOfHighSpeedMachines(unsigned int nbOfHighSpeedMachines) { Instance::nbOfHighSpeedMachines = nbOfHighSpeedMachines; }

    void setNbOfLowSpeedMachines(unsigned int nbOfHighLowMachines) { Instance::nbOfLowSpeedMachines = nbOfHighLowMachines; }

    void setHighSpeed(float highSpeed) { Instance::highSpeed = highSpeed; }

    void setLowSpeed(float lowSpeed) { Instance::lowSpeed = lowSpeed; }

    void setListJobs(const std::vector<Job>& listJobs) { Instance::listJobs = listJobs; }

};

/************************/
/*      OPERATORS       */
/************************/

inline std::ostream& operator<<(std::ostream& os, const Instance& instance) {

    os << "Instance Path : " << instance.getInstancePath() << std::endl
        << "Instance name : " << instance.getInstanceName() << std::endl
        << "Number of jobs : " << instance.getNbJobs() << std::endl
        << "Number of high speed machines : " << instance.getNbOfHighSpeedMachines() << " with the speed of : " << instance.getHighSpeed() << std::endl
        << "Number of low speed machines : " << instance.getNbOfLowSpeedMachines() << " with the speed of : " << instance.getLowSpeed() << std::endl
        << "List of jobs : [" << std::endl;

    for (const Job& job : instance.getListJobs()) {
        os << job << std::endl;
    }
    os << "]";
    return os;
}

#endif //BILEVEL_SCHEDULING_INSTANCE_H
