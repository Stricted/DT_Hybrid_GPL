import sys
import json
import os
import stat
import os.path

import atpcommon

ignored_macros = ["ATP_CMS_DEBUG", "ATP_STATIC", "ATP_CFM_INITKEYLIST", "ATP_CMS_RELEASE", "ATP_CMS_TRACE", "ATP_CMS_MSG_PRIORITY_DEFAULT_D",
				  "ATP_FREE", "ATP_CMS_DoMsgProc", "ATP_CMS_RegMsgProc", "ATP_LOG_LogItemPrint", "ATP_Exec", "ATP_DEBUG_CMS_PC_VERSION",
				  "SUPPORT_DESKTOP", "SUPPORT_UNIT_TEST", "SUPPORT_ATP_TRACE_CODE_DEBUG"]

ignored_start_with_macros = ["_", "__", "NLMSG_", "SOCK_", "CMO_", "SIG", "O_", "F_", "SEEK_", "LOCK_", "IS_", ""]

def is_macro_ignored(line):
	for item in ignored_start_with_macros:
		if line.startswith(item):
			return True
	return False

def get_macro_name(line):
	bUsed = 0
	if line.startswith("#undef "):
		line = line[7:]
	elif line.startswith("#define "):
		line = line[8:]
	elif line.startswith("#used "):
		line = line[6:]
		bUsed = 1
	else:
		return ["", bUsed]

	line = line.strip()
	endPos = line.find(" ")
	if -1 != endPos:
		line = line[:endPos]

	endPos = line.find("(")
	if -1 != endPos:
		line = line[:endPos]

	#if (not line.startswith("SUPPORT_")) and (not line.startswith("ATP_")):
	if not line.startswith("SUPPORT_"):
		return ["", bUsed]

	if line in ignored_macros:
		return ["", bUsed]

	#if is_macro_ignored(line):
	#	return ["", bUsed]

	#if line.islower():
	#	return ["", bUsed]

	return [line, bUsed]

# Load macro file
with open(sys.argv[1], "r") as f:
		content = f.readlines()

# Get macros
result = {}
for line in content:
	if "" == line:
		continue
	[line, bUsed] = get_macro_name(line)
	if "" == line:
		continue
	if line in result:
		if 0 == result[line]["used"]:
			if bUsed:
				result[line]["count"] = 1
				result[line]["used"] = 1
			else:
				result[line]["count"] += 1
				result[line]["used"] = 0
		else:
			if bUsed:
				result[line]["count"] += 1
				result[line]["used"] = 1
	else:
		result[line] = {"count":1, "used":bUsed}

def write_macros_to_xml(result, reportFile, totalReport):
	usedCnt = 0
	totalCnt = 0
	for item in result:
		reportFile.write("<macro><name>" + item + "</name><count>"+str(result[item]["count"])+"</count><used>"+str(result[item]["used"])+"</used>")
		if totalReport:
			reportFile.write("<package>"+sys.argv[2]+"</package>")
		reportFile.write("</macro>\n")
		usedCnt += result[item]["used"]
		if result[item]["used"]:
			totalCnt += result[item]["count"]
	return (usedCnt, totalCnt)

if os.path.exists(sys.argv[3]):
	reportFile = open(sys.argv[3], "a")
	write_macros_to_xml(result, reportFile, True)
	reportFile.close()

reportName = sys.argv[1].replace(".mtxt", "_macro.xml")
reportFile = open(reportName, "wb")
reportFile.write("<macros>\n")
cnts = write_macros_to_xml(result, reportFile, False)
reportFile.write("</macros>\n")
reportFile.close()

usedCnt = cnts[0]
totalCnt = cnts[1]

print("\tModule [" + sys.argv[2] + "] has used " + str(usedCnt) + " macros, totally used count is " + str(totalCnt) + ";\n\treport file: " + reportName)

if not os.path.exists(sys.argv[3]):
	info = atpcommon.get_who_am_i()
	os.system("echo 'open_xml " + info[0] + " " + reportName + "' | netcat " + info[1] + " 10000")
