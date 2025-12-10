#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geometria.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define EPSILON 1e-9

// ESTRUTURA DO PONTO
struct Ponto {
    double x;
    double y;
};

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// CRIAR_PONTO
Ponto* criar_ponto(double x, double y) {
    Ponto* p = (Ponto*) malloc(sizeof(Ponto));
    if (p == NULL) return NULL;
    p->x = x;
    p->y = y;
    return p;
}

// DESTRUIR_PONTO
void destruir_ponto(Ponto* p) {
    if (p != NULL) {
        free(p);
    }
}

// ---------------
// FUNÇÕES GETTER
// ---------------

// GET_X
double get_x(Ponto* p) {
    return p ? p->x : 0.0;
}

// GET_Y
double get_y(Ponto* p) {
    return p ? p->y : 0.0;
}

// ------------------------------
// OPERAÇÕES MATEMÁTICAS - pontos
// ------------------------------

// DISTANCIA_PONTOS
double distancia_pontos(Ponto* p1, Ponto* p2) {
    if (!p1 || !p2) return 0.0;
    double dx = p2->x - p1->x;
    double dy = p2->y - p1->y;
    return sqrt(dx * dx + dy * dy);
}

// ANGULO_ENTRE_PONTOS
double angulo_entre_pontos(Ponto* origem, Ponto* final) {
    if (!origem || !final) return 0.0;
    double dx = final->x - origem->x;
    double dy = final->y - origem->y;
    return atan2(dy, dx);
}

// DETERMINANTE
double determinante(Ponto* p1, Ponto* p2, Ponto* p3) {
    if (!p1 || !p2 || !p3) return 0.0;
    
    return (p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x);
}

// ORIENTACAO
double orientacao(Ponto* p1, Ponto* p2, Ponto* p3) {
    return determinante(p1, p2, p3);
}

// ---------------------------------
// OPERAÇÕES MATEMÁTICAS - segmentos
// ---------------------------------

// SEGMENTOS_INTERSECTAM
bool segmentos_intersectam(Ponto* p1, Ponto* p2, Ponto* q1, Ponto* q2) {
    if (!p1 || !p2 || !q1 || !q2) return false;
    
    double d1 = orientacao(q1, q2, p1);
    double d2 = orientacao(q1, q2, p2);
    double d3 = orientacao(p1, p2, q1);
    double d4 = orientacao(p1, p2, q2);
    
    if (((d1 > EPSILON && d2 < -EPSILON) || (d1 < -EPSILON && d2 > EPSILON)) &&
        ((d3 > EPSILON && d4 < -EPSILON) || (d3 < -EPSILON && d4 > EPSILON))) {
        return true;
    }
    
    // colineares
    if (fabs(d1) < EPSILON && ponto_no_segmento(p1, q1, q2)) return true;
    if (fabs(d2) < EPSILON && ponto_no_segmento(p2, q1, q2)) return true;
    if (fabs(d3) < EPSILON && ponto_no_segmento(q1, p1, p2)) return true;
    if (fabs(d4) < EPSILON && ponto_no_segmento(q2, p1, p2)) return true;
    
    return false;
}

// INTERSECAO_SEGMENTOS
Ponto* intersecao_segmentos(Ponto* p1, Ponto* p2, Ponto* q1, Ponto* q2) {
    if (!p1 || !p2 || !q1 || !q2) return NULL;
    
    double x1 = p1->x, y1 = p1->y;
    double x2 = p2->x, y2 = p2->y;
    double x3 = q1->x, y3 = q1->y;
    double x4 = q2->x, y4 = q2->y;
    
    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    if (fabs(denom) < EPSILON) return NULL;
    
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    
    double ix = x1 + t * (x2 - x1);
    double iy = y1 + t * (y2 - y1);
    
    return criar_ponto(ix, iy);
}

// DISTANCIA_PONTO_SEGMENTO
double distancia_ponto_segmento(Ponto* p, Ponto* s1, Ponto* s2) {
    if (!p || !s1 || !s2) return 0.0;
    
    double dx = s2->x - s1->x;
    double dy = s2->y - s1->y;
    double len_sq = dx * dx + dy * dy;
    
    if (len_sq < EPSILON) {
        return distancia_pontos(p, s1);
    }
    
    double t = ((p->x - s1->x) * dx + (p->y - s1->y) * dy) / len_sq;
    
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    
    double proj_x = s1->x + t * dx;
    double proj_y = s1->y + t * dy;
    
    double dist_x = p->x - proj_x;
    double dist_y = p->y - proj_y;
    
    return sqrt(dist_x * dist_x + dist_y * dist_y);
}

// PONTO_NO_SEGMENTO
bool ponto_no_segmento(Ponto* p, Ponto* s1, Ponto* s2) {
    if (!p || !s1 || !s2) return false;
    
    double min_x = fmin(s1->x, s2->x);
    double max_x = fmax(s1->x, s2->x);
    double min_y = fmin(s1->y, s2->y);
    double max_y = fmax(s1->y, s2->y);
    
    return (p->x >= min_x - EPSILON && p->x <= max_x + EPSILON && p->y >= min_y - EPSILON && p->y <= max_y + EPSILON &&
    fabs(orientacao(s1, s2, p)) < EPSILON);
}

// ---------------------------------
// OPERAÇÕES MATEMÁTICAS - polígonos
// ---------------------------------

// PONTO_EM_POLIGONO
bool ponto_em_poligono(Ponto* p, Ponto** vertices, int n) {
    if (!p || !vertices || n < 3) return false;
    
    bool dentro = false;
    Ponto* p1 = vertices[0];
    
    for (int i = 1; i <= n; i++) {
        Ponto* p2 = vertices[i % n];
        
        if (p->y > fmin(p1->y, p2->y)) {
            if (p->y <= fmax(p1->y, p2->y)) {
                if (p->x <= fmax(p1->x, p2->x)) {
                    double x_intersecao;
                    
                    if (fabs(p1->y - p2->y) > EPSILON) {
                        x_intersecao = (p->y - p1->y) * (p2->x - p1->x) / (p2->y - p1->y) + p1->x;
                    } 
                    else {
                        x_intersecao = p1->x;
                    }
                    
                    if (fabs(p1->x - p2->x) < EPSILON || p->x <= x_intersecao) {
                        dentro = !dentro;
                    }
                }
            }
        }
        p1 = p2;
    }
    
    return dentro;
}

// SEGMENTO_INTERSECTA_POLIGONO
bool segmento_intersecta_poligono(Ponto* p1, Ponto* p2, Ponto** vertices, int n) {
    if (!p1 || !p2 || !vertices || n < 3) return false;
    
    if (ponto_em_poligono(p1, vertices, n) || ponto_em_poligono(p2, vertices, n)) {
        return true;
    }
    
    for (int i = 0; i < n; i++) {
        if (segmentos_intersectam(p1, p2, vertices[i], vertices[(i + 1) % n])) {
            return true;
        }
    }
    
    return false;
}