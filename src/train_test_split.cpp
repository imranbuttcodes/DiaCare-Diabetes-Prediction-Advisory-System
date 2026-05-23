#include "../include/train_test_split.h"

#include <cstdlib>
#include <ctime>

using namespace std;



void TrainTestSplit::shuffleData(RawRow* data, int n) {

    srand(42);

    // Fisher-Yates Shuffle
    for (int i = n - 1; i > 0; i--) {

        int j = rand() % (i + 1);

        RawRow temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }
}



void TrainTestSplit::split( RawRow* data, int n, RawRow* trainData, int& trainSize,
    RawRow* testData,
    int& testSize,

    double testRatio
) {

    //create temporary copy

    RawRow shuffled[MAX_ROWS];

    for (int i = 0; i < n; i++) {
        shuffled[i] = data[i];
    }


    shuffleData(shuffled, n);

    testSize = (int)(n * testRatio);

    trainSize = n - testSize;

    for (int i = 0; i < trainSize; i++) {
        trainData[i] = shuffled[i];
    }


    for (int i = 0; i < testSize; i++) {

        testData[i] = shuffled[trainSize + i];
    }
}