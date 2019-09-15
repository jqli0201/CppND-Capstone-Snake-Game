#include "snake.h"
#include "renderer.h"

#include <cmath>
#include <iostream>

#include "SDL.h"

void Snake::Update(std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  frame_start = SDL_GetTicks();

  SDL_Point prev_cell {
    static_cast<int>(head_x),
    // We first capture the head's cell before updating.
    static_cast<int>(head_y)}; 
  UpdateHead();
  SDL_Point current_cell {
    static_cast<int>(head_x),
    static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the 
  // snake head has moved to a new cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }

  frame_end = SDL_GetTicks();

  // Keep track of how long each loop through the input/update/render cycle
  // takes.
  frame_count++;
  frame_duration = frame_end - frame_start;

  // If the time for this frame is too small (i.e. frame_duration is smaller
  // than the target ms_per_frame), delay the loop to achieve the correct
  // frame rate.
  if (frame_duration < target_frame_duration) {
    SDL_Delay(target_frame_duration - frame_duration);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;
    
    case Direction::kDown:
      head_y += speed;
      break;
    
    case Direction::kLeft:
      head_x -= speed;
      break;
    
    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the begining if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, 
                       SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  }
  else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

