#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "game.h"

class Game;

class Renderer {
  public:
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();
    
    void Render(Snake const &snake, SDL_Point const &food, Game &game);
    void UpdateWindowTitle(int score, int fps, int highest);

  private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    bool running = true;

    const std::size_t screen_width;
    const std::size_t screen_height;
    const std::size_t grid_width;
    const std::size_t grid_height;
};

#endif