import os

os.system('color')


expected = open("C:\\Users\\lejla\\Desktop\\raspi\\data\\GP2.txt",'r')
contents = expected.read()

captured = open("C:\\Users\\lejla\\Desktop\\raspi\\data\\sample2_d.txt",'r')
contents2 = captured.read().upper()


for i in range(0, len(contents), 2):
    original_byte = contents[i:i+2] #take first two bytes of the GP output file
    captured_byte = contents2[i:i+2] #take first two bytes of the captured output file

    if(original_byte == captured_byte):
        print(captured_byte, end='')
    else:
        print(f"\033[1;31;43m{captured_byte}\033[1;37;40m", end='')


expected.close()
captured.close()

