#include "Maze.h"
#include <time.h>
#include <queue>
#include "Wall.h"

#define DEFAULT_MAZE_WIDTH 10
#define DEFAULT_MAZE_HEIGHT 10

#define MAX_WEIGHT 3


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

	srand((unsigned int)time(NULL));
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

	this->walls.push_back({ {0,0},{double(this->width),0} });
	this->walls.push_back({ {0,double(this->height)},{double(this->width),double(this->height)} });
	this->walls.push_back({ {0,1},{0,double(this->height)} });
	this->walls.push_back({ {double(this->width),0},{double(this->width), double(this->height-1)} });

	this->walls.push_back({ {-1,0}, {0,0} });
	this->walls.push_back({ {-1,0}, {-1,1} });
	this->walls.push_back({ {-1,1}, {0,1} });

	this->walls.push_back({ {double(this->width),double(this->height)}, {double(this->width+1),double(this->height)} });
	this->walls.push_back({ {double(this->width + 1),double(this->height)}, {double(this->width + 1),double(this->height-1)} });
	this->walls.push_back({ {double(this->width + 1),double(this->height - 1)}, {double(this->width),double(this->height - 1)} });

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			if (x < this->width - 1 && !toKeep[x][y].east) {
				this->walls.push_back({ {double(x + 1),double(y)}, {double(x + 1),double(y + 1)}});
			}
			if (y < this->height - 1 && !toKeep[x][y].south) {
				this->walls.push_back({ {double(x),double(y + 1)}, {double(x + 1),double(y + 1)} });
				
			}
		}
	}
}

void Maze::setSize(Size size) {
	this->width = size.x;
	this->height = size.y;
}

std::vector<std::shared_ptr<Segment>> Maze::getWalls(Size cellSize, Coordinates offset, double wallThinkness) {
	std::vector<std::shared_ptr<Segment>> walls;

	Line t;

	if (wallThinkness == 0.0f) {
		
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
	}
	else {
		Line t2;
		wallThinkness /= 2.0f;
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

			if (t.p1.x == t.p2.x) {
				if (t.p2.y < t.p1.y) {
					Coordinates temp = t.p1;
					t.p1 = t.p2;
					t.p2 = temp;
				}
				t.p1.x -= wallThinkness;
				t.p1.y -= wallThinkness;
				t.p2.x -= wallThinkness;
				t.p2.y += wallThinkness;

				t2.p1.x = t.p1.x + wallThinkness*2;
				t2.p1.y = t.p1.y;
				t2.p2.x = t.p2.x + wallThinkness*2;
				t2.p2.y = t.p2.y;
			}
			else {
				if (t.p2.x < t.p1.x) {
					Coordinates temp = t.p1;
					t.p1 = t.p2;
					t.p2 = temp;
				}
				t.p1.x -= wallThinkness;
				t.p1.y -= wallThinkness;
				t.p2.x += wallThinkness;
				t.p2.y -= wallThinkness;

				t2.p1.x = t.p1.x;
				t2.p1.y = t.p1.y + wallThinkness * 2;
				t2.p2.x = t.p2.x;
				t2.p2.y = t.p2.y + wallThinkness * 2;
			}

			Wall w1(t.p1, t.p2);
			walls.push_back(std::make_shared<Segment>(w1));
			Wall w2(t2.p1, t2.p2);
			walls.push_back(std::make_shared<Segment>(w2));
			Wall w3(t.p1, t2.p1);
			walls.push_back(std::make_shared<Segment>(w3));
			Wall w4(t.p2, t2.p2);
			walls.push_back(std::make_shared<Segment>(w4));
		}
	}
	return walls;
}