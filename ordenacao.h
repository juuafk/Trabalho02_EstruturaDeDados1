#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stdio.h>
#include "visibilidade.h"

// ===========================================
// ORDENAÇÃO
// ------------------------------------------
// implementa alguns algoritmos de ordenação,
// que são aqueles que deixam elementos de
// uma estrutura em sequência.
// (MÓDULO CONTÉM: mergesort, insertionsort,
// e um wrapper de qsort).
// ===========================================

// --------------------------
//       INSERTIONSORT
// --------------------------

/*  FUNÇÃO: ordenar um array usando InsertionSort
 -> INSERTIONSORT: constrói uma sublista ordenada, inserindo
 um elemento de cada vez em sua posição correta até ordenar
 todos os elementos.
    RECEBE: array de ponteiros, tamanho do array.
*/
void insertionsort(void** array, int n);

// --------------------------
//          MERGESORT
// --------------------------

/*  FUNÇÃO: ordenar um array usando MergeSort
 -> MERGESORT: divide recursivamente o array em metades
 até cada sublista conter 1 elemento. As sublistas ordenadas
 são combinadas de volta até que todos os elementos estejam
 em ordem.
    RECEBE: array de ponteiros, tamanho do array e limite
    para usar insertionsort (10).
*/
void mergesort(void** array, int n, int limite_insert);

// --------------------------
//           QSORT
// --------------------------

/* FUNÇÃO: wrapper para qsort da biblioteca padrão
 -> QSORT: quicksort, o array é reajustado conforme um elemento pivô
 (que pode variar). Elementos menores ficam a esquerda e maiores a 
 direita, sendo reordenados até estarem em sequência.
    RECEBE: array de ponteiros, tamanho do array.
 */
void ordena_com_qsort(void** array, int n);

#endif 