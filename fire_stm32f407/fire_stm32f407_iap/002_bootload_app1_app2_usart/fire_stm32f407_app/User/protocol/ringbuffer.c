#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"
#include "protocol.h"

#define min(a, b) ((a)<(b)?(a):(b))

//static char buff[512] = {0};
void rb_new(RingBuffer* rb, int rb_buf_len)
{
    //RingBuffer *rb = (RingBuffer *)buff;//malloc(sizeof(RingBuffer) + capacity);
    if (rb == NULL) return;
    
    rb->rb_capacity = rb_buf_len;//-sizeof(RingBuffer);//capacity;
    //rb->rb_buff     = buff+sizeof(RingBuffer);//(char*)rb + sizeof(RingBuffer);
    rb->rb_head     = rb->rb_buff;
    rb->rb_tail     = rb->rb_buff;
	
		//return rb;
};

void  rb_free(RingBuffer *rb)
{
    //free((char*)rb);
}

int     rb_capacity(RingBuffer *rb)
{
    //assert(rb != NULL);
    return rb->rb_capacity;
}
int     rb_can_read(RingBuffer *rb)
{
    //assert(rb != NULL);
    if (rb->rb_head == rb->rb_tail) return 0;
    if (rb->rb_head < rb->rb_tail) return rb->rb_tail - rb->rb_head;
    return rb_capacity(rb) - (rb->rb_head - rb->rb_tail);
}
int     rb_can_write(RingBuffer *rb)
{
    //assert(rb != NULL);
    return rb_capacity(rb) - rb_can_read(rb);
}

int     rb_read(RingBuffer *rb, void *data, int count)
{
	int copy_sz = 0, read_num = 0;
    //assert(rb != NULL);
    //assert(data != NULL);
    if (rb->rb_head < rb->rb_tail)
    {
			  read_num = rb_can_read(rb);
        copy_sz = min(count, read_num);
        memcpy(data, rb->rb_head, copy_sz);
        rb->rb_head += copy_sz;
        return copy_sz;
    }
    else
    {
        if (count < rb_capacity(rb)-(rb->rb_head - rb->rb_buff))
        {
            int copy_sz = count;
            memcpy(data, rb->rb_head, copy_sz);
            rb->rb_head += copy_sz;
            return copy_sz;
        }
        else
        {
            int copy_sz = rb_capacity(rb) - (rb->rb_head - rb->rb_buff);
            memcpy(data, rb->rb_head, copy_sz);
            rb->rb_head = rb->rb_buff;
            copy_sz += rb_read(rb, (char*)data+copy_sz, count-copy_sz);
            return copy_sz;
        }
    }
}

int     rb_write(RingBuffer *rb, const void *data, int count)
{
    //assert(rb != NULL);
    //assert(data != NULL);
    
    if (count >= rb_can_write(rb)){
		return -1;
	} 
    
    if (rb->rb_head <= rb->rb_tail)
    {
        int tail_avail_sz = rb_capacity(rb) - (rb->rb_tail - rb->rb_buff);
        if (count <= tail_avail_sz)
        {
            memcpy(rb->rb_tail, data, count);
            rb->rb_tail += count;
            if (rb->rb_tail == rb->rb_buff+rb_capacity(rb))
                rb->rb_tail = rb->rb_buff;
            return count;
        }
        else
        {
            memcpy(rb->rb_tail, data, tail_avail_sz);
            rb->rb_tail = rb->rb_buff;
            
            return tail_avail_sz + rb_write(rb, (char*)data+tail_avail_sz, count-tail_avail_sz);
        }
    }
    else
    {
        memcpy(rb->rb_tail, data, count);
        rb->rb_tail += count;
        return count;
    }
}
