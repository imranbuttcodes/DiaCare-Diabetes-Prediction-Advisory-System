#include "config.h"
#include "preprocessor.h"  

class CSVParser {
public:

    static bool load(const string& filepath, RawRow* rawData, int& n);

private:

  
    static bool parseLine(const string& line, RawRow& row);
};

