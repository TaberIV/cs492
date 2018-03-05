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

int main(int argc, char** args) {
	// Ensure proper number of args
	if (argc != 8)
		throw invalid_argument("Wrong number of arguments, should be 7.");

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
	pthread_cond_init(&queue_not_full, NULL);

	// Create threads
	int message;
	int producerIds[num_producers];
	pthread_t producers[num_producers];//, *consumers;

	for (int i = 0; i < num_producers; i++) {
		producerIds[i] = i;
		message = pthread_create(&producers[i], NULL, producer_function, &producerIds[i]);
	}

	for (int i = 0; i < num_producers; i++)
		pthread_join(producers[i], NULL);

	// Destroys mutexes and conds
	pthread_mutex_destroy(&product_count_mutex);
	pthread_cond_destroy(&queue_not_full);
}
