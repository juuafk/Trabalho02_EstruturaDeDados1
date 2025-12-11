#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segmento.h"
#include "svg.h"
#include "formas.h" 
#include "lista.h" 
#include "geometria.h" 

// CRIAR_SVG 
FILE* criar_svg(char* nome_arquivo) {
    FILE* svg = fopen(nome_arquivo, "w");
    if (svg == NULL) return NULL;
    
    fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(svg, "<svg width=\"800\" height=\"600\" viewBox=\"0 0 800 600\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    
    return svg;
}

// FECHAR_SVG
void fechar_svg(FILE* svg) {
    if (svg == NULL) return;
    
    fprintf(svg, "</svg>\n");
    fclose(svg);
}

// DESENHAR_CIRCULO
static void desenhar_circulo(FILE* svg, Forma* f) {
    double cx = forma_get_x(f);
    double cy = forma_get_y(f);
    double r = forma_get_r(f);
    const char* cor_borda = forma_get_cor_borda(f);
    const char* cor_preench = forma_get_cor_preenchimento(f);
    
    fprintf(svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" ", cx, cy, r);
    fprintf(svg, "stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" />\n", cor_borda, cor_preench);
}

// DESENHAR_RETANGULO
static void desenhar_retangulo(FILE* svg, Forma* f) {
    double x = forma_get_x(f);
    double y = forma_get_y(f);
    double w = forma_get_w(f);
    double h = forma_get_h(f);
    const char* cor_borda = forma_get_cor_borda(f);
    const char* cor_preench = forma_get_cor_preenchimento(f);
    
    fprintf(svg, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" ", x, y, w, h);
    fprintf(svg, "stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" />\n", cor_borda, cor_preench);
}

// DESENHAR_LINHA
static void desenhar_linha(FILE* svg, Forma* f) {
    double x1 = forma_get_x1(f);
    double y1 = forma_get_y1(f);
    double x2 = forma_get_x2(f);
    double y2 = forma_get_y2(f);
    const char* cor = forma_get_cor_borda(f);
    
    fprintf(svg, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ", x1, y1, x2, y2);
    fprintf(svg, "stroke=\"%s\" stroke-width=\"2\" />\n", cor);
}

// DESENHAR_TEXTO
static void desenhar_texto(FILE* svg, Forma* f) {
    double x = forma_get_x(f);
    double y = forma_get_y(f);
    const char* texto = forma_get_texto(f);
    const char* ancora = forma_get_ancora(f);
    const char* cor_borda = forma_get_cor_borda(f);
    const char* cor_preench = forma_get_cor_preenchimento(f);
    
    const char* text_anchor = "start";
    if (ancora && ancora[0] == 'm') {
        text_anchor = "middle";
    } else if (ancora && (ancora[0] == 'f' || ancora[0] == 'e')) {
        text_anchor = "end";
    }
    
    fprintf(svg, "<text x=\"%.2f\" y=\"%.2f\" ", x, y);
    fprintf(svg, "text-anchor=\"%s\" ", text_anchor);
    fprintf(svg, "font-size=\"16\" ");
    fprintf(svg, "stroke=\"%s\" stroke-width=\"0.5\" fill=\"%s\">", cor_borda, cor_preench);
    fprintf(svg, "%s</text>\n", texto);
}

// DESENHAR_FORMAS
void desenhar_formas(FILE* svg, Lista* formas) {
    if (svg == NULL || formas == NULL) return;
    
    Elemento* elem = get_primeiro_elemento(formas);
    
    while (elem != NULL) {
        Forma* f = (Forma*) get_elemento(formas, elem);
        
        if (f != NULL) {
            char tipo = forma_get_tipo(f);
            
            switch (tipo) {
                case 'c':
                    desenhar_circulo(svg, f);
                    break;
                case 'r':
                    desenhar_retangulo(svg, f);
                    break;
                case 'l':
                    desenhar_linha(svg, f);
                    break;
                case 't':
                    desenhar_texto(svg, f);
                    break;
            }
        }
        
        elem = get_proximo_elemento(elem);
    }
}

// DESENHAR_POLIGONO
void desenhar_poligono(FILE* svg, Lista* vertices, char* corBorda, char* corPreench, double opacidade) {
    if (svg == NULL || vertices == NULL || lista_vazia(vertices)) return;
    
    fprintf(svg, "<polygon points=\"");
    
    Elemento* elem = get_primeiro_elemento(vertices);
    
    while (elem != NULL) {
        Ponto* p = (Ponto*) get_elemento(vertices, elem);
        
        if (p != NULL) {
            fprintf(svg, "%.2f,%.2f ", get_x(p), get_y(p));
        }
        
        elem = get_proximo_elemento(elem);
    }
    
    fprintf(svg, "\" ");
    fprintf(svg, "stroke=\"%s\" stroke-width=\"2\" ", corBorda ? corBorda : "#000000");
    fprintf(svg, "fill=\"%s\" ", corPreench ? corPreench : "none");
    fprintf(svg, "fill-opacity=\"%.2f\" />\n", opacidade);
}

// DESENHAR_SEGMENTO
void desenhar_segmento(FILE* svg, Segmento* s) { 
    if (!svg || !s) return;
    
    Ponto* ini = segmento_get_inicio(s); 
    Ponto* fim = segmento_get_fim(s); 
    
    if (!ini || !fim) return;
    
    double x1 = get_x(ini);
    double y1 = get_y(ini);
    double x2 = get_x(fim);
    double y2 = get_y(fim);
    
    const char* cor = segmento_get_cor(s); 
    
    fprintf(svg, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ", 
            x1, y1, x2, y2);
    fprintf(svg, "stroke=\"%s\" stroke-width=\"3\" opacity=\"0.7\" />\n", cor);
}

// DESENHAR_SEGMENTOS
void desenhar_segmentos(FILE* svg, Lista* segmentos) { 
    if (svg == NULL || segmentos == NULL) return;
    
    Elemento* elem = get_primeiro_elemento(segmentos);
    
    while (elem != NULL) {
        Segmento* s = (Segmento*) get_elemento(segmentos, elem); 
        
        if (s != NULL) {
            desenhar_segmento(svg, s);
        }
        
        elem = get_proximo_elemento(elem);
    }
}