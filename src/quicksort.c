#include "quicksort.h"

extern inline void swap_mem_uint8_t(uint8_t* a, uint8_t* b, int size_line_sub);
extern inline void swap_mem_int(int* a, int* b);
extern inline void swap_mem_uint64(uint64_t* a, uint64_t* b);

uint8_t* median3(uint8_t *a, int size_line, int left, int right, int* tab_ind)
{
    int center = (left + right)/2;

    int current_left = left*size_line;
    int current_right = right*size_line;
    int current_center = center*size_line;

    if (memcmp(&(a[current_left]), &(a[current_center]), size_line) > 0){
        swap_mem_uint8_t(&(a[current_left]),&(a[current_center]), size_line);
        swap_mem_int(&(tab_ind[left]),&(tab_ind[center]));
    }
    if (memcmp(&(a[current_left]), &(a[current_right]), size_line) > 0){
        swap_mem_uint8_t(&(a[current_left]),&(a[current_right]), size_line);
        swap_mem_int(&(tab_ind[left]),&(tab_ind[right]));
    }
    if (memcmp(&(a[current_center]), &(a[current_right]), size_line) > 0){
        swap_mem_uint8_t(&(a[current_center]),&(a[current_right]), size_line);
        swap_mem_int(&(tab_ind[center]),&(tab_ind[right]));
    }

    swap_mem_uint8_t(&(a[current_center]), &(a[(right-1)*size_line]), size_line);
    swap_mem_int(&(tab_ind[center]),&(tab_ind[right-1]));

    return &(a[(right-1)*size_line]);
}

uint64_t median3_uint64(uint64_t *a, uint64_t *b, uint64_t left, uint64_t right)
{
    uint64_t center = (left + right)/2;

    if (a[left] > a[center]){
        swap_mem_uint64(&(a[left]), &(a[center]));
        swap_mem_uint64(&(b[left]), &(b[center]));
    }
    if (a[left] > a[right]){
        swap_mem_uint64(&(a[left]), &(a[right]));
        swap_mem_uint64(&(b[left]) ,&(b[right]));
    }
    if (a[center] >  a[right]){
        swap_mem_uint64(&(a[center]), &(a[right]));
        swap_mem_uint64(&(b[center]), &(b[right]));
    }

    swap_mem_uint64(&(a[center]), &(a[right-1]));
    swap_mem_uint64(&(b[center]), &(b[right-1]));

    return a[right-1];
}

void manualSort(uint8_t *a, int size_line, int left, int right, int* tab_ind)
{
    int size = right-left+1;
    int current_left = left*size_line;
    int current_right = right*size_line;

    if (size <= 1) return;
    if (size == 2) { // 2-sort left and right
        if (memcmp(&(a[current_left]), &(a[current_right]), size_line) > 0){
            swap_mem_uint8_t(&(a[current_left]),&(a[current_right]), size_line);
            swap_mem_int(&(tab_ind[left]),&(tab_ind[right]));
            return;
        }
    }
    else {// size is 3, 3-sort left, center, & right
        if (memcmp(&(a[current_left]), &(a[current_right-size_line]), size_line) > 0){
            swap_mem_uint8_t(&(a[current_left]),&(a[current_right-size_line]), size_line);
            swap_mem_int(&(tab_ind[left]),&(tab_ind[right-1]));
        }
        if (memcmp(&(a[current_left]), &(a[current_right]), size_line) > 0){
            swap_mem_uint8_t(&(a[current_left]),&(a[current_right]), size_line);
            swap_mem_int(&(tab_ind[left]),&(tab_ind[right]));
        }
        if (memcmp(&(a[current_right-size_line]), &(a[current_right]), size_line) > 0){
            swap_mem_uint8_t(&(a[current_right-size_line]),&(a[current_right]), size_line);
            swap_mem_int(&(tab_ind[right-1]),&(tab_ind[right]));
        }

        return;
    }
}

