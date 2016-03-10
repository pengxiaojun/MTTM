#include "work.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_WORKER_THREAD 4

int g_numthreads;
work_queue_t g_wq;
cleanup_queue_t g_cq;

void *thread_func(void *data)
{
	work_node_t *wnode;
	cleanup_node_t *cnode;

	cnode = (cleanup_node_t*)data;

	pthread_mutex_lock(&g_wq.control.mutex);

	while (g_wq.control.active)
	{
		while (g_wq.work.head == NULL && g_wq.control.active)
		{
			pthread_cond_wait(&g_wq.control.cond, &g_wq.control.mutex);
		}
		if (!g_wq.control.active)
			break;

		wnode = (work_node_t*)queue_get(&g_wq.work);
		pthread_mutex_unlock(&g_wq.control.mutex);
		//process
		printf("Thread number %d process job number %d...\n", cnode->threadnum, wnode->jobnum);
		free(wnode);
		pthread_mutex_lock(&g_wq.control.mutex);
	}
	//thread stopping
	pthread_mutex_unlock(&g_wq.control.mutex);

	//add stop thread to cleanup queue
	pthread_mutex_lock(&g_cq.control.mutex);
	queue_put(&g_cq.cleanup, (node_t*)cnode);
	pthread_mutex_unlock(&g_cq.control.mutex);
	pthread_cond_signal(&g_cq.control.cond);
	printf("Thread %d shutting down...\n", cnode->threadnum);
}

void join_threads()
{
	cleanup_node_t *cnode = NULL;

	while (g_numthreads > 0)
	{
		pthread_mutex_lock(&g_cq.control.mutex);

		while (g_cq.cleanup.head == NULL)
			pthread_cond_wait(&g_cq.control.cond, &g_cq.control.mutex);

		cnode = (cleanup_node_t*)queue_get(&g_cq.cleanup);
		pthread_mutex_unlock(&g_cq.control.mutex);
		pthread_join(cnode->tid, NULL);
		printf("Thread %d stopped\n", cnode->threadnum);
		free(cnode);
		g_numthreads--;
	}
}

void create_threads()
{
	int i;
	cleanup_node_t *cnode;
	for (i = 0; i<MAX_WORKER_THREAD; ++i)
	{
		cnode = (cleanup_node_t*)malloc(sizeof(*cnode));	
		cnode->threadnum = i;
		pthread_create(&cnode->tid, NULL, thread_func, cnode);
		printf("Create thread %d...\n", i);
		g_numthreads++;
	}
}

void initialize_struct()
{
	queue_init(&g_wq.work);
	control_init(&g_wq.control);
	control_activate(&g_wq.control);

	queue_init(&g_cq.cleanup);
	control_init(&g_cq.control);
	control_activate(&g_cq.control);
}

void cleanup_struct()
{
	control_destroy(&g_wq.control);
	control_destroy(&g_cq.control);
}

int main()
{
	int i;
	work_node_t *wnode;

	initialize_struct();
	create_threads();

	pthread_mutex_lock(&g_wq.control.mutex);
	for (i = 0; i<20000; ++i)
	{
		wnode = (work_node_t*)malloc(sizeof(*wnode));
		wnode->jobnum = i;
		queue_put(&g_wq.work, (node_t*)wnode);
	}
	pthread_mutex_unlock(&g_wq.control.mutex);
	pthread_cond_broadcast(&g_wq.control.cond);
	printf("sleeping...\n");
	sleep(2);
	printf("Deactivate work queue...\n");
	control_deactivate(&g_wq.control);

	//cleanup
	join_threads();
	cleanup_struct();
	exit(0);
}
