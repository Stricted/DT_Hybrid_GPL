import sys
import json
import os
import stat
import os.path

match_start_pattern = "static void _init(void)"
match_end_pattern = "{\r"
subst_pattern = "MODULE_CONSTRUCTOR()\n\
{\n\
    ATP_UTIL_BOOT_TRACK(ATP_MACRO_STRINGFY(ATP_MODULE_NAME));\n"

def substute_content(content):
	global match_start_pattern, match_end_pattern, subst_pattern

	startPos = content.find(match_start_pattern)
	if -1 == startPos:
		return [-1, -1]

	endPos = content[startPos:].find(match_end_pattern)
	if -1 == endPos:
		return [-1, -1]

	return [startPos, startPos+endPos+1]

# Load macro file
with open(sys.argv[1], "r") as f:
		content = f.read()

findRes = substute_content(content)

if -1 != findRes[0]:
	print(content[findRes[0]:findRes[1]])
	reportFile = open(sys.argv[1], "w")
	reportFile.write(content[:findRes[0]])
	reportFile.write(subst_pattern)
	reportFile.write(content[findRes[1]:])
	reportFile.close()
