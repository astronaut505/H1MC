#include "StatisticsMC.h"

StatisticsMC::StatisticsMC(const std::vector<double>& numbers) : PathDependent(numbers) {
    // The constructor initializes the PathDependent vector with the numbers passed in
}

StatisticsMC::~StatisticsMC() {
    // No dynamic memory to clean up, so the destructor is empty
}