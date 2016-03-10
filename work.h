#ifndef _WORK_H
#define _WORK_H

#include "queue.h"
#include "control.h"

#ifdef __cpluscplus
extern "C" 
{
#endif

/* work queue holds tasks for various thread to complete */
typedef struct {
	queue_t work;
	data_control_t control;
} work_queue_t;

/* add additianal data need to be processed */
typedef struct {
	struct node_t *next;
	int jobnum;
} work_node_t;

/* cleanup queue holds stoppped threads 
 * before thread terminate. add to this list */
typedef struct {
	queue_t cleanup;
	data_control_t control;
} cleanup_queue_t;

/* additional data need to be process before thread terminate.
 * thread id is added */
typedef struct {
	struct node_t *next;
	int threadnum;
	pthread_t tid;
} cleanup_node_t;


#ifdef __cpluscplus
}
#endif

#endif
