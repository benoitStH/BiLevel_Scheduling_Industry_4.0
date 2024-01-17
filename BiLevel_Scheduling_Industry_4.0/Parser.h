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

#ifndef BILEVEL_SCHEDULING_PARSER_H
#define BILEVEL_SCHEDULING_PARSER_H

#include "Instance.h"
#include <string>

class Parser {

public:

    /************************/
    /*      CONSTRUCTOR     */
    /************************/

    Parser();

    /********************/
    /*      METHODS     */
    /********************/


    /**
     * This Method parse a file to generate an instance. It's a Instance constructor
     * @param filePath the path of the file to parse
     * @return the new instance
     */
    Instance readFromFile(std::string& filePath) const;

    /**
     * Method that serialize an instance into the corresponding file given by the attributeInstance::instancePath.
     * @param instance
     */
    void serializeInstance(Instance& instance);
};


#endif //BILEVEL_SCHEDULING_PARSER_H
