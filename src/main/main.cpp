//to compile: g++ -o main -no-pie -I /opt/vcpkg/packages/matplotplusplus_x64-linux/include ../src/main/main.cpp -lboost_serialization
//can also compile with cmake now using cmake --build build && ./build/DataAnalysis
//for general operations with files

//mmatplotplusplus for plot
//#include <matplot/matplot.h>

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
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

//using namespace matplot;
using namespace std;

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

//Class to keep track of analysed information
class Analysed_Data{
  public:
  uint64_t total_particle_count; //run serialize with all particles once to get that count dont forget to uncomment option in Analyse_Data()
  uint32_t Omegaminus_count;
  uint32_t Omegaplus_count;
  double Omegaminus_average;
  double Omegaplus_average;
  double average_matter_antimatter_ratio;
  std::vector<std::array<uint32_t,8>> matrix_data; 
  bool matrix_function = false;
};

//SERIALIZATION functions
void serialization(std::vector<Particle_event> Particle_event_vector, int Vectorsetfile_num){
  //store data in file .> serialize
    std::cout << " Beginning Serialization of ";
    string Filename = "Vectorset/Vectorset"+to_string(Vectorsetfile_num/1000000)+".dat";
    std::cout << Filename << endl;
    std::ofstream f(Filename, std::ofstream::binary);
    boost::archive::binary_oarchive ar(f, boost::archive::no_header); 
    ar << Particle_event_vector;
    f.close();
    std::cout << "Serialization complete." << endl;
}

void serialization_misc(int Particle_total_count, int Vectorsetfile_num){
  //store data in file .> serialize
    std::cout << " Beginning Serialization of ";
    string Filename = "Vectorset/misc.dat";
    std::cout << Filename << endl;
    std::ofstream f(Filename, std::ofstream::binary);
    boost::archive::binary_oarchive ar(f, boost::archive::no_header); 
    std::vector<int> misc = {Particle_total_count, Vectorsetfile_num};
    ar << misc;
    f.close();
    std::cout << "Serialization complete." << endl;
}
//DESERIALIZATION functions
std::vector<Particle_event> deserialization(int num_files){
  //retrieve data from file > deserialize
  std::cout << " Beginning De-serialization of ";
    string Filename = "Vectorset/Vectorset"+to_string(num_files)+".dat";
    std::cout << Filename << endl;
    std::ifstream i_f(Filename, std::ifstream::binary);
    boost::archive::binary_iarchive i_ar(i_f, boost::archive::no_header);
    std::vector<Particle_event> Partial_particle_event_vector;
    i_ar >> Partial_particle_event_vector;
    i_f.close();
    std::cout << "De-serialization complete."<< endl;
    return Partial_particle_event_vector;
}

std::vector<int> deserialization_misc(){
  //retrieve data from file > deserialize
  std::cout << " Beginning De-serialization of ";
    string Filename = "Vectorset/misc.dat";
    std::cout << Filename << endl;
    std::ifstream i_f(Filename, std::ifstream::binary);
    boost::archive::binary_iarchive i_ar(i_f, boost::archive::no_header);
    std::vector<int> misc;
    i_ar >> misc;
    i_f.close();
    std::cout << "De-serialization complete."<< endl;
    return misc;
}

//Getting paths of the different Datasets
 std::vector<string> get_paths(std::vector<string> Datasetpathvector, int dataset_num){
    //Requesting Directory if not me
    std::cout << "Do you want to use the default Dataset directory? Y/N" << endl;
    bool defaultpath;
    string answer;
    string defaultdirectory = "/home/liam/Desktop/Development/IntrotoProgramming/DataAnalysisIntrotoProgramming/Dataset/DataFiles";
    std::cin >> answer;
    std::cout << "You have choosen: ";
    if (answer == "Y" | answer == "y" ){
      defaultpath = true;
      answer = defaultdirectory;
      std::cout << "Using default directory." << endl;
    }
    else if(answer == "N" | answer == "n"){
      defaultpath = false;
      std::cout << "Please enter your directory: ";
      std::cin >> answer;
    }
    else{
      std::cout << "Error: Wrong Input." << endl;
      abort();
    }
    //delay to read
    sleep(1);
    

    //Getting all filepaths
    for (int set=0; set <= dataset_num; set++){
        string Dataset = (answer + "/output-Set" + std::to_string(set)) + ".txt";
        Datasetpathvector.push_back(Dataset);
        //std::cout << Datasetpaths[i] << std::endl;
        }
        return Datasetpathvector;
    };

