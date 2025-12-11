#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>

#include "formas.h"

#define MAX_COR_LEN 8
#define MAX_TEXTO_LEN 50
#define MAX_ANCORA_LEN 10 

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// FUNCAO AUXILIAR SUBSTITITUTA DE STRDUP
static char* duplicar_string(const char* s) {
    if (s == NULL) {
        return NULL;
    }

    size_t len = strlen(s) + 1;
    char* nova_str = (char*) malloc(len);
    if (nova_str == NULL) {
        return NULL; 
    }

    memcpy(nova_str, s, len); 
    return nova_str;
}

// DEFINIÇÃO DA STRUCT FORMA
struct forma {
    int id;
    char tipo;
    double dx;
    double dy;

    union {
    // RETANGULO
    struct {double x, y, largura, altura; } r;

    // CIRCULO
    struct {double x, y, raio; } c;

    // LINHA
    struct {double x1, y1, x2, y2; } l;

    // TEXTO
    struct {char ancora_tipo[MAX_ANCORA_LEN]; double x, y; char* texto_ptr; } t;
    } dados;

    char cor_borda[MAX_COR_LEN];
    char cor_preenchimento[MAX_COR_LEN];
    int esmagada;
};

// INICIALIZAR_FORMA_COMUM
static void inicializar_forma_comum(Forma* f, const char* cor_borda, const char* cor_preenchimento) {
    f->dx = 0.0;
    f->dy = 0.0;
    
    strncpy(f->cor_borda, cor_borda, MAX_COR_LEN - 1); 
    f->cor_borda[MAX_COR_LEN - 1] = '\0'; 

    strncpy(f->cor_preenchimento, cor_preenchimento, MAX_COR_LEN - 1);
    f->cor_preenchimento[MAX_COR_LEN - 1] = '\0';

    f->esmagada = 0;
}

// =================================
// FUNÇÕES DE CRIAÇÃO (CONSTRUTORES)
// =================================

// CRIAR_RETÂNGULO
Forma* criar_retangulo(int id, double x, double y, double largura, double altura, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    if (f == NULL) return NULL; 

    f->id = id;
    f->tipo = 'r';
    f->dados.r.x = x;
    f->dados.r.y = y;
    f->dados.r.largura = largura;
    f->dados.r.altura = altura;
    
    inicializar_forma_comum(f, cor_borda, cor_preenchimento);
    return f;
}


// CRIAR_CÍRCULO
Forma* criar_circulo(int id, double x, double y, double raio, const char* cor_borda, const char* cor_preenchimento) {
    Forma* f = (Forma*) malloc(sizeof(Forma));
    if (f == NULL) return NULL; 

    f->id = id;
    f->tipo = 'c';
    f->dados.c.x = x;
    f->dados.c.y = y;
    f->dados.c.raio = raio;

    inicializar_forma_comum(f, cor_borda, cor_preenchimento);
    return f;
}

// CRIAR_LINHA
Forma* criar_linha(int id, double x1, double y1, double x2, double y2, const char* cor) { 
    
    Forma* f = (Forma*) malloc(sizeof(Forma));
    if (f == NULL) return NULL;

    f->id = id;
    f->tipo = 'l';
    
    f->dados.l.x1 = x1;
    f->dados.l.y1 = y1;
    f->dados.l.x2 = x2;
    f->dados.l.y2 = y2;
    
    inicializar_forma_comum(f, cor, "#FFFFFF"); 
    return f;
}

// CRIAR_TEXTO
Forma* criar_texto(int id, double x, double y, const char* texto_conteudo, const char* ancora,
                   const char* cor_borda, const char* cor_preenchimento) {
    
    Forma* f = (Forma*) malloc(sizeof(Forma));
    if (f == NULL) return NULL;

    f->id = id;
    f->tipo = 't'; 
    
    f->dados.t.x = x;
    f->dados.t.y = y;

    f->dados.t.texto_ptr = duplicar_string(texto_conteudo); 

    strncpy(f->dados.t.ancora_tipo, ancora, MAX_ANCORA_LEN - 1); 
    f->dados.t.ancora_tipo[MAX_ANCORA_LEN - 1] = '\0';

    inicializar_forma_comum(f, cor_borda, cor_preenchimento);
    return f;
}

