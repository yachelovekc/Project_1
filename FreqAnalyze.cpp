#include "FreqAnalyze.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cmath>

using namespace std;

RandGen::RandGen(const vector<FreqItem>& input_items) 
    : items(input_items) {
    if (items.empty()) {
        throw invalid_argument("The sequence of values cannot be empty.");
    }
    
    vector<int> weights;
    
    ranges::transform(items, back_inserter(weights), 
        [](const FreqItem& item) { return item.frequency; });

    random_device rd;
    gen = mt19937(rd());
    dist = discrete_distribution<size_t>(weights.begin(), weights.end());
}

int RandGen::operator()() {
    return items[dist(gen)].value;
}


void ReadData (const string& inputFileName, int& N, vector<FreqItem>& items, long long& total_freq) {
    ifstream infile(inputFileName);
    if (!infile.is_open()) {
        throw runtime_error("Failed to open file " + inputFileName + ". Please check if it exists.");
    }

    string line;
    
    if (!getline(infile, line)) throw invalid_argument("Error reading N. The file is empty.");
    N = stoi(line);
    if (N <= 0) throw invalid_argument("N must be a positive natural number (greater than 0).");

    if (!getline(infile, line)) throw invalid_argument("Error reading the sequence of numbers.");
    istringstream iss_vals(line);
    vector<int> values;
    int val;
    while (iss_vals >> val) values.push_back(val);

    if (!getline(infile, line)) throw invalid_argument("Error reading frequencies.");
    istringstream iss_freqs(line);
    vector<int> frequencies;
    int freq;
    total_freq = 0;
    
    while (iss_freqs >> freq) {
        if (freq < 0) throw invalid_argument("Frequencies cannot be negative.");
        frequencies.push_back(freq);
        total_freq += freq;
    }

    if (values.size() != frequencies.size()) {
        throw invalid_argument("The number of values and the number of frequencies do not match.");
    }
    if (total_freq == 0) {
        throw invalid_argument("Total frequency must be greater than 0.");
    }

    for (size_t i = 0; i < values.size(); ++i) {
        items.push_back({values[i], frequencies[i]});
    }
}

map<int, int> GenFreq (int N, const vector<FreqItem>& items) {
    RandGen generator(items);
    map<int, int> generate;
    
    for (int i = 0; i < N; ++i) {
        generate[generator()]++;
    }
    
    return generate;
}

void WriteData (const string& outputFileName, int N, const vector<FreqItem>& items, long long total_freq, const map<int, int>& generated_counts) {
    ofstream outfile(outputFileName);
    if (!outfile.is_open()) {
        throw runtime_error("Failed to open file " + outputFileName + " for writing.");
    }

    outfile << "Input Values | Target Freq (%)  | Actual Freq (%)  | Discrepancy (%)\n";
    outfile << fixed << setprecision(4);

    vector<double> discrep;

    for (const auto& item : items) {
        double exp_prob = static_cast<double>(item.frequency) / total_freq * 100.0;
        
        int count = 0;
        if (generated_counts.contains(item.value)) {
            count = generated_counts.at(item.value);
        }
        
        double actual_prob = static_cast<double>(count) / N * 100.0;
        double discrepancy = abs(exp_prob - actual_prob);
        
        discrep.push_back(discrepancy);

        outfile << setw(12) << item.value << " | "
                << setw(16) << exp_prob << " | "
                << setw(16) << actual_prob << " | "
                << setw(15) << discrepancy << "\n";
    }

    auto max_iter = ranges::max_element(discrep, less<double>{});
    double max_discrep = (max_iter != discrep.end()) ? *max_iter : 0.0;

    outfile << "Max Frequency Discrepancy: " << max_discrep << " %\n";
}


void runFrequencyAnalysis (const string& inputFileName, const string& outputFileName) {
    int N = 0;
    vector<FreqItem> items;
    long long total_freq = 0;

    ReadData(inputFileName, N, items, total_freq);

    map<int, int> generated_counts = GenFreq(N, items);
    
    WriteData(outputFileName, N, items, total_freq, generated_counts);
}
