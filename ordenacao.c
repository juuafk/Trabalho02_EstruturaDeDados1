#include <stdio.h>
#include <stdlib.h>

#include "ordenacao.h"
#include "visibilidade.h"

// --------------------------
//       INSERTIONSORT
// --------------------------

// INSERTIONSORT
void insertionsort(void** array, int n) {
    if (!array || n <= 1) return;
    
    for (int i = 1; i < n; i++) {
        void* chave = array[i];
        int j = i - 1;
        
        while (j >= 0 && comparar_vertices((Vertice*)array[j], (Vertice*)chave) > 0) {
            array[j + 1] = array[j];
            j--;
        }
        
        array[j + 1] = chave;
    }
}

// --------------------------
//          MERGESORT
// --------------------------

// MERGE
static void merge(void** array, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    
    void** esq = (void**) malloc(n1 * sizeof(void*));
    void** dir = (void**) malloc(n2 * sizeof(void*));
    
    if (!esq || !dir) {
        if (esq) free(esq);
        if (dir) free(dir);
        return;
    }
    
    for (int i = 0; i < n1; i++) {
        esq[i] = array[inicio + i];
    }
    
    for (int j = 0; j < n2; j++) {
        dir[j] = array[meio + 1 + j];
    }
    
    int i = 0, j = 0, k = inicio;
    
    while (i < n1 && j < n2) {
        if (comparar_vertices((Vertice*)esq[i], (Vertice*)dir[j]) <= 0) {
            array[k++] = esq[i++];
        } 
        else {
            array[k++] = dir[j++];
        }
    }
    
    while (i < n1) {
        array[k++] = esq[i++];
    }
    
    while (j < n2) {
        array[k++] = dir[j++];
    }
    
    free(esq);
    free(dir);
}

// MERGESORT_REC
static void mergesort_rec(void** array, int inicio, int fim, int limite_insert) {
    if (inicio >= fim) return;
    
    int tamanho = fim - inicio + 1;
    
    // usa insertion se for um array pequeno
    if (tamanho <= limite_insert) {
        insertionsort(&array[inicio], tamanho);
        return;
    }
    
    int meio = inicio + (fim - inicio) / 2;
    
    mergesort_rec(array, inicio, meio, limite_insert);
    mergesort_rec(array, meio + 1, fim, limite_insert);
    merge(array, inicio, meio, fim);
}

// MERGESORT
void mergesort(void** array, int n, int limite_insert) {
    if (!array || n <= 1) return;
    mergesort_rec(array, 0, n - 1, limite_insert);
}

// --------------------------
//           QSORT
// --------------------------

// COMPARAR_QSORT
static int comparar_qsort(const void* a, const void* b) {
    Vertice* v1 = *(Vertice**)a;
    Vertice* v2 = *(Vertice**)b;
    return comparar_vertices(v1, v2);
}

// ORDENA_COM_QSORT
void ordena_com_qsort(void** array, int n) {
    if (!array || n <= 1) return;
    qsort(array, n, sizeof(void*), comparar_qsort);
}