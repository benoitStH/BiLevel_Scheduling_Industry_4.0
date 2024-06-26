﻿//  Copyright (C) 2024
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

#ifndef BILEVEL_SCHEDULING_SOLUTION_H
#define BILEVEL_SCHEDULING_SOLUTION_H

#include <vector>
#include <ostream>
#include "Instance.h"
#include "Machine.h"

class Solution {

private:

    std::vector<Machine> listHighSpeedMachines; // list of high speed machines
    std::vector<Machine> listLowSpeedMachines; // list of low speed machines
    float sum_wj_Uj{}; // the sum of the number weighted tardy jobs of the solution
    float sum_Cj{}; // the sum of the completion time of the solution
    Verbose verbose;

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/

    /**
     * Default Constructor
     */
    Solution() : sum_wj_Uj(0.0), sum_Cj(0.0) {};

    /**
     * Constructor of a solution from list of machines
     * @param listHighSpeedMachines the list of high speed machines
     * @param listLowSpeedMachines  the list of low speed machines
     */
    Solution(const std::vector<Machine>& listHighSpeedMachines, const std::vector<Machine>& listLowSpeedMachines)
        : listHighSpeedMachines(listHighSpeedMachines), listLowSpeedMachines(listLowSpeedMachines), sum_wj_Uj(0.0),
        sum_Cj(0.0) {}


    /**
     * Constructor of a solution that create the list of empty machines.
     * @param nbHighSpeedMachines the number of machine with high speed
     * @param nbLowSpeedMachines the number of machine with low speed
     */
    Solution(const Instance* instance) : sum_wj_Uj(0.0), sum_Cj(0.0) {
        listHighSpeedMachines = std::vector<Machine>(instance->getNbOfHighSpeedMachines(),
            Machine(instance->getHighSpeed()));
        listLowSpeedMachines = std::vector<Machine>(instance->getNbOfLowSpeedMachines(),
            Machine(instance->getLowSpeed()));
    }
    /********************/
    /*      METHODS     */
    /********************/

    /**
     * Method that reset the current solution
     */
    void reset() {
        // reset all machines
        for (Machine& machine : listHighSpeedMachines) {
            machine.reset();
        }
        for (Machine& machine : listLowSpeedMachines) {
            machine.reset();
        }
        sum_wj_Uj = 0.0;
        sum_Cj = 0.0;
    }

    /** DEPRECATED
     * Method that returns the machines where a swap can happen between their k-th job
     * @param k the k-th job in the machines' sequence
     * @param considerLowSpeedMachines Are we considering a swap between low speed machines ? otherwise we consider high speed ones
     * @return std::vector<unsigned int> of size 2. Contains the machines number. If no swap possible, the vector's size is 0.
     */
    std::vector<unsigned int> possibleSwapV(unsigned int k, bool considerLowSpeedMachines)
    {
        // The list of machines we consider
        std::vector<Machine>& listMachines = considerLowSpeedMachines ? listLowSpeedMachines : listHighSpeedMachines;

        // Two machines from the list
        unsigned int indexMachine1 = 0;
        unsigned int indexMachine2;

        float speed = listMachines[0].getSpeed();
        std::vector<unsigned int> swapableMachines;

        // For each machine of the list
        while (indexMachine1 < listMachines.size() - 1)
        {
            if (listMachines[indexMachine1].getAffectedJob().size() > k)
            {
                Job& job1 = listMachines[indexMachine1][k]; // get its k-th job

                indexMachine2 = indexMachine1;
                do
                {
                    // For each next machine
                    indexMachine2++;

                    if (listMachines[indexMachine2].getAffectedJob().size() > k)
                    {
                        Job& job2 = listMachines[indexMachine2][k]; // get its k-th job

                        // If job1 and job2 are late and job1 will be early after swapping 
                        if (job1.isLate())
                        {
                            if (job2.isLate() && (listMachines[indexMachine2].startTimeOfJob(k) + job1.getPi() / speed) < job1.getDi())
                            {
                                // These machines can swap their k-th job
                                swapableMachines.push_back(indexMachine1 + (considerLowSpeedMachines ? listHighSpeedMachines.size() : 0));
                                swapableMachines.push_back(indexMachine2 + (considerLowSpeedMachines ? listHighSpeedMachines.size() : 0));
                                return swapableMachines;
                            }
                        }
                    }

                } while (indexMachine2 < listMachines.size() - 1);

            }

            // If the swap isn't possible check with the next machine
            indexMachine1++;
        }

        // return an empty list
        return swapableMachines;
    }

    /** DEPRECATED
     * Method that returns the machines where a swap can happen between their k-th job
     * @param k the k-th job in the machines' sequence
     * @return std::vector<unsigned int> of size 2. Contains the machines number. If no swap possible, the vector's size is 0.
     */
    std::vector<unsigned int> possibleSwapVHighSpeed(unsigned int k) {
        return possibleSwapV(k, false);
    }
    
