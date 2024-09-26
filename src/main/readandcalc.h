#ifndef READANDCALC_H
#define READANDCALC_H
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

//Getting paths of the different Datasets
std::vector<std::string> get_paths(std::vector<std::string> Datasetpathvector, int dataset_num){
    //Requesting Directory if not me
    std::cout << "Do you want to use the default Dataset directory? Y/N" << "\n";
    std::string answer;
    std::string defaultdirectory = "/home/liam/Desktop/Development/IntrotoProgramming/DataAnalysisIntrotoProgramming/Dataset/DataFiles";
    std::cin >> answer;
    std::cout << "You have choosen: ";
    if (answer == "Y" | answer == "y" ){
      answer = defaultdirectory;
      std::cout << "Using default directory." << "\n";
    }
    else if(answer == "N" | answer == "n"){
      std::cout << "Please enter your directory: ";
      std::cin >> answer;
    }
    else{
      std::cout << "Error: Wrong Input." << "\n";
      abort();
    }
    //delay to read choice
    sleep(1);
    
    //Getting all filepaths
    for (int set= 0; set < dataset_num; set++){
        std::string Dataset = (answer + "/output-Set" + std::to_string(set)) + ".txt";
        Datasetpathvector.push_back(Dataset);
        }

    return Datasetpathvector;
};


//Function for calculating and reading Particles
int Read_Calc_Particles(raw_Data raw_Data, std::vector<std::string> Datasetpathvector, uint8_t Dataset_num){
    //Asking if Particles should be excluded
    bool Particle_exclusion;
    std::cout << "Do you want to only read omega particles? Y/N" << "\n";
    std::string answer;
    std::cin >> answer;
    if (answer == "Y" | answer == "y" ){
      Particle_exclusion = true;
      std::cout << "Reading only omega particles." << "\n";
    }
    else if (answer == "N" | answer == "n" ){
      Particle_exclusion = false;
      std::cout << "Reading all particles." << "\n";
    }
    else{
      std::cout << "Error: Wrong Input." << "\n";
      abort();
    }
    //pause to read choice
    sleep(1);

    //int counters
    int Particle_event_total= -1; //-1 because first one is 0
    int Particle_event_relative = -1;
    int Omegacount = 0;

    //For loop open each Datasetfile (11 total)
    std::ifstream ifs;
    for (int numpaths = 0; numpaths < Dataset_num; numpaths++){
        
        //open specific file
        ifs.open(Datasetpathvector[numpaths]);

        //Error handling of opening file
        if (!ifs.is_open()){
          std::cout << "Error: Incorrect File Directory" << "\n";
          abort();
        }
        else if (ifs.is_open()){
          std::cout << "Openend Dataset file: " << numpaths << "\n";
        }
        
        //get individual line
        std::string line_of_file = "";

        while(std::getline(ifs,line_of_file)){

          //get string of each line and get the terms put it into substring that gets put into a vector
          std::string substring = "";
          //indexing all numbers per line
          std::vector<float> substringvector;

          //for loop to get digit of every line
          for (int digit=0; digit <= line_of_file.length(); digit++){
              if (line_of_file[digit] != ' ' and digit<line_of_file.length()){
              substring += line_of_file[digit];
              }
              else{
              substringvector.push_back(stof(substring));
              substring = "";
              }
          }

          //Calculating Particle events
          if (substringvector.size() == 2){
              Particle_event_total++;
              Particle_event_relative++;
              //std::cout << "### Particle Event: " << std::fixed << Particle_event_total << " ###" << "\n";
              Particle_event Particle_event;
              Particle_event.particlesperevent =  (uint32_t)substringvector[1];
              raw_Data.Particle_event_vector.push_back(Particle_event);
              //std::cout << Particle_event_vector.size()<< "\n";
          } 

          //PGC for Omega minus is 3334 and for Omega plus is -3334
          else if ((substringvector.size() == 4) and ((!Particle_exclusion) or ((Particle_exclusion) and (abs(substringvector[3]) == 3334)))){
              Omegacount++;
              Particle Particle;
              Particle.PGC = substringvector[3];
              Particle.Px = substringvector[0];
              Particle.Py = substringvector[1];
              Particle.Pz = substringvector[2];
              Particle.P = sqrt(pow(Particle.Px, 2)+pow(Particle.Py, 2)+pow(Particle.Pz, 2)); 
              Particle.transverseP = sqrt(pow(Particle.Px, 2)+pow(Particle.Py, 2)); 
              Particle.azimutalangle = (Particle.Px / Particle.Py); 
              Particle.pseudorapidity = 0.5*(log((Particle.P+Particle.Pz) / (Particle.P-Particle.Pz)));
              raw_Data.Particle_event_vector[Particle_event_relative].Particles.push_back(Particle);
          }
        }

        //delete later
        int k = 0;
        for (int i= 0; i < raw_Data.Particle_event_vector.size(); i++){
          for (int j = 0; j < raw_Data.Particle_event_vector[i].Particles.size(); j++){
            k++;
          }
        }
        std::cout << "PARTICLES: " << k << "\n";
        sleep(1);

        //serialize per datasetfile
        serialization(raw_Data.Particle_event_vector, numpaths);
        raw_Data.Particle_event_vector.erase(raw_Data.Particle_event_vector.begin(), raw_Data.Particle_event_vector.end());
        Particle_event_relative = -1;

        std::cout << "Omega count: " << Omegacount << "\n";
        
        ifs.close();
    }
        return 0;
  }

#endif