#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

// Base class for statistics
class StatisticsMC
{
public:
    virtual void DumpOneResult(double result) = 0;
    virtual std::vector<std::vector<double>> GetResultsSoFar() const = 0;
    virtual ~StatisticsMC() {}
};

// Derived class for calculating mean and statistics
class StatisticsMean : public StatisticsMC
{
public:
    StatisticsMean(double confidenceLevel) : confidenceLevel(confidenceLevel) {}

    virtual void DumpOneResult(double result)
    {
        results.push_back(result);
    }

    virtual std::vector<std::vector<double>> GetResultsSoFar() const
    {
        std::vector<std::vector<double>> resultsSoFar(1);
        double mean = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
        resultsSoFar[0].push_back(mean);

        double sumSquaredDiff = std::accumulate(results.begin(), results.end(), 0.0, [mean](double acc, double x)
                                                { return acc + (x - mean) * (x - mean); });

        double stdDev = std::sqrt(sumSquaredDiff / (results.size() - 1));
        resultsSoFar[0].push_back(stdDev);

        double meanStdDev = stdDev / std::sqrt(results.size());
        resultsSoFar[0].push_back(meanStdDev);

        double z = 1.96; // Assuming a 95% confidence interval, change as needed
        double lowerConfidenceLimit = mean - z * meanStdDev;
        double upperConfidenceLimit = mean + z * meanStdDev;
        resultsSoFar[0].push_back(lowerConfidenceLimit);
        resultsSoFar[0].push_back(upperConfidenceLimit);

        return resultsSoFar;
    }

private:
    std::vector<double> results;
    double confidenceLevel;
};

// Function to generate a random sample from a normal distribution
double GetOneGaussianByBoxMuller()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::normal_distribution<> distribution(0, 1);
    return distribution(generator);
}

// Monte Carlo simulation function
double SimpleMonteCarlo(double Expiry,
                        double Strike,
                        double Spot,
                        double Vol,
                        double r,
                        unsigned long NumberOfPaths)
{
    double variance = Vol * Vol * Expiry;
    double rootVariance = std::sqrt(variance);
    double itoCorrection = -0.5 * variance;

    double movedSpot = Spot * std::exp(r * Expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;

    for (unsigned long i = 0; i < NumberOfPaths; i++)
    {
        double thisGaussian = GetOneGaussianByBoxMuller();
        thisSpot = movedSpot * std::exp(rootVariance * thisGaussian);
        double thisPayoff = std::max(thisSpot - Strike, 0.0);
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberOfPaths;
    mean *= std::exp(-r * Expiry);
    return mean;
}

int main()
{
    double Expiry;
    double Strike;
    double Spot;
    double Vol;
    double r;
    unsigned long NumberOfPaths;
    double alpha;

    std::cout << "Enter expiry: ";
    std::cin >> Expiry;
    std::cout << "Enter strike: ";
    std::cin >> Strike;
    std::cout << "Enter spot: ";
    std::cin >> Spot;
    std::cout << "Enter volatility: ";
    std::cin >> Vol;
    std::cout << "Enter risk-free rate: ";
    std::cin >> r;
    std::cout << "Enter number of paths: ";
    std::cin >> NumberOfPaths;
    std::cout << "Enter confidence level (alpha): ";
    std::cin >> alpha;

    StatisticsMean gatherer(alpha);
    for (unsigned long i = 0; i < NumberOfPaths; i++)
    {
        double result = SimpleMonteCarlo(Expiry, Strike, Spot, Vol, r, NumberOfPaths);
        gatherer.DumpOneResult(result);
    }

    std::vector<std::vector<double>> results = gatherer.GetResultsSoFar();

    std::cout << "Averaged discounted payoff: " << results[0][0] << std::endl;
    std::cout << "Standard deviation of discounted payoffs: " << results[0][1] << std::endl;
    std::cout << "Standard deviation of averaged discounted payoffs: " << results[0][2] << std::endl;
    std::cout << "Lower " << alpha * 100 << "% confidence limit: " << results[0][3] << std::endl;
    std::cout << "Upper " << alpha * 100 << "% confidence limit: " << results[0][4] << std::endl;

    return 0;
}
