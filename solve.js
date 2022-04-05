
function bitCount (n) {
    n = n - ((n >> 1) & 0x55555555)
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333)
    return ((n + (n >> 4) & 0xF0F0F0F) * 0x1010101) >> 24
  }

function read_input(){
    for(let i = 0; i < 9; ++i){
        for(let j = 0; j < 9; ++j){
            if(document.getElementById(i * 9 + j).textContent == ""){
                mainTable[i][j] = 0;
                continue;
            }
            mainTable[i][j] = parseInt(document.getElementById(i * 9 + j).textContent);
        }
    }
}
function write_answer(){
    for(let i = 0; i < 9; ++i){
        for(let j = 0; j < 9; ++j){
            if(mainTable[i][j] == 0){
                document.getElementById(i * 9 + j).textContent = "";
                continue;
            }
            document.getElementById(i * 9 + j).textContent = mainTable[i][j];
        }
    }
}
function initiation(){
    for(let i = 0; i < 9; ++i){
        encodeRow[i] = 0;
        encodeColumn[i] = 0;
        encodeBlock[i] = 0;
        for(let j = 0; j < 9; ++j){
            encodeTable[i][j] = 0;
            prefixBlock[i][j] = 0;
            for(let num = 0; num < 9; ++num){
                prefixRow[i][j][num] = 0;
                prefixColumn[i][j][num] = 0;
            }
        }
    }
}
function overall_encode(){
    for(let i = 0; i < 9; ++i){
        for(let j = 0; j < 9; ++j){
            if(mainTable[i][j] == 0) continue;
            encodeTable[i][j] = (1 << (mainTable[i][j]));
            encodeRow[i] = encodeRow[i] | encodeTable[i][j];
            encodeColumn[j] = encodeColumn[j] | encodeTable[i][j];
            encodeBlock[blockIndex[i][j]] = encodeBlock[blockIndex[i][j]]  | encodeTable[i][j];
        }
    }
}
function cell_encode(){
    for(let i = 0; i < 9; ++i){
        for(let j = 0; j < 9; ++j){
            if(encodeTable[i][j] != 0){
                encodeTable[i][j] = 1022;
            }
            else{
                encodeTable[i][j] = encodeRow[i] | encodeColumn[j] | encodeBlock[blockIndex[i][j]];
            }
        }
    }
}
function compute_prefix(){
    for(let num = 0; num < 9; ++num){
        for(let i = 0; i < 9; ++i){
            for(let j = 0; j < 9; ++j){
                if(!(1 & (encodeTable[i][j] >> (num + 1)))){
                    prefixRow[num][i][j] = 1;
                    prefixColumn[num][j][i] = 1;
                    ++prefixBlock[num][blockIndex[i][j]];
                }
            }
        }
    }
    for(let num = 0; num < 9; ++num){
        for(let i = 0; i < 9; ++i){
            for(let j = 1; j < 9; ++j){
                prefixRow[num][i][j] += prefixRow[num][i][j - 1];
                prefixColumn[num][i][j] += prefixColumn[num][i][j - 1];
            }
        }
    }
}
function calculate(){
    //check if we can get more information from all possible blank from row and column be exactly in one block
    //this computation step can help strategy_1 go faster 2 - 3 times and find out some special cell
    //i think it is possible to loop this process several times to make the calculating process stronger
    compute_prefix();
    for(let num = 0; num < 9; ++num){
        for(let index = 0; index < 9; ++index){
            if(prefixColumn[num][index][8] > 0 && prefixColumn[num][index][8] < 4){
                //block 0
                if(prefixColumn[num][index][2] == prefixColumn[num][index][8]){
                    for(let i = 0; i < 9; ++i){
                        if(blockElements[Math.floor(index / 3)][i][1] == index) continue;
                        encodeTable[blockElements[Math.floor(index / 3)][i][0]][blockElements[Math.floor(index / 3)][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 1
                if(prefixColumn[num][index][5] -  prefixColumn[num][index][2] == prefixColumn[num][index][8]){
                    for(let i = 0; i < 9; ++i){
                        if(blockElements[Math.floor(index / 3) + 3][i][1] == index) continue;
                        encodeTable[blockElements[Math.floor(index / 3) + 3][i][0]][blockElements[Math.floor(index / 3) + 3][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 2
                if(prefixColumn[num][index][8] -  prefixColumn[num][index][5] == prefixColumn[num][index][8]){
                    for(let i = 0; i < 9; ++i){
                        if(blockElements[Math.floor(index / 3) + 6][i][1] == index) continue;
                        encodeTable[blockElements[Math.floor(index / 3) + 6][i][0]][blockElements[Math.floor(index / 3) + 6][i][1]] |= (1 << (num + 1));
                    }
                }
            }
            if(prefixRow[num][index][8] > 0 && prefixRow[num][index][8] < 4){
                //block 0
                if(prefixRow[num][index][2] == prefixRow[num][index][8]){
                    for(let i = 0; i < 9; ++i){
                        if(blockElements[(Math.floor(index / 3)) * 3][i][0] == index) continue;
                        encodeTable[blockElements[(Math.floor(index / 3)) * 3][i][0]][blockElements[(Math.floor(index / 3)) * 3][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 1
                if(prefixRow[num][index][5] -  prefixRow[num][index][2] == prefixRow[num][index][8]){
                    for(let i = 0; i < 9; ++i){
                        if(blockElements[(Math.floor(index / 3)) * 3 + 1][i][0] == index) continue;
                        encodeTable[blockElements[(Math.floor(index / 3)) * 3 + 1][i][0]][blockElements[(Math.floor(index / 3)) * 3 + 1][i][1]] |= (1 << (num + 1));
                    }
                }
                //block 2
                if(prefixRow[num][index][8] -  prefixRow[num][index][5] == prefixRow[num][index][8]){
                    for(let i = 0; i < 9; ++i){
                        if(blockElements[(Math.floor(index / 3)) * 3 + 2][i][0] == index) continue;
                        encodeTable[blockElements[(Math.floor(index / 3)) * 3 + 2][i][0]][blockElements[(Math.floor(index / 3)) * 3 + 2][i][1]] |= (1 << (num + 1));
                    }
                }
            }
        }
    }
    //reseting the prefix after update
    for(let i = 0; i < 9; ++i){
        for(let j = 0; j < 9; ++j){
            for(let num = 0; num < 9; ++num){
                prefixRow[i][j][num] = 0;
                prefixColumn[i][j][num] = 0;
            }
        }
    }
    compute_prefix();
}
function update(){
    //this strategy will look into each number and find any rows, columns, blocks that have exactly one possible blank to fill that number
    let score = 0;
    for(let num = 0; num < 9; ++num){
        for(let index = 0; index < 9; ++index){
            if(prefixRow[num][index][8] == 1){
                for(let i = 0; i < 9; ++i){
                    if(!(1 & (encodeTable[index][i] >> (num + 1)))){
                        mainTable[index][i] = (num + 1);
                        ++score;
                    }
                }
            }
            if(prefixColumn[num][index][8] == 1){
                for(let i = 0; i < 9; ++i){
                    if(!(1 & (encodeTable[i][index] >> (num + 1)))){
                        mainTable[i][index] = (num + 1);
                        ++score;
                    }
                }
            }
            if(prefixBlock[num][index] == 1){
                for(let i = 0; i < 9; ++i){
                    if(!(1 & (encodeTable[blockElements[index][i][0]][blockElements[index][i][1]] >> (num + 1)))){
                        mainTable[blockElements[index][i][0]][blockElements[index][i][1]] = (num + 1);
                        ++score;
                    }
                }
            }
        }
    }
    for(let i = 0; i < 9; ++i){
        for(let j = 0; j < 9; ++j){
            if(bitCount(encodeTable[i][j]) == 8){
                for(let num = 0; num < 9; ++num){
                    if(!(1 & (encodeTable[i][j] >> (num + 1)))){
                        mainTable[i][j] = num + 1;
                        ++score;
                    }
                }
            }
        }
    }
    return score;
}
function solve(){
    read_input();
    let cnt = 0;
    do{
        initiation();
        overall_encode();
        cell_encode();
        calculate();
        ++cnt;
    }
    while(update());
    write_answer();
    console.log("HELLO");
}
document.getElementById("solve-btn").onclick = solve;
console.log("HELLO");
