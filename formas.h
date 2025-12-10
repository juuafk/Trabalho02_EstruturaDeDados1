#ifndef FORMAS_H
#define FORMAS_H

#include <stdio.h> 
#include <stdbool.h>

// FORMAS UTILIZADAS NO PROJETO
// ====================================================
// Serão utilizadas no projeto 4 formas:
// - Retângulos
// - Círculos
// - Linhas
// - Textos
// Todas possuem um id, cor de preenchimento 
// e de borda, menos a linha que possui apenas uma cor.
// ====================================================

// DECLARAÇÃO DA ESTRUTURA DA FORMA
typedef struct forma Forma;

// função auxiliar DUPLICAR_STRING
static char* duplicar_string(const char* s);

// =================================
// FUNÇÕES DE CRIAÇÃO (CONSTRUTORES)
// =================================

// RETÂNGULO 
// --------------------------------------------------------------
// Um retângulo é uma figura geométrica plana com quatro lados, 
// em que os lados opostos são paralelos e iguais em comprimento,
// e todos os quatro ângulos internos são retos.
// (POSSUI NO PROJETO: id, âncora (um dos pontos), largura, altura, 
// cor de borda, cor de preenchimento).
// --------------------------------------------------------------

// CRIAR_RETANGULO
// função que cria um retângulo
Forma* criar_retangulo(int id, double x, double y, double largura, double altura, 
const char* cor_borda, const char* cor_preenchimento);

// CÍRCULO
// ------------------------------------------------------------------
// Um círculo é uma figura geométrica composta pelo conjunto 
// de todos os pontos em um plano que estão a uma distância fixa (raio) 
// de um ponto central.
// (POSSUI NO PROJETO: id, âncora (centro), raio, cor de borda,
// cor de preenchimento).
// ------------------------------------------------------------------

// CRIAR_CIRCULO
// função que cria um círculo
Forma* criar_circulo(int id, double x, double y, double raio, 
const char* cor_borda, const char* cor_preenchimento);

// LINHA
// -------------------------------------------------------------------------------
// A linha, nesse projeto, adere a definicão de um segmento de reta, estabelecido
// como: uma parte finita de uma reta definida por dois pontos extremos,
// que são seus início e fim.
// Além disso, para sua implementação no trabalho, a linha também terá área,
// estipulada como: 2 * comprimento.
// (POSSUI NO PROJETO: id, duas âncoras (ínicio e fim), cor).
// -------------------------------------------------------------------------------

// CRIAR_LINHA
// função que cria uma linha
Forma* criar_linha(int id, double x1, double y1, double x2, double y2, 
const char* cor);

// TEXTO
// ---------------------------------------------------------------------------
// Um texto é um conjunto de letras, que podem formar palavras. No contexto
// do projeto ele assume uma área, definida como: 20 * quantidade de caracteres.
// (POSSUI NO PROJETO: id, âncora (como o texto ficará alinhado em relação à
// coordenada), coordenada, conteúdo da mensagem, cor de borda, cor de
// preenchimento).
// ---------------------------------------------------------------------------

// CRIAR_TEXTO
// função que cria um texto
Forma* criar_texto(int id, double x, double y, const char* texto_conteudo, const char* ancora,
const char* cor_borda, const char* cor_preenchimento);

// ===============
// FUNÇÕES GETTER
// ===============

// FORMA_GET_ID
// retorna o ID da forma
int forma_get_id(Forma* forma);

// FORMA_GET_TIPO
// retorna o tipo da forma:
// 'r','c', 'l', 't'
// (retângulo, círculo, linha, texto)
char forma_get_tipo(Forma* forma);

// FORMA_GET_X
// retorna a coordenada x da âncora principal da forma
double forma_get_x(Forma* f);

// FORMA_GET_Y
// retorna a coordenada y da âncora principal da forma
double forma_get_y(Forma* f);

// FORMA_GET_POSICAO
// retorna a posição atual da forma (x, y)
void forma_get_posicao(Forma* f, double* x, double* y);

// FORMA_GET_VETOR
// retorna o vetor de deslocamento da forma (dx, dy)
void forma_get_vetor(Forma* f, double* dx, double* dy);

// FORMA_GET_ESMAGADA
// retorna se a forma está esmagada ou não
int forma_get_esmagada(Forma* f);

