#include <iostream>

#include "include/config.h"
#include "include/csv_parser.h"
#include "include/train_test_split.h"
#include "include/preprocessor.h"
#include "include/data_printer.h"

using namespace std;

int main() {


    RawRow rawData[MAX_ROWS];

    int n = 0;

    bool loaded =
        CSVParser::load(
            DATASET_PATH,
            rawData,
            n
        );

    if (!loaded) {

        cout << "Failed to load dataset" << endl;
        return 1;
    }

    

    cout << "Dataset loaded successfully" << endl;
    cout << "Total rows: " << n << endl;


    DataPrinter::headRaw(rawData, n, 5);

    RawRow trainData[MAX_ROWS];
    RawRow testData[MAX_ROWS];

    int trainSize;
    int testSize;

    TrainTestSplit::split(
        rawData,
        n,
        trainData,
        trainSize,

        testData,
        testSize,

        0.2
    );

    cout << "\nTrain-Test Split Complete\n";

    cout << "Train Size: "  << trainSize << endl;

    cout << "Test Size: " << testSize << endl;




    Preprocessor prep;

    ProcessedRow processedTrain[MAX_ROWS];
    ProcessedRow processedTest[MAX_ROWS];

    prep.fitTransform(
        trainData,
        trainSize,
        processedTrain
    );

    prep.transformBatch(
        testData,
        testSize,
        processedTest
    );




    prep.printImputeParams();

    prep.printCapParams();

    prep.printScalerParams();


    cout << endl;
    cout << "Sample Processed Training Rows" << endl;;

    DataPrinter::headProcessed(processedTrain,trainSize, 5);
    system("pause");
    return 0;
}