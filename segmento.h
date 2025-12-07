#ifndef SEGMENTO_H
#define SEGMENTO_H

#include <stdio.h>
#include <stdbool.h>

#include "geometria.h"
#include "formas.h"
#include "lista.h"

// ===========================================
// SEGMENTO
// ------------------------------------------
// operações com o segmento de reta utilizado
// no algoritmo de visibilidade.
// ===========================================

// ESTRUTURA DO SEGMENTO
typedef struct Segmento Segmento;

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// -> criar_segmento
// FUNÇÃO: criar um segmento do ponto início ao fim
// RECEBE: id, pontos início e fim e cor
// RETORNA: um ponteiro para o segmento
Segmento* criar_segmento(int id, Ponto* inicio, Ponto* fim, char* cor);

// -> destruir_segmento
// FUNÇÃO: destrói um segmento (não destrói os pontos, apenas a estrutura)
// RECEBE: segmento para destruir
void destruir_segmento(Segmento* s);

// ---------------
// FUNÇÕES GETTER
// ---------------

// -> segmento_get_id
// FUNÇÃO: consegue o id de um segmento
// RECEBE: o segmento
// RETORNA: o id
int segmento_get_id(Segmento* s);

// -> segmento_get_inicio
// FUNÇÃO: consegue o ponto do início
// RECEBE: o segmento
// RETORNA: ponteiro para o ponto 
Ponto* segmento_get_inicio(Segmento* s);

// -> segmento_get_fim
// FUNÇÃO: consegue o ponto do fim
// RECEBE: o segmento
// RETORNA: ponteiro para o ponto
Ponto* segmento_get_fim(Segmento* s);

// -> segmento_get_cor
// FUNÇÃO: consegue a cor do segmento 
// RECEBE: o segmento
// RETORNA: ponteiro para a cor
char* segmento_get_cor(Segmento* s);

// ----------------------
// OPERAÇÕES GEOMÉTRICAS
// ----------------------

// -> segmento_distancia_ponto
// FUNÇÃO: calcula distância de um ponto até um segmento 
// RECEBE: o segmento e o ponto
// RETORNA: a distância
double segmento_distancia_ponto(Segmento* s, Ponto* p);

// -> segmento_intersecta_raio
// FUNÇÃO: verifica se um raio (origem + ângulo) intersecta um segmento
// RECEBE: o segmento, o ponto e o ângulo
// RETORNA: verdadeiro caso intercepte e falso para caso não
bool segmento_intersecta_raio(Segmento* s, Ponto* origem, double angulo);

// -> segmento_intersecao_raio
// FUNÇÃO: verifica qual é a interseção entre um segmento e um raio
// RECEBE: o segmento, o ponto e o ângulo
// RETORNA: o ponto de interseção
Ponto* segmento_intersecao_raio(Segmento* s, Ponto* origem, double angulo);

// ===============================================
// CONVERSÃO DE FORMAS PARA SEGMENTOS (ANTEPAROS)
// ===============================================

/* -> forma_para_segmentos
    FUNÇÃO: converte forma em lista de segmentos para usar como anteparo
    - círculo: 1 segmento
    - retângulo: 4 segmentos 
    - linha: 1 segmento
    - texto: 1 segmento horizontal

    RECEBE: forma para converter e a orientação para círculos (horizontal e vertical)
    RETORNA: lista de segmentos
*/
Lista* forma_para_segmentos(Forma* forma, char orientacao);

#endif 