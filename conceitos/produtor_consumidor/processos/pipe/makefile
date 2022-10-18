#!/usr/bin/make -f

# Gerando o makefile para os arquivos.

primeiro_alvo: produtor_consumidor.c
	@echo "************************************************************"
	@echo "Compilando produtor_consumidor.c ..."
	@echo "************************************************************"
	gcc -o produtor_consumidor.bin produtor_consumidor.c -lpthread

clean: produtor_consumidor.c
	@echo "************************************************************"
	@echo "apagando produtor_consumidor.bin ..."
	@echo "************************************************************"
	rm produtor_consumidor.bin
	