// ================
// FUNÇÕES GETTER
// ================

// FORMA_GET_ID
int forma_get_id(Forma* forma) { return forma->id; }

// FORMA_GET_TIPO
char forma_get_tipo(Forma* forma) { return forma->tipo; }

// FORMA_GET_X 
double forma_get_x(Forma* f) {
    if (f == NULL) return 0;
    switch (f->tipo) {
        case 'c': return f->dados.c.x;
        case 'r': return f->dados.r.x;
        case 't': return f->dados.t.x; 
        case 'l': return f->dados.l.x1; 
    }
    return 0;
}

// FORMA_GET_Y 
double forma_get_y(Forma* f) {
    if (f == NULL) return 0;
    switch (f->tipo) {
        case 'c': return f->dados.c.y;
        case 'r': return f->dados.r.y;
        case 't': return f->dados.t.y; 
        case 'l': return f->dados.l.y1; 
    }
    return 0;
}

// FORMA_GET_POSICAO
void forma_get_posicao(Forma* f, double* x, double* y) {
    if (f == NULL) { *x = 0; *y = 0; return; }
    
    switch (f->tipo) {
        case 'c': *x = f->dados.c.x; *y = f->dados.c.y; break;
        case 'r': *x = f->dados.r.x; *y = f->dados.r.y; break;
        case 'l': *x = f->dados.l.x1; *y = f->dados.l.y1; break;
        case 't': *x = f->dados.t.x; *y = f->dados.t.y; break;
        default: *x = 0; *y = 0;
    }
}

// FORMA_GET_VETOR
void forma_get_vetor(Forma* f, double* dx, double* dy) {
    if (f) { *dx = f->dx; *dy = f->dy; } else { *dx = 0; *dy = 0; }
}

// FORMA_GET_ESMAGADA
int forma_get_esmagada(Forma* f) { return f ? f->esmagada : 0; }

// FORMA_GET_COR_BORDA
const char* forma_get_cor_borda(Forma* f) { return f->cor_borda; }

// FORMA_GET_COR_PREENCHIMENTO
const char* forma_get_cor_preenchimento(Forma* f) { return f->cor_preenchimento; }

// GET_COR_GANHADORA
const char* get_cor_ganhadora(Forma* f_I, char* temp_complementar_buffer) {
    if (forma_get_tipo(f_I) == 'l') {
        cor_complementar(forma_get_cor_borda(f_I), temp_complementar_buffer);
        return temp_complementar_buffer;
    } else {
        return forma_get_cor_preenchimento(f_I);
    }
}

// FORMA_GET_R (para circulo)
double forma_get_r(Forma* f) { 
    if (f == NULL || f->tipo != 'c') return 0;
    return f->dados.c.raio;
}

// FORMA_GET_W (para retangulo)
double forma_get_w(Forma* f) {
    if (f == NULL || f->tipo != 'r') return 0;
    return f->dados.r.largura;
}

// FORMA_GET_H (para retangulo)
double forma_get_h(Forma* f) {
    if (f == NULL || f->tipo != 'r') return 0;
    return f->dados.r.altura;
}

// FORMA_GET_X1 (para linha)
double forma_get_x1(Forma* f) {
    if (f == NULL || f->tipo != 'l') return 0;
    return f->dados.l.x1;
}

// FORMA_GET_Y1 (para linha)
double forma_get_y1(Forma* f) {
    if (f == NULL || f->tipo != 'l') return 0;
    return f->dados.l.y1;
}

// FORMA_GET_X2 (para linha)
double forma_get_x2(Forma* f) {
    if (f == NULL || f->tipo != 'l') return 0;
    return f->dados.l.x2;
}

