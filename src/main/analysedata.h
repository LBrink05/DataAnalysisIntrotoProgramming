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

  //avergae number of particles iwth uncertainties
  //average of average
  //and difference with uncertainty
  //average per file
  //Gathering overall count of Omega particles in 5 million collisions
  Analysed_Data.Omegaminus_count = 0;
  Analysed_Data.Omegaplus_count = 0;

  //step for calculations
  float step_pseudorapidity = 1;
  float step_transverseP = 0.5;
  int offset_pseudorapidity = 4;
  int pseudorapdityposition = 0;
  int transversePposition = 0;
  int matter_count = 0;
  int antimatter_count = 0;

  //analysing each sample individually

  for (int sample = 0; sample < raw_Data.Samplesizes.size(); sample++){
    for (int event= 0; event < raw_Data.Samplesizes[sample]; event++){
      for (int particle = 0; particle < raw_Data.Particle_event_vector[event].Particles.size(); particle++){
        //comparing particles on pseudorapidity and transverseP of Omega-minus by counting
        //raw_Data.Particle_total_count++; add this if all particles enabled to get particle count
        if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == 3334){
          Analysed_Data.Omegaminus_count++;
        }
        else if (raw_Data.Particle_event_vector[event].Particles[particle].PGC == -3334){
          Analysed_Data.Omegaplus_count++;
        }
      }
    }
  }
  
  return Analysed_Data;
}

//Function to display data
void Display_Data(Analysed_Data Analysed_Data, raw_Data raw_Data){
  
  std::cout << "\n";
  std::cout << "Total particle event count: " << raw_Data.Particle_event_vector.size() << "\n";
  std::cout << "Total amount of Omega-minus: " <<  Analysed_Data.Omegaminus_count << " and of Omega-plus: " << Analysed_Data.Omegaplus_count << "\n";

  //std::cout << "Uncertainty: " << sqrt(Analysed_Data.Omegaminus_count) / (double)raw_Data.Particle_total_count<< "\n";
  //displaying matrix
  //Display_matrix(Analysed_Data);
}

#endif