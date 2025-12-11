#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "leitor_arq.h"
#include "visibilidade.h"
#include "svg.h"
#include "segmento.h" 
#include "geometria.h" 
#include "formas.h" 
#include "lista.h" 

#define MAX_LINE 1024

// FONTES
static char font_family[50] = "sans-serif";
static char font_weight[20] = "normal";
static int font_size = 12;

static int proximo_id_segmento = 10000; 

// LER_ARQUIVO_GEO
int ler_arquivo_geo(char* caminho_arquivo, Lista* formas) {
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "erro ao abrir arquivo .geo: %s\n", caminho_arquivo);
        return -1;
    }
    
    char linha[MAX_LINE];
    
    while (fgets(linha, MAX_LINE, arquivo) != NULL) {
        char comando;
        
        if (sscanf(linha, " %c", &comando) != 1) continue;
        if (comando == 'c') {
            int id;
            double x, y, r;
            char corb[20], corp[20];
            
            if (sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp) == 6) {
                Forma* f = criar_circulo(id, x, y, r, corb, corp);
                if (f) inserir_fim_lista(formas, f);
            }
            
        } 
        else if (comando == 'r') {
            int id;
            double x, y, w, h;
            char corb[20], corp[20];
            
            if (sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp) == 7) {
                Forma* f = criar_retangulo(id, x, y, w, h, corb, corp);
                if (f) inserir_fim_lista(formas, f);
            }
            
        } 
        else if (comando == 'l') {
            int id;
            double x1, y1, x2, y2;
            char cor[20];
            
            if (sscanf(linha, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor) == 6) {
                Forma* f = criar_linha(id, x1, y1, x2, y2, cor);
                if (f) inserir_fim_lista(formas, f);
            }
            
        }
        else if (comando == 't') {
            if (linha[1] == 's') {
                char fam[20], weight[20], size_str[20];
                
                if (sscanf(linha, "ts %s %s %s", fam, weight, size_str) == 3) {
                    strcpy(font_family, fam);
                    strcpy(font_weight, weight);
                    font_size = atoi(size_str);
                }
            } 
            else {
                int id;
                double x, y;
                char corb[20], corp[20], ancora;
                char texto[MAX_LINE];
                
                char* ptr = linha;
                
                while (*ptr && *ptr != 't') ptr++;
                if (*ptr) ptr++;
                
                sscanf(ptr, "%d", &id);
                while (*ptr && *ptr != ' ') ptr++;
                while (*ptr && *ptr == ' ') ptr++;
                
                sscanf(ptr, "%lf", &x);
                while (*ptr && *ptr != ' ') ptr++;
                while (*ptr && *ptr == ' ') ptr++;
                
                sscanf(ptr, "%lf", &y);
                while (*ptr && *ptr != ' ') ptr++;
                while (*ptr && *ptr == ' ') ptr++;
                
                sscanf(ptr, "%s", corb);
                while (*ptr && *ptr != ' ') ptr++;
                while (*ptr && *ptr == ' ') ptr++;
                
                sscanf(ptr, "%s", corp);
                while (*ptr && *ptr != ' ') ptr++;
                while (*ptr && *ptr == ' ') ptr++;
                
                sscanf(ptr, "%c", &ancora);
                while (*ptr && *ptr != ' ') ptr++;
                while (*ptr && *ptr == ' ') ptr++;
                
                strcpy(texto, ptr);
                texto[strcspn(texto, "\n")] = 0;
                
                char ancora_str[2] = {ancora, '\0'};
                Forma* f = criar_texto(id, x, y, texto, ancora_str, corb, corp);
                if (f) inserir_fim_lista(formas, f);
            }
        }
    }
    
    fclose(arquivo);
    return 0;
}

