//					ESCUELA SUPERIOR DE CÓMPUTO
//			Macías Escalona Jorge Luis
//			Desarrollo de Sistemas Distribuidos - 4CM1
//			DESCRIPCIÓN:	servidor distribuido para enviar y recibir posiciones de objetos
//			FECHA:	Julio 18 del 2015
//			ARCHIVO:	serverFlappy.cpp
//
#include <iostream>
#include <stdlib.h>
#include "SocketDatagrama.h"
#include "data.h"

using namespace std;

int main(int argc, char *argv[]){
	cout << "Servidor iniciado" << endl;
	struct birdPackage infoReceived;
	int posicionesX[3] = {0,0,0};
	int posicionesY[3] = {0,0,0};
	double angulo[3] = {330,330,330};
	int tuberiasX[TUBE_LIST_SIZE];
	int tuberiasY[TUBE_LIST_SIZE];
	int nJugadores = 0;
	int *nPaquetes;
	if(argc != 2)
	{
		cout << "Forma de uso: " << argv[0] << " puerto\n";
		exit(0);
	}
	
	for(int j = 0 ; j<TUBE_LIST_SIZE;j++){
		int altura = rand()%80 + 100;
		tuberiasY[j] = altura;
		tuberiasX[j] = (int) 864/4 * (j+4);
	}
	
	SocketDatagrama socketlocal(atoi(argv[1]));

	while(1)
	{

		bzero(&infoReceived,sizeof(birdPackage));
		PaqueteDatagrama receive(sizeof(birdPackage));
		socketlocal.recibe(receive);
		memcpy(&infoReceived, receive.obtieneDatos(), sizeof(birdPackage));
		/* Se revisan los códigos de operación para la interacción del jugador*/
		if(infoReceived.opcode == NEW) //Se quiere integrar un nuevo jugador
		{
			if(nJugadores < 3) //Es permitido
			{
				infoReceived.opcode = ALLOW;
				infoReceived.jugadorNum = nJugadores;
				infoReceived.posicionJUMP_X[nJugadores] = posicionesX[nJugadores];
				infoReceived.posicionJUMP_Y[nJugadores] = posicionesY[nJugadores];
				infoReceived.angulo[nJugadores] = angulo[nJugadores];
				cout << "X: " << infoReceived.posicionJUMP_X[nJugadores] << "Y: " << infoReceived.posicionJUMP_Y[nJugadores] << "\n";
				printf("NUEVO JUGADOR %d\n",nJugadores);
				nJugadores++;
			}
			else infoReceived.opcode = DENY;
		}
		if(infoReceived.opcode == CLOSE)
		{
			printf("JUGADOR FUERA DE PARTIDA...\n");
			nJugadores--;
		}
		if(infoReceived.opcode == DEAD) //Se informa al servidor de la muerte de un jugador
		{
			printf("JUGADOR MUERTO...\n");
			nJugadores--;
		}
		if(infoReceived.opcode != NEW && infoReceived.opcode != DEAD) //El código puede ser un salto que hizo el jugador
		{
			if(infoReceived.opcode == JUMP) //Se actualizan las posiciones en el servidor del jugador que se movio
			{
				posicionesX[infoReceived.jugadorNum] = infoReceived.posicionJUMP_X[infoReceived.jugadorNum];
				posicionesY[infoReceived.jugadorNum] = infoReceived.posicionJUMP_Y[infoReceived.jugadorNum];
				angulo[infoReceived.jugadorNum] = infoReceived.angulo[infoReceived.jugadorNum];
				//cout << "X: " << posicionesX[infoReceived.jugadorNum] << "Y: " << posicionesY[infoReceived.jugadorNum] << "\n";
			}
		}
		//int i = 0;
		for (int i = 0; i < 3; ++i) //Se actualizan las posiciones de los saltos alojados en el servidor, pero ahora en la estructura global
		{
			infoReceived.posicionJUMP_X[i] = posicionesX[i];
			infoReceived.posicionJUMP_Y[i] = posicionesY[i];
			infoReceived.angulo[i] = angulo[i];
		}
		for(int j =0; j<TUBE_LIST_SIZE; j++){
			infoReceived.posicionTUBES_X[j] = tuberiasX[j]-=5;
			infoReceived.posicionTUBES_Y[j] = tuberiasY[j];
			//cout << "Tuberia X: " << tuberiasX[j] << " Tuberia Y: " << tuberiasY[j] << "\n";
		}
		infoReceived.jugadoresTotales = nJugadores; //Se informa de igual forma de los jugadores totales existentes
		//Se empaqueta la estructura y se envía a la IP que solicito el cambio
		PaqueteDatagrama envio((char *)&infoReceived, sizeof(birdPackage),(char*)"",0);
		envio.inicializaIp(receive.obtieneDireccion());
		envio.inicializaPuerto(receive.obtienePuerto());

		envio.inicializaDatos((char*)&infoReceived);
		socketlocal.envia(envio);
	}
	return 0;
}