#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "arvore.h"

#define EPSILON 1e-9

// ESTRUTURA NÓ DA ÁRVORE
typedef struct No {
    Segmento* segmento;
    struct No* esq;
    struct No* dir;
} No;

// ESTRUTURA DA ÁRVORE
struct Arvore {
    No* raiz;
    Ponto* ponto_ref;
    int tamanho;
};

// ===================
// FUNÇÕES AUXILIARES
// ===================

// CRIAR_NO
static No* criar_no(Segmento* s) {
    No* no = (No*) malloc(sizeof(No));
    if (no == NULL) return NULL;
    
    no->segmento = s;
    no->esq = NULL;
    no->dir = NULL;
    
    return no;
}

// DESTRUIR_NO
static void destruir_no(No* no) {
    if (no == NULL) return;
    
    destruir_no(no->esq);
    destruir_no(no->dir);
    free(no);
}

// PONTO_DIREITA_SEGMENTO
// FUNÇÃO: verifica se o ponto está a direita do ponto orientado
static bool ponto_direita_segmento(Segmento* s, Ponto* p) {
    Ponto* ini = segmento_get_inicio(s);
    Ponto* fim = segmento_get_fim(s);
    
    double det = orientacao(ini, fim, p);
    return det < -EPSILON;
}

// INSERIR_NO
static No* inserir_no(No* raiz, Segmento* s, Ponto* ponto_ref) {
    if (raiz == NULL) {
        return criar_no(s);
    }
    
    Ponto* s_ini = segmento_get_inicio(s);
    
    if (ponto_direita_segmento(raiz->segmento, s_ini)) {
        raiz->dir = inserir_no(raiz->dir, s, ponto_ref);
    } else {
        raiz->esq = inserir_no(raiz->esq, s, ponto_ref);
    }
    
    return raiz;
}

// ENCONTRAR_MINIMO
static No* encontrar_minimo(No* no) {
    while (no && no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

// REMOVER_NO
static No* remover_no(No* raiz, Segmento* s, Ponto* ponto_ref) {
    if (raiz == NULL) return NULL;
    
    int id_busca = segmento_get_id(s);
    int id_raiz = segmento_get_id(raiz->segmento);
    
    if (id_busca == id_raiz) {
        if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        } 
        else if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        
        No* temp = encontrar_minimo(raiz->dir);
        raiz->segmento = temp->segmento;
        raiz->dir = remover_no(raiz->dir, temp->segmento, ponto_ref);
        
    } else {
        Ponto* s_ini = segmento_get_inicio(s);
        
        if (ponto_direita_segmento(raiz->segmento, s_ini)) {
            raiz->dir = remover_no(raiz->dir, s, ponto_ref);
        } 
        else {
            raiz->esq = remover_no(raiz->esq, s, ponto_ref);
        }
    }
    
    return raiz;
}

// BUSCAR_MAIS_PROXIMO_REC
static Segmento* buscar_mais_proximo_rec(No* raiz, Ponto* p) {
    if (raiz == NULL) return NULL;
    
    if (ponto_direita_segmento(raiz->segmento, p)) {
        Segmento* seg_dir = buscar_mais_proximo_rec(raiz->dir, p);
        if (seg_dir != NULL) return seg_dir;
        return raiz->segmento;
    } 
    else {
        Segmento* seg_esq = buscar_mais_proximo_rec(raiz->esq, p);
        if (seg_esq != NULL) return seg_esq;
        return raiz->segmento;
    }
}

// BUSCAR_ID_REC
static Segmento* buscar_id_rec(No* raiz, int id) {
    if (raiz == NULL) return NULL;
    
    if (segmento_get_id(raiz->segmento) == id) {
        return raiz->segmento;
    }
    
    Segmento* esq = buscar_id_rec(raiz->esq, id);
    if (esq != NULL) return esq;
    
    return buscar_id_rec(raiz->dir, id);
}

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// CRIAR_ARVORE
Arvore* criar_arvore(Ponto* ponto_referencia) {
    Arvore* arv = (Arvore*) malloc(sizeof(Arvore));
    if (arv == NULL) return NULL;
    
    arv->raiz = NULL;
    arv->ponto_ref = ponto_referencia;
    arv->tamanho = 0;
    
    return arv;
}

// DESTRUIR_ARVORE
void destruir_arvore(Arvore* arv) {
    if (arv == NULL) return;
    
    destruir_no(arv->raiz);
    free(arv);
}

// -----------------------------
// VERIFICAR SITUAÇÃO DA ÁRVORE
// -----------------------------

// ARVORE_VAZIA
bool arvore_vazia(Arvore* arv) {
    return (arv == NULL || arv->raiz == NULL);
}

// TAMANHO_ARVORE
int tamanho_arvore(Arvore* arv) {
    return arv ? arv->tamanho : 0;
}

// ---------------------------
// ALTERAR ÁRVORES EXISTENTES
// ---------------------------

// INSERIR_SEGMENTO
void inserir_segmento(Arvore* arv, Segmento* s) {
    if (arv == NULL || s == NULL) return;
    
    arv->raiz = inserir_no(arv->raiz, s, arv->ponto_ref);
    arv->tamanho++;
}

// REMOVER_SEGMENTO
void remover_segmento(Arvore* arv, Segmento* s) {
    if (arv == NULL || s == NULL) return;
    
    arv->raiz = remover_no(arv->raiz, s, arv->ponto_ref);
    arv->tamanho--;
}

// --------------
// FUNÇÕES BUSCA
// --------------

// SEGMENTO_MAIS_PROXIMO
Segmento* segmento_mais_proximo(Arvore* arv, Ponto* p) {
    if (arv == NULL || p == NULL) return NULL;
    
    return buscar_mais_proximo_rec(arv->raiz, p);
}

// BUSCA_SEGMENTO_ID
Segmento* busca_segmento_id(Arvore* arv, int id) {
    if (arv == NULL) return NULL;
    
    return buscar_id_rec(arv->raiz, id);
}