#include "mpi.h"

int main(int argc, char **argv) {
    MPI::Init();
    MPI::Intracomm &world = MPI::COMM_WORLD;

    int sum_ring, sum_red;

    int worldRank = world.Get_rank();
    if (world.Get_size() < 4 || world.Get_size() % 2 != 0)
    {
        if (worldRank == 0) {
            printf("Programm bitte mit geradzahliger Prozess-Anzahl >= 4 starten.\n");
        }
        MPI_Finalize();
        return 0;
    }

    const MPI::Intracomm &commLocal = world.Split(worldRank % 2, worldRank / 2);
    const int commLocalSize = commLocal.Get_size();
    const int commLocalRank = commLocal.Get_rank();

    sum_ring = worldRank;

    if (commLocalRank == 0)
    {
        commLocal.Send(&sum_ring, 1, MPI_INT, commLocalRank + 1, 0);
        commLocal.Recv(&sum_ring, 1, MPI_INT, commLocalSize - 1, 0);
    } else {
        commLocal.Recv(&sum_ring, 1, MPI_INT, commLocalRank - 1, 0);
        sum_ring += worldRank;
        if (commLocalRank == commLocalSize - 1) {
            // last send result to first
            commLocal.Send(&sum_ring, 1, MPI_INT, 0, 0);
        } else {
            // mid
            commLocal.Send(&sum_ring, 1, MPI_INT, commLocalRank + 1, 0);
        }
    }

    sum_red = 0;
    commLocal.Reduce(&worldRank, &sum_red, 1, MPI_INT, MPI_SUM, 0);
    if (commLocalRank == 0) {
        printf("comm: %d ; sum_ring = %d ; sum_red: %d\n", worldRank, sum_ring, sum_red);
    }

    MPI_Finalize();
    return 0;
}