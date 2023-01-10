#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

//mpicc A2.c -o A2 -lm
//mpiexec -n 2 ./A2 test.txt 


/*
Explanation on how my letter count works
    I search for the letter that has the maximum count
    If that maximum is smaller or equal to 40, then no scaling is present
    if the maximum is greater than 40:
        max is converted into a float and divided by 40.0 resulting in a floating point number with the variable div
        the letter counts of all the letters is then converted into a float, divided by div and finally rounded and converted into an int

        if I got 41 a's and 5 of every other letter, 41.0/40.0 = 1.025
        5.0 / 1.025 = 4.88 which after rounding and conversion is 5.
        so the letter count will display 40 of 'a' and 5 of each other letter

        similarly if I got 60 a's and 5 of every other letter
        in the end I will have 40 of 'a' and 3 of every other letter.
        in other words the results were scaled down to 2/3 of the original size. 

        Similarly - as per Denis' announcement. If a value that was previously >0 get scaled down to 0, it will be set to 1
*/


int main(int argc, char *argv[]){
    int count;
    int num_p;               /* Number of processes    */
    int my_rank;               /* My process rank        */
    int i,j;
    char * words;
    char c;
    
    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /*main proc*/
    if(my_rank == 0){
        char fileName[50];
        int is_s = 0;
        int is_l = 0;

        if(argc == 2){
            strcpy(fileName, argv[1]);
        }
        else if(argc == 3){
            strcpy(fileName, argv[2]);
            if( strcmp(argv[1] , "-l") == 0){
                is_l = 1;
            }
            else if( strcmp(argv[1] , "-s") == 0){
                is_s = 1;
            }
        }
        else if(argc == 4){
            strcpy(fileName, argv[3]);

            if( strcmp(argv[1] , "-l") == 0){
                is_l = 1;
            }
            else if( strcmp(argv[1] , "-s") == 0){
                is_s = 1;
            }

            if( strcmp(argv[2] , "-l") == 0){
                is_l = 1;
            }
            else if( strcmp(argv[2] , "-s") == 0){
                is_s = 1;
            }
        }
        else{
            printf("\n\nInvalid arguments. Usage: ./A2 [-l] [-s] filename\nAny other formatting will not work\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
            exit(0);
        }

        int let_cnt[num_p][26];
        for(j = 0; j < num_p; j++){
            for(i = 0; i < 26; i++){
                let_cnt[j][i] = 0;
            }   
        }
        
        int final_cnt[26];
        for(i = 0; i < 26; i++){
            final_cnt[i] = 0;
        }

        FILE *file, *outf;
        

        //lazy way of determining size of array -- happens at runtime though...
        file = fopen(fileName, "r");
        if(file == NULL){
            printf("\n\nFile %s does not exist\n\n", fileName);
            MPI_Abort(MPI_COMM_WORLD, -1);
            exit(0);
        }
        count = 0;
        while( (c = fgetc(file)) != EOF){
            count++;
        }
        //printf("count = %d\n", count);
        fclose(file);
        for(i = 1; i < num_p; i++){
            MPI_Send(&count, 1, MPI_INT, i, 0 , MPI_COMM_WORLD);
        }

        file = fopen(fileName, "r");
        words = malloc(count * sizeof(char));
        i = 0;
        while( (c = fgetc(file)) != EOF){
            words[i] = c;
            i++;
        }
        fclose(file);
        for(i = 1; i < num_p; i++){
            MPI_Send(words, count, MPI_CHAR, i, 1 , MPI_COMM_WORLD);
        }

        int interval = count / (num_p);
        int rem = count % (num_p);
        int start = interval * my_rank;
        int finish = interval * (my_rank + 1);
        if(my_rank == num_p - 1){
            finish = finish + rem;
        }
        //printf("process %d -----   %d %d %d %d\n\n", my_rank, start, finish, count);
        
        for(i = start; i < finish; i++){
            c = words[i];
            for(j = 0; j < 26; j++){
                if (c == (char)(j + 65) || c == (char)(j + 97)){
                    let_cnt[0][j] = let_cnt[0][j] + 1;
                    break;
                }
            }
        }

        for(i = 1; i < num_p; i++){
            MPI_Recv(let_cnt[i], 26, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for(i = 0; i < 26; i++){
            for(j = 0; j < num_p; j++){
                final_cnt[i] = final_cnt[i] + let_cnt[j][i];
            }
        }

        if(is_s == 1){
             outf = fopen("out.txt","w");
        }
        if(is_l == 0){
            for(i = 0; i<26; i++){
                printf("%c %d\n", i+65, final_cnt[i]);
                if(is_s == 1){
                    fprintf(outf, "%c %d\n", i+65, final_cnt[i]);
                }
            }
        }
        else{
            double div;
            int max = 0;
            for(i = 0; i<26; i++){
                if(final_cnt[i] > max){
                    max = final_cnt[i];
                }
            }
            if(max > 40){
                div = (double)max / (double)40.0;
                // if (max % 40 != 0){
                //     max = max / 40 + 1;
                // }
                // else{
                //     max = max / 40;
                // }
            }
            else{
                div = 1;
            }
            int repeat;
            for(i = 0; i<26; i++){
                repeat = (int)( round((double)final_cnt[i] / div));
                if (repeat == 0 && final_cnt[i] != 0){
                    repeat = 1;
                }
                for(j = 0; j < repeat; j++ ){
                    printf("%c", i+65);
                    if(is_s == 1){
                        fprintf(outf, "%c", i+65);
                    }
                }
                printf("\n");
                if(is_s == 1){
                    fprintf(outf, "\n");
                }
            }
        }
        if(is_s == 1){
             fclose(outf);
        } 



    }
    else if(my_rank != 0){
        MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //printf("%d %d\n", count, num_p);
        int interval = count / (num_p);
        int rem = count % (num_p);
        int let_cnt1[26];
        for(i = 0; i < 26; i++){
            let_cnt1[i] = 0;  
        }
        char * word = malloc(count * sizeof(char));
        MPI_Recv(word, count, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int start = interval * my_rank;
        int finish = interval * (my_rank + 1);
        if(my_rank == num_p - 1){
            finish = finish + rem;
        }
        //printf("process %d -----   %d %d\n\n", my_rank, start, finish);
        for(i = start; i < finish; i++){
            c = word[i];
            //printf("%c", c);
            for(j = 0; j < 26; j++){
                if (c == (char)(j + 65) || c == (char)(j + 97)){
                    let_cnt1[j] = let_cnt1[j] + 1;
                    break;
                }
            }
        }
        free(word);
        MPI_Send(let_cnt1, 26, MPI_INT, 0, 2, MPI_COMM_WORLD);

    }
    MPI_Finalize(); 
    return 0;

}  /* main */
