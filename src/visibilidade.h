#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include <stdio.h>

#include "geometria.h"
#include "lista.h"
#include "segmento.h"
#include "arvore.h"

// ===============================================
// VISIBILIDADE
// ----------------------------------------------
// algoritmo de cálculo de região de visibilidade.
// ===============================================

// ESTRUTURA DE UM VÉRTICE
typedef struct Vertice Vertice;

// ---------------------------
// FUNÇÃO AUXILIAR DO VÉRTICE 
// ---------------------------

/* -> comparar_vertices
    FUNÇÃO: comparar vértices (para ordenação)
    RECEBE: dois vértices
    RETORNA: negativo para v1 < v2, positivo maior que zero para v1 > v2 e 0 quando v1 = v2
 */
int comparar_vertices(Vertice* v1, Vertice* v2);

// ==========================
// ALGORITMO DE VISIBILIDADE
// ==========================

/* -> calcular_visibilidade
    FUNÇÃO: calcula a região de visibilidade a partir de um ponto
    RECEBE: 
    - origem (onde a bomba explode)
    - segmentos
    - tipo de ordenação
    - limite para insertionsort
    RETORNA: lista de pontos formando o polígono de visibilidade
*/
Lista* calcular_visibilidade(Ponto* origem, Lista* segmentos, char tipoOrdenacao, int limiteInsert);

#endif