    /** DEPRECATED
     * Method that returns the machines where a swap can happen between their k-th job
     * @param k the k-th job in the machines' sequence
     * @return std::vector<unsigned int> of size 2. Contains the machines number. If no swap possible, the vector's size is 0.
     */
    std::vector<unsigned int> possibleSwapVLowSpeed(unsigned int k) {

        return possibleSwapV(k, true);
    }

    /**
     * Method that returns the size of the longest sequence in a high speed machine
     * @return An unsigned int representing the size of the longest sequence in a high speed machine
     */
    unsigned int getMaxNumberOfHighSpeedBlocs()
    {
        unsigned int nbBlocs = 0;

        for (Machine machine : listHighSpeedMachines)
        {
            if (machine.getAffectedJob().size() > nbBlocs)
            {
                nbBlocs = machine.getAffectedJob().size();
            }
        }

        return nbBlocs;
    }
    

    /**
     * Method that returns the size of the longest sequence in a low speed machine
     * @return An unsigned int representing the size of the longest sequence in a low speed machine
     */
    unsigned int getMaxNumberOfLowSpeedBlocs()
    {
        unsigned int nbBlocs = 0;

        for (Machine machine : listLowSpeedMachines)
        {
            if (machine.getAffectedJob().size() > nbBlocs)
            {
                nbBlocs = machine.getAffectedJob().size();
            }
        }

        return nbBlocs;
    }
    
    /**
     * Method which will swap the k-th job of two given machines
     * @param swapableMachines an unsigned int vector of size 2 with the machine's index in the solution
     * @param k the position of the k-th jobs
     * @param verbose (default=True)
     */
    void swapV(std::vector<unsigned int> swapableMachines, unsigned int k)
    {
        Machine& machine1 = operator[](swapableMachines[0]);
        Machine& machine2 = operator[](swapableMachines[1]);
        Job jobTemp = machine1[k];

        machine1[k] = machine2[k];
        machine2[k] = jobTemp;

        verbose.setRequiredLevel(3);
        verbose << "swapped job " << jobTemp.getNum() << " and " << machine1[k].getNum() <<
        " from machine " << swapableMachines[0] << " and " << swapableMachines[1] << " at bloc " << k << "\n";
        verbose.endRequiredLevel();
    }
    

    /**
     * Method that evaluate the solution by computing the sum Cj and the sum wjUj
     * The sumCj and sumWjUj will be updated
     */
    void evaluate() {

        sum_Cj = 0;
        sum_wj_Uj = 0;

        // compute the objective function for the high speed machines
        for (auto& machine : listHighSpeedMachines) {
            machine.evaluate();
            sum_Cj += machine.getSumCj();
            sum_wj_Uj += machine.getSumWjUj();
        }

        // compute the objective function for the low speed machines
        for (auto& machine : listLowSpeedMachines) {
            machine.evaluate();
            sum_Cj += machine.getSumCj();
            sum_wj_Uj += machine.getSumWjUj();
        }

    }

    /**
     * Method that output the data of the solution in a custom form
     * The job number is displayed
     * The processing time are divided by the machine's speed
     * Late jobs are marked with a "*" before their number
     * The sum Cj and sum WjUj are displayed
     */
    void completeVerbose()
    {
        verbose << "Solution :\n";

        // Jobs assigned to high speed machines
        float HighSpeed = listHighSpeedMachines[0].getSpeed();
        verbose << "High Speed : " << HighSpeed << "\n";
        for (Machine& machine : listHighSpeedMachines)
        {
            verbose << "[";
            for (const Job& job : machine.getAffectedJob())
            {
                verbose << (job.isLate() ? "*" : "");
                verbose << job.getNum() << "|p " << job.getPi() / HighSpeed << " d " << job.getDi() << " w " << job.getWi() << "| ";
            }
            verbose << "]\n";
        }
        verbose << "- - -\n";

        // Jobs assigned to low speed machines
        float LowSpeed = listLowSpeedMachines[0].getSpeed();
        verbose << "Low Speed : " << LowSpeed << "\n";
        for (Machine& machine : listLowSpeedMachines)
        {
            verbose << "[";
            for (const Job& job : machine.getAffectedJob())
            {
                verbose << (job.isLate() ? "*" : "");
                verbose << job.getNum() << "|p " << job.getPi() / LowSpeed << " d " << job.getDi() << " w " << job.getWi() << "| ";
            }
            verbose << "]\n";
        }
        verbose << "- - -\n";

        // the solution's score
        verbose << "Sum Cj : " << getSumCj() << "\n";
        verbose << "Sum wjUj : " << getSumWjUj() << "\n";
    }