//Function for calculating and reading Particles
int Read_Calc_Particles(std::vector<Particle_event> Particle_event_vector, std::vector<string> Datasetpathvector, int Dataset_num){
    //Asking if Particles should be excluded
    bool Particle_exclusion;
    std::cout << "Do you want to only read omega particles? Y/N" << endl;
    string answer;
    std::cin >> answer;
    if (answer == "Y" | answer == "y" ){
      Particle_exclusion = true;
      std::cout << "Reading only omega particles." << endl;
    }
    else if (answer == "N" | answer == "n" ){
      Particle_exclusion = false;
      std::cout << "Reading all particles." << endl;
    }
    else{
      std::cout << "Error: Wrong Input." << endl;
      abort();
    }
    //pause to read choice
    sleep(1);


    //int counters
    int Particle_event_total=-1; //-1 because first one is 0
    int Particle_event_relative = -1; // relative for each serialization
    int Particle_total_count = 0;

    //For loop open each Datasetfile (11 total)
    std::ifstream ifs;
    for (int numpaths=0; numpaths <= Dataset_num; numpaths++){
        //open specific file
        ifs.open(Datasetpathvector[numpaths]);

        //Error handling of opening file
        if (!ifs.is_open()){
          std::cout << "Error: Incorrect File Directory" << endl;
          abort();
        }
        else if (ifs.is_open()){
          std::cout << "Openend Dataset file: " << numpaths << endl;
        }
        
        //get individual line
        string line_of_file = "";

        while(getline(ifs,line_of_file)){


          //get string of each line and get the terms put it into substring that gets put into a vector
          string substring = "";
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
            //serialize every million times
            if (Particle_event_total == 0 | Particle_event_relative == 999999){
              serialization(Particle_event_vector, Particle_event_total);
              Particle_event_vector.erase(Particle_event_vector.begin(), Particle_event_vector.end());
              Particle_event_relative = -1;
            }
              Particle_event_total++;
              Particle_event_relative++;
              std::cout << "### Particle Event: " << Particle_event_total << " ###" << endl;
              Particle_event Particle_event;
              Particle_event.particlesperevent = (int)substringvector[1];
              Particle_event_vector.push_back(Particle_event);
              //std::cout << Particle_event_vector.size()<< endl;
          } 
          //PGC for Omega minus is 3334 and for Omega plus is -3334
          //filtering for only omega particles
          else if ((substringvector.size() == 4)){
              Particle_total_count++;
              if ((!Particle_exclusion) or ((Particle_exclusion) and (abs(substringvector[3]) == 3334))){
              //std::cout << substringvector[3] << endl;
              Particle Particle;
              Particle.PGC = substringvector[3];
              Particle.Px = substringvector[0];
              Particle.Py = substringvector[1];
              Particle.Pz = substringvector[2];
              Particle.P = sqrt(pow(Particle.Px, 2)+pow(Particle.Py, 2)+pow(Particle.Pz, 2)); 
              Particle.transverseP = sqrt(pow(Particle.Px, 2)+pow(Particle.Py, 2)); 
              Particle.azimutalangle = (Particle.Px / Particle.Py); 
              Particle.pseudorapidity = 0.5*(log((Particle.P+Particle.Pz) / (Particle.P-Particle.Pz)));
              Particle_event_vector[Particle_event_relative].Particles.push_back(Particle);
            }
          }
        }

        ifs.close();
    }

    //serialize last time again at 5000000
    serialization(Particle_event_vector, Particle_event_total);
    Particle_event_vector.erase(Particle_event_vector.begin(), Particle_event_vector.end());
    std::cout << "Particle event total: " << Particle_event_total << endl;
    serialization_misc(Particle_total_count, (Particle_event_total / 1000000));

              
    return 0;
  }

