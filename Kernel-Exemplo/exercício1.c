//
// Exemplo simples de montagem em linha
//
// Para JOS lab 1 exercício 1
//

#include <stdio.h>

int main(int argc, char **argv)
{
  int x = 1;
  printf("Ola x = %d\n", x);
  
  //
  // Coloque a montagem em linha aqui para incrementar
  // o valor de x por 1 usando montagem em linha
  //

  printf("Ola x = %d depois do incremento\n", x);

  if(x == 2)
  {
    printf("OK\n");
	return(0);
  }
  else
  {
    printf("ERRO\n");
	return(1);
  }  
}