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
      ar & BOOST_SERIALIZATION_NVP(PGC);
      ar & BOOST_SERIALIZATION_NVP(Px); 
      ar & BOOST_SERIALIZATION_NVP(Py); 
      ar & BOOST_SERIALIZATION_NVP(Pz); 
      ar & BOOST_SERIALIZATION_NVP(P);
      ar & BOOST_SERIALIZATION_NVP(transverseP);
      ar & BOOST_SERIALIZATION_NVP(azimutalangle);
      ar & BOOST_SERIALIZATION_NVP(pseudorapidity);
    }
    
  public:
  int PGC;
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
        ar & BOOST_SERIALIZATION_NVP(particlesperevent);
        ar & BOOST_SERIALIZATION_NVP(Particles);
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
        ar & BOOST_SERIALIZATION_NVP(Particle_event_vector);
        ar & BOOST_SERIALIZATION_NVP(Samplesizes);
    }
    public:
    std::vector<Particle_event> Particle_event_vector;
    std::vector<uint32_t> Samplesizes;
};

//class to define all the numbers we care about
class Sample{
   friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(Omega_minus_mean);
      ar & BOOST_SERIALIZATION_NVP(Omega_minus_mean_uncertainty);
      ar & BOOST_SERIALIZATION_NVP(Omega_plus_mean);
      ar & BOOST_SERIALIZATION_NVP(Omega_plus_mean_uncertainty);
    }
   public:
    double Omega_minus_mean;
    double Omega_minus_mean_uncertainty;
    double Omega_plus_mean;
    double Omega_plus_mean_uncertainty;
    

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
        ar & BOOST_SERIALIZATION_NVP(Omega_event_equal_amount_count);
        ar & BOOST_SERIALIZATION_NVP(Omega_Particle_total_count);
        ar & BOOST_SERIALIZATION_NVP(Omega_Particle_total_difference);
        ar & BOOST_SERIALIZATION_NVP(Results);
        ar & BOOST_SERIALIZATION_NVP(matter_pseudo_rapidity_vector);
        ar & BOOST_SERIALIZATION_NVP(matter_transverseP_vector);
        ar & BOOST_SERIALIZATION_NVP(antimatter_pseudo_rapidity_vector);
        ar & BOOST_SERIALIZATION_NVP(antimatter_transverseP_vector);
    }
  
  public:
  uint32_t Omegaminus_count;
  uint32_t Omegaplus_count;
  uint32_t Omega_event_equal_amount_count;
  uint64_t Omega_Particle_total_count;
  uint64_t Omega_Particle_total_difference;
  Sample Results;
  std::vector<uint32_t> matter_pseudo_rapidity_vector;
  std::vector<uint32_t> matter_transverseP_vector;
  std::vector<uint32_t> antimatter_pseudo_rapidity_vector;
  std::vector<uint32_t> antimatter_transverseP_vector;
};

#endif