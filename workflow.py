import os

# assign directories
rawDataDir = os.path.join("data" + os.sep, "raw") #raw data as captured by RaspberryPi
filteredDataDir = os.path.join("data" + os.sep, "filtered") # data filtered once (whole communication except for ATR)
atrFilteredDir = os.path.join("data" + os.sep, "atr_filtered") # data filtered twice (just the ATR)
decodedDataDir = os.path.join("data" + os.sep, "decoded") # decoded atr + communication
 
# iterate over raw files and filter them
for filename in os.listdir(rawDataDir):
    f = os.path.join(rawDataDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace(".txt", "_f.txt")
        os.system(f'cmd /c .\\filter.exe "{f}" "data/filtered/{name}"')

# iterate over once filtered data files and filter them again to get the ATR
for filename in os.listdir(filteredDataDir):
    f = os.path.join(filteredDataDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace("_f", "_atr")
        os.system(f'cmd /c .\\atr_filter.exe "{f}" "data/atr_filtered/{name}"')

# append the rest of the communication to the atr file
files1 = os.listdir(filteredDataDir)
files2 = os.listdir(atrFilteredDir)
for i in range(len(files1)):
    f1 = os.path.join(filteredDataDir + os.sep, files1[i])
    f2 = os.path.join(atrFilteredDir + os.sep, files2[i])
    with open(f1, "r") as file1:
        with open(f2, "a") as file2:
            file2.write(file1.read())
        
# finally decode the combined files atr + rest of the communication
for filename in os.listdir(atrFilteredDir):
    f = os.path.join(atrFilteredDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace("_atr", "_d")
        os.system(f'cmd /c .\\decoder.exe "{f}" "data/decoded/{name}"')