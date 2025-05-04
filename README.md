# mymalloc
own versions of the standard library functions malloc() and free()


Kanran Peng
kp942

Xicheng Li
xl657



mymalloc():  
error case: malloc() will return NULL in case of error 

    1. request 0 or <0 bytes of space is not allowed.  
        WARNING: request %d byte of space is not allowed, in file %s, line %d 

    2. if malloc a space (say n bytes), but store more than n bytes things, then the array will lost connection, but this is not a strong check, 
        do not rely on this feature too much, since malloc() do not know what user store in it.  
        Waring: lost the connestion 

    3. the whole array is full, could not find enough space in, freed chunk, nor the unused part. 
        WANRING: not enough space in the whole heap to alocate %d bytes, in file %s, line %d 




myfree():
error case: free() will print out error message in case of error. 

    1. pointer is not in the range of the heap. 
        WARNING: pointer is outside the heap, can not free, in file %s, line %d 

    2. trying to free a chunk which already be freed before. (double free) 
        WARNING: trying to free a already freed chunk, in file %s, line %d 

    3. pointer is not a valid pointer, means it is not the begining of a payload. 
        WARNING: p is not a valid address, can not be freed, in file %s, line %d 



feature: 
    1. free a chunk, say it is the n's chunk, then free() will go check the chunk before n and the chunk after n. 
        if any of those 2 chunk is freed, free() will combine them to be a larger free chunk. If both are freed, 
        free() will mark the n-1 chunk as freed, with size of all three chunks. 
    2. there is a printArray() function in mymalloc.c it can print the valid matadata. it is // in mymalloc.h, if you 
        want to use it just delete the //. 




memgrind.c: 

test case 1: 
    check if malloc() works at malloc and free 120 times, repeat for 50 times.  
test case 2: 
    check if malloc() works at malloc 120 chunks, and then free 120 chunks, repeat for 50 times. 
test case 3: 
    check if malloc() works in random sequence of malloc and free. if there is no malloc item. this test will not 
    call the free() function. 


test case 4: own 
    1. check malloc() can detect "require too much space than heap". it will keep malloc 100 bytes of object, untile 
        the heap have no more free space. 
    2. after a "require too much space ERROR" printed, it will call free(). 
    3. it will use rand() to generate random number x between 0 to 10 int size, (0 to 40 bytes), add x to some pointers malloc returned. 
        since ever malloc is 100 bytes. This will make the pointer to move to the middle of an object, or not move if x == 0. 
        
        a. if x is 0, it will cause a double free, memgrind will expect ERROR DOUBLE FREE. 
        b. if x is not 0, it will move a pointer to middle of an chunk, which is an invalid pointer, memgrin will expect ERROR INVALID pointer. 


test case 5: own
    1. malloc 30 objects have different size, between 0 int size to 50 int size, (0 to 200 byte size). 
    2. this action could cause malloc() report ERROR DO NOT HAVE ENOUGH SPACE. memgrind will check if there should be an such report. 
        by sum up all the rand() number, and +2(for metadata each chunks), check if they are too big. 
    3. If malloc 0 bytes, malloc() will return NULL pointer. and this should cause free() to report ERROR  pointer is outside of the heap. 
    4. this test may output no error, if x = rand() does not gives 0. 
    5. free is in different order. this test will free from the heap end to the heap start. 
