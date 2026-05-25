#include "include/diacare_manager.h"
using namespace std;

int main() {

    
    DiaCareManager mgr;
    mgr.startup();
    mgr.run();
    
    return 0;
}