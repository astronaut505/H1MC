#include "MonteCarloOption.h"
#include <iostream>

int main() {
    double expiry = 1.0; // 1 year
    double strike = 100.0; // strike price of $100
    double spot = 100.0; // current price is $100
    double volatility = 0.2; // 20% volatility
    double riskFreeRate = 0.05; // 5% risk-free interest rate
    unsigned long numberOfPaths = 10000; // simulate 10,000 paths

    MonteCarloOption option(expiry, strike, spot, volatility, riskFreeRate, numberOfPaths);

    double price = option.optionPrice();

    std::cout << "The price of the option is: " << price << std::endl;

    return 0;
}