#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "CMU418intrin.h"
#include "logger.h"
using namespace std;


void absSerial(float* values, float* output, int N) {
    for (int i=0; i<N; i++) {
		float x = values[i];
		if (x < 0) {
			output[i] = -x;
		} else {
			output[i] = x;
		}
    }
}

// implementation of absolute value using 15418 instrinsics
void absVector(float* values, float* output, int N) {
    __cmu418_vec_float x;
    __cmu418_vec_float result;
    __cmu418_vec_float zero = _cmu418_vset_float(0.f);
    __cmu418_mask maskAll, maskIsNegative, maskIsNotNegative;

    //  Note: Take a careful look at this loop indexing.  This example
    //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
    //  Why is that the case?
    for (int i=0; i<N; i+=VECTOR_WIDTH) {

		// All ones
		maskAll = _cmu418_init_ones();

		// All zeros
		maskIsNegative = _cmu418_init_ones(0);

		// Load vector of values from contiguous memory addresses
		_cmu418_vload_float(x, values+i, maskAll);               // x = values[i];

		// Set mask according to predicate
		_cmu418_vlt_float(maskIsNegative, x, zero, maskAll);     // if (x < 0) {

		// Execute instruction using mask ("if" clause)
		_cmu418_vsub_float(result, zero, x, maskIsNegative);      //   output[i] = -x;

		// Inverse maskIsNegative to generate "else" mask
		maskIsNotNegative = _cmu418_mask_not(maskIsNegative);     // } else {

		// Execute instruction ("else" clause)
		_cmu418_vload_float(result, values+i, maskIsNotNegative); //   output[i] = x; }

		// Write results back to memory
		_cmu418_vstore_float(output+i, result, maskAll);
    }
}

// Accepts an array of values and an array of exponents
// For each element, compute values[i]^exponents[i] and clamp value to
// 4.18.  Store result in outputs.
// Uses iterative squaring, so that total iterations is proportional
// to the log_2 of the exponent
void clampedExpSerial(float* values, int* exponents, float* output, int N) {
    for (int i=0; i<N; i++) {
		float x = values[i];
		float result = 1.f;
		int y = exponents[i];
		float xpower = x;
		while (y > 0) {
			if (y & 0x1) {
				result *= xpower;
			}
			xpower = xpower * xpower;
			y >>= 1;
		}
		if (result > 4.18f) {
			result = 4.18f;
		}
		output[i] = result;
    }
}

void clampedExpVector(float* values, int* exponents, float* output, int N) {
    __cmu418_vec_float x;
	__cmu418_vec_float res;
	__cmu418_vec_float xpower;
	__cmu418_vec_float tempF;

	__cmu418_mask maskAll;
	__cmu418_mask maskExp;
	__cmu418_mask m_temp;
	__cmu418_mask m_gtZ;

	__cmu418_vec_int y;
	
	__cmu418_vec_int temp;
	__cmu418_vec_int ones = _cmu418_vset_int(1);
	__cmu418_vec_int zeroes = _cmu418_vset_int(0);
	
	// 16 0-7 8-15
	// 20 0-7 8-15 16-24 20-16+1
	// 3 0+8 >3  3-0+1
	for(int i=0; i<N; i+=VECTOR_WIDTH){
		int a = VECTOR_WIDTH;
		if(i+VECTOR_WIDTH > N) a = N-i;
		maskAll = _cmu418_init_ones(a);
		maskExp = _cmu418_init_ones(a);
		_cmu418_vload_float(x, values+i, maskAll);
		_cmu418_vload_int(y, exponents+i, maskAll);
		_cmu418_vload_float(xpower, values+i, maskAll);
		res = _cmu418_vset_float(1.0f);

		// [1,1,1,1,1,1,1,1]
		// [1,1,1,1,1,1,1,1]
		// [966, 115, 255, 236, 205, 939, 763, 326]
		// [0, 1, 1, 0, 1, 1, 1, 0]
		// [483, 57, 127, 118, 102, 469, 381, 163]
		// [241, 28, 63, 59, 51, 234, 190, 81]
		// [120, 14, 31, 29, 25, 117, 95, 40]
		// [60, 7, 15, 14, 12, 58, 47, 20]
		// [30, 3, 7, 7, 6, 29, 23, 10]
		// [15, 1, 3, 3, 3, 14, 11, 5]
		// 
		
		while(_cmu418_cntbits(maskExp) > 0){
			_cmu418_vbitand_int(temp, y, ones, maskExp);
			_cmu418_veq_int(m_gtZ, temp, ones, maskExp);
			// for(int i = 0; i < VECTOR_WIDTH; i++){
			// 	std::cout << m_gtZ.value[i] << " ";
			// }
			m_gtZ = _cmu418_mask_and(m_gtZ, maskExp);
			_cmu418_vmult_float(res, res, xpower, m_gtZ);
			_cmu418_vmult_float(xpower, xpower, xpower, maskExp);
			_cmu418_vshiftright_int(y, y, ones, maskExp);
			_cmu418_vgt_int(maskExp, y, zeroes, maskExp);
			// for(int i = 0; i < VECTOR_WIDTH; i++){
			// 	std::cout << y.value[i] << " ";
			// }
			// for(int i = 0; i < VECTOR_WIDTH; i++){
			// 	std::cout << maskExp.value[i] << " ";
			// }
			// std::cout << std::endl;
			
		}
		//std::cout << endl;
		// for(int i = 0; i < VECTOR_WIDTH; i++){
		// 	std::cout << res.value[i] << " ";
		// }
		_cmu418_vset_float(tempF, 4.18, maskAll);
		_cmu418_vgt_float(m_temp, res, tempF, maskAll);
		_cmu418_vset_float(res, 4.18, m_temp);

		_cmu418_vstore_float(output+i, res, maskAll);
	}
}


float arraySumSerial(float* values, int N) {
    float sum = 0;
    for (int i=0; i<N; i++) {
		sum += values[i];
    }

    return sum;
}

// Assume N % VECTOR_WIDTH == 0
// Assume VECTOR_WIDTH is a power of 2
// [1,2,3,4,5,6,7,8] 8
// [3, 3, 7, 7, 11, 11, 15, 15] 
// [3, 3, 7, 7, 11, 11, 15, 15]
// [3, 7, 11, 15, 3, 7, 11, 15] 4
// [10, 10, 26, 26, 10, 10, 26, 26]
// [10, 26, 10, 26, 10, 26, 10, 26] 2
// [36] 1

float arraySumVector(float* values, int N) {
    // Implement your vectorized version here
    //  ...
	__cmu418_vec_float x;
	__cmu418_vec_float res = _cmu418_vset_float(0.0);
	__cmu418_mask maskAll = _cmu418_init_ones();
	for(int i=0; i<N; i+=VECTOR_WIDTH){
		
		_cmu418_vload_float(x, values+i, maskAll);
		_cmu418_vadd_float(res, res, x, maskAll);
		// for(int i = 0; i < VECTOR_WIDTH; i++){
		// 	std::cout << x.value[i] << " ";
		// }
		// std::cout << std::endl;
	}
	int k = VECTOR_WIDTH;
	while(k != 1){
		// for(int i = 0; i < VECTOR_WIDTH; i++){
		// 	std::cout << res.value[i] << " ";
		// }
		// std::cout << std::endl;
		_cmu418_hadd_float(res, res);
		_cmu418_interleave_float(res, res);
		k >>= 1;
	}
	return res.value[0];
}
