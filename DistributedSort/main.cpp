#include "stdafx.h"
#include <mpi.h>
#include "sort.h"
#pragma comment (lib, "msmpi.lib")


int main(int argc, char** argv)
{
	int rank, size;
	const auto N = 5;
	static int* masterNode;
	static auto init = false; //to avoid memory leaks

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (!init) { masterNode = new int[size]; init = true; }
	auto worker = new sort(N);

	worker->d_sort(masterNode, rank, N, size);
	worker->print(N, rank);

	delete worker;
	MPI_Finalize();
	
	delete[] masterNode;
	return 0;
}