// FORMA_GET_COR_BORDA
// retorna a cor da borda de uma forma
const char* forma_get_cor_borda(Forma* f);

// FORMA_GET_COR_PREENCHIMENTO
// retorna a cor de preenchimento de uma forma
const char* forma_get_cor_preenchimento(Forma* f);

// GET_COR_GANHADORA
const char* get_cor_ganhadora(Forma* f_I, char* temp_complementar_buffer);

// FORMA_GET_R
// retorna o raio do circulo
double forma_get_r(Forma* f);

// FORMA_GET_W
// retorna a largura do retangulo
double forma_get_w(Forma* f);

// FORMA_GET_H
// retorna a altura do retangulo
double forma_get_h(Forma* f);

// FORMA_GET_X1
// retorna o x1 da linha
double forma_get_x1(Forma* f);

// FORMA_GET_Y1
// retorna o y1 da linha
double forma_get_y1(Forma* f);

// FORMA_GET_X2
// retorna o x2 da linha
double forma_get_x2(Forma* f);

// FORMA_GET_Y2
// retorna o y2 da linha
double forma_get_y2(Forma* f);

// FORMA_GET_TEXTO
// retorna o conteudo de texto da forma
const char* forma_get_texto(Forma* f);

// FORMA_GET_ANCORA
// retorna o tipo de ancora do texto
const char* forma_get_ancora(Forma* f);


// FORMA_GET_RETANGULO_DADOS
// verifica os dados de um retângulo
void forma_get_retangulo_dados(Forma* f, double* x, double* y, double* largura, double* altura);

// FORMA_GET_CIRCULO_DADOS
// verifica os dados de um círculo
void forma_get_circulo_dados(Forma* f, double* x, double* y, double* raio);

// FORMA_GET_LINHA_DADOS
// verifica os dados de uma linha
void forma_get_linha_dados(Forma* f, double* x1, double* y1, double* x2, double* y2);

// FORMA_GET_TEXTO_DADOS
// verifica os dados de um texto 
void forma_get_texto_dados(Forma* f, double* x, double* y, char* ancora, const char** texto);

// FORMA_GET_TEXTO_BBOX
// consegue o retângulo correspondente ao texto   
void forma_get_texto_bbox(Forma* f, double* x, double* y, double* largura, double* altura);

// COR_COMPLEMENTAR
void cor_complementar(const char* cor_hex, char* complementar_hex);

// =============================
// FUNÇÕES DE LÓGICA E CÁLCULO
// =============================

// FORMA_CALCULAR_AREA
// calcula a área da forma 
double forma_calcular_area(Forma* forma);

// ==========================================
// FUNÇÕES GERAIS PARA MANIPULAÇÃO DE FORMAS
// ==========================================

// DESTRUIR_FORMA 
// libera a memória alocada para uma forma 
void destruir_forma(Forma* forma);

// FORMA_CLONAR
// cria uma cópia de uma forma com novo ID
Forma* forma_clonar(Forma* f, int nova_id);

// FORMA_MOVER
// move uma forma de lugar
void forma_mover(Forma* f, double dx, double dy);

// FORMA_SET_POSICAO
// define a nova posição da forma (x, y)
void forma_set_posicao(Forma* f, double novo_x, double novo_y);

// FORMA_SET_VETOR
// define o vetor de deslocamento da forma (dx, dy)
void forma_set_vetor(Forma* f, double dx, double dy);

// FORMA_SET_ESMAGADA
// esmaga uma forma
void forma_set_esmagada(Forma* f, int valor);

// FORMA_SET_COR_BORDA
// define uma nova cor de borda para a forma
void forma_set_cor_borda(Forma* f, const char* nova_cor);

// FORMA_SET_COR_PREENCHIMENTO
// define uma nova cor de preenchimento para a forma
void forma_set_cor_preenchimento(Forma* f, const char* nova_cor);

// FORMA_TROCAR_CORES
// troca as cores de borda e preenchimento de uma forma
void forma_trocar_cores(Forma* f);

// ================================================
// FUNÇÃO PRINCIPAL DE SOBREPOSIÇÃO (DESPACHANTE)
// ================================================

// FORMAS_SOBREPOEM
// verifica se duas formas se sobrepõem
int formas_sobrepoem(Forma* forma1, Forma* forma2);

#endif