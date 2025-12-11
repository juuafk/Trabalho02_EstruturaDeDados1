#ifndef ARVORE_H
#define ARVORE_H

#include <stdbool.h>

#include "segmento.h"
#include "geometria.h"

// ===============================================
// ÁRVORE
// ----------------------------------------------
// estrutura de dados que organiza elementos a
// partir de uma raiz, ramificando-se em
// subestruturas até as "folhas".
// ===============================================

// ESTRUTURA DA ÁRVORE
typedef struct Arvore Arvore;

// --------------
// FUNÇÕES BUSCA
// --------------

/* -> segmento_mais_proximo
    FUNÇÃO: retorna o segmento mais próximo de um ponto
    RECEBE: a árvore e o ponto
    RETORNA: segmento mais próximo
 */
Segmento* segmento_mais_proximo(Arvore* arv, double angulo_raio); 

/* -> busca_segmento_id
    FUNÇÃO: busca um segmento na árvore pelo ID
    RECEBE: a árvore e o id
    RETORNA: ponteiro para o segmento
 */
Segmento* busca_segmento_id(Arvore* arv, int id);

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

/* -> criar_arvore
    FUNÇÃO: cria uma árvore vazia
    RECEBE: ponto de referência para ordenação
    RETORNA: árvore criada
*/
Arvore* criar_arvore(Ponto* ponto_referencia);

/* -> destruir_arvore
    FUNÇÃO: destrói a árvore (não destrói os segmentos, apenas a estrutura)
    RECEBE: a árvore
*/
void destruir_arvore(Arvore* arv);

// -----------------------------
// VERIFICAR SITUAÇÃO DA ÁRVORE
// -----------------------------

/* -> arvore_vazia
    FUNÇÃO: verifica se a árvore está vazia
    RECEBE: a árvore
    RETORNA: verdadeiro se estiver vazia, falso caso não
*/
bool arvore_vazia(Arvore* arv);

/* -> tamanho_arvore
    FUNÇÃO: verifica o número de segmentos na árvore
    RECEBE: a árvore
    RETORNA: quantidade de segmentos
*/
int tamanho_arvore(Arvore* arv);

// ---------------------------
// ALTERAR ÁRVORES EXISTENTES
// ---------------------------

/* -> inserir_segmento
    FUNÇÃO: insere um segmento na árvore
    RECEBE: a árvore e o segmento
*/
void inserir_segmento(Arvore* arv, Segmento* s);

/* -> remover_segmento
    FUNÇÃO: remove um segmento da árvore
    RECEBE: a árvore e o segmento
 */
void remover_segmento(Arvore* arv, Segmento* s);

#endif