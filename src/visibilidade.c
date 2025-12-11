#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "visibilidade.h"
#include "ordenacao.h"
#include "geometria.h"
#include "segmento.h"
#include "arvore.h"
#include "lista.h"

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
    v->angulo = atan2(get_y(p) - get_y(origem), get_x(p) - get_x(origem));
    v->distancia = distancia_pontos(origem, p);
    
    while (v->angulo < 0) v->angulo += 2 * M_PI;
    
    return v;
}

// DESTRUIR_VERTICE
static void destruir_vertice(Vertice* v) {
    if (v) free(v);
}

// VERTICE_ENCOBERTO
static bool vertice_encoberto(Vertice* v, Arvore* segs_ativos) { 
    if (!v || arvore_vazia(segs_ativos)) return false;

    Segmento* seg_proximo = segmento_mais_proximo(segs_ativos, v->angulo); 
    if (!seg_proximo || seg_proximo == v->segmento) return false;

    Ponto* origem = ponto_origem_global;
    Ponto* p_int = segmento_intersecao_raio(seg_proximo, origem, v->angulo);

    if (p_int) {
        double dist_int = distancia_pontos(origem, p_int);
        destruir_ponto(p_int);

        return dist_int < v->distancia - EPSILON;
    }
    
    return false;
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
            
            if (v_ini->angulo < v_fim->angulo) { 
                v_ini->tipo = TIPO_INICIO;
                v_fim->tipo = TIPO_FIM;
            }
            else { 
                v_fim->tipo = TIPO_INICIO;
                v_ini->tipo = TIPO_FIM;
            }

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
    
    double delta = fmax(max_x - min_x, max_y - min_y) * 0.5 + 500;
    min_x -= delta;
    max_x += delta;
    min_y -= delta;
    max_y += delta;
    
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
// FUNCAO AUXILIAR DO VERTICE 
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
    
    // adiciona retangulo envolvente
    Lista* segmentos_temp = criar_lista();
    Elemento* s_elem = get_primeiro_elemento(segmentos);
    while (s_elem != NULL) {
        Segmento* s = (Segmento*) get_elemento(segmentos, s_elem);
        inserir_fim_lista(segmentos_temp, s);
        s_elem = get_proximo_elemento(s_elem);
    }
    
    int tamanho_original = lista_tamanho(segmentos_temp);
    criar_retangulo_envolvente(segmentos_temp, origem);
    
    // extrai vertices
    Lista* vertices_lista = extrair_vertices(segmentos_temp, origem);
    if (!vertices_lista) {
        
        Elemento* temp_elem = get_primeiro_elemento(segmentos_temp);
        for(int i=0; i<tamanho_original; i++) {
            if(temp_elem) temp_elem = get_proximo_elemento(temp_elem);
        }

        for(int k=0; k<4; k++) {
            if(temp_elem) {
                Segmento* s = (Segmento*) remover_posicao_lista(segmentos_temp, temp_elem); 
                Ponto* p1 = segmento_get_inicio(s);
                Ponto* p2 = segmento_get_fim(s);
                destruir_ponto(p1);
                destruir_ponto(p2);
                destruir_segmento(s); 
                
                temp_elem = get_primeiro_elemento(segmentos_temp);
                for(int i=0; i<tamanho_original; i++) {
                    if(temp_elem) temp_elem = get_proximo_elemento(temp_elem);
                }
            }
        }
        
        destruir_lista(segmentos_temp);
        return NULL;
    }
    
    int n = lista_tamanho(vertices_lista);
    Vertice** vertices_array = (Vertice**) malloc(n * sizeof(Vertice*));
    
    Elemento* elem = get_primeiro_elemento(vertices_lista);
    int i = 0;
    while (elem != NULL) {
        vertices_array[i++] = (Vertice*) get_elemento(vertices_lista, elem);
        elem = get_proximo_elemento(elem);
    }
    
    // ordena vertices
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
    
    s_elem = get_primeiro_elemento(segmentos_temp);
    while (s_elem != NULL) {
        Segmento* seg = (Segmento*) get_elemento(segmentos_temp, s_elem); 
        if (seg) {
            double dist = segmento_distancia_ponto(seg, origem); 
            if (dist < min_dist) {
                min_dist = dist;
                seg_inicial = seg; 
            }
        }
        s_elem = get_proximo_elemento(s_elem);
    }
    
    Ponto* biombo = NULL;
    if (seg_inicial) {
        inserir_segmento(segs_ativos, seg_inicial); 
    }
    
    for (int j = 0; j < n; j++) {
        Vertice* v = vertices_array[j];
        
        Segmento* seg_antigo = segmento_mais_proximo(segs_ativos, v->angulo); 
        Ponto* p_int_antigo = NULL;
        if (seg_antigo) {
            p_int_antigo = segmento_intersecao_raio(seg_antigo, origem, v->angulo);
        }

        if (v->tipo == TIPO_INICIO) {
            inserir_segmento(segs_ativos, v->segmento); 
        } 
        else {
            remover_segmento(segs_ativos, v->segmento); 
        }
        
        Segmento* seg_novo = segmento_mais_proximo(segs_ativos, v->angulo);
        Ponto* p_int_novo = NULL;
        if (seg_novo) {
            p_int_novo = segmento_intersecao_raio(seg_novo, origem, v->angulo); 
        }
        
        if (p_int_antigo) {
            if (!biombo || distancia_pontos(biombo, p_int_antigo) > EPSILON) {
                Ponto* copia = criar_ponto(get_x(p_int_antigo), get_y(p_int_antigo));
                inserir_fim_lista(poligono, copia);
                biombo = copia; 
            }
        }

        double dist_int_novo = p_int_novo ? distancia_pontos(origem, p_int_novo) : 1e18;
        
        if (v->distancia < dist_int_novo - EPSILON) {
            if (!vertice_encoberto(v, segs_ativos)) {
                if (!biombo || distancia_pontos(biombo, v->ponto) > EPSILON) {
                    Ponto* copia = criar_ponto(get_x(v->ponto), get_y(v->ponto));
                    inserir_fim_lista(poligono, copia);
                    biombo = copia;
                }
            }
        }

        if (p_int_novo) {
            if (!biombo || distancia_pontos(biombo, p_int_novo) > EPSILON) {
                Ponto* copia = criar_ponto(get_x(p_int_novo), get_y(p_int_novo));
                inserir_fim_lista(poligono, copia);
                biombo = copia;
            }
        }
        
        if (p_int_antigo) destruir_ponto(p_int_antigo);
        if (p_int_novo) destruir_ponto(p_int_novo);
    }
    
    // limpeza! :D
    destruir_arvore(segs_ativos);
    
    for (int j = 0; j < n; j++) {
        destruir_vertice(vertices_array[j]);
    }
    free(vertices_array);
    destruir_lista(vertices_lista);
    
    Elemento* temp_elem = get_primeiro_elemento(segmentos_temp);
    for(int i=0; i<tamanho_original; i++) {
        if(temp_elem) temp_elem = get_proximo_elemento(temp_elem);
    }

    for(int k=0; k<4; k++) {
        if(temp_elem) {
            Segmento* s = (Segmento*) remover_posicao_lista(segmentos_temp, temp_elem); 
            Ponto* p1 = segmento_get_inicio(s);
            Ponto* p2 = segmento_get_fim(s);
            destruir_ponto(p1);
            destruir_ponto(p2);
            destruir_segmento(s); 
            
            temp_elem = get_primeiro_elemento(segmentos_temp);
            for(int i=0; i<tamanho_original; i++) {
                if(temp_elem) temp_elem = get_proximo_elemento(temp_elem);
            }
        }
    }
    
    destruir_lista(segmentos_temp);
    
    return poligono;
}