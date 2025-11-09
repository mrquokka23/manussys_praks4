#ifndef INC_CIRCULAR_BUFFER_H_
#define INC_CIRCULAR_BUFFER_H_

#include <inttypes.h>

typedef struct {
    uint8_t* array;
    int head;
    int tail;
    int fill;
    int size;
} circular_buffer_t;

// Prepares circular_buffer_t
void circ_init(circular_buffer_t* circ, uint8_t* data, int size);

// Returns number of bytes in circular buffer
int circ_fill_level(circular_buffer_t* circ);

// Returns number of bytes pushed
int circ_push_byte(circular_buffer_t* circ, uint8_t data);

// Returns number of bytes pushed
int circ_push(circular_buffer_t* circ, uint8_t* data, int count);

// Returns number of bytes popped
int circ_pop_byte(circular_buffer_t *circ, uint8_t *data);

// Returns number of bytes popped
int circ_pop(circular_buffer_t *circ, uint8_t *data, int count);

#endif /* INC_CIRCULAR_BUFFER_H_ */
