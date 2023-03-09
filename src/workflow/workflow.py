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
decodedDataDir = "tested_cards\\nxp_j2e081\\decoded\\cmd_15"
# gpDataDir = os.path.join("data" + os.sep, "gp_out") # GP output directory
gpDataDir = "tested_cards\\nxp_j2e081\\gp_out"
# resultDataDir = os.path.join("data" + os.sep + "result") # comparison result -> GP output vs. RaspberryPi captured output
 
def execute_command_on_directory_contents(directory:str, executable:str, outDir:str, args:list[str]):
    for filename in os.listdir(directory):
        f = os.path.join(directory + os.sep, filename)
        # checking if it is a file
        if os.path.isfile(f):
            name = str(filename).replace(args[0], args[1])
            print(f"INFO: Running {executable} on {filename}.")
            os.system(f'cmd /c .\\exec\\data_processors\\{executable} "{f}" "data/{outDir}/{name}"')

def filter_files():
    execute_command_on_directory_contents(rawDataDir, "filter.exe", "filtered", [".txt", "_f.txt"])

def filter_atr():
    execute_command_on_directory_contents(filteredDataDir, "filter2.exe", "filtered2", ["_f", "_atr"])

def decode_files():
    execute_command_on_directory_contents(os.path.join("data" + os.sep, "filtered2"), "decoder.exe", "decoded", ["_atr", "_d"])

# filter_files()
# filter_atr()
# decode_files()

# format the GP output, remove everything to leave hex dump
# ctr = 1
# for filename in os.listdir(gpDataDir):
#     f = os.path.join(gpDataDir + os.sep, filename)
#     # checking if it is a file
#     if os.path.isfile(f):
#         os.system(f'cmd /c python .\\src\\workflow\\formatFile.py "{f}" "data/gp_out/GP{ctr}_formatted.txt"')
#     ctr+=1

# format the decoded hex file - needs fixing
# ctr = 1
# for filename in os.listdir(decodedDataDir):
#     f = os.path.join(decodedDataDir + os.sep, filename)
#     # checking if it is a file
#     if os.path.isfile(f):
#         os.system(f'cmd /c python .\\src\\workflow\\formatDecoded.py "{f}" "data/decoded/{ctr}_decoded.txt"')
#     ctr+=1

# compare the decoded files to the GP output
ctr = 15
for filename in os.listdir(decodedDataDir):
    f = os.path.join(decodedDataDir + os.sep, filename)
    # checking if it is a file
    if os.path.isfile(f):
        name = str(filename).replace("data_d", "result")
        os.system(f'cmd /c python .\\src\\workflow\\compareFiles.py ".\\tested_cards\\nxp_j2e081\\gp_out\\GP{ctr}_formatted.txt" "{f}" ".\\tested_cards\\nxp_j2e081\\results_2\\cmd_{ctr}\\{name}"')
        # os.system(f'cmd /c python .\\src\\workflow\\compareFiles.py ".\\data\\gp_out\\GP{ctr}_formatted.txt" "{f}" ".\\data\\results\\cmd_{ctr}\\{name}"')
#     # ctr+=1