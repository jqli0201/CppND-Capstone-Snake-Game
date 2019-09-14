#ifdef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer {
  public:
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();
    
    void 
}