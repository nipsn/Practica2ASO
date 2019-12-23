
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "const.h"
#include <time.h>


int main(int argc, char **argv){

int size,rank;
MPI_Init(NULL,NULL);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);


if(argc < 2){
  printf("Error en los argumentos\n");
  return -1;
}
printf("Coche %d creado", rank);


MPI_Barrier(MPI_COMM_WORLD);
MPI_Status estado;
int tipo=1;
int plaza,tiempo_espera,datosRecibidos; 

for(;;){
//obtiene un valor aleatorio para intentar entrar en el parking
srand(time(NULL));
tiempo_espera=rand() % 5;
sleep(tiempo_espera);
//Peticion para entrar en el parking
printf("EL COCHE %d QUIERE APARCAR EN EL PARKING\n",rank);
//MPI_Send();

//Espera hasta que el parking tenga una plaza libre
//MPI_Recv();
//MPI_Get_count();

//pasado un tiempo aleatorio sale del parking
srand(time(NULL));
tiempo_espera=rand() % 5;
sleep(tiempo_espera);

//MPI_send();
//MPI_Send();

}
MPI_Finalize();
}