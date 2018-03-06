/***********************************************
 * CS492 - Operating Systems
 * Assignment 1
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#ifndef ASSIGN1_H
#define ASSIGN1_H

int debug;

#include <iostream>
#include <string>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#ifndef PRODUCER
#define PRODUCER
void *producer(void *id);
#endif

#ifndef CONSUMER
#define CONSUMER
void *consumer(void *id);
#endif

using namespace std;


class Product {
	public:
		int id;
		int life;
		clock_t time_created;
		clock_t processing_time;

		Product(int id) {
			this->id = id;
			this->time_created = clock();
			this->life = random() % 1024;
			this->processing_time = 0;
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
int product_count = 0, products_consumed = 0;
queue<Product*> productQueue;

clock_t t;
clock_t max_turnaround_time, average_turnaround_time = 0;
clock_t min_turnaround_time = -1;

clock_t max_wait_time, average_wait_time = 0;
clock_t min_wait_time = -1;

pthread_mutex_t product_count_mutex, products_consumed_mutex, queue_front_mutex, queue_back_mutex;
pthread_mutex_t stats_mutex;
pthread_cond_t queue_not_full, queue_not_empty;
#endif