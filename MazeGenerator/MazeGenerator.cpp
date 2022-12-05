#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Maze.h"
#include "utils.cpp"
#include "Segment.h"
#include "Ray.h"
#include "Raycaster.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#define SQUARE_WINDOW_SIZE 800
#define N_SQUARES 2
#define DEFAULT_MAZE_SIZE 16
#define DEFAULT_MARGIN 100

#define GENERATION_TIME true

int main(int argc, char *argv[]) {
    Renderer r(SQUARE_WINDOW_SIZE*N_SQUARES,SQUARE_WINDOW_SIZE,"Maze");

    int mazeSize = DEFAULT_MAZE_SIZE;
    int cellSize = -1;

    if (argc == 3) {
        mazeSize = std::stoi(argv[1]);
        cellSize = std::stoi(argv[2]);
    }
    else if (argc == 2) {
        mazeSize = std::stoi(argv[1]);
    }

    if (cellSize == -1) {
        cellSize = (SQUARE_WINDOW_SIZE - 2 * DEFAULT_MARGIN) / mazeSize;
    }

    float margin = (SQUARE_WINDOW_SIZE - mazeSize * cellSize)/2;

    Maze m({ mazeSize, mazeSize });

#if GENERATION_TIME
  
    m.setSize({ mazeSize,mazeSize });
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
    std::cout << mazeSize << "x" << mazeSize << " Maze generation took " << seconds << " seconds\n";
        
    
#else
    m.generate();
#endif

    r.addWalls(m.getWalls({ cellSize,cellSize }, { margin, margin }));

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
            running = r.update();
            unprocessed = 0.0;
            frame++;
        }
    }

    return 0;
}