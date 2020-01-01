#ifndef _CONST_H
#define _CONST_H

// Señales
#define S_ENTRADA 0
#define S_HUECO 1
#define S_SALIDA 2


#define V_COCHE 1
#define V_CAMION 2

#define VACIA -1

// Nodos
#define N_PARKING 0

void liberarPlaza(int plaza, int *parking);
int buscoHuecoYAparco(int tipoV, int idV, int *parking);
void imprimirParking(int *parking);

#endif