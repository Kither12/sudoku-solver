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
    {{3, 0}, {3, 1}, {3, 2}, {4, 0}, {4, 1}, {4, 2}, {4, 0}, {4, 1}, {4, 2}},
    {{3, 3}, {3, 4}, {3, 5}, {4, 3}, {4, 4}, {4, 5}, {4, 3}, {4, 4}, {4, 5}},
    {{3, 6}, {3, 7}, {3, 8}, {4, 6}, {4, 7}, {4, 8}, {4, 6}, {4, 7}, {4, 8}},
    {{6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {7, 0}, {7, 1}, {7, 2}},
    {{6, 3}, {6, 4}, {6, 5}, {7, 3}, {7, 4}, {7, 5}, {7, 3}, {7, 4}, {7, 5}},
    {{6, 6}, {6, 7}, {6, 8}, {7, 6}, {7, 7}, {7, 8}, {7, 6}, {7, 7}, {7, 8}}
};
int encode_table[9][9];
int encode_row[9], encode_column[9], encode_block[9];
int prefix_row[9][9], prefix_column[9][9], prefix_block[9][9];

void read_input(){
    freopen("input.txt", "r", stdin);
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j)[
            std::cin >> main_table[i][j];
        ]
    }
}
void initiation(){
    for(int i = 0; i < 9; ++i){
        encode_row[i] = 0;
        encode_column[i] = 0;
        encode_block[i] = 0;
        for(int j = 0; j < 9; ++j){
            encode_table[i][j] = 0;
            prefix_row[i][j] = 0;
            prefix_column[i][j] = 0;
            prefix_block[i][j] = 0;
        }
    }
}

void calculate(){

    // encoding rows, columns, blocks
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(main_table[i][j] == 0) continue;
            encode_table[i][j] = (1 << (main_table[i][j]));
            encode_row[i] = encode_row[i] | encode_table[i][j];
            encode_column[j] = encode_column[j] | encode_table[i][j];
            encode_block[block_index[i][j]] = encode_block[block_index[i][j]]  | encode_table[i][j];
        }
    }

    //encoding for each cells
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

    //calculate the prefix for each rows columns and blocks
    for(int num = 0; num < 9; ++num){
        for(int i = 0; i < 9; ++i){
            for(int j = 0; j < 9; ++j){
                if(!(1 & (encode_table[i][j] >> (num + 1)))){
                    ++prefix_row[num][i];
                    ++prefix_column[num][j];
                    ++prefix_block[num][block_index[i][j]];
                }
            }
        }
    }
}

int update(){
    //update when prefix[num] = 1
    // this function return score for each update
    int score = 0;
    for(int num = 0; num < 9; ++num){
        for(int index = 0; index < 9; ++index){
            if(prefix_row[num][index] == 1){
                for(int i = 0; i < 9; ++i){
                    if(!(1 & (encode_table[index][i] >> (num + 1)))){
                        main_table[index][i] = (num + 1);
                        ++score;
                    }
                }
            }
            if(prefix_column[num][index] == 1){
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
    return score;
}
int main(){
    do{
        initiation();
        calculate();
    }
    while(update());
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            std::cout << main_table[i][j] << " ";
        }
        std::cout << std::endl;
    }
    //std::cout << prefix_block[4][0];
    return 0;
}
