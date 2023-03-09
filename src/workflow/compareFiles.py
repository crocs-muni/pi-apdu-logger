import string
import sys
from collections import Counter

"""
This function compares the hex dump from gp.exe and hex dump captured by RaspberryPi.
For more info about Global Platform see https://github.com/martinpaljak/GlobalPlatformPro
"""

def calculate_offset(words :list) -> int:
    offset = 6
    for word in words:
        offset += len(word)
    
    return offset

def convert_to_list(stream :string, offset :int) -> list:
    byte_list = []
    for i in range(0, len(stream), 2):
        byte_list.append(stream[offset+i:offset+i+2])
    
    return byte_list

def compare_byte_by_byte(expected, captured, offset) -> list:
    differences = []
    expected_bytes = convert_to_list(expected, 0)
    captured_bytes = convert_to_list(captured, offset)
    
    # for i in range(0, len(expected), 2):
    #     expected_byte = expected[i:i+2]
    #     captured_byte = captured[offset+i:offset+2+i]

    #     if(expected_byte != captured_byte):
    #         pair = (expected_byte, captured_byte)
    #         differences.append(pair)

    count = Counter(expected_bytes) # count items in a
    count.subtract(captured_bytes)  # subtract items that are in b
    for byte in expected_bytes:
        if count[byte] > 0:
           count[byte] -= 1
           differences.append(byte)
    
    return differences

def compare_files(in_file1_path: string, in_file2_path: string, out_file_path: string):
    expected_in = open(in_file1_path,'r')
    captured_in = open(in_file2_path,'r')
    comparison_result = open(out_file_path,'a')

    expected_lines = expected_in.readlines()
    captured_lines = captured_in.readlines()
    
    if(len(expected_lines) != len(captured_lines)):
        comparison_result.write("Files have different lengths!")
        sys.exit()

    total_bytes = 0
    missing_bytes = 0

    for i in range(len(expected_lines)):
        exp_words = expected_lines[i].split() # expected hexwords from the template file
        search_start_index = 0
        search_end_index = len(captured_lines[i])
        
        for j in range(len(exp_words)):
            total_bytes += len(exp_words[j])
            found_word_index = captured_lines[i].upper().find(exp_words[j], search_start_index, search_end_index)
            
            if(found_word_index == -1): # the word was not found by the find() method
                if i == 0:
                    offset = 0
                else: 
                    offset = calculate_offset(exp_words[0:j])
                
                diff = compare_byte_by_byte(exp_words[j], captured_lines[i].upper(), offset)
                missing_bytes += len(diff)
                
                comparison_result.write(f"\n{len(diff)} byte(s) in word " + exp_words[j] + " not correct.")

                for elem in diff:
                       comparison_result.write(f"\n-missing byte: {elem}") 
                
                # for pair in diff:
                #     comparison_result.write(f"\nexpected: {pair[0]}, captured: {pair[1]}")
                
            else:
                comparison_result.write("\nWord: " + exp_words[j] + " found in line " + str(i+1) + " at index " + str(found_word_index) + ".")
                search_start_index = found_word_index + len(exp_words[j]) # move the search start index after the found word
        
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