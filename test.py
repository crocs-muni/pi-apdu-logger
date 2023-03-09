import re
import os

resultDataDir = "tested_cards\\nxp_cj2a081\\results"
error_rate_list = []

for dirname in os.listdir(resultDataDir):
    d = os.path.join(resultDataDir + os.sep, dirname)
    if(dirname == "cmd_10" or dirname == "cmd_11" or dirname == "cmd_15"):
        for filename in os.listdir(d):
            f = os.path.join(d + os.sep, filename)
            # checking if it is a file
            if os.path.isfile(f):
                input = open(f, "r")
                # print("Analyzing file: ", f)
                content = input.read()
                x = re.findall("Error rate: \d.\d\d|Error rate: \d.\d|Error rate: \d\d.\d\d", content)
                # print(x)
                if(len(x) != 0):
                    if (x[0]) not in error_rate_list:
                        error_rate_list.append((x[0]))


double_copy = []

for i in range(len(error_rate_list)):
    double_copy.append(float(error_rate_list[i].replace("Error rate: ", "")))

# print(error_rate_list)
print("Highest error rate: ", max(double_copy))

dict_list = []

for rate in error_rate_list:
    dic = {
        rate:0
    }
    dict_list.append(dic)

for dirname in os.listdir(resultDataDir):
    d = os.path.join(resultDataDir + os.sep, dirname)
    if(dirname == "cmd_10" or dirname == "cmd_11" or dirname == "cmd_15"):
        for filename in os.listdir(d):
            f = os.path.join(d + os.sep, filename)
            # checking if it is a file
            if os.path.isfile(f):
                input = open(f, "r")
                content = input.read()
                x = re.findall("Error rate: \d.\d\d|Error rate: \d.\d|Error rate: \d\d.\d\d", content)
                if(len(x) != 0):
                    for dic_el in dict_list:
                        if x[0] in dic_el.keys():
                            dic_el[x[0]] += 1
                            break


summarized = 0
zero_to_two = 0
two_to_four = 0
four_to_six = 0
six_to_ten = 0
over_ten = 0


for elem in dict_list:
    for key in elem.keys():
        if(key >= "Error rate: 0.1" and key <= "Error rate: 1.99"):
            zero_to_two += elem[key]
        elif(key >= "Error rate: 2.0" and key <= "Error rate: 3.99"):
            two_to_four += elem[key]
        elif(key >= "Error rate: 4.0" and key <= "Error rate: 5.99"):
            four_to_six += elem[key]
        elif(key >= "Error rate: 6.0" and key <= "Error rate: 9.99"):
            six_to_ten += elem[key]
        elif(key >= "Error rate: 10.0"):
            over_ten += elem[key]
        
    summarized += sum(elem.values())

print("Error rate 0.1-1.99%: ", zero_to_two)
print("Error rate 2.0-3.99%: ", two_to_four)
print("Error rate 4.0-5.99%: ", four_to_six)
print("Error rate 6.0-9.99%: ", six_to_ten)
print("Error rate over 10.0%: ", over_ten)
print(len(dict_list))
print("Total sum: ",summarized)
print(dict_list)
