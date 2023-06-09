#include "prototype.h"

void
dsaturColorGeneration(const vector<vector<int>> &input_graph, vector<int>& color_map){

    priority_queue<tuple<int, int, int>> queue;//saturation, degree, index
    while((connectedDijkstra(input_graph, color_map, queue)) == 0);

}

int
connectedDijkstra(const vector<vector<int>> &input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue){

    //find the initial vertex for the queue
    //for any connected subgraph, the saturation value for every node will be zero
    //initial step will thus search for the largest degree
    int max_degree = -1;
    int max_degree_vertex = -1;
    for(int i = 0; i < input_graph.size(); i++){

        if(color_map[i] != -1) continue;
        if(max_degree >= (int) input_graph[i].size()) continue;
        max_degree = (int) input_graph[i].size();
        max_degree_vertex = i;

    }

    //no available subgraph, return -1 to indicate that no need for more Dijkstra calls
    if(max_degree_vertex == -1) return -1;

    //2nd variant of Dijkstra
    //the queue will not delete or change any node, it will just insert the better node
    queue.push(std::make_tuple(1, max_degree, max_degree_vertex));
    while (!queue.empty()){
        int index = std::get<2>(queue.top()), current_saturation = std::get<0>(queue.top()); queue.pop();
        if (color_map[index] != -1) continue;

        color_map[index] = findPlausibleColor((int)input_graph.size(), input_graph[index], color_map);
        insertVertexDsatur(index, current_saturation, input_graph, color_map, queue);
    }

    //return 0 to indicate successful coloring run
    return 0;
}

int
findPlausibleColor(const int courseSize, const vector<int> &neighbourhood, vector<int>& color_map){
    for(int color = 0; color < courseSize; color++){
        bool color_available = true;
        for(auto vertex: neighbourhood){
            if(color_map[vertex] != color) continue;
            color_available = false;
            break;
        }
        if(color_available) return color;
    }
}

void
insertVertexDsatur(const int vertex_index, const int current_saturation, const vector<vector<int>> &input_graph, vector<int>& color_map, priority_queue<tuple<int, int, int>>& queue){
    //insert all adjacent node into the queue, according to dsatur implementation
    for(auto i: input_graph[vertex_index]){
        if(color_map[i] != -1) continue;
        queue.push(std::make_tuple(current_saturation + 1, input_graph[i].size(), i));
    }
}

void
largestDegreeColorGeneration(const vector<vector<int>> &input_graph, const vector<int> &students_enrolled, vector<int> &color_map) {
    priority_queue<tuple<int, int, int>> queue; //degree, student, index
    for(int i = 0; i < input_graph.size(); i++){
        queue.push(std::make_tuple(input_graph[i].size(), students_enrolled[i], i));
    }

    while(!queue.empty()){
        int index = std::get<2>(queue.top()); queue.pop();
        color_map[index] = findPlausibleColor((int)input_graph.size(), input_graph[index], color_map);
    }
}

void
largestStudentColorGeneration (const vector<vector<int>> &input_graph, const vector<int> &students_enrolled, vector<int> &color_map){
    priority_queue<tuple<int, int, int>> queue; //student, degree, index
    for(int i = 0; i < input_graph.size(); i++){
        queue.push(std::make_tuple(students_enrolled[i], input_graph[i].size(), i));
    }

    while(!queue.empty()){
        int index = std::get<2>(queue.top()); queue.pop();
        color_map[index] = findPlausibleColor((int)input_graph.size(), input_graph[index], color_map);
    }
}

void
randomStudentColorGeneration (const vector<vector<int>> &input_graph, vector<int> &color_map){
    //step 1: make a list of all the course number
    std::vector<int> node_list(input_graph.size());
    std::iota(node_list.begin(), node_list.end(), 0);

    //step 2: randomize that list
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine (seed);
    std::shuffle(std::begin(node_list), std::end(node_list), rng);

    //step 3: from 0 to last, select every node and assign a color
    for(auto vertex: node_list){
        color_map[vertex] = findPlausibleColor((int)input_graph.size(), input_graph[vertex], color_map);
    }
    node_list.clear();
}