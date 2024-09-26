#ifndef SERIALIZATION_H
#define SERIALIZATION_H
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

//custom classes
#include "particle_classes.h"

//SERIALIZATION functions
void serialization(std::vector<Particle_event> Particle_event_vector, int Vectorsetfile_num){
  //store data in file .> serialize
    std::cout << " Beginning Serialization of ";
    std::string Filename = "Vectorset/Vectorset"+std::to_string(Vectorsetfile_num)+".xml";
    std::cout << Filename << "\n";
    std::ofstream f(Filename);
    boost::archive::xml_oarchive ar(f, boost::archive::no_header); 
    ar << BOOST_SERIALIZATION_NVP(Particle_event_vector);
    f.close();
    std::cout << "Serialization complete." << "\n";
}


//DESERIALIZATION functions
std::vector<Particle_event> deserialization(std::vector<Particle_event> Partial_particle_event_vector, int num_file){
  //retrieve data from file > deserialize
  std::cout << " Beginning De-serialization of ";
    std::string Filename = "Vectorset/Vectorset"+std::to_string(num_file)+".xml";
    std::cout << Filename << "\n";
    std::ifstream i_f(Filename);
    boost::archive::xml_iarchive i_ar(i_f, boost::archive::no_header);
    i_ar >> BOOST_SERIALIZATION_NVP(Partial_particle_event_vector);
    i_f.close();
    std::cout << "De-serialization complete."<< "\n";
    return Partial_particle_event_vector;
}


void serialization_results(Analysed_Data Analysed_Data){
  //store data in file .> serialize
  //in xml for python xml parser
    std::cout << " Beginning Serialization of ";
    std::string Filename = "Vectorset/results.xml";
    std::cout << Filename << "\n";
    std::ofstream f(Filename);
    boost::archive::xml_oarchive ar(f, boost::archive::no_header); 
    ar << BOOST_SERIALIZATION_NVP(Analysed_Data);
    f.close();
    std::cout << "Serialization complete." << "\n";
}



#endif