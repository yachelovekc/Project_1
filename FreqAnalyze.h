#include <vector>
#include <random>
#include <compare>
#include <string>
#include <map>

struct FreqItem {
    int value;
    int frequency;

    auto operator<=>(const FreqItem&) const = default;
};

class RandGen {
private:
    std::vector<FreqItem> items;
    std::discrete_distribution<std::size_t> dist;
    std::mt19937 gen;

public:
    explicit RandGen(const std::vector<FreqItem>& input_items);
    int operator()();
};

void ReadData (const std::string& inputFileName, int& N, std::vector<FreqItem>& items, long long& total_freq);
std::map<int, int> GenFreq (int N, const std::vector<FreqItem>& items);
void WriteData (const std::string& outputFileName, int N, const std::vector<FreqItem>& items, long long total_freq, const std::map<int, int>& generated_counts);

void runFrequencyAnalysis (const std::string& inputFileName, const std::string& outputFileName);
