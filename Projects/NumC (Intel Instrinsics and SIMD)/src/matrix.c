#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col) {
    int colsize = mat->cols;
    int offset = row * colsize + col;
    return mat->data[offset];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
    int colsize = mat->cols;
    int offset = row * colsize + col;
    mat->data[offset] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    // Task 1.2 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Allocate space for the matrix data, initializing all entries to be 0. Return -2 if allocating memory failed.
    // 4. Set the number of rows and columns in the matrix struct according to the arguments provided.
    // 5. Set the `parent` field to NULL, since this matrix was not created from a slice.
    // 6. Set the `ref_cnt` field to 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0){
        return -1;
    }
    *mat = (matrix *) malloc(sizeof(struct matrix));
    if (*mat == NULL){
        return -2;
    }
    (*mat)->data = (double *) calloc(rows * cols, sizeof(double));
    if ((*mat)->data == NULL){
        free(*mat);
        return -2;
    }
    (*mat)->cols = cols;
    (*mat)->rows = rows;
    (*mat)->parent = NULL;
    (*mat)->ref_cnt = 1;
    return 0;


}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
    if (mat == NULL){
        return;
    }
    if (mat->parent == NULL) {
        mat->ref_cnt -= 1;
        if (mat->ref_cnt == 0){
            free(mat->data);
            free(mat);
        } 
    } else {
        deallocate_matrix(mat->parent);
        free(mat);
    }
    return;
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0){
        return -1;
    }
    *mat = (matrix *) malloc(sizeof(struct matrix));
    if (*mat == NULL){
        return -2;
    }

    (*mat)->cols = cols;
    (*mat)->rows = rows;
    (*mat)->data = from->data + offset;
    (*mat)->parent = from;
    from->ref_cnt += 1;
    (*mat)->ref_cnt = 1;
    return 0;

}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
    int size = mat->rows * mat->cols;
    #pragma omp for
    for (int i = 0; i < size; i++){
        mat->data[i] = val;
    }
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
    int size = mat->rows * mat->cols;
    __m256d neg = _mm256_set1_pd(-1.0);
    if (mat->rows >= 150 || mat->cols >= 150) {
        #pragma omp parallel 
        {
            #pragma omp for 
            for (int i =0; i < size / 16 * 16; i += 16){
                __m256d mat1data;
                __m256d mat2data;

                mat1data = _mm256_loadu_pd(mat->data + i + 0);
                mat2data = _mm256_loadu_pd(mat->data + i + 0);
                mat2data = _mm256_mul_pd(mat2data, neg);
                __m256d abs1 = _mm256_max_pd(mat1data, mat2data);

                mat1data = _mm256_loadu_pd(mat->data + i + 4);
                mat2data = _mm256_loadu_pd(mat->data + i + 4);
                mat2data = _mm256_mul_pd(mat2data, neg);
                __m256d abs2 = _mm256_max_pd(mat1data, mat2data);

                mat1data = _mm256_loadu_pd(mat->data + i + 8);
                mat2data = _mm256_loadu_pd(mat->data + i + 8);
                mat2data = _mm256_mul_pd(mat2data, neg);
                __m256d abs3 = _mm256_max_pd(mat1data, mat2data);

                mat1data = _mm256_loadu_pd(mat->data + i + 12);
                mat2data = _mm256_loadu_pd(mat->data + i + 12);
                mat2data = _mm256_mul_pd(mat2data, neg);
                __m256d abs4 = _mm256_max_pd(mat1data, mat2data);

                _mm256_storeu_pd(result->data + i + 0, abs1);
                _mm256_storeu_pd(result->data + i + 4, abs2);
                _mm256_storeu_pd(result->data + i + 8, abs3);
                _mm256_storeu_pd(result->data + i + 12, abs4);
            }
        }
        for (int i = size / 16 * 16; i < size / 4 * 4; i += 4) {
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs1 = _mm256_max_pd(mat1data, mat2data);
            _mm256_storeu_pd(result->data + i + 0, abs1);
        }
        for (int i = size / 4 * 4; i < size; i++) {
            double data = mat->data[i];
            result->data[i] = data >= 0 ? data : data * -1.0;
        }
    } else if (mat->rows >= 6 || mat->cols >= 6) {
        for (int i = 0; i < size / 16 * 16; i += 16){
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs1 = _mm256_max_pd(mat1data, mat2data);

            mat1data = _mm256_loadu_pd(mat->data + i + 4);
            mat2data = _mm256_loadu_pd(mat->data + i + 4);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs2 = _mm256_max_pd(mat1data, mat2data);

            mat1data = _mm256_loadu_pd(mat->data + i + 8);
            mat2data = _mm256_loadu_pd(mat->data + i + 8);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs3 = _mm256_max_pd(mat1data, mat2data);

            mat1data = _mm256_loadu_pd(mat->data + i + 12);
            mat2data = _mm256_loadu_pd(mat->data + i + 12);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs4 = _mm256_max_pd(mat1data, mat2data);

            _mm256_storeu_pd(result->data + i + 0, abs1);
            _mm256_storeu_pd(result->data + i + 4, abs2);
            _mm256_storeu_pd(result->data + i + 8, abs3);
            _mm256_storeu_pd(result->data + i + 12, abs4);
        }
        for (int i = size / 16 * 16; i < size / 4 * 4; i += 4) {
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs1 = _mm256_max_pd(mat1data, mat2data);
            _mm256_storeu_pd(result->data + i + 0, abs1);
        }
        for (int i = size / 4 * 4; i < size; i++) {
            double data = mat->data[i];
            result->data[i] = data >= 0 ? data : data * -1.0;
        }
    } else {
        for (int i = 0; i < size / 4 * 4; i += 4) {
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_loadu_pd(mat->data + i + 0);
            mat2data = _mm256_mul_pd(mat2data, neg);
            __m256d abs1 = _mm256_max_pd(mat1data, mat2data);
            _mm256_storeu_pd(result->data + i + 0, abs1);
        }
        for (int i = size / 4 * 4; i < size; i++) {
            double data = mat->data[i];
            result->data[i] = data >= 0 ? data : data * -1.0;
        }
    }   
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    int size = mat1->rows * mat1->cols;
    // for (int i = 0; i < size; i++){
    //     result->data[i] = mat1->data[i] + mat2->data[i];
    // }
    if (mat1->rows >= 250 || mat1->cols >=250) {
        #pragma omp parallel 
        {
            #pragma omp for 
            for (int i =0; i < size / 16 * 16; i += 16){
                __m256d mat1data;
                __m256d mat2data;

                mat1data = _mm256_loadu_pd(mat1->data + i);
                mat2data = _mm256_loadu_pd(mat2->data + i);
                __m256d add1 = _mm256_add_pd(mat1data, mat2data);

                mat1data = _mm256_loadu_pd(mat1->data + i + 4);
                mat2data = _mm256_loadu_pd(mat2->data + i + 4);
                __m256d add2 = _mm256_add_pd(mat1data, mat2data);

                mat1data = _mm256_loadu_pd(mat1->data + i + 8);
                mat2data = _mm256_loadu_pd(mat2->data + i + 8);
                __m256d add3 = _mm256_add_pd(mat1data, mat2data);

                mat1data = _mm256_loadu_pd(mat1->data + i + 12);
                mat2data = _mm256_loadu_pd(mat2->data + i + 12);
                __m256d add4 = _mm256_add_pd(mat1data, mat2data);

                _mm256_storeu_pd(result->data + i + 0, add1);
                _mm256_storeu_pd(result->data + i + 4, add2);
                _mm256_storeu_pd(result->data + i + 8, add3);
                _mm256_storeu_pd(result->data + i + 12, add4);
            }
        }
        for (int i = size / 16 * 16; i < size / 4 * 4; i += 4) {
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat1->data + i);
            mat2data = _mm256_loadu_pd(mat2->data + i);
            __m256d add1 = _mm256_add_pd(mat1data, mat2data);
            _mm256_storeu_pd(result->data + i, add1);
        }
        for (int i = size / 4 * 4; i < size; i++) {
            result->data[i] = mat1->data[i] + mat2->data[i];
        }
    } else {
        for (int i =0; i < size / 16 * 16; i += 16){
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat1->data + i);
            mat2data = _mm256_loadu_pd(mat2->data + i);
            __m256d add1 = _mm256_add_pd(mat1data, mat2data);

             mat1data = _mm256_loadu_pd(mat1->data + i + 4);
             mat2data = _mm256_loadu_pd(mat2->data + i + 4);
             __m256d add2 = _mm256_add_pd(mat1data, mat2data);

            mat1data = _mm256_loadu_pd(mat1->data + i + 8);
            mat2data = _mm256_loadu_pd(mat2->data + i + 8);
            __m256d add3 = _mm256_add_pd(mat1data, mat2data);

            mat1data = _mm256_loadu_pd(mat1->data + i + 12);
            mat2data = _mm256_loadu_pd(mat2->data + i + 12);
            __m256d add4 = _mm256_add_pd(mat1data, mat2data);

            _mm256_storeu_pd(result->data + i + 0, add1);
            _mm256_storeu_pd(result->data + i + 4, add2);
            _mm256_storeu_pd(result->data + i + 8, add3);
            _mm256_storeu_pd(result->data + i + 12, add4);
        }
        for (int i = size / 16 * 16; i < size / 4 * 4; i += 4) {
            __m256d mat1data;
            __m256d mat2data;

            mat1data = _mm256_loadu_pd(mat1->data + i);
            mat2data = _mm256_loadu_pd(mat2->data + i);
            __m256d add1 = _mm256_add_pd(mat1data, mat2data);
            _mm256_storeu_pd(result->data + i, add1);
        }
        for (int i = size / 4 * 4; i < size; i++) {
            result->data[i] = mat1->data[i] + mat2->data[i];
        }
    }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    return 0;
}


