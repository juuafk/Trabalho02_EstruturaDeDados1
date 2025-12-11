#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "segmento.h"

#define MAX_COR_LEN 20
#define EPSILON 1e-9

// ESTRUTURA DO SEGMENTO
struct Segmento {
    int id;
    Ponto* inicio;
    Ponto* fim;
    char cor[MAX_COR_LEN];
};

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// CRIAR_SEGMENTO
Segmento* criar_segmento(int id, Ponto* inicio, Ponto* fim, char* cor) {
    if (!inicio || !fim) return NULL;
    
    Segmento* s = (Segmento*) malloc(sizeof(Segmento));
    if (s == NULL) return NULL;
    
    s->id = id;
    s->inicio = inicio;
    s->fim = fim;
    
    if (cor) {
        strncpy(s->cor, cor, MAX_COR_LEN - 1);
        s->cor[MAX_COR_LEN - 1] = '\0';
    } 
    else {
        strcpy(s->cor, "#000000");
    }
    
    return s;
}

// DESTRUIR_SEGMENTO
void destruir_segmento(Segmento* s) {
    if (s != NULL) { 
        free(s); // não destrói pontos!
    }
}

// ---------------
// FUNÇÕES GETTER
// ---------------

// SEGMENTO_GET_ID
int segmento_get_id(Segmento* s) {
    return s ? s->id : -1;
}

// SEGMENTO_GET_INICIO
Ponto* segmento_get_inicio(Segmento* s) {
    return s ? s->inicio : NULL;
}

// SEGMENTO_GET_FIM
Ponto* segmento_get_fim(Segmento* s) {
    return s ? s->fim : NULL;
}

// SEGMENTO_GET_COR
char* segmento_get_cor(Segmento* s) {
    return s ? s->cor : NULL;
}

// ----------------------
// OPERAÇÕES GEOMÉTRICAS
// ----------------------

// SEGMENTO_DISTANCIA_PONTO
double segmento_distancia_ponto(Segmento* s, Ponto* p) {
    if (!s || !p) return 0.0;
    return distancia_ponto_segmento(p, s->inicio, s->fim);
}

// SEGMENTO_INTERSECTA_RAIO
bool segmento_intersecta_raio(Segmento* s, Ponto* origem, double angulo) {
    if (!s || !origem) return false;
    
    double dist_max = 1e6;
    double px = get_x(origem) + dist_max * cos(angulo);
    double py = get_y(origem) + dist_max * sin(angulo);
    Ponto* p_raio = criar_ponto(px, py);
    
    bool intersecta = segmentos_intersectam(origem, p_raio, s->inicio, s->fim);
    
    destruir_ponto(p_raio);
    return intersecta;
}

// SEGMENTO_INTERSECAO_RAIO
Ponto* segmento_intersecao_raio(Segmento* s, Ponto* origem, double angulo) {
    if (!s || !origem) return NULL;
    
    double dist_max = 1e6;
    double px = get_x(origem) + dist_max * cos(angulo);
    double py = get_y(origem) + dist_max * sin(angulo);
    Ponto* p_raio = criar_ponto(px, py);
    
    Ponto* intersecao = intersecao_segmentos(origem, p_raio, s->inicio, s->fim);
    
    destruir_ponto(p_raio);
    return intersecao;
}

// ----------------------------------
// CONVERSÃO DE FORMAS PARA SEGMENTOS
// ----------------------------------

// FORMA_PARA_SEGMENTOS
Lista* forma_para_segmentos(Forma* forma, char orientacao, int* id_base) {
    if (!forma || !id_base) return NULL;
    
    Lista* segmentos = criar_lista();
    if (!segmentos) return NULL;
    
    int id_forma = forma_get_id(forma);
    char tipo = forma_get_tipo(forma);
    const char* cor_borda = forma_get_cor_borda(forma);
    
    if (tipo == 'c') {
        // CÍRCULO -> 1 segmento
        double cx = forma_get_x(forma);
        double cy = forma_get_y(forma);
        double r = forma_get_r(forma);
        
        Ponto *p1, *p2;
        
        if (orientacao == 'h') {
            p1 = criar_ponto(cx - r, cy);
            p2 = criar_ponto(cx + r, cy);
        }
        else {
            p1 = criar_ponto(cx, cy - r);
            p2 = criar_ponto(cx, cy + r);
        }
        
        Segmento* seg = criar_segmento((*id_base)++, p1, p2, (char*)cor_borda);
        if (seg) inserir_fim_lista(segmentos, seg);
        
    }
    else if (tipo == 'r') {
        // RETÂNGULO -> 4 segmentos
        double x, y, w, h;
        forma_get_retangulo_dados(forma, &x, &y, &w, &h);
        
        Ponto* p1 = criar_ponto(x, y);
        Ponto* p2 = criar_ponto(x + w, y);
        Segmento* s1 = criar_segmento((*id_base)++, p1, p2, (char*)cor_borda);
        if (s1) inserir_fim_lista(segmentos, s1);
        
        Ponto* p3 = criar_ponto(x + w, y);
        Ponto* p4 = criar_ponto(x + w, y + h);
        Segmento* s2 = criar_segmento((*id_base)++, p3, p4, (char*)cor_borda);
        if (s2) inserir_fim_lista(segmentos, s2);
        
        Ponto* p5 = criar_ponto(x + w, y + h);
        Ponto* p6 = criar_ponto(x, y + h);
        Segmento* s3 = criar_segmento((*id_base)++, p5, p6, (char*)cor_borda);
        if (s3) inserir_fim_lista(segmentos, s3);
        
        Ponto* p7 = criar_ponto(x, y + h);
        Ponto* p8 = criar_ponto(x, y);
        Segmento* s4 = criar_segmento((*id_base)++, p7, p8, (char*)cor_borda);
        if (s4) inserir_fim_lista(segmentos, s4);
        
    }
    else if (tipo == 'l') {
        // LINHA -> 1 segmento 
        double x1 = forma_get_x1(forma);
        double y1 = forma_get_y1(forma);
        double x2 = forma_get_x2(forma);
        double y2 = forma_get_y2(forma);
        
        Ponto* p1 = criar_ponto(x1, y1);
        Ponto* p2 = criar_ponto(x2, y2);
        
        Segmento* seg = criar_segmento((*id_base)++, p1, p2, (char*)cor_borda);
        if (seg) inserir_fim_lista(segmentos, seg);
        
    } 
    else if (tipo == 't') {
        // TEXTO -> 1 segmento horizontal
        double x = forma_get_x(forma);
        double y = forma_get_y(forma);
        const char* texto = forma_get_texto(forma);
        const char* ancora = forma_get_ancora(forma);
        
        int len = strlen(texto);
        double comprimento = 10.0 * len;
        
        double x1, x2;
        
        if (ancora[0] == 'i') {
            x1 = x;
            x2 = x + comprimento;
        } 
        else if (ancora[0] == 'f' || ancora[0] == 'e') {
            x1 = x - comprimento;
            x2 = x;
        } 
        else {
            x1 = x - comprimento / 2.0;
            x2 = x + comprimento / 2.0;
        }
        
        Ponto* p1 = criar_ponto(x1, y);
        Ponto* p2 = criar_ponto(x2, y);
        
        Segmento* seg = criar_segmento((*id_base)++, p1, p2, (char*)cor_borda);
        if (seg) inserir_fim_lista(segmentos, seg);
    }
    
    return segmentos;
}