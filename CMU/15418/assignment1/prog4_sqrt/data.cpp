#include <algorithm>

// Generate random data
void initRandom(float *values, int N) {
    for (int i=0; i<N; i++)
    {
        // random input values
        values[i] = .001f + 2.998f * static_cast<float>(rand()) / RAND_MAX;
    }
}

// Generate data that gives high relative speedup
void initGood(float *values, int N) {
    for (int i=0; i<N; i++)
    {
        // Todo: Choose values
        //for the value 3, we have highest Arithmetic intensitiy, 
        //since the number of iterations to converge is max
        values[i] = 3.0f - 0.00001f;
    }
}

// Generate data that gives low relative speedup
void initBad(float *values, int N) {
    // We want Arithmetic Intensity to be least, so least iterations with 1. Increases bandwith latency
    // Along with increasing the number of iteration, now we introduce a 3 in the vector length so that the SIMD exec is bottled.
    int vector_len = 8;
    for (int i=0; i<N; i++)
    {
        // Todo: Choose values
        values[i] = 1.0f+0.00001f;
    }
    for(int i=0; i<N; i+=vector_len) values[i] = 3.0f - 0.00001f;
}

