#include "/home/oscar/.openmpi/include/mpi.h"
//#include <mpi.h>
#include <stdio.h>
#include "const.h"

unsigned int plazas, plantas, plazasLibres;
int **parking;

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


    for(int i = 0;i < plazas;i++)
        for(int j = 0;i < plantas;j++)
            parking[i][j] = -1;


    printf("Parking de %d plazas y %d plantas creado.\n", plazas, plantas);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status estado;
    int hueco,fuente,tipoV;

    for(;;){
        MPI_Recv(&tipoV, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        //0 = entrada
        //1 = salida
        if(estado.MPI_TAG == S_ENTRADA){
            //Alguien quiere entrar    
            fuente = estado.MPI_SOURCE;
            int tengoSitio = buscoHuecoYAparco(tipoV, fuente);
            while(tengoSitio == -1){
                if(tipoV == V_COCHE)  printf("ESPERA COCHE %d hasta que haya plaza.\n", fuente);
                if(tipoV == V_CAMION) printf("ESPERA CAMION %d hasta que haya plaza.\n", fuente);
                //mpi_recv
                //si recibe, busco hueco otra vez
                tengoSitio = buscoHuecoYAparco(tipoV, fuente);
            }
            
        } else if(estado.MPI_TAG == 1){
            //Sale alguien
        }
    }

    MPI_Finalize();
    return 0;
}
int buscoHuecoYAparco(int tipoV, int idV){
    if(plazasLibres < tipoV) return -1;
    int i, j;
    if(tipoV == V_COCHE){
        for(i = 0;i < plazas;i++){
            for(j = 0;j < plantas;j++){
                if(parking[i][j] == VACIA){
                    parking[i][j] = idV;
                    printf("ENTRA COCHE %d en la plaza %d de la planta %d.\n", idV, i, j);
                    plazasLibres--;
                    return tipoV;
                }
            }
        }
        return -1;
    } else if(tipoV == V_CAMION){
        for(i = 0;i < plazas;i++){
            for(j = 0;j < plantas;j++){
                if(parking[i][j] == VACIA && parking[i+1][j+1] == VACIA && i != plazas-1){
                    parking[i][j] = idV;
                    parking[i+1][j+1] = idV;
                    printf("ENTRA CAMION %d en la plaza %d de la planta %d.\n", idV, i, j);
                    plazasLibres--;
                    return tipoV;
                }
            }
        }
        return -1;
    } else {
        printf("Error. No es un vehículo válido.\n");
        return -1;  
    }
}