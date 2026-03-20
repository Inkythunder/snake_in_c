#include <raylib.h>
#include "linked_list_generic.h"
#include <stdio.h>
#include <string.h>


static const float UI_HEIGHT = 100;
static const float BORDER_SIZE = 10;
static const float PLAY_AREA_WIDTH = 720;
static const float PLAY_AREA_HEIGHT = 720;
static const float NUMBER_OF_COLUMNS = 20;
static const int32_t PIXEL_SIZE = PLAY_AREA_WIDTH / NUMBER_OF_COLUMNS;
static const int32_t FRUIT_SIZE = PIXEL_SIZE * 0.8;
static const int TICK_SPEED = 5;

typedef struct {
    int32_t x;
    int32_t y;
} i_vect;

typedef enum {
    up, down, left, right
} Direction;

typedef struct {
    struct LinkedList* list;
    i_vect fruit_pos;
    Direction current_direction;
    Direction direction_last_moved_in;
    int game_over;
    int32_t score;
    int begin;
} State;

int check_point_inside_snake(State* state, i_vect point) {

    LinkedListIterator iterator = ll_iterate(state->list);

    while(ll_iterator_has_next(&iterator)) {

        i_vect* pos = ll_iterator_next(&iterator);

        if(pos->x == point.x && pos->y == point.y) {
            return 1;
        }
    }
    return 0;
}

i_vect place_fruit(State* state) {

    while(1) {

        i_vect proposed = {rand() % (0, (int)NUMBER_OF_COLUMNS), rand() % (0, (int)NUMBER_OF_COLUMNS)};

        if(!check_point_inside_snake(state, proposed)) {
            return proposed;
        }
    }
}

void draw_fruit(State* state) {
    DrawRectangle(
        state->fruit_pos.x * PIXEL_SIZE + BORDER_SIZE,
        state->fruit_pos.y * PIXEL_SIZE + BORDER_SIZE,
        FRUIT_SIZE,
        FRUIT_SIZE,
        YELLOW
    );
}

void draw_snake(struct LinkedList* list, int32_t body_size) {

    LinkedListIterator iterator = ll_iterate(list);
    while(ll_iterator_has_next(&iterator)) {

        i_vect* pos = ll_iterator_next(&iterator);
        DrawRectangle(
            pos->x * body_size + BORDER_SIZE,
            pos->y * body_size + BORDER_SIZE,
            body_size,
            body_size,
            MAROON
        );
    }
}

void draw_UI(void) {

    // Draw border
    DrawRectangle(0, 0, GetScreenWidth(), BORDER_SIZE, BLACK);
    DrawRectangle(0, GetScreenHeight() - BORDER_SIZE, GetScreenWidth(), BORDER_SIZE, BLACK);
    DrawRectangle(0, 0, BORDER_SIZE, GetScreenHeight(), BLACK);
    DrawRectangle(GetScreenWidth() - BORDER_SIZE, 0, BORDER_SIZE, GetScreenHeight(), BLACK);

    // Draw UI separator
    DrawRectangle(0, PLAY_AREA_HEIGHT + BORDER_SIZE, GetScreenWidth(), BORDER_SIZE, BLACK);

    // Draw instructions
    DrawText("Arrow Keys: Move", BORDER_SIZE + 5, PLAY_AREA_HEIGHT + BORDER_SIZE*2 + 5, 20, DARKGRAY);
    DrawText("R: Restart", BORDER_SIZE + 5, PLAY_AREA_HEIGHT + BORDER_SIZE*2 + 50, 20, DARKGRAY);

}

void move_snake(State* state) {

    i_vect* head_pos = (i_vect*)ll_get(state->list, 0);
    i_vect new_pos = {head_pos->x, head_pos->y};

    switch (state->current_direction) {
        case up:
            new_pos.y -= 1;
            break;
        case down:
            new_pos.y += 1;
            break;
        case left:
            new_pos.x -= 1;
            break;
        case right:
            new_pos.x += 1;
            break;
    }
    state->direction_last_moved_in = state->current_direction;

    // Check for any self chomping
    if(check_point_inside_snake(state, new_pos)) {
        state->game_over = 1;
        return;
    }

    // Check for wandering off
    if (new_pos.x < 0 || new_pos.x >= NUMBER_OF_COLUMNS || new_pos.y < 0 || new_pos.y >= NUMBER_OF_COLUMNS){
        state->game_over = 1;
        return;
    }

    ll_insert_first(state->list, &new_pos);

    // Check for any tasty snacks
    if(!(new_pos.x == state->fruit_pos.x && new_pos.y == state->fruit_pos.y)) {
        ll_remove_last(state->list);
    } else {
        state->score++;
        state->fruit_pos = place_fruit(state);
    }
}

void reset_game(State* state) {

    ll_free(state->list);

    state->list = ll_create(sizeof(i_vect));
    i_vect pos1 = {3, 3};
    i_vect pos2 = {2, 3};
    i_vect pos3 = {1, 3};
    ll_insert_last(state->list, &pos1);
    ll_insert_last(state->list, &pos2);
    ll_insert_last(state->list, &pos3);

    state->fruit_pos = place_fruit(state);
    state->current_direction = right;
    state->direction_last_moved_in = right;
    state->game_over = 0;
    state->score = 0;
    state->begin = 0;
}


int main(int argc, char* argv[]) {

    State state;
    memset(&state, 0, sizeof(state));
    reset_game(&state);

    int frame_timer = TICK_SPEED;

    SetTargetFPS(60);
    InitWindow(PLAY_AREA_WIDTH + BORDER_SIZE*2, PLAY_AREA_HEIGHT + BORDER_SIZE*2 + UI_HEIGHT, "Snake");

    while(!WindowShouldClose()){

        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw_UI();

            if(IsKeyPressed(KEY_UP) && state.direction_last_moved_in != down) state.current_direction = up;
            if(IsKeyPressed(KEY_DOWN) && state.direction_last_moved_in != up) state.current_direction = down;
            if(IsKeyPressed(KEY_LEFT) && state.direction_last_moved_in != right) state.current_direction = left;
            if(IsKeyPressed(KEY_RIGHT) && state.direction_last_moved_in != left) state.current_direction = right;
            if(IsKeyPressed(KEY_R)) reset_game(&state);

            if(!state.begin) {

                char text[] = "Press Space to begin";
                int text_width = MeasureText(text, 30);
                DrawText(text, GetScreenWidth()/2 - text_width/2, PLAY_AREA_HEIGHT/2, 30, DARKGRAY);

                if(IsKeyPressed(KEY_SPACE)) state.begin = 1;

            } else if(!state.game_over) {

                // timer to make the snake "move" every 60 frames
                if(frame_timer == 0){
                    move_snake(&state);
                    frame_timer = TICK_SPEED;
                } else {
                    frame_timer -= 1;
                }
                draw_fruit(&state);
                draw_snake(state.list, PIXEL_SIZE);
            } else {
                char text[] = "Game Over";
                int text_width = MeasureText(text, 30);
                DrawText(text, GetScreenWidth()/2 - text_width/2, PLAY_AREA_HEIGHT/2, 30, DARKGRAY);
            }

            char score_text[50];
            sprintf(score_text, "Score: %d", state.score);
            DrawText(score_text, 5 + BORDER_SIZE, 5 + BORDER_SIZE, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
