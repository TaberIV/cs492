#include "assign1.h"

int fn (int n) {
	if (n < 0)
		throw invalid_argument("Must give integer less than zero.");
	else if (n == 0 || n == 1)
		return 1;
	else
		return fn(n - 1) + fn(n - 2);
}

void *consumer(void *id) {
	int idNum = *(int *) id;

	// Get product from queue
	pthread_mutex_lock(&products_consumed_mutex);
	if (products_consumed < num_products) {
		products_consumed++;
		pthread_mutex_unlock(&products_consumed_mutex);
		
		// Wait if queue is empty
		pthread_mutex_lock(&queue_front_mutex);
		while (productQueue.size() == 0) {
			if (debug) {
				printf("   Consumer %d\t|   Wait   |     -\t|  %d/%d\n", 
						idNum, (int) productQueue.size(), queue_size);
			}
			pthread_cond_wait(&queue_not_empty, &queue_front_mutex);
		}

		Product *p = productQueue.front();
		pthread_mutex_lock(&queue_back_mutex);
		productQueue.pop();

		if (debug) {
			printf("   Consumer %d\t| Acquires |     %d\t|  %d/%d\n", 
					idNum, p->id, (int) productQueue.size(), queue_size);
		}
		pthread_mutex_unlock(&queue_front_mutex);

		// Simulate work
		int process_time;
		if (schedule_type == 1) {
			// Round-Robin
			if (p->life > quantum_time)
				process_time = quantum_time;
			else
				process_time = p->life;

		} else {
			// First come first serve
			process_time = p->life;
		}

		clock_t work_started = clock();
		while (process_time > 0) {
			fn(10);

			process_time--;
			p->life--;
		}
		p->processing_time += work_started - clock();

		// If life remains add back to queue
		if (p->life > 0) {
			while (productQueue.size() == queue_size && queue_size != 0) {
				if (debug) {
					printf("   Consumer %d\t|   Wait   |     -\t|  %d/%d\n", 
							idNum, (int) productQueue.size(), queue_size);
				}
				
				pthread_cond_wait(&queue_not_full, &queue_back_mutex);
			}
			
			// Push to queue
			pthread_mutex_lock(&products_consumed_mutex);
			products_consumed--;
			pthread_mutex_unlock(&products_consumed_mutex);
			productQueue.push(p);

			if (debug) {
				printf("   Consumer %d\t|  Pushes  |     %d\t|  %d/%d\n", 
						idNum, p->id, (int) productQueue.size(), queue_size);
			}
			pthread_mutex_unlock(&queue_back_mutex);
			pthread_cond_signal(&queue_not_empty);

		} else {
			pthread_mutex_unlock(&queue_back_mutex);

			// Get process statistics
			pthread_mutex_lock(&stats_mutex);
			
			// Turnaround time statistics
			clock_t turnaround_time = clock() - p->time_created;
			if (turnaround_time > max_turnaround_time) {
				max_turnaround_time = turnaround_time;
			}
			else if (turnaround_time < min_turnaround_time || min_turnaround_time < 0) {
				min_turnaround_time = turnaround_time;
			}
			average_turnaround_time += turnaround_time / num_products;

			// Wait time statistics
			clock_t wait_time = turnaround_time - p->processing_time;
			if (wait_time > max_wait_time) {
				max_wait_time = wait_time;
			}
			else if (wait_time < min_wait_time || min_wait_time < 0) {
				min_wait_time = wait_time;
			}
			average_wait_time += wait_time / num_products;
			
			pthread_mutex_unlock(&stats_mutex);

			// Consume product
			printf("   Consumer %d\t| Consumes |     %d\t|  %d/%d\n", 
					idNum, p->id, (int) productQueue.size(), queue_size);

			delete p;
			
			pthread_cond_signal(&queue_not_full);
		}

		// Sleep and reset process
		usleep(100000);
		consumer(id);
	} else {
		// Exits after all products are consumed
		pthread_mutex_unlock(&products_consumed_mutex);
		
		if (debug)
			printf("   Consumer %d\t|   Exit   |     -\t|  -/%d\n", idNum, queue_size);

		pthread_exit(NULL);
	}
}