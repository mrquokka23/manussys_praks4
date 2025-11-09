#include "circular_buffer.h"

int next_index(int i, int size) {
	return i + 1 == size
		? 0
		: i + 1;
}

void circ_init(circular_buffer_t* circ, uint8_t* array, int size) {
	circ->array = array;
	circ->head = 0;
	circ->tail = 0;
	circ->fill = 0;
	circ->size = size;
}

int circ_fill_level(circular_buffer_t* circ) {
	return circ->fill;
}

int circ_push_byte(circular_buffer_t* circ, uint8_t data) {
	int new_head = next_index(circ->head, circ->size);

	if (new_head == circ->tail) {
		return 0;
	}

	circ->array[circ->head] = data;
	circ->head = new_head;
	circ->fill++;
	return 1;
}

// Naive implementation, better to use memcpy
int circ_push(circular_buffer_t* circ, uint8_t* data, int count) {
	int i = 0;
	for ( ; i < count; i++) {
		if (!circ_push_byte(circ, data[i])) {
			break;
		}
	}
	return i;
}

int circ_pop_byte(circular_buffer_t *circ, uint8_t *data) {
    if (circ->head == circ->tail) {
        return 0;
    }

    *data = circ->array[circ->tail];
    circ->tail = next_index(circ->tail, circ->size);
	circ->fill--;
    return 1;
}

// Naive implementation, better to use memcpy
int circ_pop(circular_buffer_t *circ, uint8_t *data, int count) {
	int i = 0;
	for ( ; i < count; i++) {
		if (!circ_pop_byte(circ, &data[i])) {
			break;
		}
	}
	return i;
}
