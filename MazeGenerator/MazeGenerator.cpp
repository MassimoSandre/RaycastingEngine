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

#define PI 3.1415

int main(void)
{
    Renderer r(800,800,"Maze");
    Maze m({ 100,100 });
    m.generate();
    r.addWalls(m.getWalls({ 6,6 }, { 100, 100 }));

    do {
        
    } while (r.update());

    return 0;
}