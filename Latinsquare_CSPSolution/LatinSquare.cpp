//
// Created by shami on 12/24/2022.
//

#include "LatinSquare.h"

bool LatinSquare::assign(Point point, uint64 value){
    //availability of the value check
    if ((point.row > size)||(point.col > size) || (value > size)) return false;
    if (!(available_rows[point.row] & (1 << (size - point.col - 1))) ) return false;
    if (!(domain_rows[point.row] & domain_cols[point.col] & (1 << value))) return false;

    //assignment
    data[point.row][point.col] = value;

    //constrain rearrange
    available_rows[point.row] &= ~(1 << (size - point.col - 1));
    domain_rows[point.row] &= ~(1 << value);
    domain_cols[point.col] &= ~(1 << value);
    return true;
}

void LatinSquare::unassign(Point point, uint64 value){
    //constrain change
    available_rows[point.row] |= (1 << (size - point.col - 1));
    domain_rows[point.row] |= (1 << value);
    domain_cols[point.col] |= (1 << value);
    data[point.row][point.col] = 0; //remove to increase performance;

    //done actually
}

bool LatinSquare::complete(){
    for(int row = 0; row < size; row++){
        if(available_rows[row]) return false;
    }
    return true;
}

bool LatinSquare::invalid(){
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if(!(available_rows[row] & (1 << (size - col - 1)))) continue;
            if(!(domain_rows[row] & domain_cols[col])) return true;
        }
    }
    return false;
}

uint64 LatinSquare::getdomain(Point point) {
    return domain_cols[point.col] & domain_rows[point.row];
}

std::ostream& operator<< (std::ostream& os, const LatinSquare& latinSquare){
    os << "Latin Square:\n";
    for (int row = 0; row < latinSquare.size; row++){
        for (int col = 0; col < latinSquare.size; col++){
            if(!(latinSquare.available_rows[row] & (1 << (latinSquare.size - col - 1))))
                os << char('a' + latinSquare.data[row][col]) << "   ";
//                os << latinSquare.data[row][col] + 1 << "   ";
            else
                os << "    ";
        }
        os << "\n";
    }
    return os;
}

int popcount(uint64 x, uint64 size){
    int count = 0;
    for(uint64 i = 0; i < size; i++){
        if(x & (1 << i)) count++;
    }
    return count;
}

Point minimum_domain(const LatinSquare& latinSquare){
    int min = latinSquare.size;
    int rem = 0;
    uint64 min_row, min_col;
    for(uint64 row = 0; row < latinSquare.size; row++){
        for(uint64 col = 0; col < latinSquare.size; col++){
            if(!(latinSquare.available_rows[row] & (1 << (latinSquare.size - col - 1)))) continue;
            rem = popcount(latinSquare.domain_rows[row] & latinSquare.domain_cols[col], latinSquare.size);
            if (rem == 1) return {.row =  row, .col =  col};
            if (rem >= min) continue;
            min = rem;
            min_row = row;
            min_col = col;
        }
    }
    return {.row =  min_row, .col =  min_col};
}

Point maximum_constrain(const LatinSquare& latinSquare){
    int max = -1;
    uint64 max_row, max_col;
    for(uint64 row = 0; row < latinSquare.size; row++){
        for(uint64 col = 0; col < latinSquare.size; col++){
            if(!(latinSquare.available_rows[row] & (1 << (latinSquare.size - col - 1)))) continue;
            int rem = 0;
            for(int i = 0; i < latinSquare.size; i++){
                if (i == row) continue;
                if (latinSquare.available_rows[i] & (1 << (latinSquare.size - col - 1))) rem++;
            }
            rem += popcount(latinSquare.available_rows[row], latinSquare.size) - 1;
            if (rem <= max) continue;
            max = rem;
            max_row = row;
            max_col = col;
        }
    }
    return {.row =  max_row, .col =  max_col};
}

int LatinSquare::getsize() {
    return size;
}

Point min_dom_max_const(const LatinSquare &latinSquare) {
    int min_dom = latinSquare.size;
    int max_const = -1;

    int rem = 0;
    uint64 min_row, min_col;
    for(uint64 row = 0; row < latinSquare.size; row++){
        for(uint64 col = 0; col < latinSquare.size; col++){
            if(!(latinSquare.available_rows[row] & (1 << (latinSquare.size - col - 1)))) continue;
            rem = popcount(latinSquare.domain_rows[row] & latinSquare.domain_cols[col], latinSquare.size);
            if (rem == 1) return {.row =  row, .col =  col};
            if (rem > min_dom) continue;
            if (rem == min_dom) {
                int constr = 0;
                for(int i = 0; i < latinSquare.size; i++){
                    if (i == row) continue;
                    if (latinSquare.available_rows[i] & (1 << (latinSquare.size - col - 1))) constr++;
                }
                constr += popcount(latinSquare.available_rows[row], latinSquare.size) - 1;
                if(constr <= max_const) continue;
                max_const = constr;
            }
            min_dom = rem;
            min_row = row;
            min_col = col;
        }
    }
    return {.row =  min_row, .col =  min_col};
}

Point min_dom_by_max_const(const LatinSquare &latinSquare) {
    double min = (double)latinSquare.size;
    uint64 min_row, min_col;
    for(uint64 row = 0; row < latinSquare.size; row++){
        for(uint64 col = 0; col < latinSquare.size; col++){
            if(!(latinSquare.available_rows[row] & (1 << (latinSquare.size - col - 1)))) continue;

            int dom = popcount(latinSquare.domain_rows[row] & latinSquare.domain_cols[col], latinSquare.size);

            int constr = 0;
            for(int i = 0; i < latinSquare.size; i++){
                if (i == row) continue;
                if (latinSquare.available_rows[i] & (1 << (latinSquare.size - col - 1))) constr++;
            }
            constr += popcount(latinSquare.available_rows[row], latinSquare.size) - 1;


            double rem = (constr != 0)? (double)dom/ (double)constr : (double)dom;
            if (rem >= min) continue;
            min = rem;
            min_row = row;
            min_col = col;
        }
    }
    return {.row =  min_row, .col =  min_col};
}

bool backtrackCSP(LatinSquare &latinSquare, Point (*heuristic)(const LatinSquare&), bool forward_check_enable){
    if (latinSquare.invalid()) return false;
    if (latinSquare.complete()) return true;

    Point var = heuristic(latinSquare);

    uint64 domain = ( 1 << latinSquare.getsize() ) - 1;

    if(forward_check_enable) domain = latinSquare.getdomain(var);

    for(int value = 0; value < latinSquare.getsize(); value++){
        if (!(domain & 1 << value)) continue;
        if (!latinSquare.assign(var, value)) latinSquare.unassign(var, value);
        if (backtrackCSP(latinSquare, heuristic, forward_check_enable)) return true;
        latinSquare.unassign(var, value);
    }

    return false;
}