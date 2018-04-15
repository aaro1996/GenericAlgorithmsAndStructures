#include "test_int.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

	
int min_rand = 0;
int max_rand = 200;
int capacity = 1000;
int num_tests = 5000;

void handle_signal(int signal);

void handle_signal(int signal) {
	printf("\e[?25h");
	printf("\033[0;0m");
	printf("\n");
    exit(0);
}

int gen_ran_num(int min, int max) {
	return (min + (rand() % (max - min)));
}

int remove_vals(struct rbt* rbt, int start, int* array) {
	int extracted = 0;
	if(start == 0) {
		return 0;
	}
	int end = gen_ran_num(0, start);
	for(int i = start-1; i >= end; i--) {
		if(rbt_extract(rbt, array[i], &extracted)) {
			assert(extracted == array[i]);
		}
	}
	return end;

}

int fill_vals(struct rbt* rbt, int start, int* array){
	int end = gen_ran_num(start, capacity);
	for(int i = start; i < end; i++) {
		array[i] = gen_ran_num(min_rand, max_rand);
		rbt_insert(rbt, array[i]);
	}
	return end;
}

int random_test() {
	struct sigaction sa;
	sa.sa_handler = &handle_signal;
	sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

	int extracted = 0;
	srand(time(NULL));
	int *values = calloc(capacity, sizeof(int));
	int i = 0;
	struct rbt* rbt;
	rbt_init(&rbt);
	printf("\e[?25l");
	for(int n = 0; n <= num_tests; n++) {
		i = fill_vals(rbt, i, values);
		i = remove_vals(rbt, i, values);
		assert(!rbt_check(rbt));
		if(!(n%(num_tests/100))) {
			printf("\r|");
			int j;
			printf("\033[0;46m");
			for(j = 0; j < (100*n/num_tests); j+=2) {
				printf(" ");
			}
			printf("\033[0;0m");
			for(; j < 100; j+=2) {
				printf(" ");
			}
			printf("| %02d%%", 100*n/num_tests);
			fflush(stdout);
		}
	}
	for(i--; i >= 0; i--) {
		if(rbt_extract(rbt, values[i], &extracted)) {
			assert(extracted == values[i]);		
		}
	}
	assert(rbt->root == NULL);
	printf("\e[?25h");
	printf(" All TESTS COMPLETE\n");
	return 0;
}

int prompt() {
	struct rbt* rbt;
	rbt_init(&rbt);
	while(1) {
		int insertable = 0;
		int value = 0;
		int extracted = 0;
		printf("%s", "Please select what you want to do:\n\t1. Insert a number\n\t2. Remove a number\n");
		scanf("%d", &insertable);
		switch(insertable) {
			case 1:
				printf("%s", "Please input a number to insert:");
				scanf("%d", &value);
				rbt_insert(rbt, value);
			break;
			case 2:
				printf("%s", "Please input a number to remove:");
				scanf("%d", &value);
				if(rbt_extract(rbt, value, &extracted)) {
					printf("Extracted value %d\n", extracted);
				} else {
					printf("Value not found\n");
				}
			break;
		}
		printf("\n");
		rbt_print_all(rbt);
	}
}

int main(int argc, char** argv) {
	int opt, mode = 1;
	while ((opt = getopt(argc, argv, "m:c:f:r:n:")) != -1) {
		switch(opt) {
			case 'm': //mode
				mode = atoi(optarg);
				break;
			case 'c': //capacity
				capacity = atoi(optarg);
				break;
			case 'f': //floor
				min_rand = atoi(optarg);
				break;
			case 'r': //roof
				max_rand = atoi(optarg);
				break;
			case 'n': //number of tests
				num_tests = atoi(optarg);
				break;
		}
	}
	switch(mode) {
		case 1: 
			prompt();
			break;
		case 2:
			random_test();
	}
	return 0;
}