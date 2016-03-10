#ifndef _CONTROL_H
#define _CONTROL_H

#include <pthread.h>

#ifdef __cpluscplus
extern "C" 
{
#endif

typedef struct data_control_t {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int active;
} data_control_t;


void control_init(data_control_t *c);
void control_destroy(data_control_t *c);
void control_activate(data_control_t *c);
void control_deactivate(data_control_t *c);

#ifdef __cpluscplus
}
#endif

#endif
