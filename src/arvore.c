#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "geometria.h"
#include "segmento.h"
#include "arvore.h"

#define EPSILON 1e-9

// ESTRUTURA NO DA ARVORE
typedef struct No {
    Segmento* segmento;
    struct No* esq;
    struct No* dir;
} No;

// ESTRUTURA DA ARVORE
struct Arvore {
    No* raiz;
    Ponto* ponto_ref;
    int tamanho;
};

// --------------------------------------
// CRIAÇÃO E DESTRUIÇÃO E ALTERAÇÃO (NÓS)
// --------------------------------------

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

// ENCONTRAR_MINIMO
static No* encontrar_minimo(No* no) {
    while (no && no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

// COMPARAR_SEGMENTOS_POR_DISTANCIA
static int comparar_segmentos_por_distancia(Segmento* s1, Segmento* s2, Ponto* origem) {
    double dist1 = segmento_distancia_ponto(s1, origem);
    double dist2 = segmento_distancia_ponto(s2, origem);
    
    if (fabs(dist1 - dist2) < EPSILON) {
        return segmento_get_id(s1) - segmento_get_id(s2);
    }
    
    return (dist1 < dist2) ? -1 : 1; 
}

// INSERIR_NO
static No* inserir_no(No* raiz, Segmento* s, Ponto* ponto_ref) {
    if (raiz == NULL) {
        return criar_no(s);
    }
    
    int comparacao = comparar_segmentos_por_distancia(s, raiz->segmento, ponto_ref);
    
    if (comparacao < 0) { 
        raiz->esq = inserir_no(raiz->esq, s, ponto_ref);
    } else { 
        raiz->dir = inserir_no(raiz->dir, s, ponto_ref);
    }
    
    return raiz;
}

// REMOVER_NO
static No* remover_no(No* raiz, Segmento* s, Ponto* ponto_ref) {
    if (raiz == NULL) return NULL;
    
    int id_busca = segmento_get_id(s);
    int id_raiz = segmento_get_id(raiz->segmento);
    
    int comparacao = comparar_segmentos_por_distancia(s, raiz->segmento, ponto_ref);
    
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
        if (comparacao > 0) { 
            raiz->dir = remover_no(raiz->dir, s, ponto_ref);
        } 
        else { 
            raiz->esq = remover_no(raiz->esq, s, ponto_ref);
        }
    }
    
    return raiz;
}

// --------------
// FUNCOES BUSCA
// --------------

// BUSCAR_MAIS_PROXIMO_REC
static void buscar_mais_proximo_rec(No* raiz, Ponto* origem, double angulo_raio, Segmento** resultado, double* menor_dist_int) {
    if (raiz == NULL) return;
    
    Ponto* p_int = segmento_intersecao_raio(raiz->segmento, origem, angulo_raio);
    
    if (p_int) {
        double dist_int = distancia_pontos(origem, p_int);

        if (dist_int < *menor_dist_int) {
            *menor_dist_int = dist_int;
            *resultado = raiz->segmento;
        }
        destruir_ponto(p_int);
    }

    if (segmento_distancia_ponto(raiz->segmento, origem) <= *menor_dist_int + EPSILON) {
        buscar_mais_proximo_rec(raiz->esq, origem, angulo_raio, resultado, menor_dist_int);
    }
    
    buscar_mais_proximo_rec(raiz->dir, origem, angulo_raio, resultado, menor_dist_int);
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

// SEGMENTO_MAIS_PROXIMO 
Segmento* segmento_mais_proximo(Arvore* arv, double angulo_raio) {
    if (arv == NULL) return NULL;
    
    Segmento* mais_proximo = NULL;
    double menor_dist = 1e18; 

    buscar_mais_proximo_rec(arv->raiz, arv->ponto_ref, angulo_raio, &mais_proximo, &menor_dist);
    
    return mais_proximo;
}

// BUSCA_SEGMENTO_ID
Segmento* busca_segmento_id(Arvore* arv, int id) {
    if (arv == NULL) return NULL;
    
    return buscar_id_rec(arv->raiz, id);
}

// --------------------------------
// FUNCOES DE CRIAÇÃO E DESTRUIÇÃO
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
// ALTERAR ARVORES EXISTENTES
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