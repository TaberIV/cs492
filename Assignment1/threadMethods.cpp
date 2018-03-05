#include "assign1.h"

void *producer_function(void *id) {
	
	Product *p;

	// Create Product and add Product to queue
	pthread_mutex_lock(&product_count_mutex);
	if (product_count < num_products) {
		product_count++;

		// Wait if queue is full
		while (productQueue.size() == queue_size)
			pthread_cond_wait(&queue_not_full, &product_count_mutex);

		p = new Product(product_count);
		productQueue.push(p);
	}
	pthread_mutex_unlock(&product_count_mutex);	

	// Re-run producer function
	if (product_count < num_products)
		producer_function(id);
	else
		return NULL;
}

/*
int fn (int n) {
	if (n < 0)
		throw invalid_argument("Must give integer less than zero.");
	else if (n == 0 || n == 1)
		return 1;
	else
		return fn(n - 1) + fn(n - 2);
}


void *consumer_function(void *id) {
	
	// Get product from queue
	Product product;

	while (timeLeft > 0 && product.life > 0) {
		// Simulate work
		for (int i = 0; i < 10; i++)
			fn(10);

		product.life--;
		timeLeft--;
	}

	if (product.life == 0) {
		cout << "Product " << product.id << "was consumed.";
		usleep(100000);
		delete product;
	}
}*/