import string
import os

"""
This function compares the hex dump from gp.exe and hex dump captured by RaspberryPi.
For more info about Global Platform see https://github.com/martinpaljak/GlobalPlatformPro
"""

def compare_files(in_file1_path: string, in_file2_path: string, output_file_path: string):
    expected_out = open(in_file1_path,'r')
    captured_out = open(in_file2_path,'r')
    comparison_result = open(output_file_path,'a')

    expected_lines = expected_out.readlines()
    contents = captured_out.read()
    start = 0
    finish = len(contents)

    for i in range(len(expected_lines)):
        words1 = expected_lines[i].split() # expected hexwords from the template file
        for j in range(len(words1)):
            res = contents.upper().find(words1[j], start, finish)
            if(res == -1):
                comparison_result.write("\nWord: " + words1[j] + " not found")
            else:
                comparison_result.write("\nWord: " + words1[j] + " found at index: " + str(res))
                start = res + len(words1[j]) # move the file cursor after the found word
        comparison_result.write("\n------------------------------------------------------")
        
    expected_out.close()
    captured_out.close()
    comparison_result.close()

if __name__ == "__main__":
    import sys

    print("INFO: Comparing expected and captured data...")
    if len(sys.argv) != 4:
        print(f"USAGE: python {sys.argv[0]} <TEMPLATE DATA FILE PATH> <CAPTURED DATA FILE PATH> <COMPARISON RESULT FILE PATH>")
        sys.exit()
    
    in_file1 = sys.argv[1]
    in_file2 = sys.argv[2]
    out_file = sys.argv[3]
    
    compare_files(in_file1, in_file2, out_file)