#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Maze.h"
#include "utils.cpp"
#include "Segment.h"
#include "Ray.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#define WINDOW_TITLE "Maze"

#define SQUARE_WINDOW_SIZE 800
#define N_SQUARES 2

#define RAYS_LENGTH 100
#define N_RAYS 100

#define PI 3.1415
#define DEFAULT_FOV PI/2

#define DEFAULT_MAZE_SIZE 200
#define DEFAULT_CELL_SIZE 36

#define GENERATION_TIME true

int main(int argc, char *argv[]) {
    Game game(N_SQUARES, 
        SQUARE_WINDOW_SIZE,
        Size{(DEFAULT_MAZE_SIZE+2)*DEFAULT_CELL_SIZE,(DEFAULT_MAZE_SIZE + 2) * DEFAULT_CELL_SIZE },
        WINDOW_TITLE, 
        Coordinates{10,10}, 
        0.0f, 
        DEFAULT_FOV, 
        N_RAYS, 
        RAYS_LENGTH);

    Maze m({ DEFAULT_MAZE_SIZE, DEFAULT_MAZE_SIZE });

#if GENERATION_TIME
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    m.generate();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    long long ns = std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count();

    std::string seconds = "";
    char nextChar;
    while (ns > 0 || seconds.length() < 8) {
        nextChar = ns % 10 + 48;
        seconds = nextChar + seconds;
        ns /= 10;
        if (seconds.length() == 6) {
            seconds = ',' + seconds;
        }
    }
    std::cout << DEFAULT_MAZE_SIZE << "x" << DEFAULT_MAZE_SIZE << " Maze generation took " << seconds << " seconds\n";
        
    
#else
    m.generate();
#endif
    
    game.addWalls(m.getWalls({ DEFAULT_CELL_SIZE, DEFAULT_CELL_SIZE }, { DEFAULT_CELL_SIZE, DEFAULT_CELL_SIZE }));

    double targetFPS = 60.0;
    double nsPerFrame = 1000000000.0 / targetFPS;
    auto lastTime = std::chrono::steady_clock::now();
    double unprocessed = 0.0;
    double timeElapsed = 0.0;
    double totalSecondsElapsed = 0.0;

    int frame = 0;

    bool canUpdate = false;

    bool running = true;



    while (running) {
        auto now = std::chrono::steady_clock::now();
        timeElapsed = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count();
        
        unprocessed += timeElapsed / nsPerFrame;
        totalSecondsElapsed += timeElapsed/1000000000.0;
        lastTime = now;
        if (totalSecondsElapsed  >= 1.0) {
            std::cout << "FPS:" << double(frame)/ totalSecondsElapsed << std::endl;
            totalSecondsElapsed = 0.0;
            frame = 0;
        }

        if (unprocessed >= 1.0) {
            running = game.update();
            game.render();
            unprocessed = 0.0;
            frame++;
        }
    }

    return 0;
}