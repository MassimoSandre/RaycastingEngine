#include "Maze.h"
#include <time.h>
#include <queue>
#include "Wall.h";

#define DEFAULT_MAZE_WIDTH 10
#define DEFAULT_MAZE_HEIGHT 10

#define MAX_WEIGHT 100

//// operator overloading for priority queue
//bool operator<(const QueueElement& qe1, const QueueElement& qe2) {
//	return qe1.distance < qe1.distance;
//}
//bool operator>(const QueueElement& qe1, const QueueElement& qe2) {
//	return qe1.distance > qe1.distance;
//}

Maze::Maze() {
	this->width = DEFAULT_MAZE_WIDTH;
	this->height = DEFAULT_MAZE_HEIGHT;
}

Maze::Maze(Size size) {
	this->width = size.x;
	this->height = size.y;
}


void  Maze::generate() {
	this->walls.clear();

	std::vector<std::vector<CellWeight>> toKeep(this->width, std::vector<CellWeight>(this->height, { false,false }));

	srand(time(NULL));
	std::vector<std::vector<CellWeight>> weights(this->width, std::vector<CellWeight>(this->height, {1,1}));
	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			if (x < this->width - 1) {
				weights[x][y].east = (rand() % MAX_WEIGHT) + 1;
			}
			
			if (y < this->height - 1) {
				weights[x][y].south = (rand() % MAX_WEIGHT) + 1;
			}
		}
	}
	/*
	std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<QueueElement>> q;
	q.push({0,{0,0},{-1,-1}});
	std::vector<std::vector<long long>> distances(this->height, std::vector<long long>(this->width, LLONG_MAX));
	//distances[0][0] = 0;

	std::vector<std::vector<Cell>> fathers(this->height, std::vector<Cell>(this->width, {-1,-1}));

	while (!q.empty()) {
		QueueElement e = q.top();
		q.pop();
		
		if (e.distance >= distances[e.cell.x][e.cell.y]) continue;
		
		
		if (e.father.x > 0 && e.father.y > 0) {
			if (fathers[e.father.x][e.father.y].x != e.cell.x || fathers[e.father.x][e.father.y].y != e.cell.y) {
				fathers[e.cell.x][e.cell.y] = e.father;
				distances[e.cell.x][e.cell.y] = e.distance;
			}
		}
		else {
			distances[e.cell.x][e.cell.y] = e.distance;
		}
		
		

		if (e.cell.x > 0) {
			long long y = weights[e.cell.x-1][e.cell.y].east;
			if (distances[e.cell.x - 1][e.cell.y] > y) {
				q.push(QueueElement{ y, {e.cell.x - 1,e.cell.y}, e.cell });
			}
		}
		if (e.cell.x < this->width - 1) {
			long long y = weights[e.cell.x][e.cell.y].east;
			if (distances[e.cell.x + 1][e.cell.y] > y) {
				q.push(QueueElement{ y, {e.cell.x + 1,e.cell.y}, e.cell });
			}
		}
		if (e.cell.y > 0) {
			long long y = weights[e.cell.x][e.cell.y-1].south;
			if (distances[e.cell.x][e.cell.y-1] > y) {
				q.push(QueueElement{ y, {e.cell.x,e.cell.y - 1}, e.cell });
			}
		}
		if (e.cell.y < this->height - 1) {
			long long y = weights[e.cell.x][e.cell.y].south;
			if (distances[e.cell.x][e.cell.y+1] > y) {
				q.push(QueueElement{ y, {e.cell.x,e.cell.y + 1}, e.cell });
			}
		}

		

	}

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			std::cout << "(" << fathers[x][y].x << "," << fathers[x][y].y << ")";
		}
		std::cout << std::endl;
	}

	this->walls.push_back({ {0,0},{this->width,0} });
	this->walls.push_back({ {0,this->height},{this->width,this->height} });
	this->walls.push_back({ {0,0},{0,this->height} });
	this->walls.push_back({ {this->width,0},{this->width, this->height} });

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			if (x < this->width - 1) {
				if (!((fathers[x][y].x == x + 1 && fathers[x][y].y == y) || (fathers[x + 1][y].x == x && fathers[x][y].y == y))) {
					this->walls.push_back({ {x + 1,y}, {x + 1,y + 1} });
				}
			}
			if (y < this->height - 1) {
				if (!((fathers[x][y].x == x && fathers[x][y].y == y + 1) || (fathers[x][y + 1].x == x && fathers[x][y].y == y))) {
					this->walls.push_back({ {x,y + 1}, {x + 1,y + 1} });
				}
			}
		}
	}*/

	int v = this->width * this->height;
	std::vector<std::vector<bool>> selected(this->width, std::vector<bool>(this->height, false));

	int nArcs = 0;
	selected[0][0] = true;
	Cell c1, c2;

	while(nArcs < v-1) {
		long long minimum = LLONG_MAX;
		c1 = { -1,-1 };
		c2 = { -1,-1 };

		for (int x = 0; x < this->width; x++) {
			for (int y = 0; y < this->height; y++) {
				if (selected[x][y]) {
					if (x > 0 && !selected[x - 1][y]) {
						if (minimum > weights[x - 1][y].east) {
							minimum = weights[x - 1][y].east;
							c1 = { x,y };
							c2 = { x - 1,y };
						}
					}
					if (x < this->width - 1 && !selected[x + 1][y]) {
						if (minimum > weights[x][y].east) {
							minimum = weights[x][y].east;
							c1 = { x,y };
							c2 = { x + 1,y };
						}
					}
					if (y > 0 && !selected[x][y - 1]) {
						if (minimum > weights[x][y-1].south) {
							minimum = weights[x][y - 1].south;
							c1 = { x,y };
							c2 = { x,y-1 };
						}
					}
					if (y < this->height - 1 && !selected[x][y + 1]) {
						if (minimum > weights[x][y].south) {
							minimum = weights[x][y].south;
							c1 = { x,y };
							c2 = { x,y+1 };
						}
					}
				}
			}
		}

		if (c2.x != -1 && c2.y != -1) {
			selected[c2.x][c2.y] = true;

			if (!(c1.x < c2.x || c1.y < c2.y)) {
				Cell temp = c1;
				c1 = c2;
				c2 = temp;
			}
			
			if (c1.x == c2.x) {
				toKeep[c1.x][c1.y].south = true;
			}
			else {
				toKeep[c1.x][c1.y].east = true;
			}

		}
		nArcs++;
	}

	this->walls.push_back({ {0,0},{float(this->width),0} });
	this->walls.push_back({ {0,float(this->height)},{float(this->width),float(this->height)} });
	this->walls.push_back({ {0,1},{0,float(this->height)} });
	this->walls.push_back({ {float(this->width),0},{float(this->width), float(this->height-1)} });

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			if (x < this->width - 1 && !toKeep[x][y].east) {
				this->walls.push_back({ {float(x + 1),float(y)}, {float(x + 1),float(y + 1)}});
			}
			if (y < this->height - 1 && !toKeep[x][y].south) {
				this->walls.push_back({ {float(x),float(y + 1)}, {float(x + 1),float(y + 1)} });
				
			}
		}
	}
}

void Maze::setSize(Size size) {
	this->width = size.x;
	this->height = size.y;
}

std::vector<std::shared_ptr<Segment>> Maze::getWalls(Size cellSize, Coordinates offset) {
	std::vector<std::shared_ptr<Segment>> walls;
	Line t;
	for (int i = 0; i < this->walls.size(); i++) {
		t = this->walls[i];
		t.p1.x *= cellSize.x;
		t.p1.y *= cellSize.y;
		t.p2.x *= cellSize.x;
		t.p2.y *= cellSize.y;

		t.p1.x += offset.x;
		t.p1.y += offset.y;
		t.p2.x += offset.x;
		t.p2.y += offset.y;

		Wall w(t.p1, t.p2);
		walls.push_back(std::make_shared<Segment>(w));
	}
	return walls;
}