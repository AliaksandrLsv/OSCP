#include <cstring>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define psleep(sec) sleep ((sec))
 
#define N_A   20
#define N_B      15
#define periode_A    10000
#define periode_B    100000  // 10000
#define min(a,b) (a<=b?a:b)

/* Structure to stock information of the meusum. */
typedef struct
{
   int AB;  // visitors that go from A to B
   int BA;  // Visitors that return to A from B.
   int y1;  // visitors staying in B after the periode T.
   int x1; // visitors staying in A after the periode T.
   int N;  // number of all visitors of the museum. 
   // to delete : pthread_t thread_A;
   // to delete : pthread_t thread_B;
}
museum_t;
static museum_t museum =
{
   .AB = 0,
   .BA = 0,
   .y1 = 0,
   .x1 = 0,
   .N = 0,
};
 
static pthread_mutex_t mutex_stock = PTHREAD_MUTEX_INITIALIZER; 
 
/* Function to choose a random number between 0 and max. */
static int get_random (int max)
{
   double val;
 
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return ((int) val);
}

static int* nxt_A(int x)
{
    int *param = (int*) malloc(sizeof(int) * 4);
    *param = get_random(x);  // visitors that leave.
    museum.AB = get_random(min(x - param[0], N_B - museum.y1));
    // Number of visitors deciding to go from A to B at instance T.
    *(param + 1) = get_random((N_A - x));  // new visitors 
    museum.N = museum.N + param[1];  // update number total of visitors.
    museum.x1 = x - param[0] - museum.AB;  // number of visitors that staying in A.
    *(param + 3) = museum.x1 + museum.BA + param[1];  // total of visitors in A at instant T+1.
    return param;
 
}

static int nxt_B(int y)
{
    int nxt_y;
    // Number of visitors deciding to go from B to A at instance T.
    museum.BA = get_random(min(y, N_A - museum.x1));
    museum.y1 = y - museum.BA;  // number of visitors that staying in B.
    nxt_y = museum.AB + museum.y1;  // total of visitors in B at instant T+1.
    return nxt_y;
};

/* Function of the thread hull A. */
static void * fn_A (void * p_data)
{
   int x = 0;  // hull A is empty at t=0
   while (1)
   {
       pthread_mutex_lock (&mutex_stock);
       int *param= nxt_A(x);
       x = param[3];  // number of visitors in A at instant T+1
       printf("Number total of vistors : %d \n", museum.N);
       printf("Number of visitors in A is:%d \n", x);
       pthread_mutex_unlock (&mutex_stock);
       usleep(periode_A);
   }
 
   return NULL;
}
 
/* Function of the thread hull B */
static void * fn_B (void * p_data)
{
   int y = 0;  // hull B is empty at t=0
   while (1)
   {
       pthread_mutex_lock (&mutex_stock);
       y = nxt_B(y);  // number of visitors in B at instant T+1.
       printf("\033[91mNumber of visitors in B %d \033[0m\n", y);
       pthread_mutex_unlock (&mutex_stock);
       usleep(periode_B);
   }
 
   return NULL;
}
int main (void)
{
   int i = 0;
   int ret = 0;
   pthread_t thread_A;
   pthread_t thread_B;

   /* Creation of thread A. */
   ret = pthread_create (
      &thread_A, NULL,
      fn_A, NULL
   );
   /* Creation of thread B. */
   ret = pthread_create (
      &thread_B, NULL,
      fn_B, NULL
   );
 
   /* waiting for the end threads. */
   pthread_join (thread_A, NULL);
   pthread_join (thread_B, NULL);

 
   return EXIT_SUCCESS;
}
