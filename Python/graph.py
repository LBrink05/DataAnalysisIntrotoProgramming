import xml.etree.ElementTree as ET
#import matplotlib.pyplot as plt
import numpy

result_path = "Vectorset/results.xml"
tree = ET.parse(result_path)
root = tree.getroot()



class Analysed_Data:
    def __init__(self, total_particle_count, Omegaminus_count, Omegaplus_count, Omegaminus_average,Omegaplus_average, average_matter_antimatter_ratio, matter_pseudo_rapidity_vector, matter_transverseP_vector, antimatter_pseudo_rapidity_vector, antimatter_transverseP_vector):
        self.total_particle_count = total_particle_count #0
        self.Omegaminus_count = Omegaminus_count #1
        self.Omegaplus_count = Omegaplus_count #2
        self.Omegaminus_average = Omegaminus_average #3
        self.Omegaplus_average = Omegaplus_average #4
        self.average_matter_antimatter_ratio = average_matter_antimatter_ratio #5
        self.matter_pseudo_rapidity_vector = matter_pseudo_rapidity_vector #6
        self.matter_transverseP_vector = matter_transverseP_vector #7
        self.antimatter_pseudo_rapidity_vector = antimatter_pseudo_rapidity_vector #8
        self.antimatter_transverseP_vector = antimatter_transverseP_vector #9

matter_pseudo_rapidity_vector = [0]* (int(root[6][0].text))
matter_transverseP_vector = [0]* (int(root[7][0].text))
antimatter_pseudo_rapidity_vector = [0]* (int(root[8][0].text))
antimatter_transverseP_vector = [0]* (int(root[9][0].text))

element_count = 2;
for element in (matter_pseudo_rapidity_vector):
    matter_pseudo_rapidity_vector[element_count-2] = int(root[6][element_count].text)
    print(matter_pseudo_rapidity_vector[element_count-2])
    element_count += 1


#fix for later

