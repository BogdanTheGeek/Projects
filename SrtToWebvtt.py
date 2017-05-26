#this is a short python script that converts .srt files into .vtt files for use in embedded videos. 
#Just run the script with the name of the file as the argument and you will get a webvtt subtitle.

import sys

argument = str(sys.argv[1]) 
print argument
inputfile = open(argument,"r")
output = "WEBVTT \n\n"
for line in inputfile :
        if "-->" in line :
                output += line.replace(',','.')
        else :
                output += line
inputfile.close()

outputfilename = argument.replace(".srt",".vtt")
outputfile = open(outputfilename , 'w+')
print outputfilename
outputfile.write(output)
outputfile.close()