//Function to calculate certain averages etc from particle events
Analysed_Data Analyse_Data(std::vector<Particle_event> Particle_event_vector, Analysed_Data Analysed_Data){
  //request if you want to calculate matrix function
  std::cout << "Do you wish to use the matrix function? Y/N" << endl;

  string answer;
  std::cin >> answer;
  std::cout << "You have choosen ";
  if (answer == "Y" | answer == "y" ){
    Analysed_Data.matrix_function = true;
    std::cout << "to use the matrix function." << endl;
  }
  else if (answer == "N" | answer == "n" ) {
    Analysed_Data.matrix_function = false;
    std::cout << "not to use the matrix function." << endl;
  }
  else{
    std::cout << "Error: Wrong Input." << endl;
    abort();
  }
  //delay to read choice
  sleep(1);

  //Gathering overall count of Omega particles in 5 million collisions
  Analysed_Data.Omegaminus_count = 0;
  Analysed_Data.Omegaplus_count = 0;

  //step for calculations
  float step_pseudorapidity = 1;
  float step_transverseP = 0.5;
  int offset_pseudorapidity = 4;
  int Vectorposition = 0;
  int Arrayposition = 0;
  int total_count = 0;
  //single nested for loop to gather data of particles
  for (int event = 0; event < Particle_event_vector.size(); event++){
    for (int particle = 0; particle < Particle_event_vector[event].Particles.size(); particle++){
      //Analysed_Data.total_particle_count++; add this if all particles enabled to get particle count
      if (Particle_event_vector[event].Particles[particle].PGC == 3334){
        Analysed_Data.Omegaminus_count++;
      }
      else if (Particle_event_vector[event].Particles[particle].PGC == -3334){
        Analysed_Data.Omegaplus_count++;
      }
      //comparing particles on pseudorapidity and transverseP by counting and grouping
      if (Analysed_Data.matrix_function and (abs(Particle_event_vector[event].Particles[particle].PGC) == 3334)){ 
        // using formula floor((value + offset) / step) to get array/vector position
        Vectorposition = floor(Particle_event_vector[event].Particles[particle].transverseP / step_transverseP);
        Arrayposition = floor((Particle_event_vector[event].Particles[particle].pseudorapidity + offset_pseudorapidity) / step_pseudorapidity);
        while (Vectorposition >= Analysed_Data.matrix_data.size()){
          //could be converted into vector but should I?
          Analysed_Data.matrix_data.push_back({0,0,0,0,0,0,0,0});
        }
        Analysed_Data.matrix_data[Vectorposition][Arrayposition]++;
        total_count++;
      }
    }
  }
  //to check for error in matrix calculation
  if (total_count != (Analysed_Data.Omegaplus_count + Analysed_Data.Omegaminus_count)){
        std::cout << "Something went wrong in the matrix calculation." << std::endl;
        std::cout << total_count << ", " << (Analysed_Data.Omegaminus_count + Analysed_Data.Omegaminus_count) <<std::endl;
        abort();
  }

  //Doing calculations with gathered data from vectors
  Analysed_Data.Omegaminus_average = Analysed_Data.Omegaminus_count / (double)Analysed_Data.total_particle_count;
  Analysed_Data.Omegaplus_average = Analysed_Data.Omegaplus_count / (double)Analysed_Data.total_particle_count;
  //Getting matter/antimatter ratio
  if (Analysed_Data.Omegaplus_count == 0){ 
    //fraction cant possibly be negative, therefore ratio is not there
    Analysed_Data.average_matter_antimatter_ratio = -1;
  }
  else{
    Analysed_Data.average_matter_antimatter_ratio = Analysed_Data.Omegaminus_count / (double)Analysed_Data.Omegaplus_count;
  }
  

  return Analysed_Data;
}

//Function to display matrix data
void matrix(Analysed_Data Analysed_Data){
  if (!Analysed_Data.matrix_function){
    return;
  }

  //defining line string and matrix string to print later
  string line;
  string matrix = "\n Transverse momentum and pseudorapidity matrix of Omega-minus and Omega-plus particles:\n\n";
  matrix += "|                             | -4 < n < -3 | -3 < n < -2 | -2 < n < -1 | -1 < n <  0 |  0 < n <  1 |  1 < n <  2 |  2 < n <  3 |  3 < n <  4 |\n";
  for (int place = 0; place < 143; place++){
    matrix += "-";
  }
  matrix += "\n";

  //iterating through data matrix  and adjusting letters to print more easy to read matrix
  for (int transverseP_row = 0; transverseP_row < Analysed_Data.matrix_data.size(); transverseP_row++){
    line += "| " + to_string(0+(transverseP_row/2.0)) + " < pT < " + to_string((0.5+(transverseP_row/2.0)));
    while (line.length() < 30){line += " ";}
    line += "| ";
    for (int pseudorapidity_column = 0; pseudorapidity_column < Analysed_Data.matrix_data[transverseP_row].size(); pseudorapidity_column++){
      line += to_string(Analysed_Data.matrix_data[transverseP_row][pseudorapidity_column]);
      while (line.length() < (44 + 14*(pseudorapidity_column))){line += " ";}
      line += "| ";
    }
    matrix += line+"\n";
    line = "";
  }

  //printing readable matrix
  std::cout << matrix << endl;
  
}

