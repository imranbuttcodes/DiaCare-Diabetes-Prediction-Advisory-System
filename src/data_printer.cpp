#include "../include/data_printer.h"
#include <iostream>

using namespace std;



void DataPrinter::headRaw(
    RawRow* data,
    int n,
    int rows
) {

    if (rows > n) {
        rows = n;
    }

    cout << endl;

    cout << "RAW DATA HEAD " << endl;;

    for (int i = 0; i < rows; i++) {

        cout << "\nRow " << i + 1 << endl;

        cout << "Pregnancies: " << data[i].Pregnancies << endl;

        cout << "Glucose: " << data[i].Glucose << endl;

        cout << "BloodPressure: " << data[i].BloodPressure << endl;

        cout << "SkinThickness: " << data[i].SkinThickness << endl;

        cout << "Insulin: " << data[i].Insulin << endl;

        cout << "BMI: " << data[i].BMI << endl;

        cout << "Pedigree: " << data[i].Pedigree << endl;

        cout << "Age: " << data[i].Age << endl;

        cout << "Label: " << data[i].label << endl;
    }
}



void DataPrinter::headProcessed(
    ProcessedRow* data,
    int n,
    int rows
) {

    if (rows > n) {
        rows = n;
    }

    cout << "PROCESSED DATA HEAD" << endl;

    for (int i = 0; i < rows; i++) {

        cout << "\nRow " << i + 1 << endl;

        for (int j = 0; j < NUM_FEATURES; j++) {

            cout << FEATURE_NAMES[j] << ": " << data[i].features[j] << endl;
        }

        cout << "Label: " << data[i].label << endl;
    }
}