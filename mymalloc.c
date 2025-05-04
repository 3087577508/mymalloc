#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>




#include<unistd.h>




#define MEMLENGTH 512
static double memory[MEMLENGTH];


void printArray(){
    int* a = (int*) memory;
    printf("\nMax: 1024\n");
    int i = 0;
    //for (int i = 0; i < 1023; i){
    while(i < 1023){
        if (1){
            //printf("Index %d is %d \n", i, a[i]);
            if(i % 2 == 0){
                printf("Index: %d    Flag: %d          ", i, a[i]);
                printf("Size(int): %d   Size(byte): %d \n", (a[i+1]/4), a[i+1]);

                i = i + (a[i+1] / 4 ) + 2 ;
                // if (a[i] == 2){
                //     i =i + 1 + (a[i + 1] /4);
                // }
            }
            
        }
        

    }
}



//void* mymalloc(size_t size){
void* mymalloc(size_t size, char* file, int line){
    int* heapstart = (int*) memory;

    int* tempstart = heapstart;

    
    int s = (int) size; //how many byte it requied


    //printf("require %d byte of space, tempstart is %p \n", s, tempstart);

    if((s == 0) || (s < 0) ){
        // 0 byte of space requied, bad
        printf("\n\nWARNING: request %d byte of space is not allowed, in file %s, line %d \n\n", s, file, line);

        return NULL;

    }



    //convert s to multiple of 8, for aliegnment sake
    for(int i = 0; i < 8; i++){
        if ((s % 8) != 0 ){
            s += 1;
        }
        else {
            break;
        }
    }
    //printf("\nmodify 8:       request %d bytes space\n", s);











    // want use pointer to go over the whole array
    while ((tempstart + (s/4)) < heapstart + (1023) ){ //compare in int
        //printf("\n\nin the while loop, with tempstart %p \n",tempstart);
        
        if(*tempstart == 1 || *tempstart == 2 || *tempstart == 0){
            
        }
        else{
            //printf("*tempstart is %d     ", *tempstart);
            printf("\n\nWaring: lost the connestion \n\n");
            return NULL;
        }


        if ( *tempstart == 0    &&  tempstart + (s/4) < heapstart + 1022       ){
            //the metadata not exist, or at end of array
            *tempstart = 1;
            int* tempsize  = tempstart+1;
            *tempsize = s;  //this much byte is used

            
            //return a void pointer at the start of metadata
            void* r = (void*) (tempstart + 2);

            tempstart = tempstart + (s/4) + 2;
            *tempstart = 2;
            *(tempstart + 1) = (heapstart + 1022 - tempstart) * 4;


            //printf("success, code 0,         location: %p\n", r);
            return r; 


        }else if(*tempstart == 1){
            //the chunck is used, looking for the next one

            int c = *(tempstart+1);
            int* d = tempstart;
            c = c/4; //for size in bytes is multiple of 8; /4 means move in size of int
            tempstart = tempstart + c + 2;  //add 2 for tempsize space and tempstart space

            //printf("working, code 1, looking for the next block. Now go from %p to %p. there are %d int size between\n", d, tempstart, c);


        }else if(*tempstart == 2){
            //this block was freed
            //printf("     s is %d, *tempstart+1 is %d", s, *(tempstart+1));
            if( s <= *(tempstart+1) ){
                // we can allocate here, set flag is 1
                //printf("s is %d\n", s);
                *tempstart = 1;

                int oldspace = *(tempstart+1); //avlible oldspace in byte

                //request s in byte
                //printf("find a freed block at %p size %d in byte, want %d byte\n", tempstart, oldspace,s);

                //r is the return pointer
                void* r = (void*) (tempstart+2);


                if (oldspace - s >8){
                    //exist more space than we need creat a new tempstart
                    //move tempstart to the new space
                    *(tempstart+1) = s;

                    tempstart = tempstart + ( s/4 ) + 2;
                    *tempstart = 2;
                    *(tempstart+1) = oldspace - 8 - s;


                    //printf("success, code 2, with new metadata \n");
                    
                }
                else{
                    //printf("success, code 2, with no new metadata \n");
                }
                return r;
                


            }else if(s >= *( tempstart+1) ){
                //this block is not enough, go to the next block 

                int c = *(tempstart+1);//c is the free block size
                c = c/4;
                tempstart = tempstart + c + 2;
                //printf("not enought space here move to %p\n", tempstart);
            }
        }

        //sleep(1);
    }



    printf("\n\nWANRING: not enough space in the whole heap to alocate %d bytes, in file %s, line %d\n\n", (int)size, file, line);
    return NULL;


}