/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */

int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    int i,j,k;
    fill_matrix(result, 0.0);
    if (mat1->cols >50 || mat1->rows > 50 || mat2->cols >50 || mat2->rows >50){
        #pragma omp parallel
        {
            #pragma omp for
            for(j = 0; j < result->cols/4*4; j+= 4 ) {

                __m256d m11;
                __m256d m12;
                __m256d m13;
                __m256d m14;

                __m256d m1;

                __m256d r1;
                __m256d r2;
                __m256d r3;
                __m256d r4;
                        
                __m256d c1;
                __m256d c2;
                __m256d c3;
                __m256d c4;

                for( i = 0; i < result->rows/4*4; i+= 4) {
                    __m256d a1 = _mm256_setzero_pd();
                    __m256d a2 = _mm256_setzero_pd();
                    __m256d a3 = _mm256_setzero_pd();
                    __m256d a4 = _mm256_setzero_pd();
                    for(k = 0; k < mat1->cols; k++){ 
                        m11 = _mm256_set1_pd (mat1->data[(i+0) * mat1->cols + k]);
                        m12 = _mm256_set1_pd (mat1->data[(i+1) * mat1->cols + k]);
                        m13 = _mm256_set1_pd (mat1->data[(i+2) * mat1->cols + k]);
                        m14 = _mm256_set1_pd (mat1->data[(i+3) * mat1->cols + k]);

                        m1 = _mm256_loadu_pd (mat2->data + (k * mat2->cols + j));

                        r1 = _mm256_mul_pd(m11, m1);
                        r2 = _mm256_mul_pd(m12, m1);
                        r3 = _mm256_mul_pd(m13, m1);
                        r4 = _mm256_mul_pd(m14, m1);
                        

                        a1 = _mm256_add_pd(a1, r1);
                        a2 = _mm256_add_pd(a2, r2);
                        a3 = _mm256_add_pd(a3, r3);
                        a4 = _mm256_add_pd(a4, r4);

                    }
                    c1 = _mm256_loadu_pd(result->data + ((i+0) * result->cols + j));
                    c2 = _mm256_loadu_pd(result->data + ((i+1) * result->cols + j));
                    c3 = _mm256_loadu_pd(result->data + ((i+2) * result->cols + j));
                    c4 = _mm256_loadu_pd(result->data + ((i+3) * result->cols + j));

                    a1 = _mm256_add_pd(a1, c1);
                    a2 = _mm256_add_pd(a2, c2);
                    a3 = _mm256_add_pd(a3, c3);
                    a4 = _mm256_add_pd(a4, c4);

                    _mm256_storeu_pd(result->data + ((i+0) * result->cols + j), a1);
                    _mm256_storeu_pd(result->data + ((i+1) * result->cols + j), a2);
                    _mm256_storeu_pd(result->data + ((i+2) * result->cols + j), a3);
                    _mm256_storeu_pd(result->data + ((i+3) * result->cols + j), a4);
                }
            }
        }
        for(j = 0; j < result->cols/4*4; j++ ) {
            for( k = 0; k < mat1->cols; k++ ) {
                for( i = result->rows/4*4; i < result->rows; i++){ 
                    result->data[i * result->cols + j] += mat1->data[i * mat1->cols + k] * mat2->data[k * mat2->cols + j];
                }
            }
        }
        #pragma omp parallel 
        {
            #pragma omp for 
            for(j = result->cols/4*4; j < result->cols; j++ ) {
                for( k = 0; k < mat1->cols; k++ ) {
                    for( i = 0; i < result->rows; i++ ){ 
                        result->data[i * result->cols + j] += mat1->data[i * mat1->cols + k] * mat2->data[k * mat2->cols + j];
                    }
                }
            }
        }
    } else if (mat1->cols >5 || mat1->rows > 5 || mat2->cols >5 || mat2->rows >5) {
        for( j = 0; j < result->cols/4*4; j+= 4 ) {
            __m256d m11;
            __m256d m2;
            __m256d r1;
            __m256d c1;
            for(i = 0; i < result->rows; i ++) {
                __m256d a1 = _mm256_setzero_pd();
                for(k = 0; k < mat1->cols; k++ ){ 
                    m11 = _mm256_set1_pd (mat1->data[(i+0) * mat1->cols + k]);
                    m2 = _mm256_loadu_pd (mat2->data + (k * mat2->cols + j));
                    r1 = _mm256_mul_pd(m11, m2);
                    a1 = _mm256_add_pd(a1, r1);   
                }
                c1 = _mm256_loadu_pd(result->data + ((i+0) * result->cols + j));
                a1 = _mm256_add_pd(a1, c1);
                _mm256_storeu_pd(result->data + ((i+0) * result->cols + j), a1);
            }
        }
        for(j = result->cols/4*4; j < result->cols; j++ ) {
            for( k = 0; k < mat1->cols; k++ ) {
                for(i = 0; i < result->rows; i++ ){ 
                    result->data[i * result->cols + j] += mat1->data[i * mat1->cols + k] * mat2->data[k * mat2->cols + j];
                }
            }
        }
    } else {
        for( j = 0; j < result->cols; j++ ) {
            for( k = 0; k < mat1->cols; k++ ) {
                for( i = 0; i < result->rows; i++ ){ 
                    result->data[i * result->cols + j] += mat1->data[i * mat1->cols + k] * mat2->data[k * mat2->cols + j];
                }
            }
        }
    }
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    if (pow == 0){
        identity(result);
    } else if (pow == 1){
        copy(result, mat);
    } else {
        matrix *res2;
        matrix *y; 
        matrix *x; 
        allocate_matrix(&y, result->rows, result->cols);
        allocate_matrix(&x, result->rows, result->cols);
        allocate_matrix(&res2, result->rows, result->cols);
        copy(x, mat);
        identity(y);
        while (pow > 1){
            if (pow % 2 == 0){
                mul_matrix(result, x, x);
                pow = pow / 2;
                copy(x, result);
            } else {
                mul_matrix(res2, y, x);
                mul_matrix(result, x, x);
                copy(y, res2);
                copy(x, result);
                pow = (pow-1) / 2;
            }
        }
        mul_matrix(result, x, y);
        deallocate_matrix(y);
        deallocate_matrix(x);
        deallocate_matrix(res2);
    }
    // } else {
    //     mul_matrix(result, mat, mat);
    //     pow--;
    // }
    // if (pow > 1){
    //     matrix *data = (matrix *) malloc(sizeof(matrix));
    //     allocate_matrix(&data, mat->rows, mat->cols);
    //     int size = mat->rows * mat->cols;
    //     while (pow > 1){
    //         for (int i = 0; i < size; i++){
    //             data->data[i] = result->data[i];
    //         }
    //         mul_matrix(result, data, mat);
    //         pow--;
    //     }
    //     deallocate_matrix(data);
    // }
    return 0;
}

void identity(matrix *mat) {
    int size = mat->rows * mat->cols;
    #pragma omp for
    for (int i = 0; i < size; i ++) {
        if (i / mat->cols == i % mat->cols) {
            mat->data[i] = 1.0;
        } else {
            mat->data[i] = 0.0;
        }
    }
}

void copy(matrix *result, matrix *mat) {
    int size = result->rows * result->cols;
    #pragma omp for
    for (int i = 0; i < size; i++) {
        result->data[i] = mat->data[i];
    }
}