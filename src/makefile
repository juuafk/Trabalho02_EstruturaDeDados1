# =========================
# INSTRUÇÕES DE COMPILAÇÃO
# =========================
PROJ_NAME = ted
ALUNO = juliagruara
LIBS = -lm
OBJETOS = geometria.o lista.o segmento.o formas.o leitor_arq.o svg.o arvore.o ordenacao.o visibilidade.o main.o

# compilador
CC = gcc

# flags
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS = -O0 -fstack-protector-all

# ---------------------
#  INSTRUÇÃO PRINCIPAL
# ---------------------
ted: $(OBJETOS)
	$(CC) -o ted $(LDFLAGS) $(OBJETOS) $(LIBS)

# ---------------------
#  INSTRUÇÃO GENÉRICA
# ---------------------
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# ---------------------
#  DEPENDÊNCIAS
# ---------------------
geometria.o: geometria.h
lista.o: lista.h
segmento.o: segmento.h geometria.h
formas.o: formas.h geometria.h
leitor_arq.o: leitor_arq.h formas.h lista.h
svg.o: svg.h formas.h lista.h geometria.h
arvore.o: arvore.h segmento.h geometria.h
ordenacao.o: ordenacao.h
visibilidade.o: visibilidade.h geometria.h segmento.h arvore.h lista.h ordenacao.h
main.o: geometria.h lista.h formas.h leitor_arq.h svg.h

# --------------------
#  TESTES UNITÁRIOS
# --------------------
teste_geometria: geometria.o
	$(CC) $(CFLAGS) ../testes/teste_geometria.c geometria.o -o ../bin/teste_geometria $(LIBS)
	@../bin/teste_geometria

teste_lista: lista.o
	$(CC) $(CFLAGS) ../testes/teste_lista.c lista.o -o ../bin/teste_lista $(LIBS)
	@../bin/teste_lista

teste_segmento: segmento.o geometria.o
	$(CC) $(CFLAGS) ../testes/teste_segmento.c segmento.o geometria.o -o ../bin/teste_segmento $(LIBS)
	@../bin/teste_segmento

# ------------
#  LIMPEZA
# ------------
clean:
	rm -f $(OBJETOS) ted
