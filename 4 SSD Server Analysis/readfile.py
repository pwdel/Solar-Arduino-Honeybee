#!/usr/bin/python

# Open a File
fo = open("2015-08-23_14:10:01.txt","r")

print "Name of the file: ", fo.name
print "Closed or not: ", fo.closed
print "Opening mode: ", fo.mode
print "Softspace flag: ", fo.softspace

str = fo.read(10);
print "Read String is : ", str


# Close File
fo.close()