    /**
     * Method that output the data of the solution in a compact form
     * Only the job's number are displayed
     * Late jobs are marked with a "*"
     * The sum Cj and sum WjUj are displayed
     * The number of selected jobs in the solution (ignoring '0' ones) is displayed
     */
    void compactVerbose()
    {
        unsigned int nbSelectedJobs = 0;
        verbose << "Solution :\n";

        // Jobs assigned to high speed machines
        float HighSpeed = listHighSpeedMachines[0].getSpeed();
        for (Machine& machine : listHighSpeedMachines)
        {
            verbose << "[" << HighSpeed << "]-|";
            for (const Job& job : machine.getAffectedJob())
            {
                verbose << (job.isLate() ? "*" : "");
                verbose << job.getNum() << " ";
                if (job.getNum() != 0) nbSelectedJobs++;
            }
            verbose << "|\n";
        }

        // Jobs assigned to low speed machines
        float LowSpeed = listLowSpeedMachines[0].getSpeed();
        for (Machine& machine : listLowSpeedMachines)
        {
            verbose << "[" << LowSpeed << "]-|";
            for (const Job& job : machine.getAffectedJob())
            {
                verbose << (job.isLate() ? "*" : "");
                verbose << job.getNum() << " ";
                if (job.getNum() != 0) nbSelectedJobs++;
            }
            verbose << "|\n";
        }
        verbose << "- - -\n";

        // Solution's score and number of selected jobs
        verbose << "Sum Cj : " << getSumCj() << "\n";
        verbose << "Sum wjUj : " << getSumWjUj() << "\n";
        verbose << "n : " << nbSelectedJobs << "\n";
    }

    /********************/
    /*      GETTER      */
    /********************/

    const std::vector<Machine>& getListHighSpeedMachines() const { return listHighSpeedMachines; }

    const std::vector<Machine>& getListLowSpeedMachines() const { return listLowSpeedMachines; }

    float getSumWjUj() const { return sum_wj_Uj; }

    float getSumCj() const { return sum_Cj; }

    /**
     * Returns a reference to the element at specified location pos.
     * If pos is greater than the number of high speed machine then the reference is from the list of low speed machine
     * Else the reference is from the list of high speed machine. No bounds checking is performed.
     */
    Machine getMachine(unsigned int pos) const { return pos < listHighSpeedMachines.size() ? listHighSpeedMachines[pos] : listLowSpeedMachines[pos - listHighSpeedMachines.size()]; }


    /********************/
    /*      SETTER      */
    /********************/

    void setListHighSpeedMachines(const std::vector<Machine>& listHighSpeedMachines) { Solution::listHighSpeedMachines = listHighSpeedMachines; }

    void setListLowSpeedMachines(const std::vector<Machine>& listLowSpeedMachines) { Solution::listLowSpeedMachines = listLowSpeedMachines; }

    void setSumWjUj(float sum_wj_Uj) { Solution::sum_wj_Uj = sum_wj_Uj; }

    void setSumCj(float sumCj) { sum_Cj = sumCj; }


    /************************/
    /*      OPERATORS       */
    /************************/

    /**
     * Returns a reference to the element at specified location pos.
     * If pos is greater than the number of high speed machine then the reference is from the list of low speed machine
     * Else the reference is from the list of high speed machine. No bounds checking is performed.
     */
    Machine& operator[](size_t pos) { return pos < listHighSpeedMachines.size() ? listHighSpeedMachines[pos] : listLowSpeedMachines[pos - listHighSpeedMachines.size()]; }

    /*
    * Operator to compare this object with another Solution object
    * We first compare the sum Cj and if both have the same value, compare the sum wjUj
    */
    bool operator<(const Solution& sol) const { return ((sum_Cj == sol.sum_Cj) ? (sum_wj_Uj < sol.sum_wj_Uj) : (sum_Cj < sol.sum_Cj)); }

    /*
    * Static method which only compare the sum wj Uj
    */
    static bool smaller_wjUj(const Solution& lsol, const Solution& rsol) { return (lsol.sum_wj_Uj < rsol.sum_wj_Uj); }
};



inline std::ostream& operator<<(std::ostream& os, const Solution& solution) {
    os << "Solution : " << std::endl
        << "high speed machines : [" << std::endl;
    for (const Machine& machine : solution.getListHighSpeedMachines()) {
        os << machine << std::endl;
    }
    os << "]" << std::endl
        << "low speed machines : [" << std::endl;
    for (const Machine& machine : solution.getListLowSpeedMachines()) {
        os << machine << std::endl;
    }
    os << "]" << std::endl << "Sum Cj : " << solution.getSumCj() << std::endl << "Sum wjUj : " << solution.getSumWjUj() << std::endl;
    return os;

}

#endif //BILEVEL_SCHEDULING_SOLUTION_H
