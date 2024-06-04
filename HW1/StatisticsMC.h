#pragma once
#ifndef STATISTICS_MC_H
#define STATISTICS_MC_H

#include <vector>

class StatisticsMC {
public:
    StatisticsMC(const std::vector<double>& numbers);
    virtual ~StatisticsMC();

    virtual void DumpOneResult(double result) = 0; // Pure virtual function
    virtual std::vector<std::vector<double>> GetResultsSoFar() const = 0; // Pure virtual function
    virtual StatisticsMC* clone() const = 0; // Pure virtual function

protected:
    std::vector<double> PathDependent; // Stores the set of numbers
};

#endif