// FORMA_GET_Y2 (para linha)
double forma_get_y2(Forma* f) {
    if (f == NULL || f->tipo != 'l') return 0;
    return f->dados.l.y2;
}

// FORMA_GET_TEXTO 
const char* forma_get_texto(Forma* f) {
    if (f == NULL || f->tipo != 't' || f->dados.t.texto_ptr == NULL) return "";
    return f->dados.t.texto_ptr;
}

// FORMA_GET_ANCORA
const char* forma_get_ancora(Forma* f) {
    if (f == NULL || f->tipo != 't') return "start";
    return f->dados.t.ancora_tipo;
}

// FORMA_GET_RETANGULO_DADOS
void forma_get_retangulo_dados(Forma* f, double* x, double* y, double* largura, double* altura) {
    if (f && f->tipo == 'r') {
        *x = f->dados.r.x; *y = f->dados.r.y; *largura = f->dados.r.largura; *altura = f->dados.r.altura;
    }
}

// FORMA_GET_CIRCULO_DADOS
void forma_get_circulo_dados(Forma* f, double* x, double* y, double* raio) {
    if (f && f->tipo == 'c') {
        *x = f->dados.c.x; *y = f->dados.c.y; *raio = f->dados.c.raio;
    }
}

// FORMA_GET_LINHA_DADOS
void forma_get_linha_dados(Forma* f, double* x1, double* y1, double* x2, double* y2) {
    if (f && f->tipo == 'l') {
        *x1 = f->dados.l.x1; *y1 = f->dados.l.y1; *x2 = f->dados.l.x2; *y2 = f->dados.l.y2;
    }
}

// FORMA_GET_TEXTO_DADOS 
void forma_get_texto_dados(Forma* f, double* x, double* y, char* ancora_char, const char** texto) {
    if (f && f->tipo == 't') { 
        *x = f->dados.t.x;
        *y = f->dados.t.y;
        *ancora_char = f->dados.t.ancora_tipo[0]; 
        *texto = f->dados.t.texto_ptr;
    }
}

// FORMA_GET_TEXTO_BBOX 
void forma_get_texto_bbox(Forma* f, double* x, double* y, double* largura, double* altura) {
    if (f == NULL || f->tipo != 't') {
        *x = *y = *largura = *altura = 0.0;
        return;
    }

    *altura = 10.0; 
    
    double largura_caractere_media = 6.0; 
    *largura = largura_caractere_media * strlen(f->dados.t.texto_ptr);

    *x = f->dados.t.x; 
    *y = f->dados.t.y;
        
    *y = *y - *altura; 

    char ancora = f->dados.t.ancora_tipo[0];
    
    if (ancora == 'm') {
        *x = *x - (*largura / 2.0);
    } else if (ancora == 'e' || ancora == 'f') {
        *x = *x - *largura;
    }
}

// COR_COMPLEMENTAR
void cor_complementar(const char* cor_hex, char* complementar_hex) {
    unsigned int r, g, b;
    if (sscanf(cor_hex + 1, "%2x%2x%2x", &r, &g, &b) != 3) {
        strcpy(complementar_hex, cor_hex);
        return;
    }
    
    r = 0xFF - r; 
    g = 0xFF - g;
    b = 0xFF - b;
    
    sprintf(complementar_hex, "#%02X%02X%02X", r, g, b);
}

// FUNÇÕES DE LÓGICA E CÁLCULO
// FORMA_CALCULAR_ÁREA 
double forma_calcular_area(Forma* forma) {
    if (forma == NULL) return 0.0;

    switch (forma->tipo) {
        case 'r':
            return forma->dados.r.largura * forma->dados.r.altura;
        case 'c':
            return M_PI * forma->dados.c.raio * forma->dados.c.raio;
        case 'l': {
            double dx = forma->dados.l.x2 - forma->dados.l.x1;
            double dy = forma->dados.l.y2 - forma->dados.l.y1;
            double comprimento = sqrt(dx * dx + dy * dy);
            return 2 * comprimento;
        }
        case 't': { 
            return 20.0 * strlen(forma->dados.t.texto_ptr);
        }
        default:
            return 0.0;
    }
}

