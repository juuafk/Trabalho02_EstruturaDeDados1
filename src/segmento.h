#ifndef SEGMENTO_H
#define SEGMENTO_H

#include <stdio.h>
#include <stdbool.h>
#include "geometria.h"
#include "formas.h"
#include "lista.h"

// ===========================================
// SEGMENTO DE RETA 
// ------------------------------------------
// estrutura e operações do segmento de reta.
// também será a base do anteparo.
// ===========================================

// ESTRUTURA DO SEGMENTO
typedef struct Segmento Segmento;

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

/* -> criar_segmento
    FUNÇÃO: criar um segmento de reta do ponto início ao fim
    RECEBE: id, pontos início e fim e cor
    RETORNA: um ponteiro para o segmento
*/
Segmento* criar_segmento(int id, Ponto* inicio, Ponto* fim, char* cor);

/* -> destruir_segmento
    FUNÇÃO: destrói um segmento (não destrói os pontos, apenas a estrutura)
    RECEBE: segmento para destruir
*/
void destruir_segmento(Segmento* s);

// ---------------
// FUNÇÕES GETTER
// ---------------

/* -> segmento_get_id
    FUNÇÃO: consegue o id de um segmento
    RECEBE: o segmento
    RETORNA: o id
*/
int segmento_get_id(Segmento* s);

/* -> segmento_get_inicio
    FUNÇÃO: consegue o ponto do início
    RECEBE: o segmento
    RETORNA: ponteiro para o ponto 
*/
Ponto* segmento_get_inicio(Segmento* s);

/* -> segmento_get_fim
    FUNÇÃO: consegue o ponto do fim
    RECEBE: o segmento
    RETORNA: ponteiro para o ponto
*/
Ponto* segmento_get_fim(Segmento* s);

/* -> segmento_get_cor
    FUNÇÃO: consegue a cor do segmento 
    RECEBE: o segmento
    RETORNA: ponteiro para a cor
*/
char* segmento_get_cor(Segmento* s);

// ----------------------
// OPERAÇÕES GEOMÉTRICAS
// ----------------------

/* -> segmento_distancia_ponto
    FUNÇÃO: calcula distância de um ponto até um segmento 
    RECEBE: o segmento e o ponto
    RETORNA: a distância
*/
double segmento_distancia_ponto(Segmento* s, Ponto* p);

/* -> segmento_intersecta_raio
    FUNÇÃO: verifica se um raio (origem + ângulo) intersecta um segmento
    RECEBE: o segmento, o ponto de origem e o ângulo
    RETORNA: verdadeiro caso intercepte e falso para caso não
*/
bool segmento_intersecta_raio(Segmento* s, Ponto* origem, double angulo);

/* -> segmento_intersecao_raio
    FUNÇÃO: calcula qual é o ponto de interseção entre um segmento e um raio
    RECEBE: o segmento, o ponto de origem e o ângulo do raio
    RETORNA: o ponto de interseção
*/
Ponto* segmento_intersecao_raio(Segmento* s, Ponto* origem, double angulo);

// ----------------------------------
// CONVERSÃO DE FORMAS PARA SEGMENTOS
// ----------------------------------

/* -> forma_para_segmentos
    FUNÇÃO: converte uma forma em lista de segmentos para bloquear explosões
    CONVERSÕES:
    - círculo: 1 segmento (horizontal ou vertical no diâmetro)
    - retângulo: 4 segmentos 
    - linha: 1 segmento 
    - texto: 1 segmento horizontal
    
    RECEBE: 
    - forma: a forma a ser convertida
    - orientação: para círculos, 'h' (horizontal) ou 'v' (vertical)
    - id_base: id base para gerar ids únicos dos segmentos criados
    
    RETORNA: lista de segmentos criados
*/
Lista* forma_para_segmentos(Forma* forma, char orientacao, int* id_base);

#endif