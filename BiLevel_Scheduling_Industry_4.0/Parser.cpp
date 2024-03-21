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
// Created by schau on 1/15/24.
//

#include "Parser.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>

Parser::Parser() {}


/**
 * This Method parse a file to generate an instance. It's an Instance constructor
 * The file must respect the intended format (cf ReadMe.md)
 * @param filePath the path of the file to parse
 * @return the new instance
*/
Instance Parser::readFromFile(std::string& filePath) const {
    Instance newInstance = Instance(filePath);
    std::fstream fileStream(newInstance.getInstancePath().lexically_normal(), std::fstream::in);
    std::string line; // new line
    unsigned int num = 1;
    // open and read the file
    if (fileStream.is_open()) {
        while (std::getline(fileStream, line)) {
            auto pos = line.find(":");
            // if we don't find the ':' then we read jobs
            if (pos == std::string::npos) {
                // create the job, the value should be separate with \t
                std::istringstream stream(line);
                unsigned int pi, di, wi;
                stream >> pi; stream >> di; stream >> wi;
                Job newJob = Job(pi, di, wi, num);
                num++;
                newInstance.add_job(newJob);
            }
            else {
                // we read so attributes
                std::string attribute = line.substr(0, pos);
                if (attribute == "name") newInstance.setInstanceName(line.substr(pos + 1));
                else if (attribute == "N") newInstance.setNbJobs(std::stoul(line.substr(pos + 1)));
                else if (attribute == "n") newInstance.setNbToSelectJob(std::stoul(line.substr(pos + 1)));
                else if (attribute == "M_max") newInstance.setNbOfHighSpeedMachines(std::stoul(line.substr(pos + 1)));
                else if (attribute == "M_0") newInstance.setNbOfLowSpeedMachines(std::stoul(line.substr(pos + 1)));
                else if (attribute == "V_max") newInstance.setHighSpeed(std::stoul(line.substr(pos + 1)));
                else if (attribute == "V_0") newInstance.setLowSpeed(std::stoul(line.substr(pos + 1)));
                else if (attribute != "Jobs") std::cout << "The file " + newInstance.getInstancePath().lexically_normal().string() + " format is incorrect\n";
            }
        };
    }
    else throw std::invalid_argument("Can't open the file " + newInstance.getInstancePath().lexically_normal().string());
    fileStream.close();

    // check if we have the right number of created job
    if (newInstance.getNbJobs() != newInstance.getListJobs().size()) throw std::invalid_argument("The number of jobs is not equals to N");
    if (newInstance.getNbToSelectJob() == 0) throw std::invalid_argument("The number of jobs to select is not defined");
    return newInstance;
}

/**
* Method that serialize an instance into the corresponding file given by the attributeInstance::instancePath.
* The file contains the instance's data in a specific format (cf ReadMe.md)
* @param instance to serialize
*/
void Parser::serializeInstance(Instance& instance) {

    std::fstream fileStream(instance.getInstancePath().lexically_normal().string(), std::fstream::out);
    if (fileStream.is_open()) {
        fileStream << "name:" << instance.getInstanceName() << std::endl
            << "N:" << instance.getNbJobs() << std::endl
            << "n:" << instance.getNbJobs() << std::endl
            << "M_max:" << instance.getNbOfHighSpeedMachines() << std::endl
            << "M_0:" << instance.getNbOfLowSpeedMachines() << std::endl
            << "V_max:" << instance.getHighSpeed() << std::endl
            << "V_0:" << instance.getLowSpeed() << std::endl
            << "Jobs:" << std::endl;

        for (const Job& job : instance.getListJobs()) {
            fileStream << job.getPi() << "\t" << job.getDi() << "\t" << job.getWi() << "\t" << std::endl;
        }
    }
    else throw std::invalid_argument("Can't open the file " + instance.getInstancePath().lexically_normal().string());
    fileStream.close();
}

/*
* Method which save the given instance's best solution found by the given solver
* The result is appended into the given file with a specific format (cf ReadMe.md)
* 
* It is assumed that the solver already solved the instance and has a solution (optimal or not)
* It is assumed that the file, whose path was given, already exist and contains the header's
* 
* @param filePath The file where data will be append
* @param instance The instance solved by the solver
* @param solver The solver which solved the instance, contains the solution and its resolution time
* @param optimal_objective The sum wjUj found by an exact method. Used to compute derivation.
*/
void Parser::saveInFile(std::string& filepath, const Instance& instance, const ISolver* solver, unsigned int optimal_objective)
{
    // Getting the instance's file path and the solver's best solution
    const std::string& instanceFile = instance.getInstancePath().lexically_normal().string();
    const Solution& solution = *(solver->getSolution());

    // Opening the file to append data
    std::fstream fileStream(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    if (fileStream.is_open()) {

        // InstanceName, instanceFile, sum Cj, fonction objective (sum wjUj), objective fonction value to compare with
        // N, n, number of machines
        fileStream << instance.getInstanceName() << ";"
            << instanceFile << ";"
            << solution.getSumCj() << ";"
            << solution.getSumWjUj() << ";"
            << optimal_objective << ";"
            << instance.getListJobs().size() << ";"
            << instance.getNbToSelectJob() << ";"
            << instance.getNbOfHighSpeedMachines() + instance.getNbOfLowSpeedMachines() <<";";

        // HighSpeed Scheduling
        fileStream << "\"";
        for (unsigned int l = 0; l < instance.getNbOfHighSpeedMachines(); l++)
        {
            unsigned int j;
            unsigned int nbJobs = solution.getMachine(l).getAffectedJob().size();
            fileStream << "M" << l << " : ";
            for (j = 0; j < nbJobs; j++)
            {
                fileStream << solution.getMachine(l)[j].getNum() << (j < nbJobs - 1 ? ", " : "");
            }
            fileStream << " | ";
        }
        fileStream << "\";";

        // LowSpeed Scheduling
        fileStream << "\"";
        for (unsigned int l = 0; l < instance.getNbOfLowSpeedMachines(); l++)
        {
            unsigned int pos = l + instance.getNbOfHighSpeedMachines();
            unsigned int j;
            unsigned int nbJobs = solution.getMachine(pos).getAffectedJob().size();
            fileStream << "M" << l << " : ";
            for (j = 0; j < solution.getMachine(pos).getAffectedJob().size(); j++)
            {
                fileStream << solution.getMachine(pos)[j].getNum() << (j < nbJobs - 1 ? ", " : "");
            }
            fileStream << " | ";
        }
        fileStream << "\";";

        // Heuristic's name
        fileStream << solver->getHeuristicName() << ";";

        // Description and paramters (rules used)
        fileStream << solver->getHeuristicDescription() << ";";

        // Time in microseconds to solve the instance saved as time in seconds
        fileStream << solver->getTimeResol()/1000000.0 <<";";

        // Deviation from the optimale_objective
        if (optimal_objective == 0) { fileStream << "-1;"; }
        else
        {
            float deviation = (solution.getSumWjUj() - optimal_objective) / (float)optimal_objective;

            fileStream << deviation * 100 << ";";
        }

        // Accuracy toward the optimale_objective
        if (optimal_objective == 0) { (solution.getSumWjUj() != 0 ? fileStream << "0;" : fileStream << "1;"); }
        else
        {
            float precision;

            if (solution.getSumWjUj() == 0) { fileStream << "1;"; }
            else 
            {

                precision = optimal_objective / float(solution.getSumWjUj());

                fileStream << precision << "";
            }
        }
        fileStream << "\n";

    }
    else throw std::invalid_argument("Can't open the file " + filepath);
    fileStream.close();
}


