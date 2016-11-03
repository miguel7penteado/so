COMPILADOR               = g++
PARAMETROS_COMPILACAO    = -c -Wall -g
PARAMETROS_LINKAGEM      = -lpthread
ARQUIVOS_FONTE           = principal.cpp thread.cpp
ARQUIVOS_CABECALHO       = -I./
ARQUIVOS_OBJETO          = $(ARQUIVOS_FONTE:.cpp=.o)
NOME_PROJETO             = variaveis_condicionais.bin

all: $(ARQUIVOS_FONTE) $(NOME_PROJETO)
	@echo ""
	@echo "************************************************************"
	@echo "Gerado o binário executável Variáveis Condicionais... "
	@echo "************************************************************" 
	@echo ""

$(NOME_PROJETO): $(ARQUIVOS_OBJETO)
	@echo ""
	@echo "************************************************************"
	@echo "Linkando arquivos objeto do projeto Variáveis Condiconais..."
	@echo "" 
	$(COMPILADOR) $(PARAMETROS_LINKAGEM) $(ARQUIVOS_OBJETO) -o $@

.cpp.o:
	@echo ""
	@echo "************************************************************"
	@echo "Compilando arquivo fonte do projeto Variáveis Condiconais..."
	@echo "" 
	$(COMPILADOR) $(PARAMETROS_COMPILACAO) $(ARQUIVOS_CABECALHO) $< -o $@

clean:
	@echo ""
	@echo "************************************************************************"
	@echo "Limpando arquivos da compilacao e linkagem do projeto Variáveis Condiconais... "
	@echo "************************************************************************" 
	rm -rf $(ARQUIVOS_OBJETO) $(NOME_PROJETO)
