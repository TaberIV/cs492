#ifndef ASSIGN1_H
#define ASSIGN1_H

#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#ifndef THREAD_METHODS
#define THREAD_METHODS
void *producer_function(void *id);
void *consumer_function(void *id);
#endif

using namespace std;

class Product {
	int id;
	int time_created;
	int life;

	public:
		Product(int id) {
			this->id = id;
			this->time_created = clock();
			this->life = random() % 1024;

			cout << id << endl;
		}
};

// Create globals
int num_producers;
int num_consumers;
int num_products;
int queue_size;
int schedule_type;
float quantum_time;
int seed;

queue<Product*> productQueue;
clock_t t;

int product_count = 0;
pthread_mutex_t product_count_mutex;
pthread_cond_t queue_not_full;
#endif