// FUNÇÕES GERAIS PARA MANIPULAÇÃO DE FORMAS
// DESTRUIR_FORMA 
void destruir_forma(Forma* forma) {
    if (forma == NULL) return;

    if (forma->tipo == 't' && forma->dados.t.texto_ptr != NULL) {
        free(forma->dados.t.texto_ptr);
    }

    free(forma);
}

// FORMA_CLONAR
Forma* forma_clonar(Forma* f, int nova_id) {
    if (f == NULL) return NULL;

    Forma* clone = NULL;

    switch(f->tipo) {
        case 'c':
            clone = criar_circulo(nova_id, f->dados.c.x, f->dados.c.y, f->dados.c.raio, f->cor_borda, f->cor_preenchimento);
            break;
        case 'r':
            clone = criar_retangulo(nova_id, f->dados.r.x, f->dados.r.y, f->dados.r.largura, f->dados.r.altura, f->cor_borda, f->cor_preenchimento);
            break;
        case 'l':
            clone = criar_linha(nova_id, 
                f->dados.l.x1, f->dados.l.y1, 
                f->dados.l.x2, f->dados.l.y2, 
                f->cor_borda); 
            break;
        case 't':
            clone = criar_texto(nova_id, 
                f->dados.t.x, f->dados.t.y, 
                f->dados.t.texto_ptr, f->dados.t.ancora_tipo, 
                f->cor_borda, f->cor_preenchimento);
            break;
    }

    if (clone != NULL) {
        clone->dx = f->dx; clone->dy = f->dy;
        clone->esmagada = f->esmagada;
    }
    
    return clone;
}

// FORMA_MOVER 
void forma_mover(Forma* f, double dx, double dy) {
    if (f == NULL) return;

    switch (f->tipo) {
        case 'c': f->dados.c.x += dx; f->dados.c.y += dy; break;
        case 'r': f->dados.r.x += dx; f->dados.r.y += dy; break;
        case 'l':
            f->dados.l.x1 += dx; f->dados.l.y1 += dy;
            f->dados.l.x2 += dx; f->dados.l.y2 += dy; break;
        case 't': f->dados.t.x += dx; f->dados.t.y += dy; break; 
    }
}

// FORMA_SET_POSICAO 
void forma_set_posicao(Forma* f, double x, double y) {
    if (f == NULL) return;

    switch (f->tipo) {
        case 'c': f->dados.c.x = x; f->dados.c.y = y; break;
        case 'r': f->dados.r.x = x; f->dados.r.y = y; break;
        case 'l': {
            double dx = f->dados.l.x2 - f->dados.l.x1;
            double dy = f->dados.l.y2 - f->dados.l.y1;
            f->dados.l.x1 = x; f->dados.l.y1 = y;
            f->dados.l.x2 = x + dx; f->dados.l.y2 = y + dy;
            break;
        }
        case 't': f->dados.t.x = x; f->dados.t.y = y; break;
    }
}

// FORMA_SET_VETOR
void forma_set_vetor(Forma* f, double dx, double dy) { if (f) { f->dx = dx; f->dy = dy; } }

// FORMA_SET_ESMAGADA
void forma_set_esmagada(Forma* f, int valor) { if (f) f->esmagada = valor; }

// FORMA_SET_COR_BORDA 
void forma_set_cor_borda(Forma* f, const char* nova_cor) {
    if (f == NULL) return;
    strncpy(f->cor_borda, nova_cor, MAX_COR_LEN - 1);
    f->cor_borda[MAX_COR_LEN - 1] = '\0';
} 

