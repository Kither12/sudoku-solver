var curCeil = null;
var elements = document.getElementsByClassName("ceil");
function updateCeil(num){
    if(curCeil != null){
        if(num == 0){
            curCeil.textContent = "";
        }
        else{
            curCeil.textContent = num;
        }
        read_input();
        initiation();
        overall_encode();
        for(let id = 0; id < 81; ++id){
            let rowId = (id - id % 9) / 9;
            let columnId = id % 9;
            let blockId = blockIndex[rowId][columnId];
            let code =parseInt(document.getElementById(id).textContent);
            if((1 & (encodeRow[rowId] >> code)) || (1 & (encodeColumn[columnId] >> code)) || (1 & (encodeBlock[blockId] >> code))){
                document.getElementById(id).classList.add('error');
            }
            else{
                document.getElementById(id).classList.remove('error');
            }
            let cnt = 0;
            for(let i = 0; i < 9; ++i){
                if(document.getElementById(id - id % 9 + i).textContent == code){
                    ++cnt;
                }
                if(document.getElementById(id % 9 + 9 * i).textContent == code){
                    ++cnt;
                }
                if(document.getElementById(blockElements[blockId][i][0] * 9 + blockElements[blockId][i][1]).textContent == code){
                    ++cnt;
                }
            }
            if(cnt == 3){
                document.getElementById(id).classList.remove('error');
            }
        }
    }
};

document.getElementById("container").addEventListener("click", 
    e => {
        if(e.target === e.currentTarget){
            if(curCeil != null){
                curCeil.classList.remove("clicked");
            }
            curCeil = null;
        }
    }, false);

var getCeil = function(){
    if(curCeil != null){
        curCeil.classList.remove("clicked");
    }
    curCeil = this;
    curCeil.classList.add("clicked");
};

for (let i = 0; i < elements.length; i++) {
    elements[i].addEventListener('click', getCeil, false);
    elements[i].id = i;
}

document.addEventListener('keydown', function(event) {
    if(event.keyCode > 48 && event.keyCode <= 57) {
        updateCeil(event.keyCode - 48);
    }
    else if(event.keyCode > 96 && event.keyCode <= 105){
        updateCeil(event.keyCode - 96);
    }
    else if(event.keyCode == 37){
        if(curCeil != null){
            curCeil.classList.remove("clicked");
        }
        let id = parseInt(curCeil.id);
        curCeil = document.getElementById(id - id % 9 + ( - 1 + id % 9 + 9) % 9);;
        curCeil.classList.add("clicked");
    }
    else if(event.keyCode == 38){
        if(curCeil != null){
            curCeil.classList.remove("clicked");
        }
        curCeil = document.getElementById((parseInt(curCeil.id) + 72) % 81);
        curCeil.classList.add("clicked");
    }
    else if(event.keyCode == 39){
        if(curCeil != null){
            curCeil.classList.remove("clicked");
        }
        let id = parseInt(curCeil.id);
        curCeil = document.getElementById(id - id % 9 + (1 + id % 9) % 9);
        curCeil.classList.add("clicked");
    }
    else if(event.keyCode == 40){
        if(curCeil != null){
            curCeil.classList.remove("clicked");
        }
        curCeil = document.getElementById((parseInt(curCeil.id) + 9) % 81);
        curCeil.classList.add("clicked");
    }
    else if(event.keyCode == 27){
        updateCeil(0);
    }
});