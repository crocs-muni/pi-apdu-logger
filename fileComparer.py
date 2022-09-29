
expected_out = open("C:\\Users\\lejla\\Desktop\\raspi\\data\\GP_out_formatted.txt",'r')
captured_out = open("C:\\Users\\lejla\\Desktop\\raspi\\data\\sample1_d.txt",'r')
comparison_result = open("C:\\Users\\lejla\\Desktop\\raspi\\results\\result_1.txt",'a')

expected_lines = expected_out.readlines()
contents = captured_out.read()
start = 0
finish = len(contents)

for i in range(len(expected_lines)):
    words1 = expected_lines[i].split() #expected hexwords from the GP_OUT file
    for j in range(len(words1)):
        res = contents.upper().find(words1[j], start, finish)
        if(res == -1):
            comparison_result.write("\nWord: " + words1[j] + " not found")
        else:
            comparison_result.write("\nWord: " + words1[j] + " found at index: " + str(res))
            start = res + len(words1[j]) #search for the hexword in the rest of the file
    comparison_result.write("\n------------------------")
        

expected_out.close()
captured_out.close()
comparison_result.close()