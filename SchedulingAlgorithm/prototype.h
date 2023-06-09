#ifndef SCHEDULINGALGORITHM_PROTOTYPE_H
#define SCHEDULINGALGORITHM_PROTOTYPE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>
#include <queue>
#include <tuple>
#include <random>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cerr;
using std::priority_queue;
using std::tuple;

//fileread.cpp
int
generateNodes
        (vector<int>& student_enrolled, string const& course_file_name);

int
generateEdges
        (vector<vector<int>> & input_graph, string const& student_file_name);

void
add_edge
        (vector<int>& student_data, vector<vector<int>>& input_graph);

void
generateLogFile
        (vector<vector<int>> &input_graph, vector<int> &color_map);


//graphcolor.cpp
void
largestDegreeColorGeneration
        (const vector<vector<int>> &input_graph, const vector<int> &students_enrolled, vector<int> &color_map);

void
largestStudentColorGeneration
        (const vector<vector<int>> &input_graph, const vector<int> &students_enrolled, vector<int> &color_map);

void
randomStudentColorGeneration
        (const vector<vector<int>> &input_graph, vector<int> &color_map);

int
findPlausibleColor
        (int courseSize, const vector<int> &neighbourhood, vector<int>& color_map);

void
insertVertexDsatur
        (int vertex_index, int current_saturation, const vector<vector<int>> &input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue);

void
dsaturColorGeneration
        (const vector<vector<int>> &input_graph, vector<int>& color_map);

//optimization.cpp
void
kempeChainInterchange
        (int index, const vector<vector<int>> &input_graph, vector<int> &color_map);

void
modifiedDFS
        (int vertex, int color1, int color2, const vector<vector<int>> &input_graph, vector<int> &color_map, vector<bool> &visited);

void
color_switch
        (int vertex, int color1, int color2, vector<int> &color_map);

vector<int>
simulatedAnnealing
        (int choice, double init_temp, double final_temp, double cooling_rate, const vector<vector<int>> &input_graph, vector<int> &color_map);

tuple<int, int>
calculate_penalty_highest_offence
        (const vector<vector<int>> &input_graph, const vector<int> &current);

int
connectedDijkstra
        (const vector<vector<int>> &input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue);

void
pairwiseSwap
        (int index, const vector<vector<int>> &input_graph, vector<int> &color_map);
#endif
