#include <stdio.h> 
#include <unistd.h> 
 
int main() { 
    int incoming, outgoing, bucket_size, n, store = 0; 
    printf("Enter bucket size, outgoing rate and no. of inputs: "); 
    scanf("%d %d %d", &bucket_size, &outgoing, &n); 
 
    while (n > 0) { 
        printf("\nEnter the incoming packet size: "); 
        scanf("%d", &incoming); 
        if (incoming <= (bucket_size - store)) { 
            store += incoming; 
            printf("Bucket buffer size %d out of %d\n", store, bucket_size); 
        } else { 
            printf("Dropped %d packets (Bucket Overflow)\n", incoming - (bucket_size - store)); 
            store = bucket_size; 
        } 
        // Leaking process 
        if (store >= outgoing) { 
            store -= outgoing; 
            printf("After outgoing, %d packets left in bucket\n", store); 
        } else { 
            printf("%d packets exited, bucket is now empty\n", store); 
            store = 0; 
        } 
        n--; 
        sleep(1); 
    } 
    return 0; 
}