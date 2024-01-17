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
    unsigned int pi;
    unsigned int di;
    unsigned int wi;

public:

    Job() : pi(0), di(0), wi(0) {};
    Job(unsigned int pi, unsigned int di, unsigned int wi) : pi(pi), di(di), wi(wi) {}

    /********************/
    /*      GETTER      */
    /********************/

    unsigned int getPi() const { return pi; }

    unsigned int getDi() const { return di; }

    unsigned int getWi() const { return wi; }

    /********************/
    /*      METHODS     */
    /********************/

    


    /************************/
    /*      OPERATORS       */
    /************************/

    bool operator==(const Job& J) const { return pi == J.pi && di == J.di && wi == J.wi; }

    bool operator!=(const Job& J) const { return !(J == *this); }

    /* Inequalities Operators compare processing times and due date in case of equalities */
    bool operator<(const Job& J) const { return (pi == J.pi) ? (di < J.di) : (pi < J.pi); }

    bool operator>(const Job& J) const { return (pi == J.pi) ? (di > J.di) : (pi > J.pi); }

    bool operator<=(const Job& J) const { return (pi == J.pi) ? (di <= J.di) : (pi <= J.pi); }

    bool operator>=(const Job& J) const { return (pi == J.pi) ? (di >= J.di) : (pi >= J.pi); }

};

inline std::ostream& operator<<(std::ostream& os, const Job& job) {
    os << "| pi: " << job.getPi() << " di: " << job.getDi() << " wi: " << job.getWi() << " |";
    return os;
}

#endif //BILEVEL_SCHEDULING_JOB_H
