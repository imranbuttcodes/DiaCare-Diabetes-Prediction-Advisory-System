#include <iostream>
#include <ctime>
using namespace std;

string getCurrentDate() {
    time_t now = time(0);
    tm time_struct;
    char buf[11];
    time_struct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d/%m/%Y", &time_struct);
    return string(buf);
}

int main() {
    
    cout << getCurrentDate() << endl;
    cout << "\n Navigate: "
             << "[N] Next  "
             << "[P] Previous  "
             << "[Q] Quit\n";
 
}