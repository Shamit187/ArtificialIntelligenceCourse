use std::io;
use binary_heap_plus::*;
use std::fs::File;
use std::io::Write;
use std::fs;
use std::collections::HashSet;
use std::env;

pub mod node;
use node::*;

fn main() {
    //input of board
    let mut board: Vec<u32> = Vec::new();
    let mut input = String::new();
    let file_path: String = format!("{}/configure_output.csv",
                            env::current_dir()
                            .unwrap()
                            .into_os_string()
                            .into_string()
                            .unwrap());

    io::stdin()
            .read_line(&mut input)
            .expect("Input Error for board size");

    let size: usize = input
                .trim()
                .parse()
                .expect("Parsing Error for board size");
    input.clear();
    println!("");

    let mut goal_board:Vec<u32> = Vec::new();
    for slot in 1..(size*size){
        goal_board.push(slot as u32);
    }

    board.resize(size * size, 0);
    
    //takes input as 
    // 1 2 3
    // 4 5 6
    // 7 8 *

    for line in 0..size{
        io::stdin()
            .read_line(&mut input)
            .expect("Input error for board entries");
        for (i, value) in input
                                    .trim()
                                    .split(' ')
                                    .enumerate(){
            if value.trim() == "*"{
                board[line * size + i] = 0;
            }else{
                board[line * size + i] = value
                            .trim()
                            .parse()
                            .expect("Parsing error for board entries");
            }
        }
        input.clear();
    }
    println!("");

    let mut amount_of_space:u32 = 0;
    let maximum_tile_number :u32 = size as u32 * size as u32;
    for (_i, value) in board
                                    .iter()
                                    .enumerate(){
        assert!(value  < &maximum_tile_number && value >= &0, "Invalid tile value given: {}", value);
        if value == &0{amount_of_space += 1};
    }
    assert!(amount_of_space == 1, "Invalid number of space tiles: {}", amount_of_space);

    //creation of start node and end node
    let starting_configuration = Node{
        board: board.clone(),
        size: size,
        step: 0,
        total_cost: 0,
        parent: 0
    };

    goal_board.push(0);
    let goal_board = goal_board; //making goal_board immutable

    //solvability check
    if !solvable(&starting_configuration){
        println!("Initial configuration not solvabe");
        return;
    }

    println!("Choice of heuristics?");
    println!("1.Hamming Distance");
    println!("2.Manhattan Distance");
    println!("3.Linear Conflict");

    io::stdin()
        .read_line(&mut input)
        .expect("Input error for Heuristics choice");
    let choice = input
                        .trim()
                        .parse::<u32>()
                        .expect("Parsing error for Heuristics choice");       
    input.clear();
    println!("");

    //main algorithm beginning
    let mut starting_vec:Vec<Node> = Vec::new();
    let mut configuration_hash:HashSet<String> = HashSet::new();
    configuration_hash.insert(configure_to_string(&starting_configuration));
    starting_vec.push(starting_configuration);

    let mut queue = BinaryHeap::from_vec_cmp(
        starting_vec,
        |a: &Node, b: &Node| b.total_cost.cmp(&a.total_cost),
    );

    //initial file for configure output
    let mut output_file = 
        File::create(&file_path)
            .expect("File Creation Error");
    write!(output_file, "Configuration, Parent\n")
            .expect("Error in Writing");

    let mut node_visited:u32 = 1;
    let maximum_node_visit_allowed:u32 = 20000;
    //main loop begins
    while !queue.is_empty() {
        let configuration_node = queue
            .peek()
            .unwrap(); 

        //file writing section
        let configuration_string = configure_to_string(configuration_node);
        if node_visited != 1 {write!(output_file, "\n").expect("Error in Writing");}
        write!(output_file, "{}, {}, {}",configuration_string, configuration_node.parent, configuration_node.total_cost).expect("Error in Writing");

        //checking if destination reached
        if goal_reached(configuration_node, &goal_board) {
            println!("Solution Found");
            println!("Nodes Visited: {}", node_visited);
            println!("Step Needed: {}", configuration_node.step);
            break;
        }

        //going to next possible moves
        let next_moves = if choice == 1{
            expand_node(
                hamming_distance,
                configuration_node,
                &goal_board,
                node_visited)
        }else if choice == 2{
            expand_node(
                manhattan_distance,
                configuration_node,
                &goal_board,
                node_visited)
        }else{
            expand_node(
                linear_conflict,
                configuration_node,
                &goal_board,
                node_visited)
        };

        queue.pop();
        for i in next_moves{
            if !configuration_hash
            .contains(
                &configure_to_string(
                    &i)) {
                configuration_hash
                    .insert(
                        configure_to_string(
                            &i));
                queue.push(i);
            }
            
        }
        node_visited += 1;
        assert!(node_visited < maximum_node_visit_allowed , "To many nodes visited, Memory overload");
    };

    //clearing memories for step finding
    queue.clear();
    configuration_hash.clear();

    //step finding
    println!("Show Path?");
    println!("1.Yes");
    println!("2.No");

    io::stdin()
        .read_line(&mut input)
        .expect("Input error for path selection choice");
    let choice: u32 = input
                        .trim()
                        .parse()
                        .expect("Parsing error for path selection choice");
    input.clear();
    println!("");

    if choice == 1{
        print_step_from_csv(&file_path, &size);
    }
    
    
}

fn print_step_from_csv(file_path: &String, size: &usize){
    let input = 
        fs::read_to_string(file_path)
            .expect("Fail to open csv file in path finding process");
    let mut first_line = true;
    let mut csv_data:Vec<Vec<&str>> = Vec::new();

    //process csv string into vector of data
    for line in input.split("\n"){
        if first_line {
            first_line = false;
            continue;
        }
        first_line = false;
        let csv_entry: Vec<&str> = line.split(",").collect();
        csv_data.push(csv_entry);
    }

    //index must start from the bottom to propagate to top
    let mut index = csv_data.len();
    //generate a string accoring to len of board
    let mut beauty_string = String::from("==");
    for _i in 0..*size{
        beauty_string.push_str("========");
    };

    let mut step_counter: usize = 0;
    while index >= 1{
        println!("{beauty_string}");
        println!("Step: {}", step_counter);
        println!("");
        let entry = &csv_data[index - 1];
        for (i, square) in entry[0]
            .trim()
            .split(' ')
            .enumerate(){
            if square.trim() == "0"{
                print!("*\t");
            }else{
                print!("{}\t", square.trim());
            }
            if i % size == size - 1{
                print!("\n");
            }
        }
        index = entry[1]
        .trim()
        .parse()
        .expect("Data processing error");
        println!("{beauty_string}");
        step_counter += 1;
    }
}