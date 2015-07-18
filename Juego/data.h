//					ESCUELA SUPERIOR DE CÓMPUTO
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
#define JUMP  1 //Salto de un pájaro
#define DEAD 	2 //Muere un jugador

/* Formato del mensaje que viaja con los datos del movimiento del jugador */
struct birdPackage{
	int opcode; //Código del aviso JUMP o DEAD
	int posicionJUMP_X; //Número representativo del salto del jugador en el eje X
	int posicionJUMP_Y; //Número representativo del salto del jugador en el eje Y
	double angulo;
	/* INSERTAR MÄS DATOS QUE REQUIERAN SER MANDADOS MEDIANTE birdPackage */
	/**********************************************************************/
};
#endif