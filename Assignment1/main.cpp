/***********************************************
 * CS492 - Operating Systems
 * Assignment 1
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign1.h"
#include "producer.cpp"
#include "consumer.cpp"

int main(int argc, char** args) {
	// Ensure proper number of args
	if (argc != 8)
		throw invalid_argument("Wrong number of arguments, should be 7.\n");

	// Initialize arg variables
	num_producers = stoi(args[1]);
	num_consumers = stoi(args[2]);
	num_products = stoi(args[3]);
	queue_size = stoi(args[4]);
	schedule_type = stoi(args[5]);
	quantum_time = stoi(args[6]);
	seed = stoi(args[7]);
	
	// Print args
	fflush(stdout);
	cout << "Number of producers: " << num_producers << endl;
	cout << "Number of consumers: " << num_consumers << endl;
	cout << "Number of products: " << num_products << endl;
	cout << "Size of queue: " << queue_size << endl;
	cout << "schedule type: " << ((schedule_type == 0) ? ("First-Come-First-Serve") : ("Round-Robin")) << endl;
	cout << "Quantum time: " << quantum_time << endl;
	cout << "Random seed: " << seed << endl;
	cout << endl;

	// Initialize globals
	srandom(seed);
	t = clock();
	pthread_mutex_init(&product_count_mutex, NULL);
	pthread_mutex_init(&products_consumed_mutex, NULL);
	pthread_cond_init(&queue_not_full, NULL);
	pthread_cond_init(&queue_not_empty, NULL);

	// Output Head
	printf("     Thread:\t|  Action  |   Process\t| Stack\n");
	printf(" ---------------|----------|------------|-------\n");


	// Create threads
	int producerIds[num_producers], consumerIds[num_consumers];
	pthread_t producers[num_producers], consumers[num_consumers];

	for (int i = 0; i < num_producers; i++) {
		printf("   Producer %d\t|  Starts  |     -\t|  -/%d\n", i, queue_size);
		producerIds[i] = i;
		pthread_create(&producers[i], NULL, producer, &producerIds[i]);
	}

	for (int i = 0; i < num_consumers; i++) {
		printf("   Consumer %d\t|  Starts  |     -\t|  -/%d\n", i, queue_size);
		consumerIds[i] = i;
		pthread_create(&consumers[i], NULL, (schedule_type == 0) ? (consumer_fifo) : (consumer_rr), &consumerIds[i]);
	}

	// Join threads
	for (int i = 0; i < num_producers; i++)
		pthread_join(producers[i], NULL);

	for (int i = 0; i < num_consumers; i++)
		pthread_join(consumers[i], NULL);

	// Destroy recources
	pthread_mutex_destroy(&product_count_mutex);
	pthread_mutex_destroy(&products_consumed_mutex);
	pthread_cond_destroy(&queue_not_full);
	pthread_cond_destroy(&queue_not_empty);
}
