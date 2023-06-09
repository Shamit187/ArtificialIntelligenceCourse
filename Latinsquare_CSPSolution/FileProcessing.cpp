//
// Created by shamit on 12/24/2022.
//

#include "FileProcessing.h"

void tokenize(std::string const &str, const char delim, std::vector<int> &out)
{
    // construct a stream from the string
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(stoi(s, nullptr, 10));
    }
}

void fileToVector(const char* filename, std::vector<std::string> &lines)
{
    std::ifstream  infile(filename);

    std::string line;
    while(std::getline(infile, line)){
        lines.push_back(line);
    }
}

LatinSquare vectorToLatinSquare(const std::vector<std::string> &lines)
{
    //first line is n=10
    std::string line = lines[0];
    line.pop_back();
    line.erase(line.begin() + 0, line.begin() + 2);
    int n = stoi(line, nullptr, 10); //size is received
    LatinSquare latinSquare(n);

    //data always from lines[3] to lines[2 + N]
    uint64 row = 0;

    for(int i = 3; i <= 2 + n; i++){
        line = lines[i];
        //remove | from end
        line.erase(line.end() - 2, line.end());
        //last line has too much stuff
        if(i == 2 + n) line.erase(line.end() - 2, line.end());
        std::vector<int> out;
        tokenize(line, ',', out);

        for(uint64 col = 0; col < n; col++)
        {
            if (!out[col]) continue;
            latinSquare.assign({.row = row, .col = col}, out[col] - 1);
        }
        row++;
    }

    return latinSquare;
}