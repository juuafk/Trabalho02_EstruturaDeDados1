#ifndef SVG_H
#define SVG_H

#include <stdio.h>

#include "segmento.h"
#include "lista.h"
#include "formas.h"
#include "geometria.h"

// ===========================================
// SVG
// ------------------------------------------
// gera arquivos svg (a saída final).
// ===========================================

// ---------------------------------------------------
//             GERAÇÃO DE ARQUIVO SVG
// ---------------------------------------------------

/* -> criar_svg
    FUNÇÃO: criar arquivo SVG e escrever o cabeçalho
    RECEBE: o nome do arquivo
    RETORNA: ponteiro para o arquivo aberto
 */
FILE* criar_svg(char* nome_arquivo);

/* -> fechar_svg
    FUNÇÃO: fechar arquivo SVG (escreve tag de fechamento)
    RECEBE: o arquivo
 */
void fechar_svg(FILE* svg);

// -----------------------------------------
//             DESENHO DE FORMAS
// -----------------------------------------

/* -> desenhar_formas
    FUNÇÃO: desenhar todas as formas de uma lista no SVG
    RECEBE: o arquivo e a lista de formas
 */
void desenhar_formas(FILE* svg, Lista* formas);

/* -> desenhar_poligono
    FUNÇÃO: desenhar um polígono no SVG 
    RECEBE: o arquivo, lista de vértices do polígono, cor da borda e de preenchimento e opacidade
 */
void desenhar_poligono(FILE* svg, Lista* vertices, char* corBorda, char* corPreench, double opacidade);

/* -> desenhar_segmento
    FUNÇÃO: desenhar segmento no SVG
    RECEBE: o arquivo e o segmento
*/
void desenhar_segmento(FILE* svg, Segmento* s);

/* -> desenhar_segmentos
    FUNÇÃO: desenhar mais de um segmento no SVG
    RECEBE: o arquivo e a lista de segmentos
*/
void desenhar_segmentos(FILE* svg, Lista* segmentos);

#endif 