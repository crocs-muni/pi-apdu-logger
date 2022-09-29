import os

'''
This script opens command line window in the working directory and executes the 
gp --applet 73696D706C656170706C6574 --apdu B0570000023131 -d command in the cmd.
Finally, it saves the output from the cmd to OUT.txt file.
After this is completed, it runs filter.exe and decoder.exe
to filter and decode raw data as captured by Raspberry Pi.
'''

os.system("cmd /c gp --applet 73696D706C656170706C6574 --apdu B0570000023131 -d > GP_out.txt & filter.exe & decoder.exe")