//Function to display data
void Display_Data(Analysed_Data Analysed_Data, std::vector<Particle_event> Particle_event_vector){
  std::cout << endl;
  std::cout << "Total amount of recorded particles: " << fixed << Analysed_Data.total_particle_count << endl;
  std::cout << "Total amount of Omega-minus: " <<  Analysed_Data.Omegaminus_count << " and of Omega-plus: " << Analysed_Data.Omegaplus_count << endl;
  std::cout << "Average of Omega-minus: " << Analysed_Data.Omegaminus_average << " and Omega-plus: " << Analysed_Data.Omegaplus_average<< endl;
  std::cout << "Ratio of Omega-minus to Omega-plus: " << Analysed_Data.average_matter_antimatter_ratio << endl;
  //std::cout << "Uncertainty: " << sqrt(Analysed_Data.Omegaminus_count) / (double)Analysed_Data.total_particle_count << endl;
  //displaying matrix
  matrix(Analysed_Data);
}

int main(){

    //Keep track of time at start of program
    auto start_time = std::chrono::high_resolution_clock::now();
    std::cout << "Starting Program." << endl;
    
    //Ask if serialization should be done
    std::cout << "If no serialization, files will be used." << endl;
    std::cout << "Do you wish to serialize? Y/N" << endl;
    bool serialize;
    string answer;
    std::cin >> answer;
    std::cout << "You have choosen: ";
    if (answer == "Y" | answer == "y" ){
      serialize = true;
      std::cout << "Serialization." << endl;
    }
    else if (answer == "N" | answer == "n" ) {
      serialize = false;
      std::cout << "Retrieving of past serialization." << endl;
    }
    else{
      std::cout << "Error: Wrong Input." << endl;
      abort();
    }

    //pause to read choice
    sleep(1);
    
    //Particle event vector
    std::vector<Particle_event> Particle_event_vector;
    //

    if (serialize == true){
    //Get paths of every dataset file
    std::cout << "Getting Datasetpaths." << endl;
    //delay to read choice
    sleep(1);
    //serializing data
    //amount of datasets !!!NEEDS TO BE CHANGED IF NUMBER CHANGES!!!
    uint8_t dataset_num = 10;
    std::vector<string> Datasetpathvector;
    Datasetpathvector = get_paths(Datasetpathvector,dataset_num);
    std::cout << "Finished getting Datasetpaths." << endl;
    //Reading datasets per file per line and adding to vector
    //Vector is automatically being serialized
    std::cout << "Reading Datasets, calculating and serializing Vectors." << endl;
    Read_Calc_Particles(Particle_event_vector, Datasetpathvector, dataset_num);
    std::cout << "Finished reading, calculating and serializing." << endl;
    }

    //Access Particle events from previous run
    //de-serialization about 6 times faster than calculating from scratch
    std::cout << "Accessing previously serialized Data." << endl;
    //delay to read choice
    sleep(1);
    //deserializing misc numbers
    std::vector<int> misc = deserialization_misc();
    //getting total particle number
    //declaring Analysed Data for particle count
    Analysed_Data Analysed_Data;
    Analysed_Data.total_particle_count = misc[0];
    uint32_t Vectorsetfile_num = misc[1];
    //std::cout << fixed << Analysed_Data.total_particle_count << endl;
    //getting particle event vector
    for (uint8_t num_files=0; num_files <= Vectorsetfile_num; num_files++){
      std::vector<Particle_event> Partial_particle_event_vector;
      Partial_particle_event_vector = deserialization(num_files);
      //concatenate vectors
      Particle_event_vector.insert(Particle_event_vector.end(), Partial_particle_event_vector.begin(), Partial_particle_event_vector.end());
    }
    std::cout << "Finished retrieving serialized Data." << endl;

    //Calculating wanted data from Vector for ease of calculation
    std::cout << "Beginning to analyse data." << endl;
    Analysed_Data = Analyse_Data(Particle_event_vector, Analysed_Data);
    std::cout << "Finished analysing data." << endl;

    //Displaying results
    std::cout << "Beginning to display data." << endl;
    Display_Data(Analysed_Data, Particle_event_vector);
    std::cout << "Finished displaying data." << endl;


    //print time
    auto current_time = std::chrono::high_resolution_clock::now();
    std::cout << "Program has been running for " << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() << " seconds." << std::endl;
    std::cout << endl;
    return 0;
}


//uncertainty and table for later