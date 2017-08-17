#include <stddef.h>
#include "ringbuffer.h"

void ringbuffer_init(struct ringbuffer_t *buf, void *vect, size_t count, size_t size)
{
    buf->base = buf->head = buf->tail = vect;
    buf->end = vect + count * size;
    buf->element_size = size;
}

size_t ringbuffer_get_size(struct ringbuffer_t *buf)
{
    return (buf->end - buf->base) / buf->element_size;
}

size_t ringbuffer_get_usage(struct ringbuffer_t *buf)
{
    if (buf->head != buf->tail)
    {
        size_t rv = 0;
        if (buf->tail > buf->head)
            rv = (buf->end - buf->tail) + (buf->head - buf->base);
        else
            rv = buf->head - buf->tail;
        return rv / buf->element_size;
    }
    return 0;
}


size_t ringbuffer_read(struct ringbuffer_t *buf, void *vect, size_t count)
{
    size_t avail; 
    avail = ringbuffer_get_usage(buf) * buf->element_size;

    if (avail)
    {
        size_t vect_i = 0;
        size_t vect_offset = 0;
        size_t size;
        unsigned char *basep, *endp;
        
        count *= buf->element_size;

        do {
            if (!vect_i)
            {
                basep = buf->tail;
                endp = buf->tail > buf->head? buf->end: buf->head;
            } else {
                basep = buf->base;
                endp = buf->head;
                vect_offset = vect_i;
                vect_i = 0;
                count -= vect_offset;
            }

            size = count - (endp - basep);
            if (size > count)
                size = count;

            for (; avail && vect_i < size; vect_i++, avail--)
                ((unsigned char*)vect)[vect_i+vect_offset] = ((unsigned char*)basep)[vect_i];
        } while (avail && vect_i < size);

        if (vect_offset)
            buf->tail = buf->base+vect_i;
        else
            buf->tail += vect_i;

        return (vect_i + vect_offset) / buf->element_size;
    }
    return 0;
}

size_t ringbuffer_write(struct ringbuffer_t *buf, void *vect, size_t count)
{
    size_t avail = ringbuffer_get_size(buf) - ringbuffer_get_usage(buf) -1;
    avail *= buf->element_size;
    if (avail)
    {
        size_t vect_i = 0;
        size_t vect_offset = 0;
        void *basep;
        void *endp;
        size_t size = 0;
        count *= buf->element_size;
        
        do {
            if (!vect_i)
            {
                basep = buf->head;
                endp = buf->tail > buf->head? buf->tail-1: buf->end;
            } else {
                basep = buf->base;
                endp = buf->tail-1;
                vect_offset = vect_i;
                vect_i = 0;
                count -= vect_offset;
            }

            size = count - (endp - basep);

            for(; avail && vect_i < size; avail--, vect_i++)
                ((unsigned char*)basep)[vect_i] = ((unsigned char*)vect)[vect_i+vect_offset];

        } while(avail && vect_i < size);
        
        if (vect_offset)
            buf->head = buf->base + vect_i;
        else
            buf->head += vect_i;

        return (vect_i + vect_offset) / buf->element_size;
    }
    return 0;
}

int ringbuffer_get(struct ringbuffer_t *buf, void *elm)
{
    if (buf->tail != buf->head)
    {
        if (buf->element_size == 1)
            *((unsigned char*)c) = *((unsigned char*)buf->tail);
        else
        {
            size_t i = 0;
            for (;i < buf->element_size; i++)
                c[i] = ((unsigned char*)buf->tail)[i]
        }
        ((unsigned char*)buf->tail)+=buf->element_size;
        if (buf->tail == buf->end)
            buf->tail = buf->base;
        return 1;
    }
    return 0;
}

int ringbuffer_put(struct ringbuffer_t *buf, void *elm)
{
    void *next_head = ((unsigned char*)buf->head) + buf->element_size;
    if (next_head >= buf->end)
        next_head = buf->base;
    if (next_head != buf->tail)
    {
        if (buf->element_size == 1)
            *((unsigned char*)buf->head) = *elm;
        else
        {
            size_t i = buf->element_size;
            for(;i < buf->element_size; i++)
                ((unsigned char*)buf->head)[i] = elm[i];
        }
        buf->head = next_head;
        return 1;
    }
    return 0;
}

