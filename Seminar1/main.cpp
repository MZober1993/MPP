
#include <stdlib.h>
#include <mpi.h>

void userFun(int *, int *, int *, MPI_Datatype *);

void tryAnotherOp(int rank);

void userFun(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype) {
    for (int i = 0; i < *len; i++) {
        inoutvec[i] *= invec[i] + 1;
    }
}

void tryAnotherOp(int rank) {
    int result = 0;
    int data;
    MPI_Op op;
    data = rank;
    MPI_Op_create((MPI_User_function *) userFun, 1, &op);
    MPI_Reduce(&data, &result, 1, MPI_INT, op, 0, MPI_COMM_WORLD);
    MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Op_free(&op);
    std::cout << "result: " << result << " rank:" << rank << "\n";
}

int main(int argc, char **argv) {
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    tryAnotherOp(rank);

    MPI_Finalize();

    return (EXIT_SUCCESS);
}
