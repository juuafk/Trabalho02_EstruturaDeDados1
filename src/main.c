#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "formas.h"
#include "leitor_arq.h"
#include "svg.h"

// ESTRUTURA PARAMETROS
typedef struct {
    char* dir_entrada;
    char* arquivo_geo;
    char* dir_saida;
    char* arquivo_qry;
    char tipo_ordenacao;
    int limite_insertionsort;
} Parametros;

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

// INICIALIZAR_PARAMETROS
void inicializar_parametros(Parametros* p) {
    p->dir_entrada = NULL;
    p->arquivo_geo = NULL;
    p->dir_saida = NULL;
    p->arquivo_qry = NULL;
    p->tipo_ordenacao = 'q';
    p->limite_insertionsort = 10;
}

// LIBERAR_PARAMETROS 
void liberar_parametros(Parametros* p) {
    if (p->dir_entrada) free(p->dir_entrada);
    if (p->dir_saida) free(p->dir_saida);
}


// PROCESSAR_ARGUMENTOS
int processar_argumentos(int argc, char* argv[], Parametros* p) { 
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            p->dir_entrada = duplicar_string(argv[++i]);
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            p->arquivo_geo = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            p->dir_saida = duplicar_string(argv[++i]);
        }
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            p->arquivo_qry = argv[++i];
        }
        else if (strcmp(argv[i], "-to") == 0 && i + 1 < argc) {
            char* tipo = argv[++i];
            if (strcmp(tipo, "q") == 0) {
                p->tipo_ordenacao = 'q';
            } 
            else if (strcmp(tipo, "m") == 0) {
                p->tipo_ordenacao = 'm';
            }
            else {
                fprintf(stderr, "tipo de ordenação inválido: %s\n", tipo);
                return -1;
            }
        } 
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            p->limite_insertionsort = atoi(argv[++i]);
        }
        else {
            fprintf(stderr, "argumento desconhecido: %s\n", argv[i]);
            return -1;
        }
    }
    
    if (p->arquivo_geo == NULL) {
        fprintf(stderr, "erro: arquivo .geo (-f) é obrigatório!\n");
        return -1;
    }
    
    if (p->dir_saida == NULL) {
        fprintf(stderr, "erro: diretório de saída (-o) é obrigatório!\n");
        return -1;
    }
    
    return 0;
}

// CONSTRUIR_CAMINHO
char* construir_caminho(char* diretorio, char* arquivo) {
    if (diretorio == NULL) {
        return duplicar_string(arquivo);
    }
    
    size_t len = strlen(diretorio) + strlen(arquivo) + 2; // +2 para '/' e '\0'
    char* caminho = (char*) malloc(len);
    
    if (caminho == NULL) return NULL;
    
    size_t dir_len = strlen(diretorio);
    if (diretorio[dir_len - 1] == '/') {
        snprintf(caminho, len, "%s%s", diretorio, arquivo);
    } 
    else {
        snprintf(caminho, len, "%s/%s", diretorio, arquivo);
    }
    
    return caminho;
}

// EXTRAIR_NOME_BASE
char* extrair_nome_base(char* arquivo) {
    if (arquivo == NULL) return NULL;

    char* ultima_barra = strrchr(arquivo, '/');
    if (ultima_barra == NULL) {
        ultima_barra = strrchr(arquivo, '\\');
    }

    const char* nome_com_extensao;
    if (ultima_barra != NULL) {
        nome_com_extensao = ultima_barra + 1;
    }
    else {
        nome_com_extensao = arquivo;
    }

    char* nome = duplicar_string(nome_com_extensao);
    if (nome == NULL) return NULL;
    
    char* ponto = strrchr(nome, '.');
    if (ponto != NULL) {
        *ponto = '\0';
    }
    
    return nome;
}

