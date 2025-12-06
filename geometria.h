#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdbool.h>

// ===========================================
// GEOMETRIA
// ------------------------------------------
// implementa operações geométricas básicas.
// ===========================================

// ESTRUTURA DO PONTO NO PLANO CARTESIANO
typedef struct Ponto Ponto;

// --------------------------------
// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO
// --------------------------------

// -> criarPonto
// função: criar um ponto
// recebe: coordenadas (x, y)
// retorna: um ponto com as coordenadas (x, y)
Ponto* criarPonto(double x, double y);

// -> destruirPonto
// função: libera a memória alocada para um ponto
// recebe: o ponto que vai ser destruído
void destruirPonto(Ponto* p);

// ---------------
// FUNÇÕES GETTER
// ---------------

// -> get_x
// função: descobrir o x de uma coordenada
// recebe: ponteiro para o ponto
// retorna: x
double get_x(Ponto* p);

// -> get_y
// função: descobrir o y de uma coordenada
// recebe: ponteiro para o ponto
// retorna: y
double get_y(Ponto* p);

// ------------------------------
// OPERAÇÕES MATEMÁTICAS - pontos
// ------------------------------

// -> distanciaPontos
// função: calcula a distância entre dois pontos
// recebe: dois pontos 
// retorna: distância entre os pontos
double distanciaPontos(Ponto* p1, Ponto* p2);

// -> anguloEntrePontos
// função: calcula o ângulo entre dois pontos
// recebe: dois pontos
// retorna: o ângulo (em radianos) entre os pontos
double anguloEntrePontos(Ponto* origem, Ponto* final);

// -> determinante
// função: calcula o determinante para 3 pontos
// recebe: três pontos
// retorna: determinante
double determinante(Ponto* p1, Ponto* p2, Ponto* p3);

// -> orientação
// função: calcula a orientação de 3 pontos, usada para verificar intersecção de segmentos
// recebe: três pontos
// retorna: a posição de p3 em relação aos outros dois pontos
double orientacao(Ponto* p1, Ponto* p2, Ponto* p3);

// ---------------------------------
// OPERAÇÕES MATEMÁTICAS - segmentos
// ---------------------------------

// -> segmentosIntersectam
// função: verificar se dois segmentos se intersectam
// recebe: ponto inicial e final de cada segmento
// retorna: verdadeiro para caso se intersectarem ou falso caso não
bool segmentosIntersectam(Ponto* p1, Ponto* p2, Ponto* q1, Ponto* q2);

// -> intersecaoSegmentos
// função: descobrir qual o ponto em que os segmentos se intersectam
// recebe: ponto inicial e final de cada segmento
// retorna: o ponto de intersecção
Ponto* intersecaoSegmentos(Ponto* p1, Ponto* p2, Ponto* q1, Ponto* q2);

// -> distanciaPontoSegmento
// função: descobrir a distância entre um ponto e um segmento
// recebe: o ponto, e os pontos inicial e final do segmento
// retorna: distância
double distanciaPontoSegmento(Ponto* p, Ponto* s1, Ponto* s2);

// -> pontoNoSegmento
// função: descobrir se um ponto está no segmento
// recebe: o ponto, e os pontos inicial e final do segmento
// retorna: verdadeiro para caso estiver ou falso caso não
bool pontoNoSegmento(Ponto* p, Ponto* s1, Ponto* s2);

// ---------------------------------
// OPERAÇÕES MATEMÁTICAS - polígonos
// ---------------------------------

// -> pontoEmPoligono
// função: descobrir se um ponto está no polígono
// recebe: o ponto, array de vértices do polígono e o número de vértices
// retorna: verdadeiro se estiver ou falso caso não
bool pontoEmPoligono(Ponto* p, Ponto** vertices, int n);

// -> segmentoIntersectaPoligono
// função: descobrir se um segmento intersecta um polígono
// recebe: os pontos inicial e final do segmento, array de vértices do polígono e o número de vértices
// retorna: verdadeiro se intersectar ou falso caso não
bool segmentoIntersectaPoligono(Ponto* p1, Ponto* p2, Ponto** vertices, int n);

#endif