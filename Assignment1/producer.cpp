#include "assign1.h"

void *producer(void *id) {
	int idNum = *(int *) id;

	Product *p;

	// Create Product and add Product to queue
	pthread_mutex_lock(&product_count_mutex);
	if (product_count < num_products) {
		product_count++;

		// Wait if queue is full
		pthread_mutex_lock(&queue_mutex);
		while (productQueue.size() == queue_size) {
			printf("   Producer %d\t|   Wait   |     -\t|  %d/%d\n", 
					idNum, (int) productQueue.size(), queue_size);
			pthread_cond_wait(&queue_not_full, &queue_mutex);
		}

		p = new Product(product_count);
		pthread_mutex_unlock(&product_count_mutex);
		
		// Push to queue
		productQueue.push(p);
		printf("   Producer %d\t| Produces |     %d\t|  %d/%d\n", 
				idNum, p->id, (int) productQueue.size(), queue_size);
		pthread_mutex_unlock(&queue_mutex);
		pthread_cond_signal(&queue_not_empty);

		// Re-run producer function
		usleep(100000);
		producer(id);
	}
	else {
		pthread_mutex_unlock(&product_count_mutex);
		printf("   Producer %d\t|   Exit   |     -\t|  -/%d\n", idNum, queue_size);
		pthread_exit(NULL);
	}
}