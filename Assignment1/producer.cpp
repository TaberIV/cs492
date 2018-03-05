#include "assign1.h"

void *producer_function(void *id) {
	int idNum = *(int *) id;
	cout << "Producer " + to_string(idNum) + " starting.\n";;

	Product *p;

	// Create Product and add Product to queue
	pthread_mutex_lock(&product_count_mutex);
	if (product_count < num_products) {
		product_count++;
		pthread_mutex_unlock(&product_count_mutex);

		// Wait if queue is full
		pthread_mutex_lock(&queue_mutex);
		while (productQueue.size() == queue_size) {
			cout << "Producer " + to_string(idNum) + " waiting for space in queue.\n";
			pthread_cond_wait(&queue_not_full, &queue_mutex);
			cout << "Producer " + to_string(idNum) + " got space in queue.\n";
		}

		p = new Product(product_count);
		cout << "Producer " + to_string(idNum) + " pushing product " + to_string(p->id) + " to queue.\n";
		
		// Push to queue
		productQueue.push(p);
		pthread_mutex_unlock(&queue_mutex);
		pthread_cond_signal(&queue_not_empty);

		// Re-run producer function
		usleep(100000);
		producer_function(id);
	}
	else {
		pthread_mutex_unlock(&product_count_mutex);
		cout <<  "Producer " + to_string(idNum) + " exiting.\n";
		pthread_exit(NULL);
	}
}