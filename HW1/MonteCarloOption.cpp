#include "MonteCarloOption.h"
#include <cmath> // for std::exp
#include <random> // for std::normal_distribution

MonteCarloOption::MonteCarloOption(double expiry, double strike, double spot, double volatility, double riskFreeRate, unsigned long numberOfPaths)
    : Expiry(expiry), Strike(strike), Spot(spot), Volatility(volatility), RiskFreeRate(riskFreeRate), NumberOfPaths(numberOfPaths) {
    // The constructor initializes the member variables with the parameters passed in
}

MonteCarloOption::~MonteCarloOption() {
    // No dynamic memory to clean up, so the destructor is empty
}

double MonteCarloOption::optionPrice() const {
    std::mt19937_64 generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    double variance = Volatility * Volatility * Expiry;
    double rootVariance = std::sqrt(variance);
    double itoCorrection = -0.5 * variance;

    double movedSpot = Spot * std::exp(RiskFreeRate * Expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;

    for (unsigned long i = 0; i < NumberOfPaths; i++) {
        double thisGaussian = distribution(generator);
        thisSpot = movedSpot * std::exp(rootVariance * thisGaussian);
        double thisPayoff = thisSpot - Strike;
        thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberOfPaths;
    mean *= std::exp(-RiskFreeRate * Expiry);

    return mean;
}