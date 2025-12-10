#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "visibilidade.h"
#include "ordenacao.h"
#include "arvore.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define EPSILON 1e-9

// TIPOS DE VÉRTICE
typedef enum {
    TIPO_INICIO,
    TIPO_FIM
} TipoVertice;

// ESTRUTURA DO VERTICE
struct Vertice {
    TipoVertice tipo;
    Segmento* segmento;
    Ponto* ponto;
    double angulo;
    double distancia;
};

// ORIGEM GLOBAL (usada em comparação)
static Ponto* ponto_origem_global = NULL;

// ===================
// FUNÇÕES AUXILIARES
// ===================

// CRIAR_VERTICE
static Vertice* criar_vertice(TipoVertice tipo, Segmento* seg, Ponto* p, Ponto* origem) {
    Vertice* v = (Vertice*) malloc(sizeof(Vertice));
    if (!v) return NULL;
    
    v->tipo = tipo;
    v->segmento = seg;
    v->ponto = p;
    v->angulo = angulo_entre_pontos(origem, p);
    v->distancia = distancia_pontos(origem, p);
    
    while (v->angulo < 0) v->angulo += 2 * M_PI;
    while (v->angulo >= 2 * M_PI) v->angulo -= 2 * M_PI;
    
    return v;
}

// DESTRUIR_VERTICE
static void destruir_vertice(Vertice* v) {
    if (v) free(v);
}

// VERTICE_ENCOBERTO
static bool vertice_encoberto(Vertice* v, Arvore* segs_ativos) { 
    if (!v || arvore_vazia(segs_ativos)) return false;
    
    Segmento* seg_proximo = segmento_mais_proximo(segs_ativos, v->ponto); 
    if (!seg_proximo) return false;
    
    double dist_seg = segmento_distancia_ponto(seg_proximo, v->ponto);
    
    return dist_seg < EPSILON;
}

// EXTRAIR_VERTICES
static Lista* extrair_vertices(Lista* segmentos, Ponto* origem) { 
    Lista* vertices = criar_lista();
    if (!vertices) return NULL;
    
    Elemento* elem = get_primeiro_elemento(segmentos);
    while (elem != NULL) {
        Segmento* seg = (Segmento*) get_elemento(segmentos, elem); 
        
        if (seg) {
            Ponto* ini = segmento_get_inicio(seg); 
            Ponto* fim = segmento_get_fim(seg); 
            
            Vertice* v_ini = criar_vertice(TIPO_INICIO, seg, ini, origem);
            Vertice* v_fim = criar_vertice(TIPO_FIM, seg, fim, origem);
            
            if (v_ini) inserir_fim_lista(vertices, v_ini);
            if (v_fim) inserir_fim_lista(vertices, v_fim);
        }
        
        elem = get_proximo_elemento(elem);
    }
    
    return vertices;
}

// CRIAR_RETANGULO_ENVOLVENTE
static void criar_retangulo_envolvente(Lista* segmentos, Ponto* origem) { 
    double min_x = get_x(origem), max_x = get_x(origem);
    double min_y = get_y(origem), max_y = get_y(origem);
    
    Elemento* elem = get_primeiro_elemento(segmentos);
    while (elem != NULL) {
        Segmento* seg = (Segmento*) get_elemento(segmentos, elem); 
        
        if (seg) {
            Ponto* ini = segmento_get_inicio(seg); 
            Ponto* fim = segmento_get_fim(seg); 
            
            double x1 = get_x(ini), y1 = get_y(ini);
            double x2 = get_x(fim), y2 = get_y(fim);
            
            if (x1 < min_x) min_x = x1;
            if (x1 > max_x) max_x = x1;
            if (x2 < min_x) min_x = x2;
            if (x2 > max_x) max_x = x2;
            
            if (y1 < min_y) min_y = y1;
            if (y1 > max_y) max_y = y1;
            if (y2 < min_y) min_y = y2;
            if (y2 > max_y) max_y = y2;
        }
        
        elem = get_proximo_elemento(elem);
    }
    
    double delta = fmax(max_x - min_x, max_y - min_y) * 0.5 + 100;
    min_x -= delta;
    max_x += delta;
    min_y -= delta;
    max_y += delta;
    
    // segmentos do retângulo
    Ponto* p1 = criar_ponto(min_x, min_y);
    Ponto* p2 = criar_ponto(max_x, min_y);
    Ponto* p3 = criar_ponto(max_x, max_y);
    Ponto* p4 = criar_ponto(min_x, max_y);
    
    Segmento* s1 = criar_segmento(-1, p1, p2, "#000000"); 
    Segmento* s2 = criar_segmento(-2, p2, p3, "#000000"); 
    Segmento* s3 = criar_segmento(-3, p3, p4, "#000000"); 
    Segmento* s4 = criar_segmento(-4, p4, p1, "#000000"); 
    
    inserir_fim_lista(segmentos, s1);
    inserir_fim_lista(segmentos, s2);
    inserir_fim_lista(segmentos, s3);
    inserir_fim_lista(segmentos, s4);
}

