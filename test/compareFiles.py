import string
import sys
from collections import Counter

"""
This function compares the hex dump from gp.exe and hex dump captured by RaspberryPi.
For more info about Global Platform see https://github.com/martinpaljak/GlobalPlatformPro
It saves the comparison log to a textual file and calculates the error rate.
"""

def calculate_offset(words: list) -> int:
    offset = 6 # first 3 bytes (6 chars) form the prologue field of the block frame for transmission protocol T=1
    for word in words:
        offset += len(word)
    
    return offset

def convert_to_list(stream: string, offset: int) -> list:
    byte_list = []
    for i in range(0, len(stream), 2):
        byte_list.append(stream[offset+i:offset+i+2])
    
    return byte_list

def compare_byte_by_byte(expected_stream: string, captured_stream: string, offset: int) -> list:
    difference = []
    expected_bytes = convert_to_list(expected_stream, 0)
    captured_bytes = convert_to_list(captured_stream, offset)
    
    count = Counter(expected_bytes) # count items in expected_bytes list
    count.subtract(captured_bytes)  # subtract items that are in the captured_bytes list
    
    for byte in expected_bytes:
        if count[byte] > 0:
           count[byte] -= 1
           difference.append(byte)
    
    return difference

def compare_files(in_file1_path: string, in_file2_path: string, out_file_path: string):
    expected_in = open(in_file1_path,'r')
    captured_in = open(in_file2_path,'r')
    comparison_result = open(out_file_path,'a')

    expected_lines = expected_in.readlines()
    captured_lines = captured_in.readlines()
    
    if(len(expected_lines) != len(captured_lines)):
        comparison_result.write("Files have different lengths! Cannot compare them.")
        sys.exit()

    total_bytes = 0
    missing_bytes = 0

    for i in range(len(expected_lines)):
        expected_words = expected_lines[i].split() # expected hexwords from the template file
        start_index = 0
        end_index = len(captured_lines[i])
        
        for j in range(len(expected_words)):
            total_bytes += len(expected_words[j])
            found_at_index = captured_lines[i].upper().find(expected_words[j], start_index, end_index)
            
            if(found_at_index == -1): # the word was not found by the find() method
                if i == 0:
                    offset = 0 # first line consists of the ATR
                else: 
                    offset = calculate_offset(expected_words[0:j])
                
                diff = compare_byte_by_byte(expected_words[j], captured_lines[i].upper(), offset)
                missing_bytes += len(diff)
                
                comparison_result.write(f"\n{len(diff)} byte(s) in word " + expected_words[j] + " not correct.")
                for byte in diff:
                       comparison_result.write(f"\n-missing byte: {byte}") 
                
            else:
                comparison_result.write("\nWord: " + expected_words[j] + " found in line " + str(i+1) + " at index " + str(found_at_index) + ".")
                start_index = found_at_index + len(expected_words[j]) # move the search start index after the found word
        
        comparison_result.write("\n------------------------------------------------------")
    
    error_rate = round((missing_bytes/float(total_bytes/2)) * 100, 2)

    comparison_result.write(f"\nResult: {missing_bytes}/{int(total_bytes/2)} bytes were not found.")
    comparison_result.write(f"\nError rate: {error_rate} %")

    expected_in.close()
    captured_in.close()
    comparison_result.close()

if __name__ == "__main__":

    if len(sys.argv) != 4:
        print(f"USAGE: python {sys.argv[0]} <TEMPLATE DATA FILE PATH> <CAPTURED DATA FILE PATH> <COMPARISON RESULT FILE PATH>")
        sys.exit()
    
    in_file1 = sys.argv[1]
    in_file2 = sys.argv[2]
    out_file = sys.argv[3]
    
    print(f"INFO: Comparing {sys.argv[1]} and {sys.argv[2]}.")
    compare_files(in_file1, in_file2, out_file)
    print(f"INFO: Comparison result in {sys.argv[3]}.")