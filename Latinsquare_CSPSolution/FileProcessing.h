//
// Created by shamit on 12/24/2022.
//

#ifndef CSP_LATINSQUARE_FILEPROCESSING_H
#define CSP_LATINSQUARE_FILEPROCESSING_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "LatinSquare.h"

void tokenize(std::string const &str, const char delim, std::vector<int> &out);
void fileToVector(const char* filename, std::vector<std::string> &lines);
LatinSquare vectorToLatinSquare(const std::vector<std::string> &lines);

#endif //CSP_LATINSQUARE_FILEPROCESSING_H