// ---------------------------
// FUNÇÃO AUXILIAR DO VÉRTICE 
// ---------------------------

// COMPARAR_VERTICES
int comparar_vertices(Vertice* v1, Vertice* v2) {
    if (!v1 || !v2) return 0;
    
    if (fabs(v1->angulo - v2->angulo) > EPSILON) {
        return (v1->angulo < v2->angulo) ? -1 : 1;
    }
    
    if (fabs(v1->distancia - v2->distancia) > EPSILON) {
        return (v1->distancia < v2->distancia) ? -1 : 1;
    }
    
    if (v1->tipo != v2->tipo) {
        return (v1->tipo == TIPO_INICIO) ? -1 : 1;
    }
    
    return 0;
}

// ==========================
// ALGORITMO DE VISIBILIDADE
// ==========================

// CALCULAR_VISIBILIDADE
Lista* calcular_visibilidade(Ponto* origem, Lista* segmentos, char tipoOrdenacao, int limiteInsert) { 
    if (!origem || !segmentos) return NULL;
    
    ponto_origem_global = origem;
    
    // adiciona retângulo envolvente
    criar_retangulo_envolvente(segmentos, origem);
    
    // extrai vértices
    Lista* vertices_lista = extrair_vertices(segmentos, origem);
    if (!vertices_lista) return NULL;
    
    // converte lista para array para ordenação
    int n = lista_tamanho(vertices_lista);
    Vertice** vertices_array = (Vertice**) malloc(n * sizeof(Vertice*));
    
    Elemento* elem = get_primeiro_elemento(vertices_lista);
    int i = 0;
    while (elem != NULL) {
        vertices_array[i++] = (Vertice*) get_elemento(vertices_lista, elem);
        elem = get_proximo_elemento(elem);
    }
    
    // ordena vértices
    if (tipoOrdenacao == 'q') {
        ordena_com_qsort((void**)vertices_array, n);
    } 
    else {
        mergesort((void**)vertices_array, n, limiteInsert);
    }
    
    // inicializa estruturas
    Arvore* segs_ativos = criar_arvore(origem); 
    Lista* poligono = criar_lista();
    
    Segmento* seg_inicial = NULL; 
    double min_dist = 1e9;
    
    elem = get_primeiro_elemento(segmentos);
    while (elem != NULL) {
        Segmento* seg = (Segmento*) get_elemento(segmentos, elem); 
        if (seg) {
            double dist = segmento_distancia_ponto(seg, origem); 
            if (dist < min_dist) {
                min_dist = dist;
                seg_inicial = seg; 
            }
        }
        elem = get_proximo_elemento(elem);
    }
    
    Ponto* biombo = NULL;
    if (seg_inicial) {
        inserir_segmento(segs_ativos, seg_inicial); 
        biombo = segmento_get_inicio(seg_inicial); 
    }
    
    for (int j = 0; j < n; j++) {
        Vertice* v = vertices_array[j];
        
        if (v->tipo == TIPO_INICIO) {
            inserir_segmento(segs_ativos, v->segmento); 
            
            if (!vertice_encoberto(v, segs_ativos)) {
                Segmento* seg_prox = segmento_mais_proximo(segs_ativos, v->ponto); 
                
                if (seg_prox) {
                    Ponto* y = segmento_intersecao_raio(seg_prox, origem, v->angulo); 
                    
                    if (biombo && y) {
                        inserir_fim_lista(poligono, biombo);
                        inserir_fim_lista(poligono, y);
                        inserir_fim_lista(poligono, y);
                        inserir_fim_lista(poligono, v->ponto);
                    }
                    
                    biombo = v->ponto;
                }
            }
        }
        else {
            remover_segmento(segs_ativos, v->segmento); 
            
            if (!vertice_encoberto(v, segs_ativos)) {
                Segmento* seg_prox = segmento_mais_proximo(segs_ativos, v->ponto);
                
                if (seg_prox) {
                    Ponto* y = segmento_intersecao_raio(seg_prox, origem, v->angulo); 
                    
                    if (biombo && y) {
                        inserir_fim_lista(poligono, biombo);
                        inserir_fim_lista(poligono, v->ponto);
                        inserir_fim_lista(poligono, v->ponto);
                        inserir_fim_lista(poligono, y);
                    }
                    
                    biombo = y;
                }
            }
        }
    }
    
    // limpeza! :D
    destruir_arvore(segs_ativos);
    
    for (int j = 0; j < n; j++) {
        destruir_vertice(vertices_array[j]);
    }
    free(vertices_array);
    destruir_lista(vertices_lista);
    
    return poligono;
}