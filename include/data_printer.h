#pragma once
#include "config.h"

class DataPrinter {
public:

    static void headRaw(
        RawRow* data,
        int n,
        int rows = 5
    );

    static void headProcessed(
        ProcessedRow* data,
        int n,
        int rows = 5
    );
};

