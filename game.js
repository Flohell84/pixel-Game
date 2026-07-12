const canvas = document.getElementById("game");
const ctx = canvas.getContext("2d");
const message = document.getElementById("message");

const tile = 32;
const map = [
  "####################",
  "#..T......##......N#",
  "#.####....##..###..#",
  "#......T..........##",
  "#..#######..###....#",
  "#...........###....#",
  "#.######...........#",
  "#..............#####",
  "#..S...............#",
  "####################",
];

const world = map.map((row) => row.split(""));
const rows = world.length;
const cols = world[0].length;

const player = findTile("S") || { x: 2, y: 8 };
world[player.y][player.x] = ".";

const npc = findTile("N") || { x: 18, y: 1 };
world[npc.y][npc.x] = ".";

let won = false;
let interactHint = false;

window.addEventListener("keydown", (event) => {
  if (won) return;

  const key = event.key.toLowerCase();
  const movement = {
    arrowup: { x: 0, y: -1 },
    w: { x: 0, y: -1 },
    arrowdown: { x: 0, y: 1 },
    s: { x: 0, y: 1 },
    arrowleft: { x: -1, y: 0 },
    a: { x: -1, y: 0 },
    arrowright: { x: 1, y: 0 },
    d: { x: 1, y: 0 },
  }[key];

  if (movement) {
    movePlayer(movement.x, movement.y);
    return;
  }

  if (key === "e" && interactHint) {
    won = true;
    message.textContent = "Du hast mit dem Dorfältesten gesprochen. Abenteuer geschafft!";
  }
});

function movePlayer(dx, dy) {
  const nextX = player.x + dx;
  const nextY = player.y + dy;
  if (isBlocked(nextX, nextY)) return;

  player.x = nextX;
  player.y = nextY;
}

function isBlocked(x, y) {
  const outOfBounds = x < 0 || y < 0 || x >= cols || y >= rows;
  if (outOfBounds) return true;
  return world[y][x] === "#";
}

function nearNpc() {
  const distance = Math.abs(player.x - npc.x) + Math.abs(player.y - npc.y);
  return distance === 1;
}

function findTile(symbol) {
  for (let y = 0; y < rows; y += 1) {
    for (let x = 0; x < cols; x += 1) {
      if (world[y][x] === symbol) {
        return { x, y };
      }
    }
  }
  return null;
}

function draw() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  for (let y = 0; y < rows; y += 1) {
    for (let x = 0; x < cols; x += 1) {
      drawTile(x, y, world[y][x]);
    }
  }

  drawNpc();
  drawPlayer();
  updateMessage();
  requestAnimationFrame(draw);
}

function drawTile(x, y, symbol) {
  const px = x * tile;
  const py = y * tile;

  if (symbol === "#") {
    ctx.fillStyle = "#3b3f58";
    ctx.fillRect(px, py, tile, tile);
    ctx.fillStyle = "#4f5578";
    ctx.fillRect(px + 4, py + 4, tile - 8, tile - 8);
    return;
  }

  ctx.fillStyle = "#386641";
  ctx.fillRect(px, py, tile, tile);

  if (symbol === "T") {
    ctx.fillStyle = "#6a994e";
    ctx.fillRect(px + 8, py + 4, 16, 16);
    ctx.fillStyle = "#7f5539";
    ctx.fillRect(px + 13, py + 20, 6, 10);
  }
}

function drawPlayer() {
  const px = player.x * tile;
  const py = player.y * tile;
  ctx.fillStyle = "#0fa3b1";
  ctx.fillRect(px + 8, py + 6, 16, 20);
  ctx.fillStyle = "#f7f4ea";
  ctx.fillRect(px + 11, py + 9, 10, 6);
}

function drawNpc() {
  const px = npc.x * tile;
  const py = npc.y * tile;
  ctx.fillStyle = "#ffb703";
  ctx.fillRect(px + 8, py + 6, 16, 20);
  ctx.fillStyle = "#fefae0";
  ctx.fillRect(px + 11, py + 9, 10, 6);
}

function updateMessage() {
  interactHint = nearNpc();
  if (won) return;

  if (interactHint) {
    message.textContent = "Drücke E, um mit dem Dorfältesten zu reden.";
  } else {
    message.textContent = "Erkunde die Karte und finde den NPC im Norden.";
  }
}

draw();
