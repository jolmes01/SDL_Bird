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
	int posicionesX[3] = {-100,-100,-100};
	int posicionesY[3] = {-100,-100,-100};
	double angulo[3] = {330,330,330};
	int tuberiasX[TUBE_LIST_SIZE];
	int tuberiasY[TUBE_LIST_SIZE];
	int puntuaciones[3]={0,0,0};
	int nJugadores = 0;
	int *nPaquetes;
	int flaginit=0;
	bool tubes = false;
	if(argc != 2)
	{
		cout << "Forma de uso: " << argv[0] << " puerto\n";
		exit(0);
	}
	
	for(int j = 0 ; j<TUBE_LIST_SIZE;j++){
		int altura = rand()%80 + 100;
		tuberiasY[j] = altura;
		tuberiasX[j] = (int) 864/4 * (j+3);
	}
	
	SocketDatagrama socketlocal(atoi(argv[1]));

	while(1)
	{

		bzero(&infoReceived,sizeof(birdPackage));
		PaqueteDatagrama receive(sizeof(birdPackage));
		socketlocal.recibe(receive);
		memcpy(&infoReceived, receive.obtieneDatos(), sizeof(birdPackage));
		/* Se revisan los códigos de operación para la interacción del jugador*/

		/*if(flaginit==1 && nJugadores==0 )
		{
			cout<<"Reiniciando servidor"<<"\n";
			posicionesX[0] = -100;
			posicionesX[1] = -100;
			posicionesX[2] = -100;
			posicionesY[0] = -100;
			posicionesY[1] = -100;
			posicionesY[2] = -100;
			angulo[0] = 330;
			angulo[1] = 330;
			angulo[2] = 330;
			puntuaciones[0] = 0;
			puntuaciones[1] = 0;
			puntuaciones[2] = 0;
			nJugadores = 0;
			flaginit=0;
		}*/
		if(infoReceived.opcode == NEW) //Se quiere integrar un nuevo jugador
		{
			if(nJugadores < 3) //Es permitido
			{
				tubes = true;
				infoReceived.opcode = WAIT;
				infoReceived.jugadorNum = nJugadores;
				infoReceived.posicionJUMP_X[nJugadores] = posicionesX[nJugadores];
				infoReceived.posicionJUMP_Y[nJugadores] = posicionesY[nJugadores];
				infoReceived.angulo[nJugadores] = angulo[nJugadores];
				infoReceived.puntuacion[nJugadores] =puntuaciones[nJugadores];
				cout << "X: " << infoReceived.posicionJUMP_X[nJugadores] << "Y: " << infoReceived.posicionJUMP_Y[nJugadores] << "\n";
				printf("NUEVO JUGADOR %d\n",nJugadores);
				nJugadores++;
				printf("Esperando mas jugadores \n");				
			}
			else 
			{
				infoReceived.opcode = DENY;
				printf("Rechazado \n");
			}
		}

		if(infoReceived.opcode == CLOSE)
		{
			printf("JUGADOR FUERA DE PARTIDA...\n");
			nJugadores--;
		}
		if(infoReceived.opcode == DEAD) //Se informa al servidor de la muerte de un jugador
		{
			printf("JUGADOR MUERTO %d...\n",nJugadores);
			
			posicionesX[infoReceived.jugadorNum] = -200;
			posicionesY[infoReceived.jugadorNum] = -200;
			if(nJugadores>0)
			nJugadores--;
			if(nJugadores==0){
				tubes = false;
				for(int i = 0 ; i < 3 ; i++){
					posicionesX[i] = -100;
					posicionesY[i] = -100;
					angulo[i] =330;
				}
				
				
				for(int j = 0 ; j<TUBE_LIST_SIZE;j++){
					int altura = rand()%80 + 100;
					tuberiasY[j] = altura;
					tuberiasX[j] = (int) 864/4 * (j+3);
				}
				infoReceived.opcode = GAME_OVER;
			}
		}
		if(infoReceived.opcode != NEW && infoReceived.opcode != DEAD) //El código puede ser un salto que hizo el jugador
		{
			if(tubes)
				for(int j =0; j<TUBE_LIST_SIZE; j++){
					infoReceived.posicionTUBES_X[j] = tuberiasX[j]-=3;
					infoReceived.posicionTUBES_Y[j] = tuberiasY[j];
					//cout << "Tuberia X: " << tuberiasX[j] << " Tuberia Y: " << tuberiasY[j] << "\n";
				}
			if(infoReceived.opcode == JUMP) //Se actualizan las posiciones en el servidor del jugador que se movio
			{
				posicionesX[infoReceived.jugadorNum] = infoReceived.posicionJUMP_X[infoReceived.jugadorNum];
				posicionesY[infoReceived.jugadorNum] = infoReceived.posicionJUMP_Y[infoReceived.jugadorNum];
				angulo[infoReceived.jugadorNum] = infoReceived.angulo[infoReceived.jugadorNum];
				puntuaciones[infoReceived.jugadorNum]=infoReceived.puntuacion[infoReceived.jugadorNum];
				//cout << "X: " << posicionesX[infoReceived.jugadorNum] << "Y: " << posicionesY[infoReceived.jugadorNum] << "\n";
			}
			if(infoReceived.opcode == WAIT) //Se actualizan las posiciones en el servidor del jugador que se movio
			{
				if(nJugadores == 3)infoReceived.opcode = ALLOW;
			}
		}

			for (int i = 0; i < 3; ++i) //Se actualizan las posiciones de los saltos alojados en el servidor, pero ahora en la estructura global
			{
				infoReceived.posicionJUMP_X[i] = posicionesX[i];
				infoReceived.posicionJUMP_Y[i] = posicionesY[i];
				infoReceived.angulo[i] = angulo[i];
				infoReceived.puntuacion[i] = puntuaciones[i];
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
