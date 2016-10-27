#include <stdio.h>

#include "mpi.h"

static const int ROOT = 0;

/* Funktion, welche integriert wird */
double f(double x) {
    return 4 / (1.0 + x * x);
}

/* lokales Integral berechnen */
double trapezoid(int p, int rank, long n, double a, double b) {
    double h; //Basisbreite des Trapez'
    double local_a; // linker Endpunkt
    double local_b; // rechter Endpunkt
    int local_n; // Anzal Trapezoide der lokalen Berechnung
    double integral; // Ergebnis

    double x;
    int i;

    h = (b - a) / n; // fuer alle Prozesse gleich
    local_n = n / p; // gilt auch fuer die Anzahl der Trapeze

    //Laenge des Integrations-Intervalls jedes Prozesses = local_n*h
    //Somit startet das lokale Intervall bei:
    local_a = a + rank * local_n * h;
    // und endet bei:
    local_b = local_a + local_n * h;

    integral = (f(local_a) + f(local_b)) / 2.0;
    x = local_a;
    for (i = 1; i <= local_n - 1; i++) {
        x += h;
        integral += f(x);
    }
    return integral *= h;
}

int main(int argc, char **argv) {
    MPI::Init();
    MPI::Intracomm &world = MPI::COMM_WORLD;

    double a = 0.0;
    double b = 1.0;
    long int n;
    double integral,total;
    
    if (argc <= 1) {
        printf("Usage: trapez n\n");
        MPI_Finalize();
        exit(0);
    } else {
        n = atol(argv[1]);
    }
    integral = trapezoid(world.Get_size(), world.Get_rank(), n, a, b);
    MPI_Reduce(&integral, &total, 1, MPI_DOUBLE, MPI_SUM, ROOT, world);
    if (MPI::COMM_WORLD.Get_rank() == ROOT) {
        std::cout << "total: " << total << std::endl;
    }
    MPI_Finalize();
    return 0;
}
