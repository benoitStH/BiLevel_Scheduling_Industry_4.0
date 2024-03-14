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

void Parser::saveInFile(std::string& filepath, const Instance& instance, const ISolver* solver, unsigned int optimal_objective)
{
    const std::string& instanceFile = instance.getInstancePath().lexically_normal().string();
    const Solution& solution = *(solver->getSolution());

    std::fstream fileStream(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    if (fileStream.is_open()) {

        // InstanceName, instanceFile, sum Cj, fonction objective (sum wjUj), precision avec optimale, description du solveur (regles, etc)
        // N, n, HighSpeedScheduling, LowSpeedScheduling
        fileStream << instance.getInstanceName() << ";"
            << instanceFile << ";"
            << solution.getSumCj() << ";"
            << solution.getSumWjUj() << ";"
            << optimal_objective << ";"
            << instance.getListJobs().size() << ";"
            << instance.getNbToSelectJob() << ";";

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

        // Nom heuristique
        fileStream << solver->getHeuristicName() << ";";

        // Description et parametres (regles utilisées)
        fileStream << solver->getHeuristicDescription() << ";";

        // Temps resolution en microsecondes
        fileStream << solver->getTimeResol()/1000000.0 <<";";

        // Precision avec l'optimale_objective
        if (optimal_objective == 0) { (solution.getSumWjUj() != 0 ? fileStream << "0;" : fileStream << "1;"); }
        else
        {
            float precision;

            if (solution.getSumWjUj() == 0) { fileStream << "1;"; }
            else 
            {

                precision = 1 - (solution.getSumWjUj() - optimal_objective) / float(solution.getSumWjUj());

                fileStream << precision << ";";
            }
        }
        fileStream << "\n";

    }
    else throw std::invalid_argument("Can't open the file " + filepath);
    fileStream.close();
}