// PROCESSAR_SEGMENTO
static void processar_segmento(int id_ini, int id_fim, char orientacao, Lista* formas, Lista* segmentos_globais, FILE* txt) { 
    
    fprintf(txt, "COMANDO 'a': transformando formas [%d, %d] em anteparos\n", id_ini, id_fim);
    
    Elemento* elem = get_primeiro_elemento(formas);
    
    while (elem != NULL) {
        Forma* f = (Forma*) get_elemento(formas, elem);
        int id = forma_get_id(f);
        
        if (id >= id_ini && id <= id_fim) {
            fprintf(txt, "Forma ID %d tipo '%c': ", id, forma_get_tipo(f));
            
            Lista* segs = forma_para_segmentos(f, orientacao, &proximo_id_segmento);
            
            if (segs) {
                Elemento* s_elem = get_primeiro_elemento(segs);
                while (s_elem != NULL) {
                    Segmento* seg = (Segmento*) get_elemento(segs, s_elem); 
                    inserir_fim_lista(segmentos_globais, seg);
                    
                    Ponto* ini = segmento_get_inicio(seg); 
                    Ponto* fim = segmento_get_fim(seg); 
                    
                    fprintf(txt, "Segmento ID %d (%.2f,%.2f) - (%.2f,%.2f) ",
                            segmento_get_id(seg),
                            get_x(ini), get_y(ini),
                            get_x(fim), get_y(fim));
                    
                    s_elem = get_proximo_elemento(s_elem);
                }
                fprintf(txt, "\n");
                destruir_lista(segs);
            }
        }
        
        elem = get_proximo_elemento(elem);
    }
}

// PROCESSAR_DESTRUICAO
static void processar_destruicao(double x, double y, char* sufixo, Lista* formas, Lista* segmentos, FILE* txt, char tipoOrd, int limInsert) { 
    
    fprintf(txt, "COMANDO 'd': Bomba de destruição em (%.2f, %.2f)\n", x, y);
    
    Ponto* origem = criar_ponto(x, y);
    Lista* poligono = calcular_visibilidade(origem, segmentos, tipoOrd, limInsert);
    
    char nome_svg_poligono[100];
    sprintf(nome_svg_poligono, "visibilidade_%s.svg", sufixo);
    FILE* svg_poligono = criar_svg(nome_svg_poligono);

    if (poligono) {
        
        int n_pts = lista_tamanho(poligono);
        int n = n_pts / 2;
        
        Ponto** vertices = NULL;
        Lista* vertices_para_desenho = NULL;
        
        if (n > 0) {
            vertices = (Ponto**) malloc(n * sizeof(Ponto*));
            vertices_para_desenho = criar_lista();

            Elemento* elem = get_primeiro_elemento(poligono);
            int idx = 0;
            
            while (elem != NULL && idx < n) {
                Ponto* p = (Ponto*) get_elemento(poligono, elem);
                
                vertices[idx++] = p; 
                
                inserir_fim_lista(vertices_para_desenho, criar_ponto(get_x(p), get_y(p)));
                
                elem = get_proximo_elemento(elem); 
                if (elem) elem = get_proximo_elemento(elem); 
            }
        }

        if (svg_poligono && vertices_para_desenho) {
            desenhar_formas(svg_poligono, formas); 
            desenhar_segmentos(svg_poligono, segmentos); 
            desenhar_poligono(svg_poligono, vertices_para_desenho, "#000000", "#FF0000", 0.5);
            fprintf(svg_poligono, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"3\" fill=\"red\" stroke=\"black\" />\n", x, y);
            fechar_svg(svg_poligono);
        }
        
        Lista* destruidas = criar_lista();
        
        Elemento* elem = get_primeiro_elemento(formas);
        while (elem != NULL) {
            Forma* f = (Forma*) get_elemento(formas, elem);
            
            bool dentro = false;
            char tipo = forma_get_tipo(f);
            
            if (n > 0) {
                if (tipo == 'c') {
                    Ponto* centro = criar_ponto(forma_get_x(f), forma_get_y(f));
                    dentro = ponto_em_poligono(centro, vertices, n);
                    destruir_ponto(centro);
                } 
                else if (tipo == 'r' || tipo == 't') {
                    Ponto* p = criar_ponto(forma_get_x(f), forma_get_y(f));
                    dentro = ponto_em_poligono(p, vertices, n);
                    destruir_ponto(p);
                } 
                else if (tipo == 'l') {
                    Ponto* p1 = criar_ponto(forma_get_x1(f), forma_get_y1(f));
                    Ponto* p2 = criar_ponto(forma_get_x2(f), forma_get_y2(f));
                    dentro = segmento_intersecta_poligono(p1, p2, vertices, n);
                    destruir_ponto(p1);
                    destruir_ponto(p2);
                }
            } 
            
            if (dentro) {
                fprintf(txt, "Forma ID %d tipo '%c' DESTRUÍDA\n", forma_get_id(f), forma_get_tipo(f));
                inserir_fim_lista(destruidas, f);
            }
            
            elem = get_proximo_elemento(elem);
        }
        
        elem = get_primeiro_elemento(destruidas);
        while (elem != NULL) {
            Forma* f = (Forma*) get_elemento(destruidas, elem);
            Elemento* busca = get_primeiro_elemento(formas);

            while (busca != NULL) {
                Forma* fb = (Forma*) get_elemento(formas, busca);
                if (fb == f) {
                    remover_posicao_lista(formas, busca);
                    destruir_forma(f);
                    break;
                }
                busca = get_proximo_elemento(busca);
            }
            
            elem = get_proximo_elemento(elem);
        }
        
        destruir_lista(destruidas);
        if (vertices) free(vertices);
        if (vertices_para_desenho) destruir_lista_de_pontos(vertices_para_desenho); 
        destruir_lista(poligono); 
    }
    
    destruir_ponto(origem);
}

