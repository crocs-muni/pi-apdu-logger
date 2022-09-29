
#reading from file1 and appending to file2 with ATR
with open("C:\\Users\\lejla\\Desktop\\raspi\\data\\sample1_f.txt", "r") as file1:
    with open("C:\\Users\\lejla\\Desktop\\raspi\\data\\sample1_atr.txt", "a") as file2:
        file2.write(file1.read())