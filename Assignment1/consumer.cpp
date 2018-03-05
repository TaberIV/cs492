#include "assign1.h"

int fn (int n) {
	if (n < 0)
		throw invalid_argument("Must give integer less than zero.");
	else if (n == 0 || n == 1)
		return 1;
	else
		return fn(n - 1) + fn(n - 2);
}

void *consumer_fifo(void *id) {
	int idNum = *(int *) id;

	// Get product from queue
	pthread_mutex_lock(&products_consumed_mutex);
	if (products_consumed < num_products) {

		pthread_mutex_lock(&queue_mutex);
		while (productQueue.size() == 0) {
			printf("   Consumer %d\t|   Wait   |     -\t|  %d/%d\n", 
					idNum, (int) productQueue.size(), queue_size);
			pthread_cond_wait(&queue_not_empty, &queue_mutex);
		}
		
		products_consumed++;
		pthread_mutex_unlock(&products_consumed_mutex);

		Product *p = productQueue.front();
		productQueue.pop();

		printf("   Consumer %d\t| Consumes |     %d\t|  %d/%d\n", 
				idNum, p->id, (int) productQueue.size(), queue_size);

		pthread_cond_signal(&queue_not_full);
		pthread_mutex_unlock(&queue_mutex);

		// Simulate work
		while (p->life > 0) {
			for (int i = 0; i < 10; i++)
				fn(10);

			p->life--;
		}

		if (p->life == 0) {
			usleep(100000);
			delete p;
			consumer_fifo(id);
		}
	} else {
		pthread_mutex_unlock(&products_consumed_mutex);
		printf("   Consumer %d\t|   Exit   |     -\t|  -/%d\n", idNum, queue_size);
		pthread_exit(NULL);
	}
}

void *consumer_rr(void *id) {
	throw invalid_argument("Round Robin not implemented.\n");
}