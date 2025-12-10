#include <stdio.h>
#include <stdlib.h>

#include "lista.h"
#include "formas.h"

// ESTRUTURA DO ELEMENTO DA LISTA (NÓ)
struct elemento {
    void* dado;
    struct elemento* proximo;
    struct elemento* anterior; 
};

// ESTRUTURA DA LISTA
struct lista {
    Elemento* primeiro;
    Elemento* ultimo;
    int tamanho;
};

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// CRIAR_LISTA
Lista* criar_lista() {
    Lista* l = (Lista*)malloc(sizeof(Lista));
    if (l == NULL) {
        perror("AVISO: erro ao alocar lista!");
        return NULL;
    }
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;
    return l;
}

// DESTRUIR_LISTA (não libera os dados)
void destruir_lista(Lista* l) {
    if (l == NULL) return;
    
    Elemento* atual = l->primeiro;
    Elemento* prox;
    
    while (atual != NULL) {
        prox = atual->proximo;
        free(atual); 
        atual = prox;
    }
    
    free(l);
}

// DESTRUIR_LISTA_COM_FORMAS
void destruir_lista_com_formas(Lista* l) {
    if (l == NULL) return;
    
    Elemento* atual = l->primeiro;
    Elemento* prox;
    
    while (atual != NULL) {
        prox = atual->proximo;
        if (atual->dado != NULL) {
            destruir_forma((Forma*)atual->dado); 
        }
        free(atual);  
        atual = prox;
    }
    
    free(l);
}

// ----------------------------
// VERIFICAR SITUAÇÃO DA LISTA
// ----------------------------

// LISTA_VAZIA
bool lista_vazia(Lista* l) {
    return l == NULL || l->primeiro == NULL;
}

// LISTA_TAMANHO
int lista_tamanho(Lista* l) {
    if (l == NULL) {
        return 0;
    }
    return l->tamanho;
}

// GET_ELEMENTO
void* get_elemento(Lista* l, Elemento* x) {
    if (x == NULL) {
        return NULL;
    }
    return x->dado;
}

// GET_PRIMEIRO_ELEMENTO
Elemento* get_primeiro_elemento(Lista* l) {
    if (l == NULL) {
        return NULL;
    }
    return l->primeiro;
}

// GET_ULTIMO_ELEMENTO
Elemento* get_ultimo_elemento(Lista* l) {
    if (l == NULL) {
        return NULL;
    }
    return l->ultimo;
}

// GET_PROXIMO_ELEMENTO
Elemento* get_proximo_elemento(Elemento* atual) {
    if (atual == NULL) {
        return NULL;
    }
    return atual->proximo;
}

// GET_ELEMENTO_ANTERIOR
Elemento* get_elemento_anterior(Elemento* atual) {
    if (atual == NULL) {
        return NULL;
    }
    return atual->anterior;
}

// --------------------------
// ALTERAR LISTAS EXISTENTES
// --------------------------

// INSERIR_INICIO_LISTA
void inserir_inicio_lista(Lista* l, void* elemento) {
    if (l == NULL) return;
    
    Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
    if (novo == NULL) {
        perror("AVISO: erro ao alocar elemento!");
        return;
    }
    
    novo->dado = elemento;
    novo->proximo = l->primeiro;
    novo->anterior = NULL;
    
    if (l->primeiro != NULL) {
        l->primeiro->anterior = novo;
    }
    
    l->primeiro = novo;
    
    if (l->ultimo == NULL) {
        l->ultimo = novo;
    }
    
    l->tamanho++;
}

// INSERIR_FIM_LISTA
void inserir_fim_lista(Lista* l, void* elemento) {
    if (l == NULL) return;
    
    Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
    if (novo == NULL) {
        perror("AVISO: erro ao alocar elemento!");
        return;
    }
    
    novo->dado = elemento;
    novo->proximo = NULL;
    novo->anterior = l->ultimo;
    
    if (l->ultimo == NULL) {
        l->primeiro = novo;
    }
    else {
        l->ultimo->proximo = novo;
    }
    
    l->ultimo = novo;
    l->tamanho++;
}

// REMOVER_INICIO_LISTA
void* remover_inicio_lista(Lista* l) {
    if (l == NULL || l->primeiro == NULL) {
        return NULL;
    }
    
    Elemento* temp = l->primeiro;
    void* dado = temp->dado;
    
    l->primeiro = l->primeiro->proximo;
    
    if (l->primeiro == NULL) {
        l->ultimo = NULL;
    } 
    else {
        l->primeiro->anterior = NULL;
    }
    
    free(temp);
    l->tamanho--;
    
    return dado;
}

// REMOVER_FIM_LISTA
void* remover_fim_lista(Lista* l) {
    if (l == NULL || l->primeiro == NULL) {
        return NULL;
    }
    
    Elemento* temp = l->ultimo;
    void* dado = temp->dado;
    
    l->ultimo = l->ultimo->anterior;
    
    if (l->ultimo == NULL) {
        l->primeiro = NULL;
    }
    else {
        l->ultimo->proximo = NULL;
    }
    
    free(temp);
    l->tamanho--;
    
    return dado;
}

// REMOVER_POSICAO_LISTA
void* remover_posicao_lista(Lista* l, Elemento* x) {
    if (l == NULL || x == NULL) {
        return NULL;
    }
    
    void* dado = x->dado;
    
    if (x->anterior != NULL) {
        x->anterior->proximo = x->proximo;
    } 
    else {
        l->primeiro = x->proximo;
    }
    
    if (x->proximo != NULL) {
        x->proximo->anterior = x->anterior;
    } 
    else {
        l->ultimo = x->anterior;
    }
    
    free(x);
    l->tamanho--;
    
    return dado;
}

// ORDENAR_LISTA_POR_ID
void ordenar_lista_por_id(Lista* l) {
    if (!l || lista_tamanho(l) <= 1) return;
    
    bool trocou;
    do {
        trocou = false;
        Elemento* atual = l->primeiro;
        
        while (atual && atual->proximo) {
            Forma* f1 = (Forma*) atual->dado;
            Forma* f2 = (Forma*) atual->proximo->dado;
            
            if (forma_get_id(f1) < forma_get_id(f2)) { 
                void* temp = atual->dado;
                atual->dado = atual->proximo->dado;
                atual->proximo->dado = temp;
                trocou = true;
            }
            
            atual = atual->proximo;
        }
    } 
    while (trocou);
}