//
//  SocketDatagrama.cpp
//  
//
//  Created by Eduardo Fernandez on 21/05/15.
//
//

#include "SocketDatagrama.h"


SocketDatagrama::SocketDatagrama(int puerto)
{
    s = socket(AF_INET, SOCK_DGRAM, 0);
    bzero((char *)&direccionLocal, sizeof(direccionLocal));
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY;
    direccionLocal.sin_port = htons(puerto);
    bind(s, (struct sockaddr *)&direccionLocal,sizeof(direccionLocal));
}

SocketDatagrama::~SocketDatagrama()
{
//    close(s);
}


int SocketDatagrama::recibe(PaqueteDatagrama & p)
{
    unsigned int addr_len = sizeof(direccionForanea);
    bzero((char *)&direccionForanea, sizeof(direccionForanea));
    int regreso = (int)recvfrom(s,p.obtieneDatos(),p.obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, &addr_len);
    
    p.inicializaPuerto(ntohs(direccionForanea.sin_port));
    p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    //std::cout << "Mensaje recibido de: " << inet_ntoa(direccionForanea.sin_addr) << ":" << ntohs(direccionForanea.sin_port) << std::endl;
    return regreso;
}


int SocketDatagrama::envia(PaqueteDatagrama & p)
{
    bzero((char *)&direccionForanea, sizeof(direccionForanea));
    direccionForanea.sin_family = AF_INET;
    direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
    direccionForanea.sin_port = htons(p.obtienePuerto());
    
    return (int)sendto(s,p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, sizeof(direccionForanea));
}

void SocketDatagrama::activaBroadcast(bool opcion)
{
	int yes = opcion ? 1 : 0;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes,sizeof(int));
}


void SocketDatagrama::setTiempoEspera(double seconds)
{
	struct timeval tiempoFuera;
	tiempoFuera.tv_sec = 0;
	tiempoFuera.tv_usec = 500000 * seconds;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tiempoFuera, sizeof(tiempoFuera));
}

