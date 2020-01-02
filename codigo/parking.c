#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "const.h"

unsigned int plazas, plantas, plazasTotales, plazasLibres;

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
    
    plazasTotales = plazas * plantas;
    plazasLibres = plazas * plantas;

    int parking[plazasTotales];

    int i = 0;
    for(i = 0;i < plazasTotales;i++)
        parking[i] = VACIA;


    printf("Parking de %d plazas y %d plantas creado.\n", plazas, plantas);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status estado;
    int fuente, dato, hueco, numDatosRecibidos;
    
    for(;;){
        MPI_Recv(&dato, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        MPI_Get_count( &estado, MPI_INT, &numDatosRecibidos );

        // dato contiene:
        // El tipo de vehiculo si se trata de una entrada al parking o 
        // La plaza si se trata de la salida del mismo
        fuente = estado.MPI_SOURCE;
        if(estado.MPI_TAG == S_ENTRADA){
            hueco = buscoHuecoYAparco(dato, fuente, parking);
            while(hueco == -1){
                if(dato == V_COCHE)  printf("ESPERA COCHE %d hasta que haya plaza.\n", fuente);
                if(dato == V_CAMION) printf("ESPERA CAMION %d hasta que haya plaza.\n", fuente);

                MPI_Recv(&hueco, 1, MPI_INT, MPI_ANY_SOURCE, S_SALIDA, MPI_COMM_WORLD, &estado);
                liberarPlaza(hueco, parking);
                hueco = buscoHuecoYAparco(dato, fuente, parking);
            }
            MPI_Send(&hueco, 1, MPI_INT, fuente, S_HUECO, MPI_COMM_WORLD);
            imprimirParking(parking);
        } else if(estado.MPI_TAG == S_SALIDA){
            printf("SALIDA del vehiculo %d.\n", fuente);
            liberarPlaza(dato, parking);
            imprimirParking(parking);
        } else printf("Tag no valida.\n");
    }

    MPI_Finalize();
    return 0;
}

void liberarPlaza(int plaza, int *parking){
    if(parking[plaza] == parking[plaza+1]) {
        parking[plaza + 1] = VACIA;
        plazasLibres++;
    }
    parking[plaza] = VACIA;
    plazasLibres++;
}

int buscoHuecoYAparco(int tipoV, int idV, int *parking){
    if(plazasLibres < tipoV) return -1;
    int i;
    if(tipoV == V_COCHE){
        for(i = 0;i < plazasTotales;i++){
            if(parking[i] == VACIA){
                parking[i] = idV;
                plazasLibres--;
                printf("ENTRA COCHE %d en la plaza %d.\n", idV, i);
                return i;
            }
            
        }
        return -1;
    } else if(tipoV == V_CAMION){
        for(i = 0;i < plazasTotales;i++){
            if(parking[i] == VACIA && parking[i+1] == VACIA && (i + 1) % plazas){
                parking[i] = idV;
                parking[i+1] = idV;
                plazasLibres -= 2;
                printf("ENTRA CAMION %d en la plaza %d.\n", idV, i);
                return i;
            }
        }
        return -1;
    } else {
        printf("Error. No es un vehículo válido.\n");
        return -2;  
    }
}

void imprimirParking(int *parking){
    int i;
    for(i = 1;i <= plazas * plantas;i++){
        printf("[%d]", parking[i-1]);
        if(i % plazas == 0)
            printf("\n");
    }
    printf("\n");
}