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

  //getting count of all events
  Analysed_Data.event_num = raw_Data.Particle_event_vector.size();
  //Gathering overall count of Omega particles in 5 million collisions
  Analysed_Data.Omegaminus_count = 0;
  Analysed_Data.Omegaplus_count = 0;
  Analysed_Data.Omega_event_equal_amount_count = 0;

  //used to calculate amount per event
  double Omega_minus_per_event_count = 0;
  double Omega_plus_per_event_count = 0;
  //per sample
  double Omega_minus_per_sample_count = 0;
  double Omega_plus_per_sample_count = 0;

  //Variables used for pseudo-rapidity and transverse momentum
  int transversePposition = 0;
  int pseudorapidityposition = 0;
  const int offset_transverseP = 0;
  const int offset_pseudorapidity = 4;
  const float step_transverseP = 0.5;
  const float step_pseudorapidity = 1;

  std::vector<double> Sample_Omega_minus_mean_vector;
  std::vector<double> Sample_Omega_minus_mean_stddeviation_vector;
  std::vector<double> Sample_Omega_minus_skewness_vector;
  std::vector<double> Sample_Omega_minus_skewness_stddeviation_vector;
  std::vector<double> Sample_Omega_minus_kurtosis_vector;

  std::vector<double> Sample_Omega_plus_mean_vector;
  std::vector<double> Sample_Omega_plus_mean_stddeviation_vector;
  std::vector<double> Sample_Omega_plus_skewness_vector;
  std::vector<double> Sample_Omega_plus_skewness_stddeviation_vector;
  std::vector<double> Sample_Omega_plus_kurtosis_vector;

  double Sample_Omega_minus_mean_stddeviation = 0;
  double Sample_Omega_minus_skewness = 0;
  double Sample_Omega_minus_kurtosis = 0;
  double Sample_Omega_plus_mean_stddeviation = 0;
  double Sample_Omega_plus_skewness = 0;
  double Sample_Omega_plus_kurtosis = 0;

  int sample = 0;
  int past_sample_events = 0;

  //gather info on pseudo-rapidity and transverse momentum
  sample = 0;
  past_sample_events = 0;
  for (int event0= 0; event0 < raw_Data.Particle_event_vector.size(); event0++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event0].Particles.size(); particle++){
      if (abs(raw_Data.Particle_event_vector[event0].Particles[particle].PGC) == 3334){
        //formula is floor((value + offset) / step)
        transversePposition = floor((raw_Data.Particle_event_vector[event0].Particles[particle].transverseP + offset_transverseP) / step_transverseP);
        pseudorapidityposition  = floor((raw_Data.Particle_event_vector[event0].Particles[particle].pseudorapidity + offset_pseudorapidity) / step_pseudorapidity);
        //MATTER OMEGA MINUS
        if (raw_Data.Particle_event_vector[event0].Particles[particle].PGC == 3334){
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
        else if (raw_Data.Particle_event_vector[event0].Particles[particle].PGC == -3334){
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
  Omega_minus_per_sample_count = 0;
  Omega_plus_per_sample_count = 0;
  for (int event1= 0; event1 < raw_Data.Particle_event_vector.size(); event1++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event1].Particles.size(); particle++){
      if (abs(raw_Data.Particle_event_vector[event1].Particles[particle].PGC) == 3334){
        if (raw_Data.Particle_event_vector[event1].Particles[particle].PGC == 3334){
          Omega_minus_per_event_count++;
        }
        else if (raw_Data.Particle_event_vector[event1].Particles[particle].PGC == -3334){
          Omega_plus_per_event_count++;
        }
      }
    }
    Omega_minus_per_sample_count += Omega_minus_per_event_count;
    Omega_plus_per_sample_count += Omega_plus_per_event_count;

      if ((event1 == (raw_Data.Samplesizes[sample] + past_sample_events)) or event1 == raw_Data.Particle_event_vector.size()-1){
        //Omega minus mean,
        Sample_Omega_minus_mean_vector.push_back((Omega_minus_per_sample_count) / (double)raw_Data.Samplesizes[sample]);
        //Omega plus mean
        Sample_Omega_plus_mean_vector.push_back(Omega_plus_per_sample_count / (double)raw_Data.Samplesizes[sample]);
        //resetting variables per sample
        Omega_minus_per_sample_count = 0;
        Omega_plus_per_sample_count = 0;
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
  Omega_minus_per_sample_count = 0;
  Omega_plus_per_sample_count = 0;
  for (int event2= 0; event2 < raw_Data.Particle_event_vector.size(); event2++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event2].Particles.size(); particle++){
      if (abs(raw_Data.Particle_event_vector[event2].Particles[particle].PGC) == 3334){
        if (raw_Data.Particle_event_vector[event2].Particles[particle].PGC == 3334){
          Omega_minus_per_event_count++;
        }
        else if (raw_Data.Particle_event_vector[event2].Particles[particle].PGC == -3334){
          Omega_plus_per_event_count++;
        }
      }
    }

    Omega_minus_per_sample_count += Omega_minus_per_event_count;
    Omega_plus_per_sample_count += Omega_plus_per_event_count;

      if ((event2 == (raw_Data.Samplesizes[sample] + past_sample_events)) or event2 == raw_Data.Particle_event_vector.size()-1){
        //Omega mean uncertainty
        Sample_Omega_minus_mean_stddeviation = pow(Omega_minus_per_sample_count - Sample_Omega_minus_mean_vector[sample],2) / (double)raw_Data.Samplesizes[sample];
        Sample_Omega_plus_mean_stddeviation = pow(Omega_plus_per_sample_count - Sample_Omega_plus_mean_vector[sample],2) / (double)raw_Data.Samplesizes[sample];
        Sample_Omega_minus_mean_stddeviation_vector.push_back(sqrt(Sample_Omega_minus_mean_stddeviation));
        Sample_Omega_plus_mean_stddeviation_vector.push_back(sqrt(Sample_Omega_plus_mean_stddeviation));
        //resetting variables per sample
        Omega_minus_per_sample_count = 0;
        Omega_plus_per_sample_count = 0;
        Sample_Omega_minus_mean_stddeviation = 0;
        Sample_Omega_plus_mean_stddeviation = 0;
        past_sample_events += raw_Data.Samplesizes[sample];
        sample++;
      }

        //reset variables per event
        Omega_minus_per_event_count = 0;
        Omega_plus_per_event_count = 0;
    }

  //overall mean and uncertainty
  Analysed_Data.Results.Omega_minus_mean = std::accumulate(Sample_Omega_minus_mean_vector.begin(), Sample_Omega_minus_mean_vector.end(), 0.0) / Sample_Omega_minus_mean_vector.size();
  for (int submean = 0; submean < Sample_Omega_minus_mean_vector.size(); submean++){
    Analysed_Data.Results.Omega_minus_mean_uncertainty += (pow(Sample_Omega_minus_mean_vector[submean] - Analysed_Data.Results.Omega_minus_mean,2) / Sample_Omega_minus_mean_vector.size()); 
  }
  Analysed_Data.Results.Omega_minus_mean_uncertainty = sqrt(Analysed_Data.Results.Omega_minus_mean_uncertainty);

  Analysed_Data.Results.Omega_plus_mean = std::accumulate(Sample_Omega_plus_mean_vector.begin(), Sample_Omega_plus_mean_vector.end(), 0.0) / Sample_Omega_plus_mean_vector.size();
  for (int submean = 0; submean < Sample_Omega_plus_mean_vector.size(); submean++){
    Analysed_Data.Results.Omega_plus_mean_uncertainty += (pow(Sample_Omega_plus_mean_vector[submean] - Analysed_Data.Results.Omega_plus_mean,2) / Sample_Omega_plus_mean_vector.size()); 
  }
  Analysed_Data.Results.Omega_plus_mean_uncertainty = sqrt(Analysed_Data.Results.Omega_plus_mean_uncertainty);

  //subsampling for difference
  for (int sample = 0; sample < Sample_Omega_minus_mean_vector.size(); sample++){
    Analysed_Data.Results.Omega_mean_difference += Sample_Omega_minus_mean_vector[sample] - Sample_Omega_plus_mean_vector[sample];
  }
  Analysed_Data.Results.Omega_mean_difference = Analysed_Data.Results.Omega_mean_difference / Sample_Omega_minus_mean_vector.size();

  Analysed_Data.Results.Omega_mean_combined_uncertainty = sqrt((pow(Analysed_Data.Results.Omega_minus_mean_uncertainty,2) + pow(Analysed_Data.Results.Omega_plus_mean_uncertainty,2)));
  Analysed_Data.Results.Omega_mean_sigma = Analysed_Data.Results.Omega_mean_difference / Analysed_Data.Results.Omega_mean_combined_uncertainty;
  
   //Total count of all Omega particles
  Analysed_Data.Omega_Particle_total_count = Analysed_Data.Omegaminus_count + Analysed_Data.Omegaplus_count;
  //difference of matter to antimatter
  Analysed_Data.Omega_Particle_total_difference = Analysed_Data.Omegaminus_count - Analysed_Data.Omegaplus_count;

  //calculating skewness and kurtosis
  sample = 0;
  past_sample_events = 0;
  int Omega_minus_sum_minus_mean_pow2 = 0;
  int Omega_minus_sum_minus_mean_pow4 = 0;
  int Omega_plus_sum_minus_mean_pow2 = 0;
  int Omega_plus_sum_minus_mean_pow4 = 0;

  for (int event3 = 0; event3 < raw_Data.Particle_event_vector.size(); event3++){
    for (int particle = 0; particle < raw_Data.Particle_event_vector[event3].Particles.size(); particle++){
      if (raw_Data.Particle_event_vector[event3].Particles[particle].PGC == 3334){
        Omega_minus_per_event_count++;
      }
      else if (raw_Data.Particle_event_vector[event3].Particles[particle].PGC == -3334){
        Omega_plus_per_event_count++;
      }
    }
    Omega_minus_sum_minus_mean_pow2 += pow((Omega_minus_per_event_count - Sample_Omega_minus_mean_vector[sample]),2) / (double)Sample_Omega_minus_mean_stddeviation_vector[sample];
    Omega_minus_sum_minus_mean_pow4 += pow((Omega_minus_per_event_count - Sample_Omega_minus_mean_vector[sample]),4)  / (double)Sample_Omega_minus_mean_stddeviation_vector[sample];
    Omega_plus_sum_minus_mean_pow2 += pow((Omega_plus_per_event_count - Sample_Omega_plus_mean_vector[sample]),2) / (double)Sample_Omega_plus_mean_stddeviation_vector[sample];
    Omega_plus_sum_minus_mean_pow4 += pow((Omega_plus_per_event_count - Sample_Omega_plus_mean_vector[sample]),4) / (double)Sample_Omega_plus_mean_stddeviation_vector[sample];

    if ((event3 == (raw_Data.Samplesizes[sample] + past_sample_events)) or event3 == raw_Data.Particle_event_vector.size()-1){
      //wrong calculation
      Sample_Omega_minus_skewness = Omega_minus_sum_minus_mean_pow2 / (double)raw_Data.Samplesizes[sample];
      Sample_Omega_minus_kurtosis = Omega_minus_sum_minus_mean_pow4 / (double)raw_Data.Samplesizes[sample];
      Sample_Omega_plus_skewness = Omega_plus_sum_minus_mean_pow2 / (double)raw_Data.Samplesizes[sample];
      Sample_Omega_plus_kurtosis = Omega_plus_sum_minus_mean_pow4 / (double)raw_Data.Samplesizes[sample];

      //Omega minus and plus skewness, kurtosis
      Sample_Omega_minus_skewness_vector.push_back(Sample_Omega_minus_skewness);
      Sample_Omega_minus_kurtosis_vector.push_back(Sample_Omega_minus_kurtosis);
      Sample_Omega_plus_skewness_vector.push_back(Sample_Omega_plus_skewness);
      Sample_Omega_plus_kurtosis_vector.push_back(Sample_Omega_plus_kurtosis);

      //resetting variables per sample
      //add sums = 0;
      Omega_minus_sum_minus_mean_pow2 = 0;
      Omega_minus_sum_minus_mean_pow4 = 0;
      Omega_plus_sum_minus_mean_pow2 = 0;
      Omega_plus_sum_minus_mean_pow4 = 0;
      Sample_Omega_minus_skewness = 0;
      Sample_Omega_minus_kurtosis = 0;
      Sample_Omega_plus_skewness = 0;
      Sample_Omega_plus_kurtosis = 0;
      past_sample_events += raw_Data.Samplesizes[sample];
      sample++;
    }
      //reset variables per event
      Omega_minus_per_event_count = 0;
      Omega_plus_per_event_count = 0;
    }
  
  //overall skewness and kurtosis
  Analysed_Data.Results.Omega_minus_skewness = std::accumulate(Sample_Omega_minus_skewness_vector.begin(), Sample_Omega_minus_skewness_vector.end(), 0.0) / Sample_Omega_minus_skewness_vector.size();
  for (int subskewness = 0; subskewness < Sample_Omega_minus_skewness_vector.size(); subskewness++){
    Analysed_Data.Results.Omega_minus_skewness_uncertainty += (pow(Sample_Omega_minus_skewness_vector[subskewness] - Analysed_Data.Results.Omega_minus_skewness,2) / Sample_Omega_minus_skewness_vector.size()); 
  }
  Analysed_Data.Results.Omega_minus_skewness_uncertainty = sqrt(Analysed_Data.Results.Omega_minus_skewness_uncertainty);

  Analysed_Data.Results.Omega_minus_kurtosis = std::accumulate(Sample_Omega_minus_kurtosis_vector.begin(), Sample_Omega_minus_kurtosis_vector.end(), 0.0) / Sample_Omega_minus_kurtosis_vector.size();
  for (int subkurtosis = 0; subkurtosis < Sample_Omega_minus_kurtosis_vector.size(); subkurtosis++){
    Analysed_Data.Results.Omega_minus_kurtosis_uncertainty += (pow(Sample_Omega_minus_kurtosis_vector[subkurtosis] - Analysed_Data.Results.Omega_minus_kurtosis,2) / Sample_Omega_minus_kurtosis_vector.size()); 
  }
  Analysed_Data.Results.Omega_minus_kurtosis_uncertainty =  sqrt(Analysed_Data.Results.Omega_minus_kurtosis_uncertainty);

  Analysed_Data.Results.Omega_plus_skewness = std::accumulate(Sample_Omega_plus_skewness_vector.begin(), Sample_Omega_plus_skewness_vector.end(), 0.0 ) / Sample_Omega_plus_skewness_vector.size();
  for (int subskewness = 0; subskewness < Sample_Omega_plus_skewness_vector.size(); subskewness++){
    Analysed_Data.Results.Omega_plus_skewness_uncertainty += (pow(Sample_Omega_plus_skewness_vector[subskewness] - Analysed_Data.Results.Omega_plus_skewness,2) / Sample_Omega_plus_skewness_vector.size()); 
  }
  Analysed_Data.Results.Omega_plus_skewness_uncertainty = sqrt(Analysed_Data.Results.Omega_plus_skewness_uncertainty);

  Analysed_Data.Results.Omega_plus_kurtosis = std::accumulate(Sample_Omega_plus_kurtosis_vector.begin(), Sample_Omega_plus_kurtosis_vector.end(), 0.0) / Sample_Omega_plus_kurtosis_vector.size();
  for (int subkurtosis = 0; subkurtosis < Sample_Omega_minus_kurtosis_vector.size(); subkurtosis++){
    Analysed_Data.Results.Omega_plus_kurtosis_uncertainty += (pow(Sample_Omega_plus_kurtosis_vector[subkurtosis] - Analysed_Data.Results.Omega_plus_kurtosis,2) / Sample_Omega_plus_kurtosis_vector.size()); 
  }
  Analysed_Data.Results.Omega_plus_kurtosis_uncertainty = sqrt(Analysed_Data.Results.Omega_plus_kurtosis_uncertainty);

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
  std::cout << "Omega-minus Skewness: " << std::fixed << Analysed_Data.Results.Omega_minus_skewness << " +- " << std::fixed <<  Analysed_Data.Results.Omega_minus_skewness_uncertainty <<"\n";
  std::cout << "Omega-plus Skewness: " << std::fixed << Analysed_Data.Results.Omega_plus_skewness << " +- " << std::fixed <<  Analysed_Data.Results.Omega_plus_skewness_uncertainty <<"\n";
  std::cout << "Omega-minus Kurtosis: " << std::fixed << Analysed_Data.Results.Omega_minus_kurtosis <<  " +- " << std::fixed <<  Analysed_Data.Results.Omega_minus_kurtosis_uncertainty << "\n";
  std::cout << "Omega-plus Kurtosis: " << std::fixed << Analysed_Data.Results.Omega_plus_kurtosis << " +- " << std::fixed <<  Analysed_Data.Results.Omega_plus_kurtosis_uncertainty << "\n";
  std::cout << "\n";

  std::cout << "Matter pseudo-rapidity (from -4 to 4 in +1 steps): \n";
  for (int number = 0; number < Analysed_Data.Results.matter_pseudo_rapidity_vector.size(); number++){
    std::cout << Analysed_Data.Results.matter_pseudo_rapidity_vector[number] << ", ";
  }
  std::cout << "\n\n";

  std::cout << "Antimatter pseudo-rapidity (from -4 to 4 in +1 steps): \n";
  for (int number = 0; number < Analysed_Data.Results.antimatter_pseudo_rapidity_vector.size(); number++){
    std::cout << Analysed_Data.Results.antimatter_pseudo_rapidity_vector[number] << ", ";
  }
  std::cout << "\n\n";

    std::cout << "Matter transverseP (from 0 to >0 in +0.5 steps): \n";
  for (int number = 0; number < Analysed_Data.Results.matter_transverseP_vector.size(); number++){
    std::cout << Analysed_Data.Results.matter_transverseP_vector[number] << ", ";
  }
  std::cout << "\n\n";
  
  std::cout << "Antimatter transverseP (from 0 to >0 in +0.5 steps): \n";
  for (int number = 0; number < Analysed_Data.Results.antimatter_transverseP_vector.size(); number++){
    std::cout << Analysed_Data.Results.antimatter_transverseP_vector[number] << ", ";
  }
  std::cout << "\n\n";
}

#endif
