#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>

double sunspotCount(int time, double verticalShift);
double mean(std::vector<double>& data);
double stDeviation(std::vector<double>& data);



std::vector<std::vector<double>> vectorSubsamples = {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}};
std::vector<double> subsampleMeans = {};
std::vector<double> subsampleUncertainties = {};

double solarActivity = 0;
const double pi = M_PI;
const double amplitude = 20;
const double period = 11;
double phaseShift = 0;



int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Normal distributions for growth rates
    std::normal_distribution<> verticalShift(100, 10);


    std::ofstream outfile;
    outfile.open ("solar_activity.txt");
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 11; j++) {
            outfile << sunspotCount(1, verticalShift(gen));
            for (int k = 2; k <= 12; k++) {
                outfile << " " << sunspotCount(k, verticalShift(gen));
            }
        }
        outfile << std::endl;
    }
    outfile.close();

    std::ifstream infile;
    infile.open ("solar_activity.txt");
    double monthlySunspotCount;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 11; j++) {
            for (int k = 0; k < 12; k++) {
                infile >> monthlySunspotCount;
                vectorSubsamples[i].push_back(monthlySunspotCount);
            }
        }
    }
    infile.close();

    for (int i = 0; i < 10; i++) {
        double Mean = mean(vectorSubsamples[i]);
        double uncertainty = stDeviation(vectorSubsamples[i]);
        std::cout << "Average number of sunspots in cycle " << i + 1 << ": " << Mean << " ± " << uncertainty << std::endl;
        subsampleMeans.push_back(Mean);
        subsampleUncertainties.push_back(uncertainty);
    }
     std::cout << "Average number of sunspots in all cycles: " << mean(subsampleMeans) << " ± " << stDeviation(subsampleMeans) << std::endl;

}

double sunspotCount(int time, double verticalShift) {
    return (amplitude * sin(((2 * pi)/(period)) * (time - phaseShift))) + verticalShift;
}

double mean(std::vector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double stDeviation(std::vector<double>& data) {
    double squareSum = 0;
    for (double x : data) {
        squareSum += std::pow(x - mean(data), 2);
    }
    return std::sqrt(squareSum / data.size());
}