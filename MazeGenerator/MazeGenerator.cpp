#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Maze.h"
#include "utils.cpp"
#include "Segment.h"
#include "Ray.h"
#include "Player.h"
#include "Entity.h"
#include "Game.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// WINDOW 
#define WINDOW_TITLE "Maze"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 576

// RAYCASTER
#define RAYS_LENGTH 100
#define N_RAYS 200
#define DEFAULT_FOV 3.1415/2

// MAZE
#define DEFAULT_MAZE_SIZE 1
#define DEFAULT_CELL_SIZE 36
#define DEFAULT_WALL_THICKNESS 2

#define DEBUG_INFO false

int main(int argc, char *argv[]) {
    Game game({WINDOW_WIDTH, WINDOW_HEIGHT},
        WINDOW_TITLE,
        0.0f, 
        DEFAULT_FOV, 
        N_RAYS, 
        RAYS_LENGTH,
        {DEFAULT_MAZE_SIZE, DEFAULT_MAZE_SIZE},
        2,
        { DEFAULT_CELL_SIZE , DEFAULT_CELL_SIZE},
        DEFAULT_WALL_THICKNESS,
        1);


    /*Entity s({DEFAULT_CELL_SIZE*3.5, DEFAULT_CELL_SIZE*3.5}, DEFAULT_CELL_SIZE/3, 0.0);
    game.addCollectible(std::make_shared<Entity>(s));*/

    // game loop info
    double targetFPS = 60.0;
    double nsPerFrame = 1000000000.0 / targetFPS;
    auto lastTime = std::chrono::steady_clock::now();
    double unprocessed = 0.0;
    double timeElapsed = 0.0;

#if DEBUG_INFO
    double totalSecondsElapsed = 0.0;
    int frame = 0;
#endif
    bool canUpdate = false;

    bool running = true;


    while (running) {
        auto now = std::chrono::steady_clock::now();
        timeElapsed = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTime).count();
        
        unprocessed += timeElapsed / nsPerFrame;
#if DEBUG_INFO
        totalSecondsElapsed += timeElapsed/1000000000.0;
        if (totalSecondsElapsed >= 1.0) {
            std::cout << "FPS:" << double(frame) / totalSecondsElapsed << std::endl;
            totalSecondsElapsed = 0.0;
            frame = 0;
        }
#endif
        lastTime = now;

        if (unprocessed >= 1.0) {
            running = game.update(unprocessed*((double)DEFAULT_CELL_SIZE/36));
            game.render();
            unprocessed = 0.0;
#if DEBUG_INFO
            frame++;
#endif
        }
    }

    return 0;
}