// FORMA_SET_COR_PREENCHIMENTO
void forma_set_cor_preenchimento(Forma* f, const char* nova_cor) {
    if (f == NULL) return;
    strncpy(f->cor_preenchimento, nova_cor, MAX_COR_LEN - 1);
    f->cor_preenchimento[MAX_COR_LEN - 1] = '\0';
}

// FORMA_TROCAR_CORES
void forma_trocar_cores(Forma* forma) {
    if (forma == NULL) return;
    
    const char* cor_atual_preenchimento = forma_get_cor_preenchimento(forma);
    const char* cor_atual_borda = forma_get_cor_borda(forma);
    
    char temp_cor_preenchimento[MAX_COR_LEN]; 
    char temp_cor_borda[MAX_COR_LEN];

    strcpy(temp_cor_preenchimento, cor_atual_preenchimento);
    strcpy(temp_cor_borda, cor_atual_borda);

    forma_set_cor_preenchimento(forma, temp_cor_borda);
    forma_set_cor_borda(forma, temp_cor_preenchimento);
    
}

// =======================================================
// FUNÇÕES AUXILIARES ESTÁTICAS PARA DETECÇÃO DE COLISÃO
// =======================================================

// CLAMP
static double clamp(double valor, double min, double max) {
    if (valor < min) return min;
    if (valor > max) return max;
    return valor;
}

// PONTO_NO_CIRCULO
static int ponto_no_circulo(double px, double py, double cx, double cy, double raio) {
    double dx = px - cx;
    double dy = py - cy;
    return (dx * dx + dy * dy) < (raio * raio);
}

// PONTO_NO_RETANGULO
static int ponto_no_retangulo(double px, double py, double rx, double ry, double rw, double rh) {
    return px >= rx && px <= rx + rw && py >= ry && py <= ry + rh;
}

// LINHA_INTERCEPTA_LINHA
static int linha_intercepta_linha(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) return 0;
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
    return t >= 0 && t <= 1 && u >= 0 && u <= 1; 
}

// LINHA_VS_LINHA_WRAPPER (simplificadora)
static int linha_vs_linha_wrapper(Forma* f1, Forma* f2) {
    double x1, y1, x2, y2, x3, y3, x4, y4;
    forma_get_linha_dados(f1, &x1, &y1, &x2, &y2);
    forma_get_linha_dados(f2, &x3, &y3, &x4, &y4);
    return linha_intercepta_linha(x1, y1, x2, y2, x3, y3, x4, y4);
}

// CIRCULO_VS_CIRCULO
static int circulo_vs_circulo(Forma* f1, Forma* f2) {
    double x1, y1, r1, x2, y2, r2;
    forma_get_circulo_dados(f1, &x1, &y1, &r1);
    forma_get_circulo_dados(f2, &x2, &y2, &r2);
    double dx = x1 - x2;
    double dy = y1 - y2;
    return (dx * dx + dy * dy) < (r1 + r2) * (r1 + r2);
}

// RETANGULO_VS_RETANGULO 
static int retangulo_vs_retangulo(Forma* f1, Forma* f2) {
    double x1, y1, w1, h1, x2, y2, w2, h2;
    if (forma_get_tipo(f1) == 'r') forma_get_retangulo_dados(f1, &x1, &y1, &w1, &h1);
    else forma_get_texto_bbox(f1, &x1, &y1, &w1, &h1); 
    
    if (forma_get_tipo(f2) == 'r') forma_get_retangulo_dados(f2, &x2, &y2, &w2, &h2);
    else forma_get_texto_bbox(f2, &x2, &y2, &w2, &h2); 

    if (x1 + w1 < x2 || x1 > x2 + w2) return 0;
    if (y1 + h1 < y2 || y1 > y2 + h2) return 0;
    return 1;
}

