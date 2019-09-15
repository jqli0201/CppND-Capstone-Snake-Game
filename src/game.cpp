#include "game.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <functional>

#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : _snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
        PlaceFood();
      }

void Game::Run(Controller& controller,
               Renderer& renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  std::vector<std::thread> threads;
  threads.emplace_back(std::thread(&Game::Update, this, target_frame_duration));
  threads.emplace_back(std::thread(&Renderer::Render, 
                                   &renderer, 
                                   std::ref(_snake), 
                                   std::ref(_food),
                                   std::ref(*this)));

  
  while (running) {
    frame_start = SDL_GetTicks();
    controller.HandleInput(running, _snake);
    

    // Input, Update, Render - the main game loop.
    // controller.HandleInput(running, snake);
    // Update();
    // renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    // frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count, highest);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is smaller
    // than the target ms_per_frame), delay the loop to achieve the correct
    // frame rate.
    // if (frame_duration < target_frame_duration) {
    //   SDL_Delay(target_frame_duration - frame_duration);
    // }
    
    for (auto &t : threads) t.join();
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by 
    // a snake item before placing food.
    if (!_snake.SnakeCell(x, y)) {
      _food.x = x;
      _food.y = y;
      return;
    }
  }
}

void Game::Update(std::size_t target_frame_duration) {
  while (true) {
    if (!_snake.alive) return;

    _snake.Update(target_frame_duration);

    int new_x = static_cast<int>(_snake.head_x);
    int new_y = static_cast<int>(_snake.head_y);

    // Check if there's food over here
    if (_food.x == new_x && _food.y == new_y) {
      score++;
      PlaceFood();
      // Grow sanke and increase speed.
      _snake.GrowBody();
      _snake.speed += 0.02;
    }

    // Sleep 1ms between iterations
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return _snake.size; }
int Game::GetHighest() { return highest; }
bool Game::GetStatus() { return running; }