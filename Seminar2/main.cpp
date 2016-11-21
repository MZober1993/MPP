#include <stdio.h>

#include "mpi.h"

static const int ROOT = 0;

double f(double x) {
    return 4 / (1.0 + x * x);
}

double trapezoid(MPI::Intracomm world, long n, double a, double b) {
    long local_n = n / world.Get_size();
    double baseWidth= (b - a) / n;
    double leftEndPoint = a +  world.Get_rank() * local_n * baseWidth;
    double rightEndPoint = leftEndPoint + local_n * baseWidth;

    double integral = (f(leftEndPoint) + f(rightEndPoint)) / 2.0;
    double x = leftEndPoint;
    for (int i = 1; i <= local_n - 1; i++) {
        x += baseWidth;
        integral += f(x);
    }
    integral *= baseWidth;
    return integral;
}

int main(int argc, char **argv) {
    MPI::Init();
    MPI::Intracomm &world = MPI::COMM_WORLD;
    int worldRank = world.Get_rank();

    double a = 0.0;
    double b = 1.0;
    long int n;
    double integral, total;

    if (argc <= 1) {
        printf("Usage: trapez n\n");
        MPI_Finalize();
        exit(0);
    } else {
        n = atol(argv[1]);
    }
    integral = trapezoid(world, n, a, b);
    world.Reduce(&integral, &total, 1, MPI_DOUBLE, MPI_SUM, ROOT);

    if (worldRank == ROOT) {
        std::cout << "total: " << total << std::endl;
    }
    MPI_Finalize();
    return 0;
}
