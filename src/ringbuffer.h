#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_


#define RINGBUFFER_SIZE 56
#define RINGBUFFER_CONSUMER_COUNT 2

#define RINGBUFFER_FAILURE (-1)
#define RINGBUFFER_SUCCESS 0
#define RINGBUFFER_OVERTAKE 1


typedef struct ringbuffer ringbuffer_t;


/*
 * Allocates and initializes a new ring buffer.
 *
 * Return:
 *      RINGBUFFER_SUCCESS on success
 *      RINGBUFFER_FAILURE when *buffer cannot be allocated
 */
int ringbuffer_init(ringbuffer_t **buffer);


/*
 * Free buffer memory.
 *
 * Return:
 *      RINGBUFFER_SUCCESS on success
 *      RINGBUFFER_FAILURE on error
 */
int ringbuffer_free(ringbuffer_t **buffer);


/*
 * Inserts data pointer at the current write index of the buffer. Non-blocking; if the
 * producer will overtake one of the consumers, RINGBUFFER_OVERTAKE is returned instead.
 *
 * Non-blocking
 *
 * Return:
 *      RINGBUFFER_SUCCESS on success
 *      RINGBUFFER_FAILURE on error
 *      RINGBUFFER_OVERTAKE when producer overtakes one of the consumers
*/
int ringbuffer_insert(ringbuffer_t *buffer, uint8_t data);


/*
 * Read next data element for consumer thread with index. Non-blocking; if the
 * consumer will overtake the producer, RINGBUFFER_OVERTAKE is returned instead.
 *
 * Non-blocking
 *
 * Return:
 *      RINGBUFFER_SUCCESS on success
 *      RINGBUFFER_FAILURE on error
 *      RINGBUFFER_OVERTAKE when consumer overtakes producer
 */
int ringbuffer_peek(ringbuffer_t *buffer, uint8_t *data, size_t index);



#endif  //_RINGBUFFER_H_

