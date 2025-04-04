#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) {
  size_t stored_state = (size_t)serialize(state);
  insert_at_tail(&(q->data), stored_state);
}

struct game_state dequeue(struct queue *q) {
  size_t serialized_state = remove_from_head(&(q->data));
  if (serialized_state == (size_t)-1) {
    return (struct game_state){0};
  }
  return deserialize(serialized_state);
}

static bool is_final_state(struct game_state state) {
  if (state.empty_row != 3 || state.empty_col != 3) {
    return false;
  }
  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      if (i == 3 && j == 3) continue;
      if (state.tiles[i][j] != (i * 4 + j + 1)) {
        return false;
      }
    }
  }
  return true;
}

static bool list_contains(struct linked_list *list, size_t v) {
  struct list_node *p = list->head;
  while (p != NULL) {
    if (p->value == v) {
      return true;
    }
    p = p->next;
  }
  return false;
}

int number_of_moves(struct game_state start) {
  struct linked_list queue_ll = {.head = NULL};
  struct queue state_q = {.data = queue_ll};

  struct linked_list visited_list = {.head = NULL};

  enqueue(&state_q, start);

  struct game_state start_key = start;
  start_key.num_steps = 0;
  size_t start_serial = (size_t)serialize(start_key);
  insert_at_head(&visited_list, start_serial);

  int moves = -1;

  while (state_q.data.head != NULL) {
    struct game_state curr = dequeue(&state_q);

    if (is_final_state(curr)) {
      moves = curr.num_steps;
      break;
    }

    struct game_state neighbors[4];
    neighbors[0] = curr;
    move_up(&neighbors[0]);
    neighbors[1] = curr;
    move_down(&neighbors[1]);
    neighbors[2] = curr;
    move_left(&neighbors[2]);
    neighbors[3] = curr;
    move_right(&neighbors[3]);

    struct game_state curr_key = curr;
    curr_key.num_steps = 0;
    size_t curr_serial_key = (size_t)serialize(curr_key);

    for (int i = 0; i < 4; ++i) {
      struct game_state next = neighbors[i];

      struct game_state next_key = next;
      next_key.num_steps = 0;
      size_t next_serial_key = (size_t)serialize(next_key);

      if (next_serial_key != curr_serial_key &&
          !list_contains(&visited_list, next_serial_key)) {
        insert_at_head(&visited_list, next_serial_key);
        enqueue(&state_q, next);
      }
    }
  }

  free_list(state_q.data);
  free_list(visited_list);

  return moves;
}