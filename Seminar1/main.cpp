#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void userFun(int *, int *, int *, MPI_Datatype *);

int checkAddOp(int size, int i, int errors, int result, int correct_result);

void tryAnotherOp(int rank);

void easyReduceSample(int rank);

void userFun(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype) {
    for (int i = 0; i < *len; i++){
        inoutvec[i] *= invec[i]+1;
    }
}

void easyReduceSample(int rank) {
    int localsum[2] = {1, 2};
    int globalsum[2] = {0, 0};

    MPI_Reduce(localsum, globalsum, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("globalsum 1 = %d \n", globalsum[0]);
        printf("globalsum 2 = %d \n", globalsum[1]);
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
    std::cout << "result: " << result << " rank:"<<rank<< "\n";
}

int checkAddOp(int size, int i, int errors, int result, int correct_result) {
    correct_result = 0;
    for (i = 0; i < size; i++)
        correct_result += i;
    if (result != correct_result) errors++;
    return errors;
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