//void myfree (void * ptr){
void myfree (void * ptr, char * file, int line){

    //printf("\n\nmyfree is called p is %p \n", ptr);
    int* p = (int*) ptr; // p is the thng we want to free
    
    int* heapstart = (int*) memory;

    int* tempstart = heapstart;

    int* prev = heapstart; //store the last pointer.

    if (p > heapstart + 1022 || p < heapstart){
        printf("\n\nWARNING: pointer is outside the heap, can not free, in file %s, line %d\n\n", file, line);
        
        return;
    }

    //printf("           target want to be free at %p  \n", p-2  );
    while (tempstart < heapstart + (1025)){
        //printf("now at  %p  \n", tempstart  );

        //printf("\n\nin to while loop now at %p \n", tempstart );

        
        if (p - 2 == tempstart){  //for p is a int pointer, (p - 2) means 8 byte before
            //get to the target, and this is a valid p
            //check if free a already freed block
            if (*tempstart == 1){
                *tempstart = 2;
                

                int* next = tempstart + 2 + (*(tempstart+1)/4);
                if (*next == 2){
                    // the next one is freed
                    //printf("the old size is %d \n", *(tempstart+1));

                    *(tempstart+1) = *(tempstart+1) + *(next+1) + 8;

                    //printf("the new size is %d \n", *(tempstart+1));
                }

                if(*prev == 2 && prev != tempstart){
                    //the last block is freed
                    //printf("prev is also freed\n");
                    if(*tempstart == 2){
                        *(prev+1) = *(prev+1) + *(tempstart+1) + 8;
                        //printf("prev become: %d", *(prev+1));
                    }
                    if (*tempstart == 0){
                        *prev = 0;
                    }
                    
                }



                //printf("block at %p was freed \n", tempstart);
                break;

            }else if(*tempstart == 2){
                //free a freed block
                printf("\n\nWARNING: trying to free a already freed block, in file %s, line %d\n\n", file, line );
                break;
            }
            

            

        }else if(tempstart < (p - 2) ){
            //not yet get to the targer, move and try next block
            int c = *(tempstart + 1); //c is the byte of a block
            //printf("        s1111111\n");

            c = c/4;

            prev = tempstart; 
            tempstart = tempstart + c + 2 ; //this should be the next block

            //printf("2121212 move to %p    c steps %d \n", tempstart,c);
            


            

        }else if(tempstart > ( p - 2 )){

            //after go through all chuck before p, p does not match to any metadata, then p is not a valid pointer
            printf("\n\nWARNING: p is not a valid address, can not be freed, in file %s, line %d\n\n", file, line );
            return;
        }
    }
    

    return;

}














// int main(int argc, char *argv[]) {

//     int* a = mymalloc(11);//16 = 4

//     int* b = mymalloc(18);//24 = 6
//     int* c = mymalloc(1);//8 = 2
//     int* d = mymalloc(30);
//     int* e = mymalloc(118);//120 = 30
//     int* f = mymalloc(200);// 50
//     int* g = mymalloc(300);//304 = 76
//     int* h = mymalloc(3000);
//     myfree(h);
//     int* i = mymalloc(1000);
//     int* j = mymalloc(1000);


//     printArray(memory);

//     myfree(a);
//     a = mymalloc(14);
//     //myfree(a);
//     myfree(b);
//     myfree(f);
//     myfree(f);
//     myfree(d);myfree(c);myfree(g);
//     myfree(e);
    
    
//     printArray(memory);


//     return 0;

// }