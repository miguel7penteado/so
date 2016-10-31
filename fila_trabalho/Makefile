COMPILADOR					= g++
PARAMETROS_COMPILACAO		= -c -Wall
PARAMETROS_LINKAGEM			= -lpthread
ARQUIVOS_FONTE				= principal.cpp thread.cpp
DIRETORIOS_CABECALHO		= -I.
OBJETOS_GERADOS				= $(ARQUIVOS_FONTE:.cpp=.o)
SAIDA						= minha_tarefa.bin

all: $(ARQUIVOS_FONTE) $(SAIDA)

$(SAIDA): $(OBJETOS_GERADOS)
	@echo "************************************************************"
	@echo "LINKANDO "
	@echo "************************************************************" 
	$(COMPILADOR) $(PARAMETROS_LINKAGEM) $(OBJETOS_GERADOS) -o $@

.cpp.o:
	@echo "************************************************************"
	@echo "COMPILANDO..."
	@echo "************************************************************" 
	$(COMPILADOR) $(PARAMETROS_COMPILACAO) $(DIRETORIOS_CABECALHO) $< -o $@

clean:
	@echo "************************************************************"
	@echo "LIMPANDO COMPILACAO ANTERIOR... "
	@echo "************************************************************" 
	rm -rf $(OBJETOS_GERADOS) $(SAIDA)
