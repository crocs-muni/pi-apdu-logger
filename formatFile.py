import re
import string

"""
This function formats the output from gp.exe as
stored by the command line. The end result is a unformatted hex dump.
For more info about Global Platform see https://github.com/martinpaljak/GlobalPlatformPro
"""

def format_file(input_file_path: string, output_file_path: string):
    
    input = open(input_file_path, "r")
    output = open(output_file_path, "w")
    
    input_file_lines = input.readlines()
    input_file_size = len(input_file_lines)
    ctr = 0

    for line in input_file_lines:
	
        x = re.findall("^#", line) # reading all lines that begin with "#"
        y = re.findall("^SCardDisconnect", line) # reading all lines that begin with "SCardDisconnect"
	
        if not x and not y:
    
            words = line.split()
            for word in words:
                if all(c in string.hexdigits for c in word):
                    output.write(word + ' ') # include spaces between commands
            ctr += 1
            if(ctr != input_file_size - 3): # GP log has three extra lines
                output.write('\n')
		
    input.close()
    output.close()

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 3:
        print(f"USAGE: python {sys.argv[0]} <INPUT FILE> <OUTPUT FILE>")
        sys.exit()
    
    in_file = sys.argv[1]
    out_file = sys.argv[2]
    
    format_file(in_file, out_file)
