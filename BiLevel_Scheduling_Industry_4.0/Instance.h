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
#include <algorithm>

class Instance {

private:

    std::string instanceName;
    std::filesystem::path instancePath; // the path to the instance
    unsigned int nbJobs; // the nb of job
    unsigned int nbToSelectJob; // the number of jobs to select
    unsigned int nbOfHighSpeedMachines; // the nb of high speed machine
    unsigned int nbOfLowSpeedMachines; // the nb of high low machine
    unsigned int highSpeed;  // speed of high speed machines
    unsigned int lowSpeed;   // speed of low speed machines
    std::vector<Job> listJobs; // the list of jobs

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/

    /**
     * Default Constructor
     */
    Instance() : listJobs(std::vector<Job>()) {}

    /**
     * Constructor by instance's path. It construct an instance with setting the attribute path.
     * The instance name is extracted from the path and set to the instanceName attribute.
     * @param newInstancePath the path to set
     */
    Instance(std::string &newInstancePath) : listJobs(std::vector<Job>()) {
        std::filesystem::path newPath = std::filesystem::path(newInstancePath);
        instancePath = newPath ;
        instanceName = instancePath.stem().string();
    }

    /********************/
    /*      METHODS     */
    /********************/

    /**
     * Method that add a new job to the list of jobs of this instance
     * @param newJob the job to add
     */
    void add_job(const Job& newJob){listJobs.push_back(newJob);}

    /**
     * Sort job according to the Longest Processing Time rule. If processing times are equals then sort according to increasing due date.
     */
    void sort_by_LPT(){std::sort(listJobs.begin(),listJobs.end(),std::greater<Job>());}

    /**
     * Sort job according to the Longest Processing Time rule. If processing times are equals then sort according to decreasing due date.
     */
    void sort_by_LPT_inv_EDD(){std::sort(listJobs.begin(),listJobs.end(),Job::LPT_inv_EDD);}

    /********************/
    /*      GETTER      */
    /********************/


    const std::filesystem::path &getInstancePath() const {return instancePath;}

    const std::string &getInstanceName() const {return instanceName;}

    unsigned int getNbJobs() const {return nbJobs;}

    unsigned int getNbToSelectJob() const {return nbToSelectJob;}

    unsigned int getNbOfHighSpeedMachines() const {return nbOfHighSpeedMachines;}

    unsigned int getNbOfLowSpeedMachines() const {return nbOfLowSpeedMachines;}

    float getHighSpeed() const {return highSpeed;}

    float getLowSpeed() const {return lowSpeed;}

    const std::vector<Job> &getListJobs() const {return listJobs;}


    /********************/
    /*      SETTER      */
    /********************/


    void setInstanceName(const std::string &instanceName){Instance::instanceName = instanceName;}

    void setInstancePath(const std::filesystem::path &newInstancePath) {instancePath = newInstancePath;}

    void setNbJobs(unsigned int nbJobs) {Instance::nbJobs = nbJobs;}

    void setNbToSelectJob(unsigned int nbToSelectJob) {Instance::nbToSelectJob = nbToSelectJob;}

    void setNbOfHighSpeedMachines(unsigned int nbOfHighSpeedMachines) {Instance::nbOfHighSpeedMachines = nbOfHighSpeedMachines;}

    void setNbOfLowSpeedMachines(unsigned int nbOfHighLowMachines) {Instance::nbOfLowSpeedMachines = nbOfHighLowMachines;}

    void setHighSpeed(unsigned int highSpeed) {Instance::highSpeed = highSpeed;}

    void setLowSpeed(unsigned int lowSpeed) {Instance::lowSpeed = lowSpeed;}

    void setListJobs(const std::vector<Job> &listJobs) {Instance::listJobs = listJobs;}


};

/************************/
/*      OPERATORS       */
/************************/

inline std::ostream &operator<<(std::ostream &os, const Instance &instance) {

    os  << "Instance Path : " << instance.getInstancePath() << std::endl
        << "Instance name : "<< instance.getInstanceName() << std::endl
        << "Number of jobs to select / Total number of jobs : " << instance.getNbToSelectJob() << "/" << instance.getNbJobs() << std::endl
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
