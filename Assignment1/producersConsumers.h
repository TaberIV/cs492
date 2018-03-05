#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "threadMethods.cpp"
#include "Product.cpp"

using namespace std;

// Create globals
queue<Product> productQueue;
clock_t t;

int product_count = 0;
pthread_mutex_t product_count_mutex;