void manualSort_uint64(uint64_t *a, uint64_t *b, uint64_t left, uint64_t right)
{
    uint64_t size = right-left+1;

    if (size <= 1) return;
    if (size == 2) { // 2-sort left and right
        if (a[left] > a[right]){
            swap_mem_uint64(&(a[left]), &(a[right]));
            swap_mem_uint64(&(b[left]), &(b[right]));
            return;
        }
    }
    else {// size is 3, 3-sort left, center, & right
        if (a[left] > a[right-1]){
            swap_mem_uint64(&(a[left]),&(a[right-1]));
            swap_mem_uint64(&(b[left]),&(b[right-1]));
        }
        if (a[left] > a[right]){
            swap_mem_uint64(&(a[left]),&(a[right]));
            swap_mem_uint64(&(b[left]),&(b[right]));
        }
        if (a[right-1] > a[right]){
            swap_mem_uint64(&(a[right-1]),&(a[right]));
            swap_mem_uint64(&(b[right-1]),&(b[right]));
        }

        return;
    }
}

void quicksort(uint8_t* substrings, int size_line_sub, int p, int r, int* tab_ind)
{
    if (r-p+1 <= 3) manualSort(substrings, size_line_sub, p, r, tab_ind);
    else{
        uint8_t* pivot = median3(substrings, size_line_sub, p,r, tab_ind);
        int i = p;
        int j = r-1;

        while (1){
            while(memcmp(&(substrings[(++i)*size_line_sub]), pivot, size_line_sub) < 0) {}
            while(memcmp(&(substrings[(--j)*size_line_sub]), pivot, size_line_sub) > 0) {}

            if (i < j){
                swap_mem_uint8_t(&(substrings[i*size_line_sub]),&(substrings[j*size_line_sub]), size_line_sub);
                swap_mem_int(&(tab_ind[i]),&(tab_ind[j]));
            }
            else break ;
        }

        swap_mem_uint8_t(&(substrings[i*size_line_sub]), &(substrings[(r-1)*size_line_sub]), size_line_sub);
        swap_mem_int(&(tab_ind[i]),&(tab_ind[r-1]));

        quicksort(substrings, size_line_sub, p, i-1, tab_ind);
        quicksort(substrings, size_line_sub, i+1, r, tab_ind);
    }

    return;
}

void quicksort_uint64(uint64_t* a, uint64_t* b, uint64_t p, uint64_t r)
{
    if (r-p+1 <= 3) manualSort_uint64(a, b, p, r);
    else{

        uint64_t pivot = median3_uint64(a, b, p, r);
        uint64_t i = p;
        uint64_t j = r-1;

        while (1){

            while (a[++i] < pivot) {}
            while (a[--j] > pivot) {}

            if (i < j){
                swap_mem_uint64(&(a[i]),&(a[j]));
                swap_mem_uint64(&(b[i]),&(b[j]));
            }
            else break;
        }

        swap_mem_uint64(&(a[i]), &(a[r-1]));
        swap_mem_uint64(&(b[i]),&(b[r-1]));

        quicksort_uint64(a, b, p, i-1);
        quicksort_uint64(a, b, i+1, r);
    }

    return;
}

int* quicksort_init(uint8_t* substrings, int size_line_sub, int p, int r)
{
    int* tab_ind = (int*) malloc((r+1)*sizeof(int));

    ASSERT_NULL_PTR(substrings,"quicksort_init()")
    ASSERT_NULL_PTR(tab_ind,"quicksort_init()")

    for (int q = 0; q<=r; q++) tab_ind[q] = q;

    if (r-p+1 <= 3) manualSort(substrings, size_line_sub, p, r, tab_ind);
    else{
        uint8_t* pivot = median3(substrings, size_line_sub, p,r, tab_ind);
        int i = p;
        int j = r-1;

        while (1){
            while(memcmp(&(substrings[(++i)*size_line_sub]), pivot, size_line_sub) < 0) {}
            while(memcmp(&(substrings[(--j)*size_line_sub]), pivot, size_line_sub) > 0) {}

            if (i < j){
                swap_mem_uint8_t(&(substrings[i*size_line_sub]),&(substrings[j*size_line_sub]), size_line_sub);
                swap_mem_int(&(tab_ind[i]),&(tab_ind[j]));
            }
            else break;
        }

        swap_mem_uint8_t(&(substrings[i*size_line_sub]), &(substrings[(r-1)*size_line_sub]), size_line_sub);
        swap_mem_int(&(tab_ind[i]),&(tab_ind[r-1]));

        quicksort(substrings, size_line_sub, p, i-1, tab_ind);
        quicksort(substrings, size_line_sub, i+1, r, tab_ind);
    }

    return tab_ind;
}


