#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "ringbuffer.h"


/*
 * Implementation of a simple ring buffer for a single producer and multiple consumers.
 *
 * Each thread has its own index. Calling `ringbuffer_insert` (write) and `ringbuffer_peek` (read)
 * increments the caller's index. Thread safety is guaranteed by the fact that the producer can
 * never overtake the slowest consumer, and none of the producers are not allowed to overtake the
 * producer.
 *
 * Written while learning C and based on LMAX Disruptor (https://lmax-exchange.github.io/disruptor/).
 */


#define ERROR_HANDLER(condition, ...) \
    do { \
      if (condition) { \
        printf("\nError: in %s - function %s at line %d: ", __FILE__, __func__, __LINE__); \
        if (errno != 0) \
            printf("%s - ", strerror(errno)); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        fflush(stdout); \
        exit(EXIT_FAILURE); \
      }    \
    } while(0)


typedef size_t thread_index_t;


struct ringbuffer {
    uint8_t ring[RINGBUFFER_SIZE];

    thread_index_t producer;
    thread_index_t consumers[RINGBUFFER_CONSUMER_COUNT];
};


static bool producer_overtakes_consumer(ringbuffer_t *buffer) {
    for (int i = 0; i < RINGBUFFER_CONSUMER_COUNT; i++) {
        if ((buffer->producer + 1) % RINGBUFFER_SIZE == buffer->consumers[i]) // avoid deadlock at start
            return true;
    }
    return false;
}

static bool consumer_overtakes_producer(ringbuffer_t *buffer, size_t reader_index) {
    if ((buffer->consumers[reader_index]) == buffer->producer)
        return true;
    return false;
}


int ringbuffer_init(ringbuffer_t **buffer) {
    *buffer = (ringbuffer_t *) malloc(sizeof(ringbuffer_t));
    if (*buffer == NULL) return RINGBUFFER_FAILURE;

    (*buffer)->producer = 0;
    for (int i = 0; i < RINGBUFFER_CONSUMER_COUNT; i++)
        (*buffer)->consumers[i] = 0;

    return RINGBUFFER_SUCCESS;
}


int ringbuffer_free(ringbuffer_t **buffer) {
    if ((buffer == NULL) || (*buffer == NULL)) return RINGBUFFER_FAILURE;

    free(*buffer);
    *buffer = NULL;
    return RINGBUFFER_SUCCESS;
}


int ringbuffer_insert(ringbuffer_t *buffer, uint8_t data) {
    if (buffer == NULL) return RINGBUFFER_FAILURE;

    if (producer_overtakes_consumer(buffer)) return RINGBUFFER_OVERTAKE;

    buffer->ring[buffer->producer] = data;
    buffer->producer = (buffer->producer + 1) % RINGBUFFER_SIZE;

    return RINGBUFFER_SUCCESS;
}


int ringbuffer_peek(ringbuffer_t *buffer, uint8_t *data, size_t index) {
    if (buffer == NULL) return RINGBUFFER_FAILURE;
    ERROR_HANDLER(index > RINGBUFFER_CONSUMER_COUNT, "Cannot access reader index %zu", index);

    if (consumer_overtakes_producer(buffer, index)) return RINGBUFFER_OVERTAKE;

    *data = buffer->ring[buffer->consumers[index]];
    buffer->consumers[index] = (buffer->consumers[index] + 1) % RINGBUFFER_SIZE;

    return RINGBUFFER_SUCCESS;
}
