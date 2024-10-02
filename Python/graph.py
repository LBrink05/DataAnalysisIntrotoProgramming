import xml.etree.ElementTree as ET
import numpy as np
import matplotlib.pyplot as plt
import math


#importing all the data from main.cpp
result_path = "Vectorset/results.xml"
tree = ET.parse(result_path)
root = tree.getroot()

class Analysed_Data:
    def __init__(self, event_num, Omegaminus_count, Omegaplus_count, Omega_event_equal_amount_count, Omega_Particle_total_count, Omega_Particle_total_difference, Omega_minus_mean, Omega_minus_mean_uncertainty, Omega_plus_mean, Omega_plus_mean_uncertainty, Omega_mean_difference, Omega_mean_combined_uncertainty, Omega_mean_sigma, Omega_minus_skewness, Omega_minus_skewness_uncertainty, Omega_plus_skewness, Omega_plus_skewness_uncertainty, Omega_minus_kurtosis, Omega_minus_kurtosis_uncertainty, Omega_plus_kurtosis, Omega_plus_kurtosis_uncertainty):
        self.event_num = event_num
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

Analysed_Data_obj = Analysed_Data(float(root[0].text),float(root[1].text),float(root[2].text),float(root[3].text),float(root[4].text),float(root[5].text),float(root[6][0].text),float(root[6][1].text),float(root[6][2].text),float(root[6][3].text),float(root[6][4].text),float(root[6][5].text),float(root[6][6].text),float(root[6][7].text),float(root[6][8].text),float(root[6][9].text),float(root[6][10].text),float(root[6][11].text),float(root[6][12].text),float(root[6][13].text),float(root[6][14].text))

#defining vectors / lists
matter_pseudo_rapidity_vector = [0] * (int(root[6][15][0].text))
matter_transverseP_vector = [0] * (int(root[6][16][0].text))
antimatter_pseudo_rapidity_vector = [0]* (int(root[6][17][0].text))
antimatter_transverseP_vector = [0]* (int(root[6][18][0].text))

