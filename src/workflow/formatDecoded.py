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
    content = input.read()

    x = re.findall("3b[0-9A-Fa-f]{36,}|0{4,}[0-9A-Fa-f]{,}|0{2,}4[0-9A-Fa-f]{,}", content) # reading the atr, all lines beginning with 0000 or 0004

    for line in x:
        output.write(line)
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