// PROCESSAR_PINTURA
static void processar_pintura(double x, double y, char* cor, char* sufixo, Lista* formas, Lista* segmentos, FILE* txt, char tipoOrd, int limInsert) {
    
    fprintf(txt, "COMANDO 'p': Bomba de pintura em (%.2f, %.2f) cor %s\n", x, y, cor);
    
    Ponto* origem = criar_ponto(x, y);
    Lista* poligono = calcular_visibilidade(origem, segmentos, tipoOrd, limInsert); 
    
    if (poligono) {
        int n_pts = lista_tamanho(poligono);
        int n = n_pts / 2;
        Ponto** vertices = (Ponto**) malloc(n * sizeof(Ponto*));
        
        Elemento* elem = get_primeiro_elemento(poligono);
        int idx = 0;
        while (elem != NULL && idx < n) {
            vertices[idx++] = (Ponto*) get_elemento(poligono, elem);
            elem = get_proximo_elemento(elem);
            if (elem) elem = get_proximo_elemento(elem);
        }
        
        elem = get_primeiro_elemento(formas);
        while (elem != NULL) {
            Forma* f = (Forma*) get_elemento(formas, elem);
            
            bool dentro = false;
            char tipo = forma_get_tipo(f);
            
            if (tipo == 'c') {
                Ponto* centro = criar_ponto(forma_get_x(f), forma_get_y(f));
                dentro = ponto_em_poligono(centro, vertices, n);
                destruir_ponto(centro);
            } 
            else if (tipo == 'r' || tipo == 't') {
                Ponto* p = criar_ponto(forma_get_x(f), forma_get_y(f));
                dentro = ponto_em_poligono(p, vertices, n);
                destruir_ponto(p);
            }
            else if (tipo == 'l') {
                Ponto* p1 = criar_ponto(forma_get_x1(f), forma_get_y1(f));
                Ponto* p2 = criar_ponto(forma_get_x2(f), forma_get_y2(f));
                dentro = segmento_intersecta_poligono(p1, p2, vertices, n);
                destruir_ponto(p1);
                destruir_ponto(p2);
            }
            
            if (dentro) {
                fprintf(txt, "Forma ID %d tipo '%c' PINTADA\n", forma_get_id(f), forma_get_tipo(f));
                forma_set_cor_borda(f, cor);
                forma_set_cor_preenchimento(f, cor);
            }
            
            elem = get_proximo_elemento(elem);
        }
        
        free(vertices);
        destruir_lista(poligono);
    }
    
    destruir_ponto(origem);
}

