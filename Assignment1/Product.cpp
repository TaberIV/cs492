#include "assign1.h"

class Product {
	int id;
	int time_created;
	int life;

	public:
	Product(int id) {
		this->id = id;
		this->time_created = clock();
		this->life = random() % 1024;


	}
};