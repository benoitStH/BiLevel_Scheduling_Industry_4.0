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
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

Parser::Parser() {}

Instance Parser::readFromFile(std::string& filePath) const {
    Instance newInstance = Instance(filePath);
    std::fstream fileStream(newInstance.getInstancePath().lexically_normal(), std::fstream::in);
    std::string line; // new line
    unsigned int num_job = 0; // number of job generated
    // open and read the file
    if (fileStream.is_open()) {
        while (std::getline(fileStream, line)) {
            auto pos = line.find(":");
            // if we don't find the ':' then we read jobs
            if (pos == std::string::npos) {
                // read the job's data, the value should be separate with \t
                std::istringstream stream(line);
                unsigned int pi, di, wi;
                stream >> pi; stream >> di; stream >> wi;

                // create the job
                num_job++;
                Job newJob = Job(pi, di, wi, num_job);
                newInstance.add_job(newJob);
            }
            else {
                // we read so attributes
                std::string attribute = line.substr(0, pos);
                if (attribute == "name") newInstance.setInstanceName(line.substr(pos + 1));
                else if (attribute == "N") newInstance.setNbJobs(std::stoul(line.substr(pos + 1)));
                else if (attribute == "M_max") newInstance.setNbOfHighSpeedMachines(std::stoul(line.substr(pos + 1)));
                else if (attribute == "M_0") newInstance.setNbOfLowSpeedMachines(std::stoul(line.substr(pos + 1)));
                else if (attribute == "V_max") newInstance.setHighSpeed(std::stof(line.substr(pos + 1)));
                else if (attribute == "V_0") newInstance.setLowSpeed(std::stof(line.substr(pos + 1)));
            }
        };
    }
    else throw std::invalid_argument("Can't open the file " + newInstance.getInstancePath().lexically_normal().string());
    fileStream.close();
    return newInstance;
}

void Parser::serializeInstance(Instance& instance) {

    std::fstream fileStream(instance.getInstancePath().lexically_normal().string(), std::fstream::out);
    if (fileStream.is_open()) {
        fileStream << "name:" << instance.getInstanceName() << std::endl
            << "N:" << instance.getNbJobs() << std::endl
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


