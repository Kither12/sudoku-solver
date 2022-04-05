#include <iostream>
#include <fstream>

int main_table[9][9];
int block_index[9][9] = {
    {0, 0, 0, 1, 1, 1, 2, 2, 2},
    {0, 0, 0, 1, 1, 1, 2, 2, 2},
    {0, 0, 0, 1, 1, 1, 2, 2, 2},
    {3, 3, 3, 4, 4, 4, 5, 5, 5},
    {3, 3, 3, 4, 4, 4, 5, 5, 5},
    {3, 3, 3, 4, 4, 4, 5, 5, 5},
    {6, 6, 6, 7, 7, 7, 8, 8, 8},
    {6, 6, 6, 7, 7, 7, 8, 8, 8},
    {6, 6, 6, 7, 7, 7, 8, 8, 8}
};
int block_elements[9][9][2] = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}},
    {{0, 3}, {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}},
    {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}},
    {{3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {5, 0}, {5, 1}, {5, 2}},
    {{3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}},
    {{3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {5, 6}, {5, 7}, {5, 8}},
    {{6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 0}, {8, 1}, {8, 2}},
    {{6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {8, 3}, {8, 4}, {8, 5}},
    {{6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {8, 6}, {8, 7}, {8, 8}}
};

int encode_table[9][9];
int encode_row[9], encode_column[9], encode_block[9];
int prefix_row[9][9][9], prefix_column[9][9][9], prefix_block[9][9];

void read_input(){
    freopen("input.txt", "r", stdin);
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            std::cin >> main_table[i][j];
        }
    }
}
void write_answer(){
    freopen("output.txt", "w", stdout);
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            std::cout << main_table[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void initiation(){
    for(int i = 0; i < 9; ++i){
        encode_row[i] = 0;
        encode_column[i] = 0;
        encode_block[i] = 0;
        for(int j = 0; j < 9; ++j){
            encode_table[i][j] = 0;
            prefix_block[i][j] = 0;
            for(int num = 0; num < 9; ++num){
                prefix_row[i][j][num] = 0;
                prefix_column[i][j][num] = 0;
            }
        }
    }
}
void overall_ecode(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(main_table[i][j] == 0) continue;
            encode_table[i][j] = (1 << (main_table[i][j]));
            encode_row[i] = encode_row[i] | encode_table[i][j];
            encode_column[j] = encode_column[j] | encode_table[i][j];
            encode_block[block_index[i][j]] = encode_block[block_index[i][j]]  | encode_table[i][j];
        }
    }
}
void cell_encode(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(encode_table[i][j] != 0){
                encode_table[i][j] = 1022;
            }
            else{
                encode_table[i][j] = encode_row[i] | encode_column[j] | encode_block[block_index[i][j]];
            }
        }
    }
}
void compute_prefix(){
    for(int num = 0; num < 9; ++num){
        for(int i = 0; i < 9; ++i){
            for(int j = 0; j < 9; ++j){
                if(!(1 & (encode_table[i][j] >> (num + 1)))){
                    prefix_row[num][i][j] = 1;
                    prefix_column[num][j][i] = 1;
                    ++prefix_block[num][block_index[i][j]];
                }
            }
        }
    }
    for(int num = 0; num < 9; ++num){
        for(int i = 0; i < 9; ++i){
            for(int j = 1; j < 9; ++j){
                prefix_row[num][i][j] += prefix_row[num][i][j - 1];
                prefix_column[num][i][j] += prefix_column[num][i][j - 1];
            }
        }
    }
}
void calculate(){
    //check if we can get more information from all possible blank from row and column be exactly in one block
    //this computation step can help strategy_1 go faster 2 - 3 times and find out some special cell
    //i think it is possible to loop this process several times to make the calculating process stronger
    compute_prefix();
    for(int num = 0; num < 9; ++num){
        for(int index = 0; index < 9; ++index){
            if(prefix_column[num][index][8] > 0 && prefix_column[num][index][8] < 4){
                //block 0
                if(prefix_column[num][index][2] == prefix_column[num][index][8]){
                    for(int i = 0; i < 9; ++i){
                        if(block_elements[index / 3][i][1] == index) continue;
                        encode_table[block_elements[index / 3][i][0]][block_elements[index / 3][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 1
                if(prefix_column[num][index][5] -  prefix_column[num][index][2] == prefix_column[num][index][8]){
                    for(int i = 0; i < 9; ++i){
                        if(block_elements[index / 3 + 3][i][1] == index) continue;
                        encode_table[block_elements[index / 3 + 3][i][0]][block_elements[index / 3 + 3][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 2
                if(prefix_column[num][index][8] -  prefix_column[num][index][5] == prefix_column[num][index][8]){
                    for(int i = 0; i < 9; ++i){
                        if(block_elements[index / 3 + 6][i][1] == index) continue;
                        encode_table[block_elements[index / 3 + 6][i][0]][block_elements[index / 3 + 6][i][1]] |= (1 << (num + 1));
                    }
                }
            }
            if(prefix_row[num][index][8] > 0 && prefix_row[num][index][8] < 4){
                //block 0
                if(prefix_row[num][index][2] == prefix_row[num][index][8]){
                    for(int i = 0; i < 9; ++i){
                        if(block_elements[(index / 3) * 3][i][0] == index) continue;
                        encode_table[block_elements[(index / 3) * 3][i][0]][block_elements[(index / 3) * 3][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 1
                if(prefix_row[num][index][5] -  prefix_row[num][index][2] == prefix_row[num][index][8]){
                    for(int i = 0; i < 9; ++i){
                        if(block_elements[(index / 3) * 3 + 1][i][0] == index) continue;
                        encode_table[block_elements[(index / 3) * 3 + 1][i][0]][block_elements[(index / 3) * 3 + 1][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 2
                if(prefix_row[num][index][8] -  prefix_row[num][index][5] == prefix_row[num][index][8]){
                    for(int i = 0; i < 9; ++i){
                        if(block_elements[(index / 3) * 3 + 2][i][0] == index) continue;
                        encode_table[block_elements[(index / 3) * 3 + 2][i][0]][block_elements[(index / 3) * 3 + 2][i][1]] |= (1 << (num + 1));
                    }
                }
            }
        }
    }
    //reseting the prefix after update
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            for(int num = 0; num < 9; ++num){
                prefix_row[i][j][num] = 0;
                prefix_column[i][j][num] = 0;
            }
        }
    }
    compute_prefix();
}
int update(){
    //this strategy will look into each number and find any rows, columns, blocks that have exactly one possible blank to fill that number
    int score = 0;
    for(int num = 0; num < 9; ++num){
        for(int index = 0; index < 9; ++index){
            if(prefix_row[num][index][8] == 1){
                for(int i = 0; i < 9; ++i){
                    if(!(1 & (encode_table[index][i] >> (num + 1)))){
                        main_table[index][i] = (num + 1);
                        ++score;
                    }
                }
            }
            if(prefix_column[num][index][8] == 1){
                for(int i = 0; i < 9; ++i){
                    if(!(1 & (encode_table[i][index] >> (num + 1)))){
                        main_table[i][index] = (num + 1);
                        ++score;
                    }
                }
            }
            if(prefix_block[num][index] == 1){
                for(int i = 0; i < 9; ++i){
                    if(!(1 & (encode_table[block_elements[index][i][0]][block_elements[index][i][1]] >> (num + 1)))){
                        main_table[block_elements[index][i][0]][block_elements[index][i][1]] = (num + 1);
                        ++score;
                    }
                }
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(__builtin_popcount(encode_table[i][j]) == 8){
                for(int num = 0; num < 9; ++num){
                    if(!(1 & (encode_table[i][j] >> (num + 1)))){
                        main_table[i][j] = num + 1;
                        ++score;
                    }
                }
            }
        }
    }
    return score;
}
int main(){
    read_input();
    int cnt = 0;
    do{
        initiation();
        overall_ecode();
        cell_encode();
        calculate();
        ++cnt;
    }
    while(update());
    write_answer();
    std::cout << cnt << "\n";
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            std::cout << encode_table[i][j] << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
