#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdbool.h>

// ============================================
// LISTA
// -------------------------------------------
// uma estrutura de dados que permite armazenar
// um conjunto de valores relacionados em uma
// variável.
// ============================================

// ESTRUTURA DO ELEMENTO DA LISTA
typedef struct elemento Elemento;

// ESTRUTURA DA LISTA
typedef struct lista Lista;

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// -> criar_lista
// função: criar uma lista vazia
// retorna: ponteiro para a lista
Lista* criar_lista();

// -> destruir_lista
// função: liberar memória alocada para uma lista
// recebe: a lista que vai ser destruída
void destruir_lista(Lista* l);

// -> destruir_lista_com_formas
// função: liberar memória alocada para a lista e para as formas nela
// recebe: a lista com formas que vai ser destruída
void destruir_lista_com_formas(Lista* l);

// Declaração da nova função
void destruir_lista_de_pontos(Lista* l);
// ----------------------------
// VERIFICAR SITUAÇÃO DA LISTA
// ----------------------------

// -> lista_vazia
// função: verifica se a lista está vazia
// recebe: a lista para verificação
// retorna: verdadeiro para lista vazia, falso para lista não vazia
bool lista_vazia(Lista* l);

// -> lista_tamanho
// função: verifica o tamanho da lista
// recebe: a lista para verificação
// retorna: tamanho da lista
int lista_tamanho(Lista* l);

// -> get_elemento
// função: obtém um elemento em uma posição específica
// recebe: lista e posição
// retorna: o elemento na posição
void* get_elemento(Lista* l, Elemento* x);

// -> get_primeiro_elemento
// função: obtém o primeiro elemento da lista
// recebe: a lista para verificação
// retorna: ponteiro para o elemento
Elemento* get_primeiro_elemento(Lista* l);

// -> get_ultimo_elemento
// função: obtém o último elemento da lista
// recebe: a lista para verificação
// retorna: ponteiro para o elemento
Elemento* get_ultimo_elemento(Lista* l);

// -> getProximoElemento
// função: obtém o próximo elemento
// recebe: elemento atual
// retorna: o próximo elemento em relação a ele
Elemento* get_proximo_elemento(Elemento* atual);

// -> getElementoAnterior
// função: obtém o elemento anterior
// recebe: elemento atual
// retorna: o elemento anterior a ele
Elemento* get_elemento_anterior(Elemento* atual);

// --------------------------
// ALTERAR LISTAS EXISTENTES
// --------------------------

// -> Inserir_inicio_lista
// função: inserir um elemento no início da lista
// recebe: a lista e o elemento para inserir
void inserir_inicio_lista(Lista* l, void* elemento);

// -> inserir_fim_lista
// função: inserir um elemento no fim da lista
// recebe: a lista e o elemento para inserir
void inserir_fim_lista(Lista* l, void* elemento);

// -> remover_inicio_lista
// função: remove o elemento no início da lista
// recebe: a lista
// retorna: primeiro elemento (removido)
void* remover_inicio_lista(Lista* l);

// -> remover_fim_lista
// função: remove o elemento do fim da lista
// recebe: a lista
// retorna: último elemento (removido)
void* remover_fim_lista(Lista* l);

// -> remover_posicao_lista
// função: remove um elemento específico da lista
// recebe: a lista e o elemento
// retorna: o elemento removido
void* remover_posicao_lista(Lista* l, Elemento* x);

// -> ordenar_lista_por_id
// função: ordenar elementos de uma lista pelo id
// recebe: a lista
void ordenar_lista_por_id(Lista* l);

#endif