#print("matter_pseudo_rapidity_vector:")
element_count = 2
vec = ""
for element in (matter_pseudo_rapidity_vector):
    matter_pseudo_rapidity_vector[element_count-2] = int(root[6][15][element_count].text)
    vec += str(matter_pseudo_rapidity_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")

#print("matter_transverseP_vector:")
element_count = 2
vec = ""
for element in (matter_transverseP_vector):
    matter_transverseP_vector[element_count-2] = int(root[6][16][element_count].text)
    vec += str(matter_transverseP_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")

#print("antimatter_pseudo_rapidity_vector:")
element_count = 2
vec = ""
for element in (antimatter_pseudo_rapidity_vector):
    antimatter_pseudo_rapidity_vector[element_count-2] = int(root[6][17][element_count].text)
    vec += str(antimatter_pseudo_rapidity_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")

#print("matter_transverseP_vector:")
element_count = 2
vec = ""
for element in (antimatter_transverseP_vector):
    antimatter_transverseP_vector[element_count-2] = int(root[6][18][element_count].text)
    vec += str(antimatter_transverseP_vector[element_count-2]) + ", "
    element_count += 1
#print(vec + "\n")


#using matplotlib to draw plots

#pseudo-rapifity of both Omega-particle -> calculate components of diagrams
pseudo_rapidity_ranges = ["-4 to -3","-3 to -2" , "-2 to -1", "-1 to 0","0 to 1",  "1 to 2",  "2 to 3", "3 to 4"]
pseudo_rapidity_ranges_num = np.arange(len(pseudo_rapidity_ranges)) 
pseudo_rapidity_tick_distance = 0

y_err_matter_pseudo_rapidity = [None] * len(matter_pseudo_rapidity_vector)
y_err_antimatter_pseudo_rapidity = [None] * len(antimatter_pseudo_rapidity_vector)
matter_pseudo_rapidity_normalised = [None] * len(matter_pseudo_rapidity_vector)
antimatter_pseudo_rapidity_normalised = [None] * len(antimatter_pseudo_rapidity_vector)

for element in range(0, len(matter_pseudo_rapidity_vector)):
    matter_pseudo_rapidity_normalised[element] = antimatter_pseudo_rapidity_vector[element] / Analysed_Data_obj.event_num
    y_err_matter_pseudo_rapidity[element] = np.sqrt(matter_pseudo_rapidity_vector[element]) / Analysed_Data_obj.event_num
for element in  range(0, len(antimatter_pseudo_rapidity_vector)):
    antimatter_pseudo_rapidity_normalised[element] = antimatter_pseudo_rapidity_vector[element] / Analysed_Data_obj.event_num
    y_err_antimatter_pseudo_rapidity[element] = np.sqrt(antimatter_pseudo_rapidity_vector[element]) / Analysed_Data_obj.event_num

fig, ax_pseudo_rapidity = plt.subplots()
#matter
ax_pseudo_rapidity.errorbar(pseudo_rapidity_ranges_num, matter_pseudo_rapidity_normalised, fmt='o', yerr= y_err_matter_pseudo_rapidity, color = '#1fa1f9', 
        label='Omega-minus') 

#antimatter
ax_pseudo_rapidity.errorbar(pseudo_rapidity_ranges_num + pseudo_rapidity_tick_distance, antimatter_pseudo_rapidity_normalised, fmt='o', yerr=y_err_antimatter_pseudo_rapidity, color = '#FF5733', 
        label='Omega-plus') 

ax_pseudo_rapidity.set_xticks(pseudo_rapidity_ranges_num + pseudo_rapidity_tick_distance/2,pseudo_rapidity_ranges)


#plt.errorbar(ranges_pseudo_rapidity,antimatter_pseudo_rapidity_normalised,  yerr=y_err_antimatter_pseudo_rapidity, fmt="o", color= '#1fa1f9')
ax_pseudo_rapidity.grid()
ax_pseudo_rapidity.set_xlabel("ranges (unit-less)") 
ax_pseudo_rapidity.set_ylabel("Number of Omega particles in each range (normalised)") 
ax_pseudo_rapidity.set_title("Number of Omega particles in pseudo-rapidity ranges\n normalised to total number of events ("+str(Analysed_Data_obj.event_num)+")") 
ax_pseudo_rapidity.legend() 


#matter only
fig, ax_pseudo_rapidity_matter_only = plt.subplots()
ax_pseudo_rapidity_matter_only.errorbar(pseudo_rapidity_ranges_num, matter_pseudo_rapidity_normalised, fmt='o', yerr= y_err_matter_pseudo_rapidity, color = '#1fa1f9', 
        label='Omega-minus') 

ax_pseudo_rapidity_matter_only.set_xticks(pseudo_rapidity_ranges_num,pseudo_rapidity_ranges)
ax_pseudo_rapidity_matter_only.grid()
ax_pseudo_rapidity_matter_only.set_xlabel("ranges (unit-less)") 
ax_pseudo_rapidity_matter_only.set_ylabel("Number of Omega-minus in each range (normalised") 
ax_pseudo_rapidity_matter_only.set_title("Number of Omega-minus particles in pseudo-rapidity ranges\n normalised to total number of events ("+str(Analysed_Data_obj.event_num)+")") 
ax_pseudo_rapidity_matter_only.legend() 

#antimatter only
fig, ax_pseudo_rapidity_antimatter_only = plt.subplots()
ax_pseudo_rapidity_antimatter_only.errorbar(pseudo_rapidity_ranges_num, antimatter_pseudo_rapidity_normalised, fmt='o', yerr= y_err_antimatter_pseudo_rapidity, color = '#FF5733', 
        label='Omega-minus') 
ax_pseudo_rapidity_antimatter_only.set_xticks(pseudo_rapidity_ranges_num,pseudo_rapidity_ranges)
ax_pseudo_rapidity_antimatter_only.grid()
ax_pseudo_rapidity_antimatter_only.set_xlabel("ranges (unit-less)") 
ax_pseudo_rapidity_antimatter_only.set_ylabel("Number of Omega-plus in each range (normalised)") 
ax_pseudo_rapidity_antimatter_only.set_title("Number of Omega-plus particles in pseudo-rapidity ranges\n normalised to total number of events ("+str(Analysed_Data_obj.event_num)+")") 
ax_pseudo_rapidity_antimatter_only.legend() 


#transverse momentum
#redefine vector to fit bin requirements of |0 to 0.5, 0.5 to 1, 1 to 1.5, 1.5 to 2, 2 to 3, 3 to 5, 5 to 10, 10 to 20, 20 to 100
#rewrite!!! 
matter_transverseP_vector[4] = sum(matter_transverseP_vector[4:5]) #
matter_transverseP_vector[5] = sum(matter_transverseP_vector[6:9])
matter_transverseP_vector[6] = sum(matter_transverseP_vector[10:20])
matter_transverseP_vector[7] = sum(matter_transverseP_vector[21:41])
matter_transverseP_vector[8] = sum(matter_transverseP_vector[42:])
matter_transverseP_vector = matter_transverseP_vector [0:9]

antimatter_transverseP_vector[4] = sum(antimatter_transverseP_vector[4:5])
antimatter_transverseP_vector[5] = sum(antimatter_transverseP_vector[6:9])
antimatter_transverseP_vector[6] = sum(antimatter_transverseP_vector[10:20])
antimatter_transverseP_vector[7] = sum(antimatter_transverseP_vector[21:41])
antimatter_transverseP_vector[8] = sum(antimatter_transverseP_vector[42:])
antimatter_transverseP_vector = antimatter_transverseP_vector [0:9]


transverseP_ranges = [None] * len(matter_transverseP_vector)
transverseP_ranges = ["0 to 0.5", "0.5 to 1", "1 to 1.5", "1.5 to 2","2 to 3", "3 to 5", "5 to 10", "10 to 20", "20 to 100"]

transverseP_ranges_num = np.arange(len(transverseP_ranges)) 
transverseP_tick_distance = 0


y_err_matter_transverseP = [None] * len(matter_transverseP_vector)
y_err_antimatter_transverseP = [None] * len(antimatter_transverseP_vector)
matter_transverseP_normalised = [None] * len(matter_transverseP_vector)
antimatter_transverseP_normalised = [None] * len(antimatter_transverseP_vector)

for element in range(0, len(matter_transverseP_vector)):
    matter_transverseP_normalised[element] = matter_transverseP_vector[element] / Analysed_Data_obj.event_num
    y_err_matter_transverseP[element] = np.sqrt(matter_transverseP_vector[element]) / Analysed_Data_obj.event_num

for element in  range(0, len(antimatter_transverseP_vector)):
    antimatter_transverseP_normalised[element] = antimatter_transverseP_vector[element] / Analysed_Data_obj.event_num
    y_err_antimatter_transverseP[element] = np.sqrt(antimatter_transverseP_vector[element]) / Analysed_Data_obj.event_num


fig, ax_transverseP = plt.subplots()


#matter
ax_transverseP.errorbar(transverseP_ranges_num , matter_transverseP_normalised, fmt='o', yerr= y_err_matter_transverseP, color = '#6409ba', 
        label='Omega-minus') 

#antimatter
ax_transverseP.errorbar(transverseP_ranges_num + transverseP_tick_distance, antimatter_transverseP_normalised, fmt='o', yerr=y_err_antimatter_transverseP, color = '#e29b02', 
        label='Omega-plus') 


#plt.errorbar(ranges_pseudo_rapidity,antimatter_pseudo_rapidity_normalised,  yerr=y_err_antimatter_pseudo_rapidity, fmt="o", color= '#1fa1f9')
ax_transverseP.set_xticks(transverseP_ranges_num,transverseP_ranges)
ax_transverseP.grid()
ax_transverseP.set_xlabel("ranges (in GeV)") 
ax_transverseP.set_ylabel("Number of Omega particles in each range") 
ax_transverseP.set_title("Number of Omega particles in transverse momentum ranges\n normalised to total number of events ("+str(Analysed_Data_obj.event_num)+")") 
ax_transverseP.legend() 

#matter only

#matter
fig, ax_transverseP_matter_only = plt.subplots()
ax_transverseP_matter_only.errorbar(transverseP_ranges_num , matter_transverseP_normalised, fmt='o', yerr= y_err_matter_transverseP, color = '#6409ba', 
        label='Omega-minus') 
ax_transverseP_matter_only.set_xticks(transverseP_ranges_num,transverseP_ranges)
ax_transverseP_matter_only.grid()
ax_transverseP_matter_only.set_xlabel("ranges (in GeV)") 
ax_transverseP_matter_only.set_ylabel("Number of Omega particles in each range") 
ax_transverseP_matter_only.set_title("Number of Omega particles in transverse momentum ranges\n normalised to total number of events ("+str(Analysed_Data_obj.event_num)+")") 
ax_transverseP_matter_only.legend() 


#antimatter
fig, ax_transverseP_antimatter_only = plt.subplots()
ax_transverseP_antimatter_only.errorbar(transverseP_ranges_num + transverseP_tick_distance, antimatter_transverseP_normalised, fmt='o', yerr=y_err_antimatter_transverseP, color = '#e29b02', 
        label='Omega-plus') 
ax_transverseP_antimatter_only.set_xticks(transverseP_ranges_num,transverseP_ranges)
ax_transverseP_antimatter_only.grid()
ax_transverseP_antimatter_only.set_xlabel("ranges (in GeV)") 
ax_transverseP_antimatter_only.set_ylabel("Number of Omega particles in each range") 
ax_transverseP_antimatter_only.set_title("Number of Omega particles in transverse momentum ranges\n normalised to total number of events ("+str(Analysed_Data_obj.event_num)+")") 
ax_transverseP_antimatter_only.legend() 

plt.show() 

#transverseP
'''
transverseP_x_axis = [0,0.5]
if (matter_transverseP_vector > antimatter_transverseP_vector):
    for element in range(1, len(matter_transverseP_vector)):
        transverseP_x_axis.append(transverseP_x_axis[element-1] + 0.5)
else:
     for element in antimatter_transverseP_vector:
        transverseP_x_axis.append(transverseP_x_axis[element-1] + 0.5)'''


#Pt spectra plot
#transverse momentum plot