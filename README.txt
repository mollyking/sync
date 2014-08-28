Molly King
CS283 Lab 3

This program will sync two directories. If there are files in Dir1 that are
not in Dir2, it will add files to Dir2. If there are files in Dir2 that are
not in Dir1, it will delete those Dir2 files. If a file exists in both
directories, it will take the most recent change and update the other one.

To run the program:
DIR1=Dir1 DIR2=Dir2 make sync

Dir1 and Dir2 are directories included in the zip file. The contents are:
Dir1 = a.txt b.txt c.txt d.txt(most recent update)
Dir2 = c.txt(most recent update) d.txt e.txt

So, when the program is run on these two directories, a.txt and b.txt should
be added to Dir2. e.txt should be removed from Dir2. Since c.txt in Dir2 is
the most recently updated, it should copy c.txt from Dir2 to Dir1. Since
dt.xt in Dir1 is the most recently updated, it should copy d.txt from Dir1
to Dir2
