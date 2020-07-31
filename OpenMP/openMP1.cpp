#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <fstream>
#include <iostream>

int vectorFunction(int *array, int arraySize, int numThreads);

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        std::cout << "Program requires 3 arguments: <executable> <number of threads> <number of elements>\n";
        exit(1);
    }

    int numThreads = atoi(argv[1]);
    int numElements = atoi(argv[2]);
    int array[numElements];
    
    int i;
    int result, totalResult = 0;
    double totalTime = 0;
    double average = 0;

    for (int i = 0; i < 10; i++)
    {
        double start, end;
        start = omp_get_wtime();

        result = vectorFunction(array, numElements, numThreads);

        end = omp_get_wtime();

        totalTime = end - start;

        //std::cout << "Elements' sum: " << result << std::endl;
        //std::cout << "Time passed: " << totalTime << " seconds" << std::endl;

        average += totalTime;
        totalResult += result;
    }

    average /= 10;

    std::fstream file;
    file.open("outOpenMPSum", std::fstream::in | std::fstream::out | std::fstream::app);

    if (file.is_open())
    {
        file << "Number of Threads:" << numThreads << " ; Number of elements: "
             << numElements << " ;Result: " << totalResult/10 << " ;Total time: " << average << "\n";
    }
    else
    {
        std::cout << "ERROR! Could not write to file!" << std::endl;
    }

    file.close();

    return 0;
}

int vectorFunction(int *array, int arraySize, int numThreads)
{
    int sum = 0;

    int i=0;

#pragma omp parallel for shared(array) private(i) reduction(+ : sum) num_threads(numThreads)
    for (i = 0; i < arraySize; i++)
    {
        //#pragma omp critical
        array[i] = 1000;
        sum += array[i];

        //std::cout << "Index: " << i << "; Number inserted: " << i << std::endl;
        //std::cout << "Current thread number: " << omp_get_thread_num() << std::endl;
    }
    return sum;
}
