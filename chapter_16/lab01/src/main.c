#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

uint64_t data[100] = {0};

typedef struct not_padding_data_s {
    uint64_t a;
	uint64_t b;	
}not_padding_data_t __attribute__((__aligned__(64)));

typedef struct padding_s {
	char x[0];
}padding_t __attribute__((__aligned__(64)));

typedef struct padding_data_s {
    uint64_t a;
	padding_t _pad;
	uint64_t b;	
}padding_data_t __attribute__((__aligned__(64)));

typedef struct thread_arg_s 
{
	int fasle_sharing;
	int use_array;
}thread_arg_t;

static not_padding_data_t not_padding_data;
static padding_data_t padding_data;

#define MAX_LOOP 10000000000

void* thread_1(void* args)
{
	thread_arg_t* thread_arg = (thread_arg_t*) args;

	if(thread_arg->use_array)
	{
		for(uint64_t i = 0; i < MAX_LOOP; i++)
		{
			data[0] += 1;
		}
	}
	else
	{
		if(thread_arg->fasle_sharing)
		{
			for(uint64_t i = 0; i < MAX_LOOP; i++)
			{
				not_padding_data.a += 1;
			}
		}
		else
		{
			for(uint64_t i = 0; i < MAX_LOOP; i++)
			{
				padding_data.a += 1;
			}
		}
	}

	return NULL;
}

void* thread_2(void* args)
{
	thread_arg_t* thread_arg = (thread_arg_t*) args;

	if(thread_arg->use_array)
	{
		if(thread_arg->fasle_sharing)
		{
			for(uint64_t i = 0; i < MAX_LOOP; i++)
			{
				data[1] += 1;
			}
		}
		else
		{
			for(uint64_t i = 0; i < MAX_LOOP; i++)
			{
				data[90] += 1;
			}	
		}		
	}
	else
	{
		if(thread_arg->fasle_sharing)
		{
			for(uint64_t i = 0; i < MAX_LOOP; i++)
			{
				not_padding_data.b += 1;
			}
		}
		else
		{
			for(uint64_t i = 0; i < MAX_LOOP; i++)
			{
				padding_data.b += 1;
			}
		}
	}

	return NULL;
}


static int test_false_sharing(thread_arg_t* arg)
{
	struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);

	printf("test_false_sharing, %s , %s.\r\n",
		arg->fasle_sharing ? "cache false sharing" : "cache without false sharing",
		arg->use_array? "use use_array" : "use data struct");

	pthread_t thread1, thread2;
    
    // Create threads
    if (pthread_create(&thread1, NULL, thread_1, arg) != 0) {
        printf("Failed to create thread_1\r\n");
        return -1;
    }

    if (pthread_create(&thread2, NULL, thread_2, arg) != 0) {
        printf("Failed to create thread_2\r\n");
        return -1;
    }

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    clock_gettime(CLOCK_REALTIME, &end);

	double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                        (end.tv_nsec - start.tv_nsec) / 1000000.0;

	printf("Total execution time: %.3f ms\n", elapsed_ms);
    return 0;
} 

int main(void) 
{
	thread_arg_t args;

	args.fasle_sharing = 0;
	args.use_array = 0;
	
	test_false_sharing(&args);

	args.fasle_sharing = 1;
	test_false_sharing(&args);

	args.fasle_sharing = 0;
	args.use_array = 1;
	
	test_false_sharing(&args);

	args.fasle_sharing = 1;
	test_false_sharing(&args);

    return 0;
}