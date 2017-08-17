#ifndef RINGBUFFER_H
#define RINGBUFFER_H

struct ringbuffer_t
{
    void *base;
    void *head;
    void *tail;
    void *end;
    size_t element_size;
};

/**
 * Initialize a ringbuffer_t structure using a pre-allocated memory vector.
 *
 * @param buf A pointer to a pre-allocated struct ringbuffer_t.
 * @param vect A pointer to a pre-allocated memory vector.
 * @param count The amount of elements that fits into the memory vector.
 * @param size Tha size, in byte, of a single element.
 *
 */
void ringbuffer_init(struct ringbuffer_t *buf, void *vect, size_t count, size_t size);

/**
 * Get the size in, in byte, of the memory buffer.
 *
 * @param buf The ringbuffer to operate on.
 * @return The size of the ringbuffer's vector or 0 if uninitialized.
 */
size_t ringbuffer_get_size(struct ringbuffer_t *buf);

/**
 * Get the current usage, in byte, of the memory buffer.
 *
 * @param buf The ringbuffer to operate on.
 * @return The amount of byte currently in use in the memory buffer.
 */
size_t ringbuffer_get_usage(struct ringbuffer_t *buf);

/**
 * Retrieve a set amount of character from the ringbuffer. The elements will be
 * copied into the provided memory vector. The function will attempts to fill the
 * vector completly up to the amount of element into the buffer.
 *
 * @param buf The ringbuffer to work on.
 * @param vect The memory vector to place the characters into.
 * @param size The size of the memory vector.
 * @return The amount of element placed into the output vector.
 */
size_t ringbuffer_read(struct ringbuffer_t *buf, void *vect, size_t size);

/**
 * Attempt to place a vector of element into the buffer. The function will return
 * the amount of element placed.
 *
 * @param buf The ringbuffer to work on.
 * @param vect The vector from which the elements will be taken.
 * @param size The size of the input vector.
 * @return The amount of element succesfuly copied.
 *
 */
size_t ringbuffer_write(struct ringbuffer_t *buf, void *vect, size_t size);

/**
 * Attempt to retrieve a single element from the ringbuffer. The element will be
 * placed into the `c` argument.
 *
 * @param buf The ringbuffer to work on.
 * @param c A pointer to the memory location where to place the read element.
 * @return The amount of element copied.
 */
int ringbuffer_get(struct ringbuffer_t *buf, void *c);

/**
 * Attempt to place a single element into the ringbuffer.
 *
 * @param buf The ringbuffer to work on.
 * @param c The element's memory location.
 * @return the amount of element imported into the buffer.
 */
int ringbuffer_put(struct ringbuffer_t *buf, void *c);


#endif

