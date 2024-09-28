#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>


//class to define all the numbers we care about
class Sample{
   public:
    float mean;
    float mean_uncertainty;
};

//function to calculate solar activity based on parameters
float Solar_activity(double vertical_shift, int time_point){ 
    const int time_shift = 0;
    const int Period = 11;
    const int Amplitude = 20;
    float Solar_activity;
    Solar_activity = (Amplitude*sin((2*M_PI/Period)*(time_point - time_shift))) + vertical_shift;
    return Solar_activity;
}

//calculate results for individual samples -> subset technique
Sample Calculate_Sample(std::vector<float> substringvector){
    Sample Sample;
    Sample.mean = std::accumulate(substringvector.begin(), substringvector.end(), 0.0) / substringvector.size();
    int sum = 0;
    for (float number = 0; number < substringvector.size(); number++) {
        sum += std::pow((substringvector[number] - Sample.mean), 2);
    }
    Sample.mean_uncertainty = std::sqrt(sum / substringvector.size());

    return Sample;
}

//bring the subsets together with function
Sample Calculate_Results(std::vector<Sample> Samplevector){
    Sample Results;
    std::vector<float> Mean_vector;
    std::vector<float> Uncertainty_vector;
    for (int number = 0; number < Samplevector.size(); number++){
        Mean_vector.push_back(Samplevector[number].mean);
        Uncertainty_vector.push_back(Samplevector[number].mean_uncertainty);
    }

    Results.mean = std::accumulate(Mean_vector.begin(), Mean_vector.end(), 0.0) / Mean_vector.size();
    
    float sum = 0;
    for (float number = 0; number < Mean_vector.size(); number++) {
        sum += std::pow((Mean_vector[number] - Results.mean), 2);
    }

    Results.mean_uncertainty =  std::sqrt(sum / Mean_vector.size());
    return Results;
}

int main(){
    //variables needed to generate pseudo data
    std::string outputstring = "";
    std::random_device randomd;
    std::mt19937 generator(randomd());
    std::normal_distribution<> vertical_shift(100, 10);

    //generating pseudo data for each cycle
    const int max_cycles = 10;
    const int max_year = 11;
    const int max_month = 12;
    for (int cycles = 0; cycles < max_cycles; cycles++){
        for (int year = 0; year < max_year; year ++){
            for (int month = 0; month < max_month; month++){
                std::cout << "Cycle: " << cycles << ", Year: " << year << ", Month: " << month << "\n";
                outputstring += std::to_string((Solar_activity(vertical_shift(generator), year))) + " ";
            }
        }
        outputstring.pop_back();
        outputstring += "\n";
    }

    //printing output to file
    std::cout << outputstring;

    //writing to file
    std::ofstream ofile;
    ofile.open ("Solar_activity.txt");
    ofile << outputstring;
    ofile.close();

    //variables needed to read file and calculate results
    Sample Results;
    std::string line;
    std::string substring;
    std::vector<float> substringvector;
    std::vector<Sample> Samplevector;

    //opening and reading file line by line
    std::ifstream ifile ("Solar_activity.txt");
    if (ifile.is_open()){
        while (std::getline(ifile, line)){

            for (int digit=0; digit <= line.length(); digit++){
                if (line[digit] != ' ' and digit<line.length()){
                substring += line[digit];
                }
                else{
                substringvector.push_back(stof(substring));
                substring = "";
                }
            }

            //Calculate the result
            Samplevector.push_back(Calculate_Sample(substringvector));
        }
            Results = Calculate_Results(Samplevector);

        
            //printing results to terminal
            for (int sample = 0; sample < Samplevector.size(); sample++){
                std::cout <<  std::fixed << "Mean of sample " <<  sample <<": " << Samplevector[sample].mean << " +- " << Samplevector[sample].mean_uncertainty << "\n";
            }
            std::cout <<  std::fixed << "Amount of Samples: " << Samplevector.size() << "\n";
            std::cout <<  std::fixed << "Mean: " << Results.mean << " +- " << Results.mean_uncertainty << "\n";
        
    }
}
