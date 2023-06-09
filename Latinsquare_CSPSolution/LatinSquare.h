//
// Created by shamit on 12/24/2022.
//

#ifndef CSP_LATINSQUARE_LATINSQUARE_H
#define CSP_LATINSQUARE_LATINSQUARE_H

#include <vector>
#include <iostream>
#include <bit>

typedef unsigned long long uint64;

struct Point{
    uint64 row;
    uint64 col;
};

class LatinSquare {
private:
    std::vector<std::vector<uint64>> data;
    std::vector<uint64> available_rows;
    std::vector<uint64> domain_rows;
    std::vector<uint64> domain_cols;
    uint64 size;
public:

    LatinSquare(uint64 size)
        :size{size}
    {
        uint64 default_mask = (1 << size) - 1;
        data.resize(size, std::vector<uint64>(size, 0));
        available_rows.resize(size, default_mask);
        domain_rows.resize(size, default_mask);
        domain_cols.resize(size, default_mask);
    }

    // main implication
    bool assign(Point point, uint64 value);
    void unassign(Point point, uint64 value);

    // helping implication
    bool complete();
    bool invalid();
    uint64 getdomain(Point point);

    //friend based heuristics
    friend Point minimum_domain(const LatinSquare& latinSquare);
    friend Point maximum_constrain(const LatinSquare& latinSquare);
    friend Point min_dom_max_const(const LatinSquare& latinSquare);
    friend Point min_dom_by_max_const(const LatinSquare& latinSquare);
    //printing function
    friend std::ostream& operator<<(std::ostream& os, const LatinSquare& latinSquare);

    int getsize();
};

int popcount(uint64 i, uint64 size);
bool backtrackCSP(LatinSquare &latinSquare, Point (*func)(const LatinSquare&), bool forward_check_enable);
Point minimum_domain(const LatinSquare& latinSquare);
Point maximum_constrain(const LatinSquare& latinSquare);
Point min_dom_max_const(const LatinSquare& latinSquare);
Point min_dom_by_max_const(const LatinSquare& latinSquare);

#endif //CSP_LATINSQUARE_LATINSQUARE_H