// MAIN
int main(int argc, char* argv[]) {

    Parametros params;
    inicializar_parametros(&params);
    
    if (processar_argumentos(argc, argv, &params) != 0) {
        liberar_parametros(&params);
        return 1;
    }
    
    char* caminho_geo = construir_caminho(params.dir_entrada, params.arquivo_geo);
    char* nome_base = extrair_nome_base(params.arquivo_geo);
    
    printf("lendo arquivo .geo: %s\n", caminho_geo);
    
    Lista* formas = criar_lista();
    if (formas == NULL) {
        fprintf(stderr, "erro ao criar lista de formas\n");
        liberar_parametros(&params);
        free(caminho_geo);
        free(nome_base);
        return 1;
    }
    
    if (ler_arquivo_geo(caminho_geo, formas) != 0) {
        fprintf(stderr, "erro ao ler arquivo .geo\n");
        Elemento* elem = get_primeiro_elemento(formas);
        while (elem != NULL) {
            Forma* f = (Forma*) get_elemento(formas, elem);
            destruir_forma(f);
            elem = get_proximo_elemento(elem);
        }
        destruir_lista(formas);
        liberar_parametros(&params);
        free(caminho_geo);
        free(nome_base);
        return 1;
    }
    
    printf("formas carregadas: %d\n", lista_tamanho(formas));
    
    ordenar_lista_por_id(formas);

    char caminho_svg_inicial[512];
    snprintf(caminho_svg_inicial, 512, "%s/%s.svg", params.dir_saida, nome_base);
    
    printf("gerando SVG inicial: %s\n", caminho_svg_inicial);
    
    FILE* svg_inicial = criar_svg(caminho_svg_inicial);
    if (svg_inicial) {
        desenhar_formas(svg_inicial, formas);
        fechar_svg(svg_inicial);
        printf("SVG inicial gerado com sucesso!\n");
    } 
    else {
        fprintf(stderr, "erro ao criar SVG inicial\n");
    }
    
    if (params.arquivo_qry != NULL) {
        char* caminho_qry = construir_caminho(params.dir_entrada, params.arquivo_qry);
        char* nome_qry = extrair_nome_base(params.arquivo_qry);
        
        printf("processando arquivo .qry: %s\n", caminho_qry);
        printf("tipo de ordenação: %s\n", params.tipo_ordenacao == 'q' ? "qsort" : "mergesort");
        printf("limite insertionsort: %d\n", params.limite_insertionsort);
        
        char caminho_txt[512];
        snprintf(caminho_txt, 512, "%s/%s-%s.txt", params.dir_saida, nome_base, nome_qry);
        
        FILE* arquivo_txt = fopen(caminho_txt, "w");
        if (arquivo_txt == NULL) {
            fprintf(stderr, "erro ao criar arquivo .txt\n");
            free(caminho_qry);
            free(nome_qry);
        } 
        else {
            fprintf(arquivo_txt, "Relatório de processamento\n");
            fprintf(arquivo_txt, "Arquivo .geo: %s\n", params.arquivo_geo);
            fprintf(arquivo_txt, "Arquivo .qry: %s\n\n", params.arquivo_qry);
            
            if (ler_arquivo_qry(caminho_qry, formas, arquivo_txt) != 0) {
                fprintf(stderr, "Erro ao processar arquivo .qry\n");
            } 
            else {
                printf("Comandos processados com sucesso!\n");
            }
            
            fclose(arquivo_txt);
            printf("Relatório gerado: %s\n", caminho_txt);
        }
        
        char caminho_svg_final[512];
        snprintf(caminho_svg_final, 512, "%s/%s-%s.svg", params.dir_saida, nome_base, nome_qry);
        
        printf("gerando SVG final: %s\n", caminho_svg_final);
        
        FILE* svg_final = criar_svg(caminho_svg_final);
        if (svg_final) {
            desenhar_formas(svg_final, formas);
            fechar_svg(svg_final);
            printf("SVG final gerado com sucesso!\n");
        }
        else {
            fprintf(stderr, "erro ao criar SVG final\n");
        }
        
        free(caminho_qry);
        free(nome_qry);
    }
    
    Elemento* elem = get_primeiro_elemento(formas);
    while (elem != NULL) {
        Forma* f = (Forma*) get_elemento(formas, elem);
        destruir_forma(f);
        elem = get_proximo_elemento(elem);
    }
    destruir_lista(formas);

    free(caminho_geo);
    free(nome_base);
    liberar_parametros(&params);
    
    printf("processamento concluido\n");
    return 0;
}