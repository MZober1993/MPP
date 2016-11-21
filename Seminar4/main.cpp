#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char** argv)
{
    int rank;		// Rang des Prozesses
    int p;		// Anzahl Prozesse
    int root;		// Master-Prozess
    MPI_Status status;

    int buf=0;
    int cntMsgs = 100;
    int i;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    root = 0;
    if( rank == root) // Master-Prozess
    {
        // Auswertungsvariablen
        int *cntRecv;
        cntRecv = (int*)calloc( cntMsgs, sizeof( int ));
        for(i = 0; i< cntMsgs; i++)
        {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            cntRecv[status.MPI_SOURCE]++;
        }
        printf("Anzahl CPUs: %d\n", p);
        for(i = 1; i < p; i++)
        {
            printf("Empfangene Nachrichten von Prozess %d: %d\n", i, cntRecv[i]);
        }
    }else
    {
        for(i = 0; i< cntMsgs; i++)
        {
            MPI_Send( &buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}

