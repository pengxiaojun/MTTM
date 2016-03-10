#include "control.h"
#include <assert.h>

void control_init(data_control_t *c)
{
	assert(c);
	
	pthread_mutex_init(&c->mutex, NULL);
	pthread_cond_init(&c->cond, NULL);
	c->active = 0;
}

void control_destroy(data_control_t *c)
{
	pthread_mutex_destroy(&c->mutex);
	pthread_cond_destroy(&c->cond);
	c->active = 0;
}

void control_activate(data_control_t *c)
{
	pthread_mutex_lock(&c->mutex);
	c->active = 1;
	pthread_mutex_unlock(&c->mutex);
	pthread_cond_broadcast(&c->cond);
}

void control_deactivate(data_control_t *c)
{
	pthread_mutex_lock(&c->mutex);
	c->active = 0;
	pthread_mutex_unlock(&c->mutex);
	pthread_cond_broadcast(&c->cond);
}
