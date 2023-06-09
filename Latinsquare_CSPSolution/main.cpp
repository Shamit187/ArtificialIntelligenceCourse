#include <iostream>
#include <string>
#include <chrono>
#include "LatinSquare.h"
#include "FileProcessing.h"

typedef Point (*func)(const LatinSquare&);
func heuristics[4] ={minimum_domain, maximum_constrain, min_dom_max_const, min_dom_by_max_const};

int main(int argc, char **argv) {
    std::vector<std::string> out;
    fileToVector(argv[1], out);
    LatinSquare latinSquare = vectorToLatinSquare(out);

    auto start = std::chrono::high_resolution_clock::now();
    backtrackCSP(latinSquare, heuristics[0], true);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() ;

    std::cout << "\n" << latinSquare;
}