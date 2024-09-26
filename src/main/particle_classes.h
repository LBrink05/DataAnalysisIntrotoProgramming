#ifndef PARTICLE_CLASSES_H
#define PARTICLE_CLASSES_H
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

//CLASSES
//Particle Structs with 6 Properties (Px,Py,Pz, total momentum p, transverse momentum, azimuthal angle, pseudo rapidity)
class Particle {
  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & PGC;
      ar & Px; 
      ar & Py; 
      ar & Pz; 
      ar & P;
      ar & transverseP;
      ar & azimutalangle;
      ar & pseudorapidity;
    }
    
  public:
  int32_t PGC;
  float Px; 
  float Py; 
  float Pz; 
  float P;
  float transverseP;
  float azimutalangle;
  float pseudorapidity;
};

//Class to keep track of information per event
class Particle_event{
  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & particlesperevent;
        ar & Particles;
    }

  public:
  uint32_t particlesperevent;
  std::vector<Particle> Particles;
};

class raw_Data{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & Particle_event_vector;
        ar & Samplesizes;
    }
    public:
    std::vector<Particle_event> Particle_event_vector;
    std::vector<uint32_t> Samplesizes;
};

//Class to keep track of analysed information
class Analysed_Data{
  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      //uses BOOST_SERIALIZATION_NVP for xml conversion
        ar & BOOST_SERIALIZATION_NVP(Omegaminus_count);
        ar & BOOST_SERIALIZATION_NVP(Omegaplus_count);
        ar & BOOST_SERIALIZATION_NVP(Omegaminus_average);
        ar & BOOST_SERIALIZATION_NVP(Omegaplus_average);
        ar & BOOST_SERIALIZATION_NVP(average_matter_antimatter_ratio);
        ar & BOOST_SERIALIZATION_NVP(matter_pseudo_rapidity_vector);
        ar & BOOST_SERIALIZATION_NVP(matter_transverseP_vector);
        ar & BOOST_SERIALIZATION_NVP(antimatter_pseudo_rapidity_vector);
        ar & BOOST_SERIALIZATION_NVP(antimatter_transverseP_vector);
    }
  
  public:
  uint32_t Omegaminus_count;
  uint32_t Omegaplus_count;
  double_t Omegaminus_average;
  double_t Omegaplus_average;
  double_t average_matter_antimatter_ratio;
  //introduce float uncertainty too
  std::vector<uint32_t> matter_pseudo_rapidity_vector;
  std::vector<uint32_t> matter_transverseP_vector;
  std::vector<uint32_t> antimatter_pseudo_rapidity_vector;
  std::vector<uint32_t> antimatter_transverseP_vector;
  //do not encode
  std::vector<uint32_t> particle_count_mean;
};

#endif