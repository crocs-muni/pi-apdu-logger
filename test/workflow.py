import os

"""
This script automates the whole workflow starting from binary data as captured by RaspberryPi
to a decoded hex dump representing the APDUs.
"""

cmd = "cmd_11"
card = "nxp_j2e081"

# assign directories
rawDataDir = f"..\\tested_cards\\{card}\\raw\\{cmd}" # raw data as captured by RaspberryPi
filteredDataDir = f"..\\tested_cards\\{card}\\filtered\\{cmd}" # data filtered once (whole communication except for ATR)
resultDataDir = f"..\\tested_cards\\{card}\\results\\{cmd}"
decodedDataDir = f"..\\tested_cards\\{card}\\decoded\\{cmd}" # decoded atr + communication
# gpDataDir = f"tested_cards\\{card}\\gp_out"
# resultDataDir = os.path.join("data" + os.sep + "result") # comparison result -> GP output vs. RaspberryPi captured output
 
def execute_cmnd_in_dir(directory:str, executable:str, outDir:str, args:list[str]):
    for filename in os.listdir(directory):
        f = os.path.join(directory + os.sep, filename)
        if os.path.isfile(f):
            name = str(filename).replace(args[0], args[1])
            print(f"INFO: Running {executable} on {filename}.")
            os.system(f'cmd /c ..\\build\\{executable} "{f}" "..\\tested_cards\\{card}\\{outDir}\\{cmd}\\{name}"')

def filter_files():
    execute_cmnd_in_dir(rawDataDir, "filter.exe", "filtered", [".txt", "_f.txt"])

def decode_files():
    execute_cmnd_in_dir(filteredDataDir, "decoder.exe", "decoded", ["_f", "_d"])

# filter_files()
# decode_files()

# format the GP output, remove everything to leave hex dump
# ctr = 1
# for filename in os.listdir(gpDataDir):
#     f = os.path.join(gpDataDir + os.sep, filename)
#     # checking if it is a file
#     if os.path.isfile(f):
#         os.system(f'cmd /c python .\\src\\workflow\\formatFile.py "{f}" "data/gp_out/GP{ctr}_formatted.txt"')
#     ctr+=1


# compare the decoded files to the GP output
# ctr = 11
# for filename in os.listdir(decodedDataDir):
#     f = os.path.join(decodedDataDir + os.sep, filename)
#     # checking if it is a file
#     if os.path.isfile(f):
#         name = str(filename).replace("data_d", "result")
#         os.system(f'cmd /c python .\\compareFiles.py ".\\tested_cards\\{card}\\gp_out\\GP{ctr}_formatted.txt" "{f}" ".\\tested_cards\\{card}\\results\\cmd_{ctr}\\{name}"')
