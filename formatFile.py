import re
import string

file1 = open("C:\\Users\\lejla\\Desktop\\raspi\\data\\GP_out.txt", "r")

file2 = open("C:\\Users\\lejla\\Desktop\\raspi\\data\\GP2.txt", "w")

file_lines = file1.readlines()

file_size = len(file_lines)

written = False

ctr = 0

for line in file_lines:
	
	# reading all lines that begin with "#"
    x = re.findall("^#", line)
	
    if not x:
    
        words = line.split()
        for word in words:
            if all(c in string.hexdigits for c in word):
                #file2.write(word + ' ') include spaces between commands
                file2.write(word)
                written = True
        ctr += 1
        if(written and ctr < file_size):
            #file2.write('\n')
            written = False
		
file1.close()
file2.close()