// CIRCULO_VS_RETANGULO 
static int circulo_vs_retangulo(Forma* circ, Forma* rect) {
    double cx, cy, raio, rx, ry, rw, rh;
    forma_get_circulo_dados(circ, &cx, &cy, &raio);
    if (forma_get_tipo(rect) == 'r') forma_get_retangulo_dados(rect, &rx, &ry, &rw, &rh);
    else forma_get_texto_bbox(rect, &rx, &ry, &rw, &rh); 
    
    double ponto_prox_x = clamp(cx, rx, rx + rw);
    double ponto_prox_y = clamp(cy, ry, ry + rh);

    return ponto_no_circulo(ponto_prox_x, ponto_prox_y, cx, cy, raio);
}

// LINHA_VS_CIRCULO
static int linha_vs_circulo(Forma* linha, Forma* circ) {
    double x1, y1, x2, y2, cx, cy, r;
    forma_get_linha_dados(linha, &x1, &y1, &x2, &y2);
    forma_get_circulo_dados(circ, &cx, &cy, &r);

    if (ponto_no_circulo(x1, y1, cx, cy, r) || ponto_no_circulo(x2, y2, cx, cy, r)) return 1;
    
    double dx = x2 - x1, dy = y2 - y1;
    double len_sq = dx*dx + dy*dy;
    if (len_sq == 0) return 0; 
    
    double t = ((cx - x1) * dx + (cy - y1) * dy) / len_sq;
    t = clamp(t, 0, 1);
    double prox_x = x1 + t * dx;
    double prox_y = y1 + t * dy;

    return ponto_no_circulo(prox_x, prox_y, cx, cy, r);
}

// LINHA_VS_RETANGULO
static int linha_vs_retangulo(Forma* linha, Forma* rect) {
    double lx1, ly1, lx2, ly2, rx, ry, rw, rh;
    forma_get_linha_dados(linha, &lx1, &ly1, &lx2, &ly2);
    if (forma_get_tipo(rect) == 'r') forma_get_retangulo_dados(rect, &rx, &ry, &rw, &rh);
    else forma_get_texto_bbox(rect, &rx, &ry, &rw, &rh); 

    if (ponto_no_retangulo(lx1, ly1, rx, ry, rw, rh) || ponto_no_retangulo(lx2, ly2, rx, ry, rw, rh)) return 1;

    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx, ry, rx + rw, ry)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx + rw, ry, rx + rw, ry + rh)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx + rw, ry + rh, rx, ry + rh)) return 1;
    if (linha_intercepta_linha(lx1, ly1, lx2, ly2, rx, ry + rh, rx, ry)) return 1;
    
    return 0;
}

// ================================================
// FUNCAO PRINCIPAL DE SOBREPOSICAO (DESPACHANTE)
// ================================================

int formas_sobrepoem(Forma* f1, Forma* f2) {
    char t1 = forma_get_tipo(f1);
    char t2 = forma_get_tipo(f2);
    
    // texto tratado como retângulo
    char tipo_real1 = (t1 == 't') ? 'r' : t1; 
    char tipo_real2 = (t2 == 't') ? 'r' : t2;

    if (tipo_real1 > tipo_real2) {
        Forma* temp_f = f1; f1 = f2; f2 = temp_f;
        char temp_t = tipo_real1; tipo_real1 = tipo_real2; tipo_real2 = temp_t;
    }

    if (tipo_real1 == 'c' && tipo_real2 == 'c') return circulo_vs_circulo(f1, f2);
    if (tipo_real1 == 'c' && tipo_real2 == 'l') return linha_vs_circulo(f2, f1);
    if (tipo_real1 == 'c' && tipo_real2 == 'r') return circulo_vs_retangulo(f1, f2);

    if (tipo_real1 == 'l' && tipo_real2 == 'l') return linha_vs_linha_wrapper(f1, f2);
    if (tipo_real1 == 'l' && tipo_real2 == 'r') return linha_vs_retangulo(f1, f2);

    if (tipo_real1 == 'r' && tipo_real2 == 'r') return retangulo_vs_retangulo(f1, f2);

    return 0; 
}