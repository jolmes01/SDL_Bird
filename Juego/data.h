//					ESCUELA SUPERIOR DE CÓMPUTO
//			Gonzalez Cruz Edgar
//			Macías Escalona Jorge Luis
//			Desarrollo de Sistemas Distribuidos - 4CM1
//			DESCRIPCIÓN:	estructura para el envío de datos
//			FECHA:	Julio 18 del 2015
//			ARCHIVO:	data.h
//
#ifndef DATA_H_
#define DATA_H_
/* Definiciones necesarias para los clientes y el servidor de archivos */
#define BUF_SIZE 1024 //Cantidad de bytes que se transfieren por paquete

/* Definición de códigos de aviso */
#define JUMP  	1 //Salto de un pájaro
#define DEAD 	2 //Muere un jugador
#define NEW		3 //Jugador Nuevo en la partida
#define ALLOW	4 //Jugador permitido en la partida
#define DENY	5 //Jugador rechazado en la partida
#define CLOSE	6 //El jugador salio de la partida

/* Formato del mensaje que viaja con los datos del movimiento del jugador */
struct birdPackage{
	int opcode; //Código del aviso JUMP o DEAD
	int jugadorNum; //Número de jugador que se unio a la partida
	int jugadoresTotales; //Número de jugadores dentro de la partida
	int posicionJUMP_X[3]; //Número representativo del salto del jugador en el eje X
	int posicionJUMP_Y[3]; //Número representativo del salto del jugador en el eje Y
	double angulo[3]; //Número representativo del angulo del jugador en el eje Y
	/* INSERTAR MÄS DATOS QUE REQUIERAN SER MANDADOS MEDIANTE birdPackage */
	/**********************************************************************/
};
#endif
