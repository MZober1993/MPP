
#include <stdlib.h>
#include <mpi.h>

void userFun(const int *invec, int *inoutvec, int len, const MPI::Datatype &dtype) {
    for (int i = 0; i < len; i++) {
        inoutvec[i] *= invec[i] + 1;
    }
}

void tryAnotherOp(MPI::Intracomm world) {
    int worldRank = world.Get_rank();
    MPI::Op op = MPI::OP_NULL;
    op.Init((void (*)(const void *, void *, int, const MPI::Datatype &)) userFun, true);
    int result = 0;
    world.Reduce(&worldRank, &result, 1, MPI_INT, op, 0);
    world.Bcast(&result, 1, MPI_INT, 0);
    std::cout << "result: " << result << " rank:" << worldRank << "\n";
}

int main(int argc, char **argv) {
    MPI::Init();
    MPI::Intracomm &world = MPI::COMM_WORLD;

    tryAnotherOp(world);

    MPI_Finalize();

    return (EXIT_SUCCESS);
}
