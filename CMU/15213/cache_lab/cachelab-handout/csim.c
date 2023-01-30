#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct line{
    short int valid;
    int tag;
    int block;
}CACHE_LINE;


typedef struct sll_node{
    int idx;
    struct sll_node *next;
}NODE;

typedef struct sll_queue{
    NODE* head;
    int size;
}QUEUE;

typedef struct cache{
    CACHE_LINE **cache_sets;
    QUEUE** cache_queues;
}CACHE;


QUEUE* initq(){
    QUEUE* q = (QUEUE*)malloc(sizeof(QUEUE));
    q->head = NULL;
    q->size = 0;
    return q;
}



CACHE* init_cache(int S, int E, int B){
    CACHE* new_cache = (CACHE*)malloc(sizeof(CACHE));
    new_cache->cache_queues = (QUEUE**)malloc(sizeof(QUEUE)*S);

    new_cache->cache_sets = (CACHE_LINE**)malloc(sizeof(CACHE_LINE)*S);
    for(int i=0; i<S; i++){
        new_cache->cache_sets[i] = (CACHE_LINE*)malloc(sizeof(CACHE_LINE)*E);
        new_cache->cache_queues[i] = initq();
        for(int j=0; j<E; j++){
            new_cache->cache_sets[i][j].valid = 0; //dereferencing using [], so dot operator
            new_cache->cache_sets[i][j].tag = 0;
            new_cache->cache_sets[i][j].block = B;
        }
    }
    return new_cache;
}



int enqueue(QUEUE* q, int ele){
    NODE* new_node = (NODE*)malloc(sizeof(NODE));
    new_node->idx = ele;
    new_node->next = NULL;
    if(!q->size){
        q->head = new_node;
        q->size++;
        return 1;
    }
    NODE* trav = q->head;
    while(trav->next){
        trav = trav->next;
    }
    trav->next = new_node;
    q->size++;
    return 1;
}

int dequeue(QUEUE* q,int E){
    if(q->size < E){
        return q->size;
    }
    NODE* temp = q->head;
    int ele;
    q->head = q->head->next;
    ele = temp->idx;
    free(temp);
    q->size--;
    return ele;
}

void print_queue(QUEUE* q){
    printf("%d\n", q->size);
    NODE* trav = q->head;
    while(trav){
        printf("%d ", trav->idx);
        trav = trav->next;
    }
    printf("\n");
    
}


int remove_item(QUEUE* q, int ele){
    NODE* trav = q->head;
    NODE* prev = NULL;
    if(!trav){
        return 0;
    }
    while(trav && trav->idx != ele){
        prev = trav;
        trav = trav->next;
    }
    if(trav){
        if(prev){
            prev->next = trav->next;
        }
        else{
            q->head = trav->next;
        }
        free(trav);
    }
    else{
        return 0;
    }
    q->size--;
    return 1;
}

//start is 1 indexed, as in its the start'th bit to start at
long long int read_bits(long long int num, long int bits, long int start){
    long long int res = 0;
    num = num >> (start-1);
    for(int i=0; i<bits; i++){
        res = res | ((num&1) << i);
        num >>= 1;
    }
    return res;
}

int cache_read(CACHE* cache, long long int addr, int size, int mode, int s, int b, int t, int E, int v,int *hits, int *misses, int *evictions){
    int hit, miss;
    int eviction = 0;
    hit = miss = 0;
    //printf("%d\n", addr);
    //int offset = read_bits(addr, b, 1);
    int set_idx = read_bits(addr, s, b+1);
    int tag = read_bits(addr, t, s+b+1);
    // printf("%x %x %x\n", tag, set_idx, offset);
    QUEUE* q = cache->cache_queues[set_idx];
    for(int i=0; i<E; i++){
        if(cache->cache_sets[set_idx][i].tag == tag && cache->cache_sets[set_idx][i].valid){
            hit = 1;
            //printf("hit\n");
            // print_queue(q);
            // printf("idx: %d \n", i);
            remove_item(q, i);
            enqueue(q, i);
            // print_queue(q);
            (*hits)++;
            break;
        }        
    }
    if(!hit) miss = 1;
    
    if(miss){
        //fill LRU functionality
        //printf("miss\n");
        //print_queue(q);
        int idx  = dequeue(q, E);
        //printf("idx: %d \n", idx);
        if(cache->cache_sets[set_idx][idx].valid) eviction = 1;
        cache->cache_sets[set_idx][idx].tag = tag;
        cache->cache_sets[set_idx][idx].valid = 1;
        remove_item(q, idx);
        enqueue(q, idx);
        //print_queue(q); 
        (*misses)++;
    }
    if(eviction) {
        // printf("eviction\n");
        (*evictions)++;
    }

    if(mode==4){
        
        (*hits)++;        
    }

    if(v){
        switch(mode){
            case 1:
                printf("L ");
                break;
            case 2:
                printf("S ");
                break;
            case 4:
                printf("M ");
                break;
        }
        printf("%llx,%d ", addr, size);
        if(hit) printf("hit ");
        else if(miss) printf("miss ");
        if(eviction) printf("eviction ");
        if(mode==4) printf("hit ");
        printf("\n");
    } 


    return 0;
}


