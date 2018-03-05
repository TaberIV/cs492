#include "assign1.h"

int fn (int n) {
	if (n < 0)
		throw invalid_argument("Must give integer less than zero.");
	else if (n == 0 || n == 1)
		return 1;
	else
		return fn(n - 1) + fn(n - 2);
}

void *consumer_function(void *id) {
	int idNum = *(int *) id;
	cout << "Consumer " + to_string(idNum) + " starting.\n";

	// Get product from queue
	pthread_mutex_lock(&queue_mutex);
	while (productQueue.size() == 0) {
		cout << "Consumer " + to_string(idNum) + " waiting for product.\n";
		pthread_cond_wait(&queue_not_empty, &queue_mutex);
	}

	Product *p = productQueue.front();
	productQueue.pop();

	cout << "Consumer " + to_string(idNum) + " got product " + to_string(p->id) + "\n";
	
	pthread_cond_signal(&queue_not_full);
	pthread_mutex_unlock(&queue_mutex);

	// Simulate work
	while (p->life > 0) {
		for (int i = 0; i < 10; i++)
			fn(10);

		p->life--;
	}

	if (p->life == 0) {
		cout << "Consumer " + to_string(idNum) + " consumed process " + to_string(p->id) + "\n";
		usleep(100000);
		delete p;
	}

	consumer_function(id);
}