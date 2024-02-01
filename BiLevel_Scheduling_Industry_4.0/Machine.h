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

#ifndef BILEVEL_SCHEDULING_MACHINE_H
#define BILEVEL_SCHEDULING_MACHINE_H

#include <vector>
#include <ostream>
#include "Job.h"

class Machine {

private:
    std::vector<Job> listAffectedJobs; // the list of jobs that be processed on the machine
    float speed; // the speed of the machine
    float sum_wj_Uj; // the sum of the number weighted tardy jobs on the machine
    float sum_Cj; // the sum of the completion time on the machine

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/

    Machine() : listAffectedJobs(std::vector<Job>()), speed(0.0), sum_wj_Uj(0.0), sum_Cj(0.0) {}

    /**
     * Constructor a machine with the speed.
     * @param speed the speed of the current machine
     */
    Machine(float speed) : listAffectedJobs(std::vector<Job>()), speed(speed), sum_wj_Uj(0.0), sum_Cj(0.0) {}

    /********************/
    /*      METHODS     */
    /********************/

    /**
     * Method that reset the machine
     */
    void reset() { listAffectedJobs.clear(); sum_wj_Uj = 0.0; sum_Cj = 0.0; }

    /**
     * Method that return the starting time of a given job
     * @param j the j-th job in the machine's sequence.
     * @return float The starting time of the job
     */
    float startTimeOfJob(unsigned int j) const { 
        if (j == 0) { return 0; }
        
        return startTimeOfJob(j - 1) + listAffectedJobs[j].getPi() / speed;
    }

    /**
     * Method that evaluate the scheduling on the machine. It compute the sum of completion times and the sum of weighted tardy jobs.
     */
    void evaluate() {
        float cumulativeWeightedTardyJob = 0.0; float cumulativeCompletionTimes = 0.0; float completionTimes = 0.0;
        size_t nbJobs = listAffectedJobs.size();
        for (size_t i = 0; i < nbJobs; i++) {
            // compute cumulative completion times, a job at position i count (nbJobs-i) times
            cumulativeCompletionTimes += (nbJobs - i) * listAffectedJobs[i].getPi() / speed;
            completionTimes += listAffectedJobs[i].getPi() / speed;

            // the job is late if the completion time is past its due date
            listAffectedJobs[i].setLate((listAffectedJobs[i].getDi() < completionTimes));
            cumulativeWeightedTardyJob += listAffectedJobs[i].isLate() ? listAffectedJobs[i].getWi() : 0.0;
        }
        sum_Cj = cumulativeCompletionTimes;
        sum_wj_Uj = cumulativeWeightedTardyJob;
    }

    /**
     * Method that add a job to be scheduled on machine. This job is added at the given position.
     * @param pos iterator before which the content will be inserted
     * @param affectedJob job to insert
     */
    void add_job(unsigned int position, const Job& affectedJob) { listAffectedJobs.insert(listAffectedJobs.begin() + position, affectedJob); }

    /**
     * Method that add a job to be scheduled on machine. This job is added at the end of the machine.
     * @param affectedJob job to insert
     */
    void add_job(const Job& affectedJob) { listAffectedJobs.push_back(affectedJob); }


    /********************/
    /*      GETTER      */
    /********************/

    const std::vector<Job>& getAffectedJob() const { return listAffectedJobs; }

    float getSpeed() const { return speed; }

    float getSumWjUj() const { return sum_wj_Uj; }

    float getSumCj() const { return sum_Cj; }

    /********************/
    /*      SETTER      */
    /********************/


    void setAffectedJob(const std::vector<Job>& affectedJob) { Machine::listAffectedJobs = affectedJob; }

    void setSpeed(float speed) { Machine::speed = speed; }

    void setSumWjUj(float sum_wjUj) { sum_wj_Uj = sum_wjUj; }

    void setSumCj(float sumCj) { sum_Cj = sumCj; }

    /************************/
    /*      OPERATORS       */
    /************************/

    Job& operator[](size_t pos) { return listAffectedJobs[pos]; }

};


inline std::ostream& operator<<(std::ostream& os, const Machine& machine) {
    os << "[";
    for (const Job& job : machine.getAffectedJob()) {
        os << job;
    }
    os << "]";
    return os;
}

#endif //BILEVEL_SCHEDULING_MACHINE_H
