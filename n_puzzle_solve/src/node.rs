pub struct Node{
    pub board: Vec<u32>,
    pub size: usize,
    pub step: u32,
    pub total_cost: u32,
    pub parent: u32
}

pub fn hamming_distance(
    current_configure: &Vec<u32>, 
    goal: &Vec<u32>, _size: usize) -> u32{
    let mut count_misplaced: u32 = 0;
    for (i, value) in current_configure
                                    .iter()
                                    .enumerate(){
        if 
            value != &goal[i] 
            && 
            value != &0{
            count_misplaced += 1;
        }
    };
    count_misplaced
}

pub fn manhattan_distance(
    current_configure: &Vec<u32>, 
    _goal: &Vec<u32>, 
    size: usize) -> u32{
    let mut total_distance: u32 = 0;
    let mut current_row: u32 = 0;
    let mut current_col: u32 = 0;

    for (i, value) in current_configure
                                    .iter()
                                    .enumerate(){
        if value != &0 {
            let given_row = (*value - 1) / size as u32;
            let given_col = (*value - 1) % size as u32;

            if current_col > given_col {
                total_distance += current_col - given_col;
            }else{
                total_distance += given_col - current_col;
            }

            if current_row > given_row {
                total_distance += current_row - given_row;
            }else{
                total_distance += given_row - current_row;
            }
        }

        if i % size == size - 1 {
            current_row += 1;
            current_col = 0;
        }else{
            current_col += 1;
        }

    };
    total_distance
}

pub fn linear_conflict(
    current_configure: &Vec<u32>,
    _goal: &Vec<u32>,
    size: usize) -> u32{
        let mut total_conflict: u32 = 0;
        //row conflicts
        for row in 0..size{
            for i in 0..size{
                for j in i+1..size{
                    if current_configure[row * size + i] != 0 &&
                        current_configure[row * size + j] != 0 &&
                        current_configure[row * size + i] - 1 / size as u32 == row as u32 &&
                        current_configure[row * size + j] - 1 / size as u32 == row as u32 && 
                        current_configure[row * size + i] > current_configure[row * size + j]{
                            total_conflict += 2;
                        }
                }
            }
        }

        //col conflicts
        for col in 0..size{
            for i in 0..size{
                for j in i+1..size{
                    if current_configure[i * size + col] != 0 &&
                        current_configure[j * size + col] != 0 &&
                        current_configure[i * size + col] - 1 % size as u32 == col as u32 &&
                        current_configure[j * size + col] - 1 % size as u32 == col as u32 && 
                        current_configure[i * size + col] > current_configure[j * size + col]{
                            total_conflict += 2;
                        }
                }
            }
        }

        total_conflict += hamming_distance(current_configure, _goal, size);
        total_conflict
    }

pub fn expand_node(
    heuristic_calculator:fn(
        &Vec<u32>,
        &Vec<u32>, 
        usize)->u32, 
    current_node: &Node, 
    goal_node: &Vec<u32>, 
    parent: u32) 
-> Vec<Node>{

    let mut i:usize = 0;
    for (index, value) in current_node
                                        .board
                                        .iter()
                                        .enumerate(){
        if value == &0{
            i = index;
            break;
        }
    }
    let k = current_node.size;

    let mut expanded_nodes: Vec<Node> = Vec::new();

    //left expansion
    if i % k != 0{
        let mut new_board = current_node.board.clone();
        new_board.swap(i, i - 1);
        let heuristics = heuristic_calculator(&new_board, &goal_node, k);

        let new_move = Node{
            board: new_board,
            size: k,
            step: current_node.step + 1,
            total_cost: current_node.step + heuristics,
            parent: parent
        };
        expanded_nodes.push(new_move);
    }

    //right expansion
    if i % k != k - 1{
        let mut new_board = current_node.board.clone();
        new_board.swap(i, i + 1);
        let heuristics = heuristic_calculator(&new_board, &goal_node, k);

        let new_move = Node{
            board: new_board,
            size: k,
            step: current_node.step + 1,
            total_cost: current_node.step + heuristics,
            parent: parent
        };
        expanded_nodes.push(new_move);
    }
    
    //up expansion
    if i / k != 0{
        let mut new_board = current_node.board.clone();
        new_board.swap(i, i - k);
        let heuristics = heuristic_calculator(&new_board, &goal_node, k);

        let new_move = Node{
            board: new_board,
            size: k,
            step: current_node.step + 1,
            total_cost: current_node.step + heuristics,
            parent: parent
        };
        expanded_nodes.push(new_move);
    }

    //down expansion
    if i / k != k - 1{
        let mut new_board = current_node.board.clone();
        new_board.swap(i, i + k);
        let heuristics = heuristic_calculator(&new_board, &goal_node, k);

        let new_move = Node{
            board: new_board,
            size: k,
            step: current_node.step + 1,
            total_cost: current_node.step + heuristics,
            parent: parent
        };
        expanded_nodes.push(new_move);
    }

    expanded_nodes
}

pub fn solvable(current_configure: &Node) -> bool{
    let mut i:usize = 0;
    for (index, value) in current_configure
                                        .board
                                        .iter()
                                        .enumerate(){
        if value == &0{
            i = index;
            break;
        }
    }
    let mut count_inversion: usize = 0;
    for (i, value_i) in current_configure
                                        .board
                                        .iter()
                                        .enumerate(){
        for (_j, value_j) in current_configure
                                            .board[(i + 1)..]
                                            .iter()
                                            .enumerate(){
            if value_i == &0 || value_j == &0 {continue;}
            if value_i > value_j {count_inversion += 1;}
        }
    };

    if current_configure.size % 2 == 1{
        return count_inversion % 2 == 0;
    }
    else {
        return count_inversion % 2 != (i / current_configure.size) % 2;
    }
}

pub fn goal_reached(current_configure: &Node, goal: &Vec<u32>)->bool{
    for (i, value) in current_configure
                                    .board
                                    .iter()
                                    .enumerate(){
        if value != &goal[i] {
            return false;
        }
    };
    return true;
}

pub fn configure_to_string(current_configure: &Node) -> String{
    let mut config_string = String::new();
    for (_i, value) in current_configure
                                        .board
                                        .iter()
                                        .enumerate(){
        config_string.push_str(&value.to_string());
        config_string.push_str(" ");
    }
    config_string
}