#ifndef ANALYSEDATA_H
#define ANALYSEDATA_H
//to compile: g++ -o main -no-pie ../src/main/main.cpp -lboost_serialization
//RECOMMENDED: compile with cmake using 'cmake --build build && ./build/DataAnalysis' in the DATAANALYSISINTROTOPROGRAMMING directory (no subdirectory)
//standard libs
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <numeric>
//for timekeeping and pausing
#include <chrono>
#include <thread>
//for serialization
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include "particle_classes.h"
#include "serialization.h"
#include "readandcalc.h"



//Function to calculate certain averages etc from particle events
Analysed_Data Analyse_Data(raw_Data raw_Data, Analysed_Data Analysed_Data){

  //Gathering overall count of Omega particles in 5 million collisions
  Analysed_Data.Omegaminus_count = 0;
  Analysed_Data.Omegaplus_count = 0;
  Analysed_Data.Omega_event_equal_amount_count = 0;

  //used to calculate amount per event
  uint32_t Omega_minus_per_event_count = 0;
  uint32_t Omega_plus_per_event_count = 0;

  //Variables used for pseudo-rapidity and transverse momentum
  int transversePposition = 0;
  int pseudorapidityposition = 0;
  const int offset_transverseP = 0;
  const int offset_pseudorapidity = 4;
  const float step_transverseP = 0.5;
  const float step_pseudorapidity = 1;

  std::vector<double> Sample_Omega_minus_mean_vector;
  std::vector<double> Sample_Omega_minus_mean_uncertainty_vector;
  std::vector<double> Sample_Omega_minus_skewness_vector;
  std::vector<double> Sample_Omega_minus_skewness_uncertainty_vector;
  std::vector<double> Sample_Omega_minus_kurtosis_vector;

  std::vector<double> Sample_Omega_plus_mean_vector;
  std::vector<double> Sample_Omega_plus_mean_uncertainty_vector;
  std::vector<double> Sample_Omega_plus_skewness_vector;
  std::vector<double> Sample_Omega_plus_skewness_uncertainty_vector;
  std::vector<double> Sample_Omega_plus_kurtosis_vector;

  //gather info on pseudo-rapidity and transverse momentum
  int sample = 0;
  int past_sample_events = 0;
  int past_Omega_minus_count = 0;
  int past_Omega_plus_count = 0;
  for (int event= 0; event < raw_Data.Particle_event_vector.size(); event++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event].Particles.size(); particle++){
      if (abs(raw_Data.Particle_event_vector[event].Particles[particle].PGC) == 3334){
        //formula is floor((value + offset) / step)
        transversePposition = floor(raw_Data.Particle_event_vector[event].Particles[particle].transverseP + offset_transverseP / step_transverseP);
        pseudorapidityposition  = floor((raw_Data.Particle_event_vector[event].Particles[particle].pseudorapidity + offset_pseudorapidity) / step_pseudorapidity);
        //MATTER OMEGA MINUS
        if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == 3334){
          Analysed_Data.Omegaminus_count++;
          Omega_minus_per_event_count++;
          
          while (transversePposition >= Analysed_Data.Results.matter_transverseP_vector.size()){
            Analysed_Data.Results.matter_transverseP_vector.push_back(0);
          }
          Analysed_Data.Results.matter_transverseP_vector[transversePposition]++;
          while (pseudorapidityposition >= Analysed_Data.Results.matter_pseudo_rapidity_vector.size()){
            Analysed_Data.Results.matter_pseudo_rapidity_vector.push_back(0);
          }
          Analysed_Data.Results.matter_pseudo_rapidity_vector[pseudorapidityposition]++;
        }
        //ANTI MATTER OMEGA PLUS
        else if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == -3334){
          Analysed_Data.Omegaplus_count++;
          Omega_plus_per_event_count++;

          while (transversePposition >= Analysed_Data.Results.antimatter_transverseP_vector.size()){
            Analysed_Data.Results.antimatter_transverseP_vector.push_back(0);
          }
          Analysed_Data.Results.antimatter_transverseP_vector[transversePposition]++;
          while (pseudorapidityposition >= Analysed_Data.Results.antimatter_pseudo_rapidity_vector.size()){
            Analysed_Data.Results.antimatter_pseudo_rapidity_vector.push_back(0);
          }
          Analysed_Data.Results.antimatter_pseudo_rapidity_vector[pseudorapidityposition]++;
        }
      }
    }

    //check for equal Omega plus and Omega minus count events
    if ((Omega_minus_per_event_count == Omega_plus_per_event_count) and ((Omega_minus_per_event_count != 0) or (Omega_plus_per_event_count != 0))){
      Analysed_Data.Omega_event_equal_amount_count++;
    }

    //reset variables per event
    Omega_minus_per_event_count = 0;
    Omega_plus_per_event_count = 0;
  }

  //calculating mean
  sample = 0;
  past_sample_events = 0;
  past_Omega_minus_count = 0;
  past_Omega_plus_count = 0;
  for (int event= 0; event < raw_Data.Particle_event_vector.size(); event++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event].Particles.size(); particle++){
      if (abs(raw_Data.Particle_event_vector[event].Particles[particle].PGC) == 3334){
        if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == 3334){
          Omega_minus_per_event_count++;
        }
        else if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == -3334){
          Omega_plus_per_event_count++;
        }
      }
    }
      if ((event == (raw_Data.Samplesizes[sample] + past_sample_events)) or event == raw_Data.Particle_event_vector.size()-1){
        //Omega minus mean,
        Sample_Omega_minus_mean_vector.push_back((Analysed_Data.Omegaminus_count - past_Omega_minus_count) / (double)raw_Data.Samplesizes[sample]);
        //Omega plus mean
        Sample_Omega_plus_mean_vector.push_back((Analysed_Data.Omegaplus_count - past_Omega_plus_count) / (double)raw_Data.Samplesizes[sample]);
        //resetting variables per sample
        past_Omega_minus_count = Analysed_Data.Omegaminus_count;
        past_Omega_plus_count = Analysed_Data.Omegaplus_count;
        past_sample_events += raw_Data.Samplesizes[sample];
        sample++;
      }

        //reset variables per event
        Omega_minus_per_event_count = 0;
        Omega_plus_per_event_count = 0;
    }

  //calculating mean uncertainty
  sample = 0;
  past_sample_events = 0;
  past_Omega_minus_count = 0;
  past_Omega_plus_count = 0;
  double Sample_Omega_minus_subtr_mean_sum_squared = 0;
  double Sample_Omega_minus_subtr_mean_sum_cubed = 0;
  double Sample_Omega_minus_subtr_mean_sum_pow4 = 0;
  double Sample_Omega_plus_subtr_mean_sum_squared = 0;
  double Sample_Omega_plus_subtr_mean_sum_cubed = 0;
  double Sample_Omega_plus_subtr_mean_sum_pow4 = 0;

  for (int event= 0; event < raw_Data.Particle_event_vector.size(); event++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event].Particles.size(); particle++){
      if (abs(raw_Data.Particle_event_vector[event].Particles[particle].PGC) == 3334){
        if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == 3334){
          Sample_Omega_minus_subtr_mean_sum_squared += pow(1 - Sample_Omega_minus_mean_vector[sample],2);
          Sample_Omega_minus_subtr_mean_sum_cubed += pow(1 - Sample_Omega_minus_mean_vector[sample],3);
          Sample_Omega_minus_subtr_mean_sum_pow4 += pow(1 - Sample_Omega_minus_mean_vector[sample],4);
        }
        else if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == -3334){
          Sample_Omega_plus_subtr_mean_sum_squared += pow(1 - Sample_Omega_plus_mean_vector[sample],2);
          Sample_Omega_plus_subtr_mean_sum_cubed +=  pow(1 - Sample_Omega_plus_mean_vector[sample],3);
          Sample_Omega_plus_subtr_mean_sum_pow4 +=  pow(1 - Sample_Omega_plus_mean_vector[sample],4);
        }
      }
    }
      if ((event == (raw_Data.Samplesizes[sample] + past_sample_events)) or event == raw_Data.Particle_event_vector.size()-1){
        //Omega mean uncertainty
        Sample_Omega_minus_mean_uncertainty_vector.push_back(sqrt(Sample_Omega_minus_subtr_mean_sum_squared / (double)raw_Data.Samplesizes[sample]));
        Sample_Omega_plus_mean_uncertainty_vector.push_back(sqrt(Sample_Omega_plus_subtr_mean_sum_squared / (double)raw_Data.Samplesizes[sample]));
        //Omega skewness
        Sample_Omega_minus_skewness_vector.push_back(Sample_Omega_minus_subtr_mean_sum_cubed / (pow(Sample_Omega_minus_mean_uncertainty_vector[sample],3) * raw_Data.Samplesizes[sample]));
        Sample_Omega_plus_skewness_vector.push_back(Sample_Omega_plus_subtr_mean_sum_cubed / (pow(Sample_Omega_plus_mean_uncertainty_vector[sample],3) * raw_Data.Samplesizes[sample]));
        //Omega kurtosis
        Sample_Omega_minus_kurtosis_vector.push_back((Sample_Omega_minus_subtr_mean_sum_pow4 / (pow(Sample_Omega_minus_mean_uncertainty_vector[sample], 4) * raw_Data.Samplesizes[sample]))-3);
        Sample_Omega_plus_kurtosis_vector.push_back((Sample_Omega_plus_subtr_mean_sum_pow4 / (pow(Sample_Omega_plus_mean_uncertainty_vector[sample], 4) * raw_Data.Samplesizes[sample]))-3);

        //resetting variables per sample
        Sample_Omega_minus_subtr_mean_sum_squared = 0;
        Sample_Omega_minus_subtr_mean_sum_cubed = 0;
        Sample_Omega_plus_subtr_mean_sum_squared = 0;
        Sample_Omega_plus_subtr_mean_sum_cubed = 0;
        past_Omega_minus_count = Analysed_Data.Omegaminus_count;
        past_Omega_plus_count = Analysed_Data.Omegaplus_count;
        past_sample_events += raw_Data.Samplesizes[sample];
        sample++;
      }

        //reset variables per event
        Omega_minus_per_event_count = 0;
        Omega_plus_per_event_count = 0;
    }

  //overall mean and uncertainty
  Analysed_Data.Results.Omega_minus_mean = std::accumulate(Sample_Omega_minus_mean_vector.begin(), Sample_Omega_minus_mean_vector.end(), 0.0) / Sample_Omega_minus_mean_vector.size();
  Analysed_Data.Results.Omega_minus_mean_uncertainty = std::accumulate(Sample_Omega_minus_mean_uncertainty_vector.begin(), Sample_Omega_minus_mean_uncertainty_vector.end(), 0.0) / Sample_Omega_minus_mean_uncertainty_vector.size();
  Analysed_Data.Results.Omega_plus_mean = std::accumulate(Sample_Omega_plus_mean_vector.begin(), Sample_Omega_plus_mean_vector.end(), 0.0) / Sample_Omega_plus_mean_vector.size();
  Analysed_Data.Results.Omega_plus_mean_uncertainty = std::accumulate(Sample_Omega_plus_mean_uncertainty_vector.begin(), Sample_Omega_plus_mean_uncertainty_vector.end(), 0.0) / Sample_Omega_plus_mean_uncertainty_vector.size();
  
  //fix?
  Analysed_Data.Results.Omega_mean_difference = Analysed_Data.Results.Omega_minus_mean - Analysed_Data.Results.Omega_plus_mean;
  Analysed_Data.Results.Omega_mean_combined_uncertainty = Analysed_Data.Results.Omega_minus_mean_uncertainty + Analysed_Data.Results.Omega_plus_mean_uncertainty;
  Analysed_Data.Results.Omega_mean_sigma = Analysed_Data.Results.Omega_plus_mean_uncertainty / Analysed_Data.Results.Omega_mean_combined_uncertainty;
  
  //overall skewness and kurtosis
  Analysed_Data.Results.Omega_minus_skewness = std::accumulate(Sample_Omega_minus_skewness_vector.begin(), Sample_Omega_minus_skewness_vector.end(), 0.0) / Sample_Omega_minus_skewness_vector.size();
  Analysed_Data.Results.Omega_plus_skewness = std::accumulate(Sample_Omega_plus_skewness_vector.begin(), Sample_Omega_plus_skewness_vector.end(), 0.0 ) / Sample_Omega_plus_skewness_vector.size();
  
  Analysed_Data.Results.Omega_minus_kurtosis = std::accumulate(Sample_Omega_minus_kurtosis_vector.begin(), Sample_Omega_minus_kurtosis_vector.end(), 0.0) / Sample_Omega_minus_kurtosis_vector.size();
  Analysed_Data.Results.Omega_plus_kurtosis = std::accumulate(Sample_Omega_plus_kurtosis_vector.begin(), Sample_Omega_plus_kurtosis_vector.end(), 0.0) / Sample_Omega_plus_kurtosis_vector.size();
  //calculations of skewness and kurtosis


  //Total count of all Omega particles
  Analysed_Data.Omega_Particle_total_count = Analysed_Data.Omegaminus_count + Analysed_Data.Omegaplus_count;
  //difference of matter to antimatter
  Analysed_Data.Omega_Particle_total_difference = Analysed_Data.Omegaminus_count - Analysed_Data.Omegaplus_count;


  return Analysed_Data;
}

