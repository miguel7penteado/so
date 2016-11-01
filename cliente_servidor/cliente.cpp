/*
   cliente.cpp


*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include "tcpconnector.h"

using namespace std;

int main(int quantidade_argumentos, char** vetor_argumentos)
{
    int tamanho;
    string minha_mensagem;
    char linha[256];
    TCPConnector* meu_conector;
	TCPStream* meu_segmento;

    if (quantidade_argumentos != 3)
    {
        printf("uso: %s <port> <ip>\n", vetor_argumentos[0]);
        exit(1);
    }

	meu_conector = new TCPConnector();
    meu_segmento = meu_conector->connect(vetor_argumentos[2], atoi(vetor_argumentos[1]));
    
    if (meu_segmento)
    {
        minha_mensagem = "Existe vida em Marte ?";
        meu_segmento->send(minha_mensagem.c_str(), minha_mensagem.size());
        printf("enviado - %s\n", minha_mensagem.c_str());
        tamanho = meu_segmento->receive(linha, sizeof(linha));
        linha[tamanho] = '\0';
        printf("recebido - %s\n", linha);
        delete meu_segmento;
    }

    meu_segmento = meu_conector->connect(vetor_argumentos[2], atoi(vetor_argumentos[1]));

    if (meu_segmento)
    {
        minha_mensagem = "E ai mano ?";
        meu_segmento->send(minha_mensagem.c_str(), minha_mensagem.size());
        printf("enviado - %s\n", minha_mensagem.c_str());
        tamanho = meu_segmento->receive(linha, sizeof(linha));
        linha[tamanho] = '\0';
        printf("recebido - %s\n", linha);
        delete meu_segmento;
    }
    exit(0);
}