int power_of_two(int x){
    int res = 1;
    res <<= x;
    return res;
}

int main(int argc, char* argv[])
{

    const int m = 64;

    int s, E, b;
    int S, B;
    char *traceFile = (char*)malloc(sizeof(char)*100);
    int v=0, h=0;
    int t;

    int hits =0;
    int misses =0;
    int evictions =0;

    int req = 4;
    int count = 0;

    for(int i=1; i<argc; i++){
        if(!strcmp(argv[i], "-h")) h = 1;
        else if(!strcmp(argv[i], "-v")) v=1;
        else if(!strcmp(argv[i], "-hv")){
            h=1;
            v=1;
        } 
        else{
            if(!strcmp(argv[i], "-s")){
                i++;
                s = atoi(argv[i]);
                count++;
            }
            else if(!strcmp(argv[i], "-E")){
                i++;
                E = atoi(argv[i]);
                count++;
            }
            else if(!strcmp(argv[i], "-b")){
                i++;
                b = atoi(argv[i]);
                count++;
            }
            else if(!strcmp(argv[i], "-t")){
                i++;
                strcpy(traceFile, argv[i]);
                count++;
            }
        }
    }
    if(req > count){
        printf("Not enough CLI Arguments, Enter -h for help.\n");
        return 1;
    }
    t = m-(s+b);
    
    S = power_of_two(s);
    B = power_of_two(b);

    //printf("%d %d\n", S, B);
    
    char helpStr[1000] = "Options:\n\t-h         Print this help message.\n\t-v         Optional verbose flag.\n\t-s <num>   Number of set index bits.\n\t-E <num>   Number of lines per set.\n\t-b <num>   Number of block offset bits.\n\t-t <file>  Trace file.\n\nExamples:\n\tlinux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n\tlinux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n";
    if(h){
        printf("%s", helpStr);
        return 0;
    }
    CACHE* cache = init_cache(S, B, E);
    //printf("%d %d %d\n", cache->cache_sets[0][1].valid, cache->cache_sets[0][1].tag, cache->cache_sets[0][1].block);
    //Deal with tracefile
    FILE* val_f = fopen(traceFile, "r");
    char curr_instr[1000];

    //printf("%d\n", read_bits(37, 7, 2));
    //cache_read(3411, 64, 2, 3, 2, 61);
    while(fgets(curr_instr, 1000, val_f)){
        
        //printf("%s", curr_instr);
        if(curr_instr[0] == 'I') continue;
        long long int addr;
        int size;
        int instr_type = 0; // 1 2 4 -> L S M
        char* trav = strtok(curr_instr, " ");
        
        switch(trav[0]){
            case 'L':
                instr_type=1;
                break;
            case 'S':
                instr_type=2;
                break;
            case 'M':
                instr_type=4;
                break;
        }
        
        addr = strtoll(strtok(NULL, ","), NULL, 16);
        size = atoi(strtok(NULL, "\n"));
        //printf("%llx %d\n", addr, size);
        cache_read(cache, addr, size, instr_type, s, b, t, E, v, &hits, &misses, &evictions);

    }
    //printf("%d %d %d\n", hits, misses, evictions);
    printSummary(hits, misses, evictions);
    return 0;
}
