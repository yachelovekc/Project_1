#include "FreqAnalyze.h"
#include <iostream>
using namespace std;

int main() {
    try {
        runFrequencyAnalysis("input.txt", "output.txt");
        
        cout << "Program executed successfully. Results saved in output.txt\n";
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}