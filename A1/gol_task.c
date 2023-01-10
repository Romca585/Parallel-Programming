#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>

int done;
int eqs;
int lqs;
int siz;
int g_siz;
char * r_grid;
char ** w_grid;
int * e_queue;
int * l_queue;
int nIterations;
char * arr;
//char arr[25] = {'-', '-', '-', '-', '-', '-', '-', 'x', '-', '-', '-', '-', 'x', '-', '-', '-', '-', 'x', '-', '-', '-', '-', '-', '-', '-'};
pthread_mutex_t mutex1;
pthread_t write_el, read_e, read_l;
pthread_barrier_t barrier;
pthread_cond_t cond;


void * writeQueues(void *rank){
    int i,j;
    int x,y;
    for(j = 0; j < nIterations; j++){
        //printf("%d\n", j);
        eqs = lqs = 0;
        //printf("thread 0 is starting\n");
        for(i = 0; i < siz; i++){
            int dead = 0;
            int live = 0;
            if (i != 0){
                x = i / g_siz;
                y = i % g_siz;
            }
            else{
                x = 0;
                y = 0;
            }

            if (x != 0 && x != g_siz - 1 && y != 0 && y != g_siz - 1){ // middle
                if(r_grid[g_siz * (x - 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }

            }

            else if(x == 0 && y != 0 && y != g_siz - 1){  //top row excluding corners
                if(r_grid[g_siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }

            else if(x == g_siz - 1 && y != 0 && y != g_siz - 1){  //bottom row excluding corners
                if(r_grid[g_siz * (x - 1) + y -1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }


            else if(y == 0 && x != 0 && x != g_siz - 1){  //left row excluding corners
                if(r_grid[g_siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }

            else if(y == g_siz - 1 && x != 0 && x != g_siz - 1){  //right row excluding corners
            if(r_grid[g_siz * (x - 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            
            }
            else if (x == 0 && y == 0){   //top left corner
                if(r_grid[g_siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }

            }
            
            else if(x == 0 && y == g_siz - 1){ //top right corner
                if(r_grid[g_siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }

            else if (x == g_siz - 1 && y == 0){ //bottom left corner
                if(r_grid[g_siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }

            }

            else if (x == g_siz - 1 && y == g_siz - 1){ //bottom right corner
                if(r_grid[g_siz * (x - 1) + y -1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[g_siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }
            
            //printf("%d = %c    live =  %d      dead =  %d\n", i, r_grid[i], live, dead);
            pthread_mutex_lock( &mutex1 );
            if(r_grid[i] == 'x'){
                if(live < 2 || live > 3){
                    //printf("a   %d\n",eqs);
                    e_queue[eqs] = i;
                    eqs++;
                }
                else{
                    l_queue[lqs] = i;
                    lqs++;
                }
            }
            else if(r_grid[i] == '-'){
                if(live == 3){
                    l_queue[lqs] = i;
                    lqs++;
                }
                else{
                    //printf("b   %d\n",eqs);
                    e_queue[eqs] = i;
                    eqs++;
                }
            }
            pthread_mutex_unlock( &mutex1 );

        }
        //pthread_mutex_lock( &mutex1 );
        done = 1;
        //pthread_mutex_unlock( &mutex1 );
        //printf("thread 0 reached checkpoint 1\n");
        pthread_barrier_wait(&barrier);
        //printf("thread 0 continuining from checkpoint 1\n");
        pthread_mutex_lock( &mutex1 );
        done = 0;
        for(i = 0; i < siz; i++){
            r_grid[i] = w_grid[j][i];
        }
        // for(i = 0; i < siz; i++){
        //     if(i%g_siz == 0){
        //         printf("\n");
        //     }
        //     printf(" %c ", w_grid[j][i]);
        // }
        // printf("\n");

        pthread_mutex_unlock( &mutex1 );
        //printf("thread 0 reached checkpoint 2\n");
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}




void * fillEmpty(void *rank){
    int j;
    for(j = 0; j < nIterations;j++){
        //printf("thread 1 is starting - done =  %d\n", done);
            while(eqs != 0 || done == 0){
                pthread_mutex_lock( &mutex1 );
                if(eqs != 0){
                    //printf("dead -> %d\n", e_queue[0]);
                    w_grid[j][e_queue[eqs-1]] = '-';
                    e_queue[eqs-1] = -1;
                    eqs--;
                }
                pthread_mutex_unlock( &mutex1 );
            }
        //printf("thread 1 reached checkpoint 1\n");
        pthread_barrier_wait(&barrier);
        //printf("thread 1 reached checkpoint 2\n");
        pthread_barrier_wait(&barrier);
        
    }
    return NULL;
}

void * fillLive(void *rank){
    int j;
    for(j = 0; j < nIterations;j++){
        //printf("thread 2 is starting done = %d\n", done);
        while(lqs != 0 || done == 0){
            pthread_mutex_lock( &mutex1 );
            if(lqs != 0){
                //printf("live -> %d\n", l_queue[0]);
                w_grid[j][l_queue[lqs-1]] = 'x';
                l_queue[lqs-1] = -1;
                lqs--;
            }
            pthread_mutex_unlock( &mutex1 );
        }
        //printf("thread 2 reached checkpoint 1\n");
        pthread_barrier_wait(&barrier);
        //printf("thread 2 reached checkpoint 2\n");
        pthread_barrier_wait(&barrier);
        
    }
    return NULL;
}

void * createRand(int siz, char * arr){
    int i, j;
    time_t t;
    srand((unsigned) time(&t));
    for(j = 0; j < siz; j++){
        for(i = 0; i < 10; i++){
            if(rand() % 2 == 0){
                arr[j] = 'x';
            }
            else{
                arr[j] = '-';
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    //prereq stuff
    if ( (argc < 3 && strcmp(argv[argc-1], "-d") != 0) || (argc < 4 && strcmp(argv[argc-1], "-d") == 0)){   // getting and setting arguments
        printf("Too few arguments to start program\n");
        exit(-1);
    }
    int i, j;
    int gridSize = atoi(argv[1]);
    nIterations = atoi(argv[2]);
    int display = 0;
    if (argc == 4){
        display = 1;
    }
    siz = gridSize * gridSize;
    arr = malloc(siz * sizeof(char));
    createRand(siz, arr);
    if(display==1){
        for(i = 0; i < siz; i++){
            if(i%gridSize == 0){
                printf("\n");
            }
            printf(" %c ", arr[i]);
        }
        printf("\n");
    }
    g_siz = gridSize;
    r_grid = malloc(siz * sizeof(char));
    w_grid = malloc(nIterations * sizeof(char*));
    for(i=0; i<nIterations;i++){
        w_grid[i] = malloc(siz * sizeof(char));
    }
    e_queue = malloc(siz * sizeof(int));
    l_queue = malloc(siz * sizeof(int));

    for(j = 0; j < siz; j++){
        r_grid[j] = arr[j];  //the grid used to read
        w_grid[0][j] = arr[j];  //the grid used to write      
    }

    if ( pthread_barrier_init(&barrier, NULL, 3) != 0){
        printf( "barrier init failed\n" );
        exit(0);
    }
    if ( pthread_mutex_init( &mutex1, NULL) != 0 ){
        printf( "mutex init failed\n" );
        exit(0);
    }


    clock_t begin = clock();
    pthread_create(&write_el, NULL, writeQueues, (void*)0);
    pthread_create(&read_e, NULL, fillEmpty, (void*)1);
    pthread_create(&read_l, NULL, fillLive, (void*)2);

    pthread_join(write_el, NULL);
    pthread_join(read_e, NULL);
    pthread_join(read_l, NULL);
    clock_t end = clock();
    double time_elap = (double)(end - begin) / CLOCKS_PER_SEC;

    if(display==1){
        for(j = 0; j < nIterations;j++){
            for(i = 0; i < siz; i++){
                if(i%gridSize == 0){
                    printf("\n");
                }
                printf(" %c ", w_grid[j][i]);
            }
            printf("\n");
        }
    }
    printf("Elapsed time = %f seconds\n\n", time_elap);

    pthread_barrier_destroy(&barrier);
    for(i = 0; i < nIterations; i++){
        free(w_grid[i]);
    }
    free(w_grid);
    free(r_grid);
    free(e_queue);
    free(l_queue);
    free(arr);


}