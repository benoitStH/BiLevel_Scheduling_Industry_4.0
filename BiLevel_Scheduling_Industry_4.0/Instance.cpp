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

#include "Instance.h"

void Instance::generateInstance(unsigned int nbJobs, unsigned int nbOfHighSpeedMachines,
    unsigned int nbOfLowSpeedMachines, float highSpeed, float lowSpeed) {

    // set attributes
    setNbJobs(nbJobs);
    setNbOfHighSpeedMachines(nbOfHighSpeedMachines);
    setNbOfLowSpeedMachines(nbOfLowSpeedMachines);
    setHighSpeed(highSpeed);
    setLowSpeed(lowSpeed);

    // generate Jobs
    listJobs.reserve(nbJobs);
    for (unsigned int i = 0; i < nbJobs; i++) {
        Job newJob = Job();
        newJob.generate();
        listJobs.push_back(newJob);
    }

}




