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

    /**
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

    /**
     * Method that returns the machines where a swap can happen between their k-th job
     * @param k the k-th job in the machines' sequence
     * @return std::vector<unsigned int> of size 2. Contains the machines number. If no swap possible, the vector's size is 0.
     */
    std::vector<unsigned int> possibleSwapVHighSpeed(unsigned int k) {
        return possibleSwapV(k, false);
    }
    
    /**
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

        for (Machine machine : listHighSpeedMachines)
        {
            if (machine.getAffectedJob().size() > nbBlocs)
            {
                nbBlocs = machine.getAffectedJob().size();
            }
        }

        return nbBlocs;
    }
    

    void swapV(std::vector<unsigned int> swapableMachines, unsigned int k)
    {
        Machine& machine1 = operator[](swapableMachines[0]);
        Machine& machine2 = operator[](swapableMachines[1]);
        Job jobTemp = machine1[k];

        machine1[k] = machine2[k];
        machine2[k] = jobTemp;

        std::cout << "swapped job " << jobTemp.getNum() << " and " << machine1[k].getNum() <<
            " from machine " << swapableMachines[0] << " and " << swapableMachines[1] << " at bloc " << k << std::endl;
    }
    

    /**
     * Method that evaluate the solution
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
     * Late jobs are marked with a "*"
     */
    void print()
    {
        std::cout << "Solution :\n";

        // Jobs assigned to high speed machines
        float HighSpeed = listHighSpeedMachines[0].getSpeed();
        std::cout << "High Speed : " << HighSpeed << std::endl;
        for (Machine& machine : listHighSpeedMachines)
        {
            std::cout << "[";
            for (const Job& job : machine.getAffectedJob())
            {
                std::cout << (job.isLate() ? "*" : "");
                std::cout << job.getNum() << "|p " << job.getPi() / HighSpeed << " d " << job.getDi() << " w " << job.getWi() << "| ";
            }
            std::cout << "]\n";
        }
        std::cout << "- - -\n";

        // Jobs assigned to low speed machines
        float LowSpeed = listLowSpeedMachines[0].getSpeed();
        std::cout << "Low Speed : " << LowSpeed << std::endl;
        for (Machine& machine : listLowSpeedMachines)
        {
            std::cout << "[";
            for (const Job& job : machine.getAffectedJob())
            {
                std::cout << (job.isLate() ? "*" : "");
                std::cout << job.getNum() << "|p " << job.getPi() / LowSpeed << " d " << job.getDi() << " w " << job.getWi() << "| ";
            }
            std::cout << "]\n";
        }
        std::cout << "- - -\n";

        // the solution's score
        std::cout << "Sum Cj :" << getSumCj() << std::endl;
        std::cout << "Sum wjUj : " << getSumWjUj() << std::endl;
    }

    /**
     * Method that output the data of the solution in a compact form
     * Only the job's number are displayed
     * Late jobs are marked with a "*"
     */
    void compactPrint()
    {
        std::cout << "Solution :\n";

        // Jobs assigned to high speed machines
        float HighSpeed = listHighSpeedMachines[0].getSpeed();
        for (Machine& machine : listHighSpeedMachines)
        {
            std::cout << "[" << HighSpeed << "]-|";
            for (const Job& job : machine.getAffectedJob())
            {
                std::cout << (job.isLate() ? "*" : "");
                std::cout << job.getNum() << " ";
            }
            std::cout << "|\n";
        }

        // Jobs assigned to low speed machines
        float LowSpeed = listLowSpeedMachines[0].getSpeed();
        for (Machine& machine : listLowSpeedMachines)
        {
            std::cout << "[" << LowSpeed << "]-|";
            for (const Job& job : machine.getAffectedJob())
            {
                std::cout << (job.isLate() ? "*" : "");
                std::cout << job.getNum() << " ";
            }
            std::cout << "|\n";
        }
        std::cout << "- - -\n";

        std::cout << "Sum Cj :" << getSumCj() << std::endl;
        std::cout << "Sum wjUj : " << getSumWjUj() << std::endl;
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
    * We only compare the sum wj Uj and assume the sum Cj are equal
    */
    bool operator<(const Solution& sol) const { return (sum_wj_Uj < sol.sum_wj_Uj); }
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
