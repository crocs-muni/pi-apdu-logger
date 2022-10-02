import os

"""
This script automates the whole workflow starting from binary data as captured by RaspberryPi
to a decoded hex dump representing the APDUs.
"""

# assign directories
rawDataDir = os.path.join("data" + os.sep, "raw") # raw data as captured by RaspberryPi
filteredDataDir = os.path.join("data" + os.sep, "filtered") # data filtered once (whole communication except for ATR)
atrFilteredDir = os.path.join("data" + os.sep, "atr_filtered") # data filtered twice (just the ATR)
decodedDataDir = os.path.join("data" + os.sep, "decoded") # decoded atr + communication
resultDataDir = os.path.join("data" + os.sep + "result") # comparison result -> GP output vs. RaspberryPi captured output
 
# iterate over raw files and filter them
for filename in os.listdir(rawDataDir):
    f = os.path.join(rawDataDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace(".txt", "_f.txt")
        os.system(f'cmd /c .\\exec\\data_processors\\filter.exe "{f}" "data/filtered/{name}"')

# iterate over once filtered data files and filter them again to get the ATR
for filename in os.listdir(filteredDataDir):
    f = os.path.join(filteredDataDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace("_f", "_atr")
        os.system(f'cmd /c .\\exec\\data_processors\\atr_filter.exe "{f}" "data/atr_filtered/{name}"')

# append the rest of the communication to the atr file
files1 = os.listdir(filteredDataDir)
files2 = os.listdir(atrFilteredDir)
for i in range(len(files1)):
    f1 = os.path.join(filteredDataDir + os.sep, files1[i])
    f2 = os.path.join(atrFilteredDir + os.sep, files2[i])
    with open(f1, "r") as file1:
        with open(f2, "a") as file2:
            file2.write(file1.read())
        
# finally decode the file, atr + the communication
for filename in os.listdir(atrFilteredDir):
    f = os.path.join(atrFilteredDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace("_atr", "_d")
        os.system(f'cmd /c .\\exec\\data_processors\\decoder.exe "{f}" "data/decoded/{name}"')

# compare the decoded files to the GP output
for filename in os.listdir(decodedDataDir):
    f = os.path.join(decodedDataDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace("_d", "_r")
        os.system(f'cmd /c python .\\src\\workflow\\compareFiles.py ".\\data\GP_out_formatted.txt" "{f}" "data/results/{name}"')