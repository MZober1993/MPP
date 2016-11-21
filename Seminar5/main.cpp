#include "mpi.h"

int main(int argc, char **argv) {
    MPI::Init();
    MPI::Intracomm &world = MPI::COMM_WORLD;
    MPI_Status status;

    int sum_ring, sum_red;

    if (world.Get_size() < 4 || world.Get_size() % 2 != 0)
    {
        if (world.Get_rank() == 0) {
            printf("Programm bitte mit geradzahliger Prozess-Anzahl >= 4 starten.\n");
        }
        MPI_Finalize();
        return 0;
    }

    const MPI::Intracomm &commLocal = world.Split(world.Get_rank() % 2, world.Get_rank() / 2);

    sum_ring = world.Get_rank();

    if (commLocal.Get_rank() == 0)
    {
        MPI_Send(&sum_ring, 1, MPI_INT, commLocal.Get_rank() + 1, 0, commLocal);
        MPI_Recv(&sum_ring, 1, MPI_INT, commLocal.Get_size() - 1, 0, commLocal, &status);
    } else {
        MPI_Recv(&sum_ring, 1, MPI_INT, commLocal.Get_rank() - 1, 0, commLocal, &status);
        sum_ring += world.Get_rank();
        if (commLocal.Get_rank() == commLocal.Get_size() - 1) {
            // last send result to first
            MPI_Send(&sum_ring, 1, MPI_INT, 0, 0, commLocal);
        } else {
            // mid
            MPI_Send(&sum_ring, 1, MPI_INT, commLocal.Get_rank() + 1, 0, commLocal);
        }
    }
    const int rank = world.Get_rank();

    sum_red = 0;
    MPI_Reduce(&rank, &sum_red, 1, MPI_INT, MPI_SUM, 0, commLocal);
    if (commLocal.Get_rank() == 0) {
        printf("comm: %d ; sum_ring = %d ; sum_red: %d\n", world.Get_rank(), sum_ring, sum_red);
    }

    MPI_Finalize();
    return 0;
}