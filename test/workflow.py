import os

"""
This script automates the whole workflow 
of data processing starting with binary data as captured by RaspberryPi
to a decoded hex dump representing the APDUs.
"""

ctr = "27"
cmd = f"cmd_{ctr}"
card = "javacos_a40"

rawDataDir = f"..\\tested_cards\\{card}\\raw\\{cmd}" # raw bit stream as captured by RaspberryPi
filteredDataDir = f"..\\tested_cards\\{card}\\filtered\\{cmd}" # filtered bit stream
decodedDataDir = f"..\\tested_cards\\{card}\\decoded\\{cmd}" # hex dump
gpDataDir = f"..\\tested_cards\\{card}\\gp_out"
resultDataDir = f"..\\tested_cards\\{card}\\results\\{cmd}" # comparison result -> GP output vs. RaspberryPi captured output
 
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

def format_gp_out():
  for file in os.listdir(gpDataDir):
    os.system(f'cmd /c python .\\formatFile.py "{gpDataDir}\\GP{ctr}_out" "{gpDataDir}\\GP{ctr}_formatted.txt"')

def compare_outputs():
    for filename in os.listdir(decodedDataDir):
        f = os.path.join(decodedDataDir + os.sep, filename)
        if os.path.isfile(f):
            name = str(filename).replace("data_d", "result")
            os.system(f'cmd /c python .\\compareFiles.py "{gpDataDir}\\GP{ctr}_formatted.txt" "{f}" "{resultDataDir}\\{name}"')


filter_files()
decode_files()
format_gp_out()
compare_outputs()