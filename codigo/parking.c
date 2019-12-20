#include "/home/oscar/.openmpi/include/mpi.h"
//#include <mpi.h>
#include <stdio.h>
#include "const.h"

unsigned int plazas, plantas, plazasLibres;

int main(int argc, char **argv){
    int rank,size;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    

    if(argc == 3){
        plantas = atoi(argv[2]);
    } else if (argc == 2){
        plantas = 1;
    } else {
        printf("Error en los argumentos\n");
        return -1;
    }
    plazas = atoi(argv[1]);

    plazasLibres = plazas * plantas;

    int parking[plazas][plantas];

    for(int i = 0;i < plazas;i++)
        for(int j = 0;i < plantas;j++)
            parking[i][j] = -1;


    printf("Parking de %d plazas y %d plantas creado.\n", plazas, plantas);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status estado;
    int hueco,fuente,tipoV;

    for(;;){
        MPI_Recv(&tipoV, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        if(estado.MPI_TAG == ENTRADA){
            //Alguien quiere entrar            
        }
    }

    MPI_Finalize();
    return 0;
}