#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>


char ** w_grid;
char * r_grid;
char * arr;
//char arr[25] = {'-', '-', '-', '-', '-', '-', '-', 'x', '-', '-', '-', '-', 'x', '-', '-', '-', '-', 'x', '-', '-', '-', '-', '-', '-', '-'};
int * start;
int * finish;
int siz, nThreads, nIterations, display, gridSize;
pthread_barrier_t barrier;


void * cgol(void* rank){
    long my_rank = (long) rank;
    int i, x, y, k, iter;
    for(iter = 0; iter < nIterations; iter++){
        for(i = start[my_rank]; i < finish[my_rank]; i++){

            int dead = 0;
            int live = 0;
            if (i != 0){
                x = i / siz;
                y = i % siz;
            }
            else{
                x = 0;
                y = 0;
            }

            if (x != 0 && x != siz - 1 && y != 0 && y != siz - 1){ // middle
                if(r_grid[siz * (x - 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }

            }

            else if(x == 0 && y != 0 && y != siz - 1){  //top row excluding corners
                if(r_grid[siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }

            else if(x == siz - 1 && y != 0 && y != siz - 1){  //bottom row excluding corners
                if(r_grid[siz * (x - 1) + y -1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }


            else if(y == 0 && x != 0 && x != siz - 1){  //left row excluding corners
                if(r_grid[siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }

            else if(y == siz - 1 && x != 0 && x != siz - 1){  //right row excluding corners
            if(r_grid[siz * (x - 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            
            }
            else if (x == 0 && y == 0){   //top left corner
                if(r_grid[siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }

            }
            
            else if(x == 0 && y == siz - 1){ //top right corner
                if(r_grid[siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x + 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }

            else if (x == siz - 1 && y == 0){ //bottom left corner
                if(r_grid[siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y + 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }

            }

            else if (x == siz - 1 && y == siz - 1){ //bottom right corner
                if(r_grid[siz * (x - 1) + y -1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * (x - 1) + y] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
                if(r_grid[siz * x + y - 1] == 'x'){
                    live++;
                }
                else{
                    dead++;
                }
            }
            
            //printf("%d = %c    live =  %d      dead =  %d\n", i, r_grid[i], live, dead);
            if(r_grid[i] == 'x'){
                if(live < 2 || live > 3){
                    w_grid[iter][i] = '-';
                }
                else{
                    w_grid[iter][i] = 'x';
                }
            }
            else if(r_grid[i] == '-'){
                if(live == 3){
                    w_grid[iter][i] = 'x';
                }
                else{
                    w_grid[iter][i] = '-';
                }
            }
        }
        for(k = start[my_rank];k<finish[my_rank];k++){
                arr[k] = w_grid[iter][k];
        }
        pthread_barrier_wait(&barrier);
        for(k = start[my_rank];k<finish[my_rank];k++){
                r_grid[k] = arr[k];
        }
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
    long thread;
    int i, j, k;
    if ( (argc < 4 && strcmp(argv[argc-1], "-d") != 0) || (argc < 5 && strcmp(argv[argc-1], "-d") == 0)){   // getting and setting arguments
        printf("Too few arguments to start program\n");
        exit(-1);
    }
    nThreads = atoi(argv[1]);
    gridSize = atoi(argv[2]);
    nIterations = atoi(argv[3]);
    display = 0;
    if(nThreads > gridSize){
        nThreads = gridSize;
    }
    if (argc == 5){
        display = 1;
    }
    int idx = gridSize * gridSize;  //creating structs to pass to the thread function
    int num = idx / nThreads;
    int rem = idx % nThreads;
    r_grid = malloc(idx * sizeof(char));
    w_grid = malloc(nIterations * sizeof(char*));
    for(i = 0; i < nIterations; i++){
        w_grid[i] = malloc(idx * sizeof(char));
    }
    start = malloc(nThreads * sizeof(int));
    finish = malloc(nThreads * sizeof(int));
    arr = malloc(idx * sizeof(char*));
    createRand(idx, arr);

    for(j=0; j < idx;j++){
            r_grid[j] = arr[j];  //the grid used to read
            w_grid[0][j] = arr[j];  //the grid used to write      
    } 
    for(i=0;i<nThreads;i++){
        siz = gridSize;  //full grid size -- might delete     
        if(i != nThreads - 1){
            start[i] = i*num;     //range of indeces in the array the thread is allowed to edit
            finish[i] = i*num + num;
        }
        else{
            start[i] = i*num;
            finish[i] = i*num + num + rem;
        }
        //printf("%d %d\n", grids[i].start, grids[i].finish);
    }
    if(display == 1){
        for(i = 0; i < idx; i++){
            if(i % gridSize == 0){
                printf("\n");
            }
            printf(" %c ", arr[i]);
        }
        printf("\n\n");

    }
    pthread_barrier_init(&barrier, NULL, nThreads);    
    pthread_t grid_secs[nThreads];
    clock_t begin = clock();
    for(thread = 0; thread < nThreads; thread++){
        pthread_create(&grid_secs[thread], NULL, cgol, (void*) thread);
        //printf("thread %d finished\n", grid_secs[i]);
    }

    for(i = 0; i < nThreads; i++){
        pthread_join(grid_secs[i], NULL);
    }
    clock_t end = clock();
    double time_elap = (double)(end - begin) / CLOCKS_PER_SEC;
    if(display == 1){
        for(j = 0; j < nIterations; j++){
            for(i = 0; i < nThreads; i++){
                for(k = start[i];k<finish[i];k++){
                    if(k % gridSize == 0){
                        printf("\n");
                    }
                    printf(" %c ", w_grid[j][k]);
                }
            }
            printf("\n\n");
        }
    }

    printf("Elapsed time = %f seconds\n\n", time_elap);
    pthread_barrier_destroy(&barrier);
    for(i = 0; i < nThreads; i++){
        free(w_grid[i]);
    }
    free(w_grid);
    free(r_grid);
    free(start);
    free(finish);
    
}