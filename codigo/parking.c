#include "/home/oscar/.openmpi/include/mpi.h"
//#include <mpi.h>
#include <stdio.h>
#include "const.h"

unsigned int plazas, plantas, plazasTotales, plazasLibres;
int *parking;

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
    
    plazasTotales, plazasLibres = plazas * plantas;


    for(int i = 0;i < plazasTotales;i++)
        parking[i] = VACIA;


    printf("Parking de %d plazas y %d plantas creado.\n", plazas, plantas);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status estado;
    int fuente, dato, hueco;

    for(;;){
        MPI_Recv(&dato, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        // dato contiene:
        // El tipo de vehiculo si se trata de una entrada al parking o 
        // La plaza si se trata de la salida del mismo
        fuente = estado.MPI_SOURCE;
        if(estado.MPI_TAG == S_ENTRADA){
            hueco = buscoHuecoYAparco(dato, fuente);
            while(hueco == -1){
                if(dato == V_COCHE)  printf("ESPERA COCHE %d hasta que haya plaza.\n", fuente);
                if(dato == V_CAMION) printf("ESPERA CAMION %d hasta que haya plaza.\n", fuente);

                MPI_Recv(&hueco, 1, MPI_INT, MPI_ANY_SOURCE, S_SALIDA, MPI_COMM_WORLD, &estado);
                liberarPlaza(hueco);
                hueco = buscoHuecoYAparco(dato, fuente);
            }
            MPI_Send(&hueco, 1, MPI_INT, fuente, S_HUECO, MPI_COMM_WORLD);
            imprimirParking();
        } else if(estado.MPI_TAG == S_SALIDA){
            printf("SALIDA del vehiculo %d.\n", fuente);
            liberarPlaza(dato);
            imprimirParking();
        } else printf("Tag %d no valida.\n");
    }

    MPI_Finalize();
    return 0;
}

void liberarPlaza(int plaza){
    if(parking[plaza] == parking[plaza+1]) parking[plaza + 1] = VACIA;
    parking[plaza] = VACIA;
}

int buscoHuecoYAparco(int tipoV, int idV){
    if(plazasLibres < tipoV) return -1;
    int i, j;
    if(tipoV == V_COCHE){
        for(i = 0;i < plazasTotales;i++){
            if(parking[i] == VACIA){
                parking[i] = idV;
                plazasLibres--;
                printf("ENTRA COCHE %d en la plaza %d de la planta %d.\n", idV, i, j);
                return i;
            }
            
        }
        return -1;
    } else if(tipoV == V_CAMION){
        for(i = 0;i < plazasTotales;i++){
            if(parking[i] == VACIA && parking[i+1] == VACIA && !((i + 1) % plantas)){
                parking[i] = idV;
                parking[i+1] = idV;
                plazasLibres--;
                printf("ENTRA CAMION %d en la plaza %d de la planta %d.\n", idV, i, j);
                return i;
            }
        }
        return -1;
    } else {
        printf("Error. No es un vehículo válido.\n");
        return -2;  
    }
}

void imprimirParking(){
    int i, j;
    for(i = 0;i < plazas;i++){
        for(j = 0;j < plantas;j++){
            printf("[%d]");
        }
        printf("\n");
    }
}