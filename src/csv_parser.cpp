#include "../include/csv_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

bool CSVParser::parseLine(const string& line, RawRow& row) {
    stringstream ss(line);
    string token;
    double values[TOTAL_RAW_FEATURES_INC_LABEL];

    for (int i = 0; i < TOTAL_RAW_FEATURES_INC_LABEL; i++) {
        
        if (!getline(ss, token, ',')) {
            return false;
        }

        string clean = "";
        for (char c : token) {
            if (c != ' ' && c != '\r' && c != '\n')
                clean += c;
        }

        if (clean.empty()) 
        { 
            return false;
        }

        try {
            values[i] = stod(clean);
        } catch (...) {
            return false;   
        }
    }

 
    row.Pregnancies   = values[0];
    row.Glucose       = values[1];
    row.BloodPressure = values[2];
    row.SkinThickness = values[3];
    row.Insulin       = values[4];
    row.BMI           = values[5];
    row.Pedigree      = values[6];
    row.Age           = values[7];
    row.label         = (int)values[8];

    return true;
}


bool CSVParser::load(const string& filepath, RawRow* rawData, int& n) {
    ifstream file(filepath);

    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << filepath << endl;
        cout << "Make sure diabetes.csv is in " << "the data/ folder." << endl;
        return false;
    }

    string line;
    n = 0;

    // Skip header row
    getline(file, line);

    // Read data rows
    while (getline(file, line) && n < MAX_ROWS) {

        if (line.empty() || line == "\r") continue;

        RawRow row;
        if (parseLine(line, row)) {
            rawData[n++] = row;
        } else {
            cout << "Warning: Skipping malformed row " << n+1 << "\n";
        }
    }

    file.close();

    if (n == 0) {
        cout << "Error: No valid rows loaded" << endl;
        return false;
    }

    cout << "Loaded " << n << " rows from " << filepath << "\n";
    return true;

}