#ifndef _QUEUE_H
#define _QUEUE_H


#ifdef __cpluscplus
extern "C" 
{
#endif

typedef struct node_t {
	struct node_t *next;
	/* define your data here */
} node_t;

typedef struct {
	node_t *head, *tail;
} queue_t;


void queue_init(queue_t *q);
void queue_put(queue_t *q, node_t *node);
node_t *queue_get(queue_t *q);

#ifdef __cpluscplus
}
#endif

#endif
