import sys
import json
import os
import stat
import os.path

def get_macro_name(line):
	line = line.strip()
	endPos = line.find("SUPPORT_")
	if -1 == endPos:
		return "", ""

	line = line[endPos:]
	endPos = 0
	while endPos < len(line) and (str.isalnum(line[endPos]) or "_" == line[endPos]):
		endPos += 1
	name = line[:endPos]
	line = line[endPos:]

	return name, line

# Load macro file
with open(sys.argv[1], "r") as f:
		content = f.readlines()

# Write to report file
reportFile = open(sys.argv[2], "a")

# Get macros
for line in content:
	if "" == line:
		continue
	while "" != line:
		line, endPart = get_macro_name(line)
		if "" == line:
			continue
		reportFile.write("#used " + line + "\n")
		line = endPart

reportFile.close()
