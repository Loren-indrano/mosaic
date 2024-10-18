Module.onRuntimeInitialized = () => { start(); }

const EMPTY = 0;
const WHITE = 1;
const BLACK = 2;
const UNCONSTRAINED = -1;
const FULL = 0;
const ORTHO = 1;
const FULL_EXCLUDE = 2;
const ORTHO_EXCLUDE = 3;
const ERROR = 0;
const UNSATISFIED = 1;
const SATISFIED = 2;

let col2str = [" ", "□", "■"];
let num2str = [
    ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"],     // empty
    ["🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"],  // white
    ["⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"],     // black
];

function drawGame(g) {
    var canvas = document.getElementById('gameCanvas');
    canvas.width = window.innerWidth/2;
    canvas.height = window.innerWidth/2;
    var ctx = canvas.getContext('2d');
    var cellSize = canvas.width / Module._nb_cols(g);

    for (var row = 0; row < Module._nb_rows(g); row++) {
        for (var col = 0; col < Module._nb_cols(g); col++) {
            var color = Module._get_color(g, row, col);
            var n = Module._get_constraint(g, row, col);
            var status = Module._get_status(g, row, col);

            ctx.fillStyle = (color === EMPTY) ? '#8FC7D4' : (color === WHITE) ? 'white' : 'black';
            ctx.fillRect(col * cellSize, row * cellSize, cellSize, cellSize);

            if (status===ERROR){
                ctx.strokeStyle = 'red';
            }
            else{
                ctx.strokeStyle = 'black';
            }
            ctx.strokeRect(col * cellSize, row * cellSize, cellSize, cellSize);

            if (n !== UNCONSTRAINED) {
                ctx.fillStyle = (color === EMPTY) ? 'black' : (color === WHITE) ? 'black' : 'white';
                ctx.textAlign = "center";
                ctx.textBaseline = "middle";
                ctx.fillText(n.toString(), col * cellSize + cellSize / 2 , row * cellSize + cellSize / 2);
            }
        }
    }
}

function handleClick(g, event) {
    var canvas = document.getElementById('gameCanvas');
    //var rect = canvas.getBoundingClientRect();
    var cellSize = canvas.width / Module._nb_cols(g);
    let x = event.offsetX;
    let y = event.offsetY;

    var col = Math.floor(x / cellSize);
    var row = Math.floor(y / cellSize);

    var color = Module._get_color(g, row, col);
    var newColor = (color === BLACK) ? EMPTY : (color === EMPTY) ? WHITE : BLACK;

    Module._play_move(g, row, col, newColor);
    drawGame(g);
}

function start() {
    console.log("call start routine");
    //var g = Module._new_default();
    var g = Module._new_random(6, 6, false, FULL, 0.6, 0.5);
    drawGame(g);
    document.getElementById('gameCanvas').addEventListener('click', function (event) {
        handleClick(g, event);
    });
    Module._delete(g);
}

