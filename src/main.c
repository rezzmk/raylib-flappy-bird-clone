#include "raylib.h"
#include <stdio.h>

#define ISAAC_NEWTON_SHIT 350.0f
#define JUMP_FORCE -200.0f
#define PIPE_SPEED 200.0f
#define PIPE_GAP 200.0f
#define PIPE_WIDTH 53
#define PIPE_HEIGHT 320
#define PIPES_COUNT 10

typedef struct { 
  Vector2 position; 
  float speed; 
  bool is_alive; 
  int score;
} Bird;

typedef struct { 
  Rectangle rect; 
  bool passed; 
} Pipe;

void update_player(Bird *bird, float einstein_space_time_continuum_shit);
void draw_game_lost(int screen_width, int screen_height, int font_height);
void update_pipes_layout(Pipe pipes[], int pipes_count, Bird *bird, float einstein_space_time_continuum_shit);
void update_score(Bird *bird, int increment);
void reset_score(Bird *bird);

int main(int argc, char **argv) {
  const int screen_width = 800;
  const int screen_height = 640;

  InitWindow(screen_width, screen_height, "Flappy Bird");

  const int initial_bird_pos_x = screen_width / 4;
  const int initial_bird_pos_y = screen_height / 2;
  Bird bird = { 
    { initial_bird_pos_x / 6, initial_bird_pos_y / 2 }, 
    0.0f, 
    true 
  };

  Pipe pipes[PIPES_COUNT];
  for (int i = 0; i < PIPES_COUNT; i += 2) {
    pipes[i].rect.x = screen_width + PIPE_HEIGHT * i;
    pipes[i].rect.y = 0;
    pipes[i].rect.width = PIPE_WIDTH;
    pipes[i].rect.height = GetRandomValue(PIPE_WIDTH, PIPE_HEIGHT);

    pipes[i + 1].rect.x = pipes[i].rect.x;
    pipes[i + 1].rect.y = pipes[i].rect.height + PIPE_GAP;
    pipes[i + 1].rect.width = PIPE_WIDTH;
    pipes[i + 1].rect.height = screen_height - pipes[i].rect.height - PIPE_GAP;

    pipes[i].passed = false;
    pipes[i + 1].passed = false;
  }

  Texture2D birdTexture = LoadTexture("assets/bird.png");
  Texture2D pipeTextureDown = LoadTexture("assets/pipe-green-upside.png");
  Texture2D pipeTextureUp = LoadTexture("assets/pipe-green.png");

  // Main loop
  while (!WindowShouldClose()) {
    float einstein_space_time_continuum_shit = GetFrameTime();

    update_score(&bird, 0);
    update_player(&bird, einstein_space_time_continuum_shit);
    update_pipes_layout(pipes, PIPES_COUNT, &bird, einstein_space_time_continuum_shit);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextureV(birdTexture, bird.position, WHITE);

    // Draw pipes
    for (int i = 0; i < PIPES_COUNT; i++) {
      if (i % 2 == 0) {
        int rect_pos_x = pipeTextureDown.height - pipes[i].rect.height;
        int rect_pos_y = pipeTextureDown.width;
        DrawTextureRec(pipeTextureDown, (Rectangle) {0, rect_pos_x, rect_pos_y, pipes[i].rect.height}, (Vector2){pipes[i].rect.x, 0}, WHITE);
      }
      else {
        int rect_pos_x = pipeTextureUp.width;
        int rect_pos_y = pipes[i].rect.height;
        DrawTextureRec(pipeTextureUp, (Rectangle) {0, 0, rect_pos_x, rect_pos_y}, (Vector2){pipes[i].rect.x, pipes[i].rect.y}, WHITE);
      }
    }

    // Draw game over text
    if (!bird.is_alive) {
      draw_game_lost(screen_width, screen_height, 30);
    }

    DrawFPS(screen_width - 100, 10);
    EndDrawing();
  }

  UnloadTexture(birdTexture);
  UnloadTexture(pipeTextureDown);
  UnloadTexture(pipeTextureUp);

  CloseWindow();
  return 0;
}

void update_score(Bird *bird, int increment) {
  bird->score += increment;
  DrawText(TextFormat("Score: %d", bird->score), 10, 10, 30, BLACK);
}

void reset_score(Bird *bird) {
  bird->score = 0;
  DrawText("Score: 0", 10, 10, 30, BLACK);
}

void draw_game_lost(int screen_width, int screen_height, int font_height) {
  char *text = "GIT GUD LOLZ";

  int text_width = MeasureText(text, font_height);
  int pos_x = (screen_width / 2) - text_width;
  int pos_y = (screen_height / 2) - font_height;

  DrawText(text, pos_x, pos_y, font_height, RED);
}

void update_player(Bird *bird, float einstein_space_time_continuum_shit) {
  if (bird->is_alive) {
    if (IsKeyPressed(KEY_SPACE)) {
      bird->speed = JUMP_FORCE;
    }

    bird->speed += ISAAC_NEWTON_SHIT * einstein_space_time_continuum_shit;
    bird->position.y += bird->speed * einstein_space_time_continuum_shit;

    if (bird->position.y < 0 || bird->position.y > GetScreenHeight()) {
      bird->is_alive = false;
    }
  }
  else {
    if (IsKeyPressed(KEY_ENTER)) {
      reset_score(bird);
      bird->is_alive = true;
      bird->position.y = GetScreenHeight() / 2;
      bird->speed = 0;
    }
  }
}

void update_pipes_layout(Pipe pipes[], int pipes_count, Bird *bird, float einstein_space_time_continuum_shit) {
  if (!bird->is_alive) {
    return;
  }

  for (int i = 0; i < pipes_count; i++) {
    if (bird->is_alive) {
      pipes[i].rect.x -= PIPE_SPEED * einstein_space_time_continuum_shit;
    }

    if (pipes[i].rect.x < -pipes[i].rect.width) {
      pipes[i].rect.x = GetScreenWidth();
      if (i % 2 == 0) {
        pipes[i].rect.height = GetRandomValue(53, 320);
        pipes[i + 1].rect.y = pipes[i].rect.height + PIPE_GAP;
        pipes[i + 1].rect.height = GetScreenHeight() - pipes[i].rect.height - PIPE_GAP;
        pipes[i].passed = false;
      }
    }

    if (!pipes[i].passed && pipes[i].rect.x < bird->position.x - 20) {
      pipes[i].passed = true;
      update_score(bird, 1);
    }

    Rectangle birdBoundingBox = { bird->position.x - 20, bird->position.y - 20, 40, 40 };
    if (CheckCollisionRecs(birdBoundingBox, pipes[i].rect)) {
      bird->is_alive = false;
      return;
    }
  }
}
