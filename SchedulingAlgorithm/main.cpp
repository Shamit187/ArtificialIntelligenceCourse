#include "prototype.h"

int main(int argc, char **argv) {
    if(argc <= 2) {
        cerr << "Usage: " << argv[0] << " Datafile Heuristic1 Heuristic2\n";
        return -1;
    }
    int hard_choice = std::stoi(argv[2]);
    if(hard_choice < 0 || hard_choice > 3) return -1;

    string course_file = (argv[1]); course_file.append(".crs");
    string student_file = (argv[1]); student_file.append(".stu");

    //graph generation phase
    vector<int> student_enrolled;
    int course_amount;
    if((course_amount = generateNodes(student_enrolled, course_file)) == -1) return -1;

    vector<vector<int>> input_graph(course_amount);
    int student_number;
    if((student_number = generateEdges(input_graph, student_file)) == -1) return -1;

    //graph coloring phase
    vector<int> color_map(course_amount, -1);
    switch (hard_choice) {
        case 0: //Highest Degree
            largestDegreeColorGeneration(input_graph, student_enrolled, color_map);
            break;
        case 1: //Highest Student
            largestStudentColorGeneration(input_graph, student_enrolled, color_map);
            break;
        case 2: //DSat Algo
            dsaturColorGeneration(input_graph, color_map);
            break;
        case 3: //Random Assignment
            randomStudentColorGeneration(input_graph, color_map);
            break;
        default:
            cerr << "Something Wrong\n";
            return -1;
    }

    //find the highest color
    int max = 0;
    for(auto i: color_map){
        if(max > i) continue;
        max = i;
    }
    cout << "Total time slots for choice (" << hard_choice << "): " << max + 1 << "\n";

    //optimization phase
    cout << "Initial penalty: " << std::get<0>(calculate_penalty_highest_offence(input_graph, color_map)) / (double) student_number << "\n";
    color_map = simulatedAnnealing(0, 150, 1e-3, 0.98, input_graph, color_map);
    cout << "After Kempe-Chain: " << std::get<0>(calculate_penalty_highest_offence(input_graph, color_map)) / (double) student_number << "\n";
    color_map = simulatedAnnealing(1, 100, 1e-2, 0.99, input_graph, color_map);
    cout << "After Pair-Swap: " << std::get<0>(calculate_penalty_highest_offence(input_graph, color_map)) / (double) student_number << "\n";

    generateLogFile(input_graph, color_map);
    return 0;
}


