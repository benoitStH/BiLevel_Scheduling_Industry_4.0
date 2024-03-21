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

#ifndef BILEVEL_SCHEDULING_JOB_H
#define BILEVEL_SCHEDULING_JOB_H

#include <random>
#include <iostream>
class Job {

private:
    unsigned int pi;  // job's processing time for a machine whose speed equals 1
    unsigned int di;  // job's deadline
    unsigned int wi;  // job's weight
    unsigned int num; // job number (0 means its a ghost job)
    bool late;  // is the job late ? (always initialized to false)

public:

    /* Generate a 'ghost' job with all attributes equal to 0*/
    Job() : pi(0), di(0), wi(0), num(0), late(false) {};
    Job(unsigned int pi, unsigned int di, unsigned int wi, unsigned int num) : pi(pi), di(di), wi(wi), num(num), late(false) {}

    /********************/
    /*      GETTER      */
    /********************/

    /**
    * Processing time with a machine whose speed is 1
    */
    unsigned int getPi() const { return pi; }

    unsigned int getDi() const { return di; }

    unsigned int getWi() const { return wi; }

    unsigned int getNum() const { return num; }

    bool isLate() const { return late; }

    /********************/
    /*      SETTER      */
    /********************/

    void setLate(bool isLate) { late = isLate; }

    /************************/
    /*      OPERATORS       */
    /************************/

    static bool LPT_inv_EDD(const Job& lhs, const Job& rhs) { return (lhs.pi == rhs.pi) ? (lhs.di > rhs.di) : (lhs.pi > rhs.pi); }

    static bool LWPT(const Job& lhs, const Job& rhs) {

        if (lhs.wi == 0)
        {
            return false;
        }

        if (rhs.wi == 0)
        {
            return true;
        }

        return ((lhs.pi / float(lhs.wi)) < (rhs.pi / float(rhs.wi)));
    }

    static bool EDD(const Job& lhs, const Job& rhs) { return (lhs.di == rhs.di ? (lhs.pi < rhs.pi) : (lhs.di < rhs.di)); }

    static bool SmallestWeight(const Job& lhs, const Job& rhs) { return (lhs.wi < rhs.wi); }

    bool operator==(const Job& J) const { return pi == J.pi && di == J.di && wi == J.wi; }

    bool operator!=(const Job& J) const { return !(J == *this); }

    /* Inequalities Operators compare processing times and due date in case of equalities */
    bool operator<(const Job& J) const { return (pi == J.pi) ? (di < J.di) : (pi < J.pi); }

    bool operator>(const Job& J) const { return (pi == J.pi) ? (di > J.di) : (pi > J.pi); }

    bool operator<=(const Job& J) const { return (pi == J.pi) ? (di <= J.di) : (pi <= J.pi); }

    bool operator>=(const Job& J) const { return (pi == J.pi) ? (di >= J.di) : (pi >= J.pi); }

};

inline std::ostream& operator<<(std::ostream& os, const Job& job) {
    os << "| pi: " << job.getPi() << " di: " << job.getDi() << " wi: " << job.getWi() << (job.isLate() ? "*" : "") << "|";
    return os;
}

#endif //BILEVEL_SCHEDULING_JOB_H
