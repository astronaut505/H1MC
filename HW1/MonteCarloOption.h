#pragma once
#ifndef MONTE_CARLO_OPTION_H
#define MONTE_CARLO_OPTION_H

class MonteCarloOption {
public:
    MonteCarloOption(double expiry, double strike, double spot, double volatility, double riskFreeRate, unsigned long numberOfPaths);
    ~MonteCarloOption();

    double optionPrice() const; // method to calculate the option price using Monte Carlo simulation

private:
    double Expiry;
    double Strike;
    double Spot;
    double Volatility;
    double RiskFreeRate;
    unsigned long NumberOfPaths;
};

#endif