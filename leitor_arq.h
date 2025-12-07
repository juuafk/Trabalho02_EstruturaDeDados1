#ifndef LEITOR_H
#define LEITOR_H

#include "lista.h"
#include "formas.h"

// ===========================================
// LEITOR DE ARQUIVOS
// ------------------------------------------
// processa os arquivos geo e qry.
// ===========================================


// ---------------------------------------------------
//             LEITURA DE ARQUIVO .GEO
// ---------------------------------------------------

/* -> ler_arquivo_geo
    FUNÇÃO: ler o arquivo .geo e preencher a lista de formas
    RECEBE: caminho do arquivo e a lista para colocar formas
    RETORNA: 0 se for executada com sucesso
 */
int ler_arquivo_geo(char* caminho_arquivo, Lista* formas);

// ---------------------------------------------------
//             LEITURA DE ARQUIVO .QRY
// ---------------------------------------------------

/* -> ler_arquivo_qry
    FUNÇÃO: processar arquivo .qry executando comandos
    RECEBE: caminho do arquivo, lista de formas a ser modificada pelos comandos
    e arquivo txt para relatório
    RETORNA: 0 se for executada com sucesso
 */
int ler_arquivo_qry(char* caminho_arquivo, Lista* formas, FILE* arquivo_txt);

#endif 