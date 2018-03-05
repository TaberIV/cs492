#include "assign1.h"

void *producer_function(void *id) {
	int idNum = *(int *) id;
	string message = "Producer " + to_string(idNum) + " starting.\n";
	cout << message;

	Product *p;

	// Create Product and add Product to queue
	pthread_mutex_lock(&product_count_mutex);
	if (product_count < num_products) {
		product_count++;

		// Wait if queue is full
		while (productQueue.size() == queue_size)
			pthread_cond_wait(&queue_not_full, &product_count_mutex);

		p = new Product(product_count);
		message = "Producer " + to_string(idNum) + " pushing product " + to_string(p->id) + " to queue.\n";
		cout << message;
		productQueue.push(p);
	}
	pthread_mutex_unlock(&product_count_mutex);

	// Re-run producer function
	if (product_count < num_products) {
		usleep(100000);
		producer_function(id);
	}
	else
		return NULL;
}