//Function to display data
void Display_Data(Analysed_Data Analysed_Data, raw_Data raw_Data){
  
  std::cout << "\n== Analysis Results ==\n";
  std::cout << "Total particle event count: " << raw_Data.Particle_event_vector.size() << "\n";
  std::cout << "Total amount of Omega-minus: " <<  Analysed_Data.Omegaminus_count << " and of Omega-plus: " << Analysed_Data.Omegaplus_count << "\n";
  std::cout << "Total amount of Omega-particles: " << Analysed_Data.Omega_Particle_total_count << "\n";
  std::cout << "Difference of total amount of Omega-particles: " << std::fixed << Analysed_Data.Omega_Particle_total_difference << "\n";
  std::cout << "Total amount of events with equal Omega-minus and Omega-plus (Not 0,0): " << std::fixed << Analysed_Data.Omega_event_equal_amount_count << "\n";
  std::cout << "Mean of Omega-minus: " << std::fixed << Analysed_Data.Results.Omega_minus_mean << " +- " << Analysed_Data.Results.Omega_minus_mean_uncertainty << "\n";
  std::cout << "Mean of Omega-plus: " << std::fixed << Analysed_Data.Results.Omega_plus_mean << " +- " << Analysed_Data.Results.Omega_plus_mean_uncertainty <<  "\n";
  std::cout << "Mean difference of Omega-minus and Omega-plus: " << std::fixed << Analysed_Data.Results.Omega_mean_difference << ", Combined Uncertainty: " << Analysed_Data.Results.Omega_mean_combined_uncertainty << "\n";
  std::cout << "Sigma away: " << std::fixed << Analysed_Data.Results.Omega_mean_sigma << "\n";
  std::cout << "Omega-minus Skewness: " << Analysed_Data.Results.Omega_minus_skewness << "\n";
  std::cout << "Omega-plus Skewness: " << Analysed_Data.Results.Omega_plus_skewness << "\n";
  std::cout << "Omega-minus Kurtosis: " << Analysed_Data.Results.Omega_minus_kurtosis << "\n";
  std::cout << "Omega-plus Kurtosis: " << Analysed_Data.Results.Omega_plus_kurtosis << "\n";
  std::cout << "\n";
}

#endif
