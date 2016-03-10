#include "queue.h"
#include <stdlib.h>
#include <assert.h>


void queue_init(queue_t *q)
{
	assert(q);
	q->head = q->tail = NULL;
}

void queue_put(queue_t *q, node_t *node)
{
	assert(q);
	
	if (node == NULL)
	{
		return;
	}
	node->next = NULL;
	if (q->head == NULL)
	{
		q->head = node;
	}
	if (q->tail)
	{
		q->tail->next = node;
	}
	q->tail = node;
}

node_t *queue_get(queue_t *q)
{
	assert(q);

	node_t *node = q->head;
	if (node)
	{
		q->head = node->next;
	}
	return node;
}
