#include "prototype.h"

vector<int>
simulatedAnnealing(const int choice, const double init_temp, const double final_temp, const double cooling_rate, const vector<vector<int>> &input_graph, vector<int> &color_map) {

    vector<int> current(color_map), neighbour(color_map);
    vector<bool> switched_vertex(input_graph.size(), false); //keeping a track to help allow more nodes to change color
    auto tuple_value = calculate_penalty_highest_offence(input_graph, current);
    int most_offensive_index = std::get<1>(tuple_value), currentPenalty = std::get<0>(tuple_value), neighbourPenalty;
    double del_E, current_temp = init_temp, error, acceptanceProbability;

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> continuous_distribution(-1, 1);
    std::uniform_int_distribution<> discrete_distribution(0 , (int)input_graph.size() - 1); //input_graph must be greater than 0... hoping that

    while (current_temp > final_temp){
        switched_vertex[most_offensive_index] = true;

        if(choice == 0) kempeChainInterchange(most_offensive_index, input_graph, neighbour);
        else pairwiseSwap(most_offensive_index, input_graph, neighbour);

        tuple_value = calculate_penalty_highest_offence(input_graph, neighbour);
        neighbourPenalty = std::get<0>(tuple_value);
        most_offensive_index = std::get<1>(tuple_value);
        if(switched_vertex[most_offensive_index]) most_offensive_index = discrete_distribution(generator);

        del_E = (double) (neighbourPenalty - currentPenalty);

        error  = del_E;
        acceptanceProbability  = exp(- error / (double) current_temp);
        if(choice == 1) acceptanceProbability -= 1; // for pairwise swap, decrease acceptance rate, seems to perform better

        if (del_E < 0 || continuous_distribution(generator) < acceptanceProbability){
            current = neighbour;
            currentPenalty = neighbourPenalty;
        }

        current_temp *= cooling_rate;
    }
    return current;
}

void pairwiseSwap(const int index, const vector<vector<int>> &input_graph, vector<int> &color_map) {
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int>distribution = std::uniform_int_distribution<int>(0, (int) input_graph.size() - 1);

    int init_vertex = index, color1 = color_map[index];;
    vector<int> init_vertex_color_conflicts(0);
    for(auto neighbours: input_graph[init_vertex]){
        init_vertex_color_conflicts.push_back(color_map[neighbours]);
    }

    //when we generate new random vertex
    //check 1: make sure it is not the same initial vertex, and don't share same color
    //check 2: the color it has should not be the color that original one's neighbours have
    //check 3: vice versa for new vertex's neighbours
    //note:
    //keeping a count so that the algorithm doesn't run too long
    //it may be difficult to find such a vertex with above-mentioned conditions

    int new_vertex, color2, max_count = 1000, count = 0;
    bool vertex_found = false;
    while (!vertex_found){
        if(count > max_count) return;
        count++;

        new_vertex = distribution(generator);
        color2 = color_map[new_vertex];

        //check 1:
        if(new_vertex == init_vertex || color1 == color2) continue;
        vertex_found = true;

        //check 2:
        for(auto neighbours: input_graph[new_vertex]){
            if(color_map[neighbours] == color1) {
                vertex_found = false;
                continue;
            }
        }
        if(!vertex_found) continue;

        //check 3:
        auto it = std::find (init_vertex_color_conflicts.begin(), init_vertex_color_conflicts.end(), color2);
        if (it != init_vertex_color_conflicts.end()) vertex_found = false;
    }
    //cout << "Switched " << init_vertex << "," << new_vertex << ": " << color1 << "," << color2 << "\n";
    color_switch(init_vertex, color1, color2, color_map);
    color_switch(new_vertex, color1, color2, color_map);
}

tuple<int, int>
calculate_penalty_highest_offence
(const vector<vector<int>> &input_graph, const vector<int> &current) {
    long long int totalPenalty = 0;
    int penalty = 0;
    unsigned distance = 0;
    int max_penalty = 0;
    int max_degree = 0;
    unsigned max_penalty_index = 0;
    for(int vertex = 0; vertex < input_graph.size(); vertex++){
        penalty = 0;
        for(auto neighbour: input_graph[vertex]){
            distance = abs(current[vertex] - current[neighbour]);
            if(distance < 5) {
                penalty += pow(2, (5 - distance));
                //penalty = 2 * (5 - distance);
            }
        }
        totalPenalty += penalty;
        if(penalty > max_penalty) {
            max_penalty_index = vertex;
            max_penalty = (int) penalty;
            max_degree = input_graph[vertex].size();
        }else if(penalty == max_penalty && max_degree < input_graph[vertex].size() ) {
            max_penalty_index = vertex;
            max_degree = input_graph[vertex].size();
        }
    }
    return std::make_tuple(totalPenalty / 2, max_penalty_index);
}

void
kempeChainInterchange(const int index, const vector<vector<int>> &input_graph, vector<int> &color_map) {
    int len = (int) input_graph[index].size();
    if(len == 0) return;
    //if the selected chain is 1 vertex long, we got nothing to do pretty much

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution = std::uniform_int_distribution<int>(0,  len - 1);

    int random_number = distribution(generator);
    const int color_1 = color_map[index], color_2 = color_map[input_graph[index][random_number]];

    vector<bool> visited(input_graph.size(), false);
    modifiedDFS(index, color_1, color_2, input_graph, color_map, visited);
}

void
modifiedDFS(const int vertex, const int color1, const int color2, const vector<vector<int>> &input_graph, vector<int> &color_map, vector<bool> &visited) {
    visited[vertex] = true;
    color_switch(vertex, color1, color2, color_map);

    for(auto neighbour: input_graph[vertex]){
        if (visited[neighbour]) continue;
        if (color_map[neighbour] != color1 && color_map[neighbour] != color2) continue;
        modifiedDFS(neighbour, color1, color2, input_graph, color_map, visited);
    }
}

void
color_switch
(const int vertex, const int color1, const int color2, vector<int> &color_map) {
    if(color_map[vertex] == color1) color_map[vertex] = color2;
    else color_map[vertex] = color1;
}
