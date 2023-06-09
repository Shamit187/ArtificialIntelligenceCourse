#include "prototype.h"

int generateNodes(vector<int>& student_enrolled, string const& course_file_name){
    std::ifstream course_file(course_file_name, std::ifstream::in);
    if(!course_file.is_open()){
        cerr << "Unable to open file: " << course_file_name << endl;
        return -1;
    }
    string line;
    int course_number, student_amount, course_amount = 0;
    while(std::getline(course_file, line)){
        sscanf(line.c_str(), "%d %d", &course_number, &student_amount);
        student_enrolled.push_back(student_amount);
        course_amount++;
    }
    course_file.close();
    return course_amount;
}

void add_edge(vector<int>& student_data, vector<vector<int>>& input_graph){
    for(auto i: student_data){
        for(auto j: student_data){
            if(i != j) input_graph[i].push_back(j);
        }
    }
}

int generateEdges(vector<vector<int>> & input_graph, string const& student_file_name){
    std::ifstream student_file(student_file_name, std::ifstream::in);
    if(!student_file.is_open()){
        cerr << "Unable to open file: " << student_file_name << endl;
        return -1;
    }
    string line;
    int number;
    int student_number = 0;
    vector<int> student_data;
    while(std::getline(student_file, line)){
        student_number++;
        std::stringstream ss(line);
        student_data.clear();
        while (ss >> number) student_data.push_back(number - 1); //courses in file start from 1, we start from 0
        if(student_data.size() <= 1) continue;
        add_edge(student_data, input_graph);
    }
    student_file.close();
    return student_number;
}

void generateLogFile (vector<vector<int>> &input_graph, vector<int> &color_map){
    std::ofstream log_file("log_file.txt", std::ifstream::out);

    for(int vertex = 0; vertex < input_graph.size(); vertex++){
        log_file << std::setw(4) << std::setfill('0') << vertex << ":" << std::setw(4) << std::setfill('0') << color_map[vertex] << "\n";
        for(auto neighbours: input_graph[vertex]){
            if(color_map[vertex] == color_map[neighbours]){
                cerr << "Optimization Screwed up\n";
                exit(-1);
            }
        }
    }
}
