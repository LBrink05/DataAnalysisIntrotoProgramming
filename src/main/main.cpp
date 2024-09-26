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
#include "analysedata.h"


int main(){

    //Keep track of time at start of program
    auto start_time = std::chrono::high_resolution_clock::now();
    std::cout << "Starting Program." << "\n";
    
    //Ask if serialization should be done
    std::cout << "If no serialization, files will be used." << "\n";
    std::cout << "Do you wish to serialize? Y/N" << "\n";
    bool serialize;
    std::string answer;
    std::cin >> answer;
    std::cout << "You have choosen: ";
    if (answer == "Y" | answer == "y" ){
      serialize = true;
      std::cout << "Serialization." << "\n";
    }
    else if (answer == "N" | answer == "n" ) {
      serialize = false;
      std::cout << "Retrieving of past serialization." << "\n";
    }
    else{
      std::cout << "Error: Wrong Input." << "\n";
      abort();
    }

    //pause to read choice
    sleep(1);
    
    //object for raw_Data
    raw_Data raw_Data;
    //object for analysed Data
    Analysed_Data Analysed_Data;
    //amount of datasets
    uint8_t dataset_num = 11;

    //serializing data
    if (serialize == true){
    //Get paths of every dataset file
    std::cout << "Getting Datasetpaths." << "\n";
    std::vector<std::string> Datasetpathvector;
    Datasetpathvector = get_paths(Datasetpathvector, dataset_num);
    std::cout << "Finished getting Datasetpaths." << "\n";

    //Reading dataset files and serializing
    std::cout << "Reading Datasets, calculating and serializing Vectors." << "\n";
    Read_Calc_Particles(raw_Data, Datasetpathvector, dataset_num);
    std::cout << "Finished reading, calculating and serializing." << "\n";
    }

    //Access Particle events from previous run
    std::cout << "Accessing previously serialized Data." << "\n";

    std::vector<Particle_event> Partial_Particle_event_vector;
    for (float num_files= 0; num_files < dataset_num; num_files++){
      //getting sample data and inserting them into the entire particle event vector
      Partial_Particle_event_vector = deserialization(num_files);
      raw_Data.Samplesizes.push_back(Partial_Particle_event_vector.size());
      raw_Data.Particle_event_vector.insert(raw_Data.Particle_event_vector.end(),Partial_Particle_event_vector.begin(), Partial_Particle_event_vector.end());
      
    };
    //std::cout << "Size of Particle_event_vector: " << Particle_event_vector.size() << "\n";
    std::cout << "Finished retrieving serialized Data." << "\n";

    //Calculating wanted data from Vector for ease of calculation
    std::cout << "Beginning to analyse data." << "\n";
    Analysed_Data = Analyse_Data(raw_Data, Analysed_Data);
    std::cout << "Finished analysing data." << "\n";

    //Displaying results
    std::cout << "Beginning to display data." << "\n";
    Display_Data(Analysed_Data, raw_Data);
    std::cout << "Finished displaying data." << "\n";

    //serializing results for python to read
    serialization_results(Analysed_Data);

    //print time
    auto current_time = std::chrono::high_resolution_clock::now();
    std::cout << "Program has been running for " << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() << " seconds." << "\n";
    std::cout << "\n";
    return 0;
}

//FIX PARTICLE COUNT
//uncertainty and table for later