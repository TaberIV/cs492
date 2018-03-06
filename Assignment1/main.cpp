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
	if (argc < 8)
		throw invalid_argument("Wrong number of arguments, should be 7.\n");

	// Initialize arg variables
	num_producers = stoi(args[1]);
	num_consumers = stoi(args[2]);
	num_products = stoi(args[3]);
	queue_size = stoi(args[4]);
	schedule_type = stoi(args[5]);
	quantum_time = stoi(args[6]);
	seed = stoi(args[7]);
	
	if (argc > 8)
		debug = stoi(args[8]) == 1;
	else
		debug = false;
	
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

	pthread_mutex_init(&product_count_mutex, NULL);
	pthread_mutex_init(&products_consumed_mutex, NULL);
	pthread_mutex_init(&queue_back_mutex, NULL);
	pthread_mutex_init(&queue_front_mutex, NULL);
	pthread_mutex_init(&stats_mutex, NULL);
	pthread_cond_init(&queue_not_full, NULL);
	pthread_cond_init(&queue_not_empty, NULL);

	// Output Head
	printf("     Thread:\t|  Action  |   Process\t| Queue\n");
	printf(" ---------------|----------|------------|-------\n");

	t = clock();

	// Create threads
	int producerIds[num_producers], consumerIds[num_consumers];
	pthread_t producers[num_producers], consumers[num_consumers];

	clock_t producer_time = clock();
	for (int i = 0; i < num_producers; i++) {
		if (debug)
			printf("   Producer %d\t|  Starts  |     -\t|  -/%d\n", i, queue_size);

		producerIds[i] = i;
		pthread_create(&producers[i], NULL, producer, &producerIds[i]);
	}
	

	clock_t consumer_time = clock();
	for (int i = 0; i < num_consumers; i++) {
		if (debug)
			printf("   Consumer %d\t|  Starts  |     -\t|  -/%d\n", i, queue_size);

		consumerIds[i] = i;
		pthread_create(&consumers[i], NULL, (schedule_type == 0) ? (consumer) : (consumer), &consumerIds[i]);
	}

	// Join threads
	for (int i = 0; i < num_producers; i++)
		pthread_join(producers[i], NULL);
	producer_time = clock() - producer_time;

	for (int i = 0; i < num_consumers; i++)
		pthread_join(consumers[i], NULL);
	consumer_time = clock() - consumer_time;

	t = clock() - t;

	// Output statistics
	printf("\nTime Elapsed: %lf\n\n", t / (double) CLOCKS_PER_SEC);

	printf("Min Turnaround Time: %f\n", min_turnaround_time / (double) CLOCKS_PER_SEC);
	printf("Max Turnaround Time: %f\n", max_turnaround_time / (double) CLOCKS_PER_SEC);
	printf("Avg Turnaround Time: %f\n\n", average_turnaround_time / (double) CLOCKS_PER_SEC);

	printf("Min Wait Time: %f\n", min_wait_time / (double) CLOCKS_PER_SEC);
	printf("Max Wait Time: %f\n", max_wait_time / (double) CLOCKS_PER_SEC);
	printf("Avg Wait Time: %f\n\n", average_wait_time / (double) CLOCKS_PER_SEC);

	printf("Producer throughput: %f\n", num_products / (producer_time / (double) CLOCKS_PER_SEC) * 60);
	printf("Consumer throughput: %f\n", num_products / (consumer_time / (double) CLOCKS_PER_SEC) * 60);

	// Destroy recources
	pthread_mutex_destroy(&queue_back_mutex);
	pthread_mutex_destroy(&queue_front_mutex);
	pthread_mutex_destroy(&product_count_mutex);
	pthread_mutex_destroy(&products_consumed_mutex);
	pthread_mutex_destroy(&stats_mutex);
	pthread_cond_destroy(&queue_not_full);
	pthread_cond_destroy(&queue_not_empty);
}
