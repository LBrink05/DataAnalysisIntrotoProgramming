import xml.etree.ElementTree as ET

result_path = "Vectorset/results.xml"
tree = ET.parse(result_path)
root = tree.getroot()


class Analysed_Data:
    def __init__(self, total_particle_count, Omegaminus_count, Omegaplus_count, Omegaminus_average,Omegaplus_average, average_matter_antimatter_ratio, matter_pseudo_rapidity_vector, matter_transverseP_vector, antimatter_pseudo_rapidity_vector, antimatter_transverseP_vector):
        self.total_particle_count = total_particle_count
        self.Omegaminus_count = Omegaminus_count
        self.Omegaplus_count = Omegaplus_count
        self.Omegaminus_average = Omegaminus_average
        self.Omegaplus_average = Omegaplus_average
        self.average_matter_antimatter_ratio = average_matter_antimatter_ratio
        self.matter_pseudo_rapidity_vector = matter_pseudo_rapidity_vector
        self.matter_transverseP_vector = matter_transverseP_vector
        self.antimatter_pseudo_rapidity_vector = antimatter_pseudo_rapidity_vector
        self.antimatter_transverseP_vector = antimatter_transverseP_vector

Data = Analysed_Data(root[0],root[1],root[2],root[3],root[4],root[5],root[6],root[7],root[8],root[9])
print(root[0].text)