COMPILADOR                  = g++
PARAMETROS_COMPILACAO       = -c -Wall
PARAMETROS_LINKAGEM         = -lpthread
CODIGO_FONTE                = principal.cpp thread.cpp
CABECALHOS                  = -I./
ARQUIVOS_OBJETO             = $(CODIGO_FONTE:.cpp=.o)
NOME_PROJETO                = exclusao_mutua.bin

all: $(CODIGO_FONTE) $(NOME_PROJETO)
	@echo ""
	@echo "************************************************************"
	@echo "Gerado o binário executável Exclusão Mútua... "
	@echo "************************************************************" 
	@echo ""
	
$(NOME_PROJETO): $(ARQUIVOS_OBJETO) 
	@echo ""
	@echo "************************************************************"
	@echo "Linkando arquivos objeto do projeto Exclusão Mútua... "
	@echo "" 
	$(COMPILADOR) $(PARAMETROS_LINKAGEM) $(ARQUIVOS_OBJETO) -o $@

.cpp.o:
	@echo ""
	@echo "************************************************************"
	@echo "Compilando arquivo fonte do projeto Exclusão Mútua... "
	@echo "" 
	$(COMPILADOR) $(PARAMETROS_COMPILACAO) $(CABECALHOS) $< -o $@

clean:
	@echo ""
	@echo "************************************************************************"
	@echo "Limpando arquivos da compilacao e linkagem do projeto Exclusão Mútua... "
	@echo "************************************************************************" 
	rm -rf $(ARQUIVOS_OBJETO) $(NOME_PROJETO)
