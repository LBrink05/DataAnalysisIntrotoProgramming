import xml.etree.ElementTree as ET
#import matplotlib.pyplot as plt
import numpy
import math
import string
import matplotlib
import pandas

#importing all the data from main.cpp

result_path = "Vectorset/results.xml"
tree = ET.parse(result_path)
root = tree.getroot()

class Analysed_Data:
    def __init__(self, Omegaminus_count, Omegaplus_count, Omega_event_equal_amount_count, Omega_Particle_total_count, Omega_Particle_total_difference, Omega_minus_mean, Omega_minus_mean_uncertainty, Omega_plus_mean, Omega_plus_mean_uncertainty, Omega_mean_difference, Omega_mean_combined_uncertainty, Omega_mean_sigma, Omega_minus_skewness, Omega_minus_skewness_uncertainty, Omega_plus_skewness, Omega_plus_skewness_uncertainty, Omega_minus_kurtosis, Omega_minus_kurtosis_uncertainty, Omega_plus_kurtosis, Omega_plus_kurtosis_uncertainty):
        self.Omegaminus_count = Omegaminus_count
        self.Omegaplus_count = Omegaplus_count
        self.Omega_event_equal_amount_count = Omega_event_equal_amount_count
        self.Omega_Particle_total_count = Omega_Particle_total_count
        self.Omega_Particle_total_difference = Omega_Particle_total_difference
        self.Omega_minus_mean = Omega_minus_mean
        self.Omega_minus_mean_uncertainty = Omega_minus_mean_uncertainty
        self.Omega_plus_mean = Omega_plus_mean
        self.Omega_plus_mean_uncertainty = Omega_plus_mean_uncertainty
        self.Omega_mean_difference = Omega_mean_difference
        self.Omega_mean_combined_uncertainty = Omega_mean_combined_uncertainty
        self.Omega_mean_sigma = Omega_mean_sigma
        self.Omega_minus_skewness = Omega_minus_skewness
        self.Omega_minus_skewness_uncertainty = Omega_minus_skewness_uncertainty
        self.Omega_plus_skewness = Omega_plus_skewness
        self.Omega_plus_skewness_uncertainty = Omega_plus_skewness_uncertainty
        self.Omega_minus_kurtosis = Omega_minus_kurtosis
        self.Omega_minus_kurtosis_uncertainty = Omega_minus_kurtosis_uncertainty
        self.Omega_plus_kurtosis = Omega_plus_kurtosis
        self.Omega_plus_kurtosis_uncertainty = Omega_plus_kurtosis_uncertainty

Analysed_Data_obj = Analysed_Data(float(root[0].text),float(root[1].text),float(root[2].text),float(root[3].text),float(root[4].text),float(root[5][0].text),float(root[5][1].text),float(root[5][2].text),float(root[5][3].text),float(root[5][4].text),float(root[5][5].text),float(root[5][6].text),float(root[5][7].text),float(root[5][8].text),float(root[5][9].text),float(root[5][10].text),float(root[5][11].text),float(root[5][12].text),float(root[5][13].text),float(root[5][14].text))

#defining vectors / lists
matter_pseudo_rapidity_vector = [0] * (int(root[5][15][0].text))
matter_transverseP_vector = [0] * (int(root[5][16][0].text))
antimatter_pseudo_rapidity_vector = [0]* (int(root[5][17][0].text))
antimatter_transverseP_vector = [0]* (int(root[5][18][0].text))

#print("matter_pseudo_rapidity_vector:")
element_count = 2
vec = ""
for element in (matter_pseudo_rapidity_vector):
    matter_pseudo_rapidity_vector[element_count-2] = int(root[5][15][element_count].text)
    vec += str(matter_pseudo_rapidity_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")

#print("matter_transverseP_vector:")
element_count = 2
vec = ""
for element in (matter_transverseP_vector):
    matter_transverseP_vector[element_count-2] = int(root[5][16][element_count].text)
    vec += str(matter_transverseP_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")

#print("antimatter_pseudo_rapidity_vector:")
element_count = 2
vec = ""
for element in (antimatter_pseudo_rapidity_vector):
    antimatter_pseudo_rapidity_vector[element_count-2] = int(root[5][17][element_count].text)
    vec += str(antimatter_pseudo_rapidity_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")

#print("matter_transverseP_vector:")
element_count = 2
vec = ""
for element in (antimatter_transverseP_vector):
    antimatter_transverseP_vector[element_count-2] = int(root[5][18][element_count].text)
    vec += str(antimatter_transverseP_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")


#using matplotlib 

