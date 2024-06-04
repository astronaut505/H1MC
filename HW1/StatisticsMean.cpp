#pragma once
#ifndef STATISTICS_MEAN_H
#define STATISTICS_MEAN_H

#include "StatisticsMC.h"
#include <vector>

class StatisticsMean : public StatisticsMC {
public:
    StatisticsMean();
    virtual ~StatisticsMean();

    virtual void DumpOneResult(double result);
    virtual std::vector<std::vector<double>> GetResultsSoFar() const;
    virtual StatisticsMC* clone() const;

private:
    std::vector<double> PathDependent;
    std::vector<std::vector<double>> ResultsSoFar;
};

#endif