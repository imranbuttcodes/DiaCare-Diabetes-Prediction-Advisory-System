#include "config.h"

class TrainTestSplit {
public:

    static void split(
        RawRow* data,
        int n,

        RawRow* trainData,
        int& trainSize,

        RawRow* testData,
        int& testSize,

        double testRatio = 0.2
    );

private:

    static void shuffleData(
        RawRow* data,
        int n
    );
};