// PROCESSAR_CLONAGEM
static void processar_clonagem(double x, double y, double dx, double dy, char* sufixo, Lista* formas, Lista* segmentos, FILE* txt, char tipoOrd, int limInsert) {
    
    fprintf(txt, "COMANDO 'cln': Bomba de clonagem em (%.2f, %.2f)\n", x, y);
    fprintf(txt, "Deslocamento: dx= %.2f, dy= %.2f\n", dx, dy);
    
    Ponto* origem = criar_ponto(x, y);
    Lista* poligono = calcular_visibilidade(origem, segmentos, tipoOrd, limInsert); 
    
    if (poligono) {
        int n_pts = lista_tamanho(poligono);
        int n = n_pts / 2;
        Ponto** vertices = (Ponto**) malloc(n * sizeof(Ponto*));
        
        Elemento* elem = get_primeiro_elemento(poligono);
        int idx = 0;
        
        while (elem != NULL && idx < n) {
            vertices[idx++] = (Ponto*) get_elemento(poligono, elem);
            elem = get_proximo_elemento(elem);
            if (elem) elem = get_proximo_elemento(elem);
        }
        
        static int proximo_id_clone = 50000;
        
        elem = get_primeiro_elemento(formas);
        while (elem != NULL) {
            Forma* f = (Forma*) get_elemento(formas, elem);
            
            bool dentro = false;
            char tipo = forma_get_tipo(f);
            
            if (tipo == 'c') {
                Ponto* centro = criar_ponto(forma_get_x(f), forma_get_y(f));
                dentro = ponto_em_poligono(centro, vertices, n);
                destruir_ponto(centro);
            }
            else if (tipo == 'r' || tipo == 't') {
                Ponto* p = criar_ponto(forma_get_x(f), forma_get_y(f));
                dentro = ponto_em_poligono(p, vertices, n);
                destruir_ponto(p);
            } 
            else if (tipo == 'l') {
                Ponto* p1 = criar_ponto(forma_get_x1(f), forma_get_y1(f));
                Ponto* p2 = criar_ponto(forma_get_x2(f), forma_get_y2(f));
                dentro = segmento_intersecta_poligono(p1, p2, vertices, n);
                destruir_ponto(p1);
                destruir_ponto(p2);
            }
            
            if (dentro) {
                int id_original = forma_get_id(f);
                Forma* clone = forma_clonar(f, proximo_id_clone++);
                
                if (clone) {
                    forma_mover(clone, dx, dy);
                    inserir_fim_lista(formas, clone);
                    
                    fprintf(txt, "Forma ID %d tipo '%c' -> Clone ID %d\n", id_original, tipo, forma_get_id(clone));
                }
            }
            
            elem = get_proximo_elemento(elem);
        }
        
        free(vertices);
        destruir_lista(poligono);
    }
    
    destruir_ponto(origem);
}

// LER_ARQUIVO_QRY
int ler_arquivo_qry(char* caminho_arquivo, Lista* formas, FILE* arquivo_txt) {
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "erro ao abrir arquivo .qry: %s\n", caminho_arquivo);
        return -1;
    }
    
    Lista* segmentos_globais = criar_lista();
    char linha[MAX_LINE];
    char tipo_ordenacao = 'q';
    int limite_insert = 10;
    
    while (fgets(linha, MAX_LINE, arquivo) != NULL) {
        char comando;
        
        if (sscanf(linha, " %c", &comando) != 1) continue;
        
        if (comando == 'a') {
            int i, j;
            char orient = 'h';
            
            sscanf(linha, "a %d %d %c", &i, &j, &orient);
            processar_segmento(i, j, orient, formas, segmentos_globais, arquivo_txt); 
            
        } 
        else if (comando == 'd') {
            double x, y;
            char sufixo[50];
            
            if (sscanf(linha, "d %lf %lf %s", &x, &y, sufixo) == 3) {
                processar_destruicao(x, y, sufixo, formas, segmentos_globais, arquivo_txt, tipo_ordenacao, limite_insert); 
            }
            
        } 
        else if (comando == 'p') {
            double x, y;
            char cor[20], sufixo[50];
            
            if (sscanf(linha, "p %lf %lf %s %s", &x, &y, cor, sufixo) == 4) {
                processar_pintura(x, y, cor, sufixo, formas, segmentos_globais, arquivo_txt, tipo_ordenacao, limite_insert); 
            }
            
        } 
        else if (comando == 'c' && linha[1] == 'l' && linha[2] == 'n') {
            double x, y, dx, dy;
            char sufixo[50];
            
            if (sscanf(linha, "cln %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sufixo) == 5) {
                processar_clonagem(x, y, dx, dy, sufixo, formas, segmentos_globais, arquivo_txt, tipo_ordenacao, limite_insert);
            }
        }
    }
    
    fclose(arquivo);
    destruir_lista(segmentos_globais); 
    return 0;
}