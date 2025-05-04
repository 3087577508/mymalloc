#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// #include<mymalloc.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#include<unistd.h>
#include<time.h>

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)



int main(int argc, char *argv[]) {

    // int* a = malloc(11);//16 = 4

    // int* b = malloc(18);//24 = 6
    // int* c = malloc(1);//8 = 2
    // int* d = malloc(30);
    // int* e = malloc(118);//120 = 30
    // int* f = malloc(200);// 50
    // int* g = malloc(300);//304 = 76
    // int* h = malloc(3000);
    // free(h);
    // int* i = malloc(1000);
    // int* j = malloc(1000);


    // //printArray(memory);

    // free(a);
    // a = malloc(14);
    // //myfree(a);
    // free(b);
    // free(f);
    // free(f);
    // free(d);free(c);free(g);
    // free(e);
    
    
    // //printArray(memory);




    srand(time(0));
    

    //test case 1: 50 times of 120 times of allocate and free right away
    struct timespec begin1;
    timespec_get(&begin1, TIME_UTC);
    
    for (int i = 0; i <  50; i++){
        for(int j = 0; j < 120; j++){
            int* a = malloc(1);
            free(a);
        }
        //printArray();
    }
    struct timespec end1;
    timespec_get(&end1, TIME_UTC);

    double time1 = ((end1.tv_sec - begin1.tv_sec)+ (end1.tv_nsec - begin1.tv_nsec)) / 1000000.0;

    //printArray();
    printf("\n\nFIRST TEST CASE took %f Milliseconds\n\n", time1);






    //test case 2: 120 1 byte allocate, then free, repeate 50 times
    int* immediate [120];
    struct timespec begin2;
    timespec_get(&begin2, TIME_UTC);



    for(int i = 0; i < 50; i ++){
        for(int j = 0; j < 120; j++){
            immediate[j] = malloc(1);
            // if(immediate[j] == NULL) {
            //     printf( "something went wrong i = %d  j = %d\n " ,i, j);
            // }
        }
        //printArray();
        for(int j = 0; j < 120; j++){
            free(immediate[j]);
        }
        //printArray();
    }

    struct timespec end2;
    timespec_get(&end2, TIME_UTC);

    double time2 = ((end2.tv_sec - begin2.tv_sec)+ (end2.tv_nsec - begin2.tv_nsec)) / 1000000.0;

    //printArray();
    printf("\n\nSECOND TEST CASE took %f Milliseconds\n\n\n", time2);






    //case three randome:
    struct timespec begin3;
    timespec_get(&begin3, TIME_UTC);




    
    int* r [120];
    int i = 0; //how many times have allocated
    int j = 0; //the last's next pointer index

    while ( i < 120 || j > 0 ){
        int random = rand() % 100;   //random is between 0 to 100
        
        //printf("i is %d   j is %d      random is %d\n",i, j, random);
        if (random > 50){
            // allocate a new 1 byte
            
            r[j] = malloc(1);

            i += 1;
            j += 1;
        }else{
            //deallocate an object
            if(j > 0){
                free(r[j - 1]);
                j -= 1;
                //printf("           %d\n", *r[j]);
                
            }
        }
    }

    struct timespec end3;
    timespec_get(&end3, TIME_UTC);

    double time3 = ((end3.tv_sec - begin3.tv_sec)+ (end3.tv_nsec - begin3.tv_nsec)) / 1000000.0;

    //printArray();
    printf("\n\nTHIRD TEST CASE took %f Milliseconds\n\n\n", time3);


    







    //MY test 4: check if require too much space for 10 times， and free a invalid pointer 
    printf("\n\n\nTEST 4 BEGIN");
    printf("\n\nexpect error, not enough space in whole heap");

    struct timespec begin4;
    timespec_get(&begin4, TIME_UTC);

    int* bigarray [600];
    j = 0;
    for (int i = 0; i < 1; i++){
        bigarray[j] = malloc(100);
        while (bigarray[j]  != NULL){
            j += 1;
            bigarray[j] = malloc(100);
        }
        //printArray();
        //printf("     i == %d,  j == %d\n\n" , i , j);
        while (j > 0){
            j -= 1;
            free(bigarray[j]);

            
            //wrong free      100% of chance will see this happen
            int x = rand() % 100;
            //printf("222222222 %p   %d \n", bigarray[j], x);
            if (x < 10){
                
                if ( x == 0 ){
                    printf("\n\nx = %d, expect error on double free", x);
                }else{
                    printf("\n\nx = %d, expece error on invalid pointer",x);
                }

                //printf("\n\n bigarray [j] is %p,    bigarray[j] + 1 is %p\n", bigarray[j], bigarray[j] + 1);
                free( (bigarray[j]) + x);
                //printf("1111111 %p\n", bigarray[j] + x);
            }
            
        }
        //printArray();
        //printf("i == %d,  j == %d\n\n" , i , j);
    }



    struct timespec end4;
    timespec_get(&end4, TIME_UTC);

    double time4 = ((end4.tv_sec - begin4.tv_sec)+ (end4.tv_nsec - begin4.tv_nsec)) / 1000000.0;

    
    printf("\n\nFORTH TEST CASE took %f Milliseconds\n\n\n", time4);






    printf("\n\n\nTEST 5 BEGIN");
    struct timespec begin5;
    timespec_get(&begin5, TIME_UTC);


    for(int j = 0; j < 1; j++){
        int* testFour [30];
        int total = 0;
        for(int i = 0; i < 30; i++){
            int ran = rand() % 50 ; 
            total += ran + 2;

            //printf(" i is %d      ran is %d\n",i, ran);

            if(total > 1024){
                printf("\n\nexpect error, do not have enought space");
            }
            if (ran == 0){
                printf("\n\nexpect error, request 0 byte is not allowed");
            }

            testFour[i] = malloc(ran);


        }
        for(int i = 29; i > -1; i--){
            if (testFour[i] == NULL){
                printf("\n\nexpect error, pointer is outside the heap");
            }
            
            free(testFour[i]);
        }

        //printArray();
    }
    

    //printArray();

    struct timespec end5;
    timespec_get(&end5, TIME_UTC);

    double time5 = ((end5.tv_sec - begin5.tv_sec)+ (end5.tv_nsec - begin5.tv_nsec)) / 1000000.0;

    
    printf("\n\nFIFTH TEST CASE took %f Milliseconds\n\n", time5);



    return 0;

}