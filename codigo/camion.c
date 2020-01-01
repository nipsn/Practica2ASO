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


/*if(argc < 2){
  printf("Error en los argumentos\n");
  return -1;
}*/
printf("Camion %d creado", rank);


MPI_Barrier(MPI_COMM_WORLD);
MPI_Status estado;
int tipo=V_CAMION;
int plaza,tiempo_espera; 

for(;;){
//obtiene un valor aleatorio para intentar entrar en el parking
srand(time(NULL));
tiempo_espera=rand() % 5;
sleep(tiempo_espera);
//Peticion para entrar en el parking
printf("EL CAMION %d QUIERE APARCAR en el parking\n",rank);
MPI_Send(&tipo,1,MPI_INT, N_PARKING,S_ENTRADA,MPI_COMM_WORLD);

//Espera recibir señal hasta que el parking tenga una plaza libre
MPI_Recv(&plaza,1,MPI_INT,N_PARKING,S_HUECO,MPI_COMM_WORLD,&estado);


//pasado un tiempo aleatorio sale del parking
srand(time(NULL));
tiempo_espera=rand() % 5;
sleep(tiempo_espera);


MPI_Send(&plaza,1,MPI_INT,N_PARKING,S_SALIDA,MPI_COMM_WORLD); 
}
MPI_Finalize();
}