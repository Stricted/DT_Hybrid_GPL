import sys
import json
import os
import stat
import os.path

import xml.etree.ElementTree as etree
from xml.etree.ElementTree import tostring

call_symbols = ["ATP_CMS_RegGetObj", "ATP_CMS_RegSObj", "ATP_CMS_RegMObj"]

file_name = ""

def get_item_macro(item):
	tmp = item.findall("macro")[0]
	return tmp.text

def get_item_name(item):
	tmp = item.findall("name")[0]
	return tmp.text

macros = {}
def load_macros(macro_file):
	xmldoc = etree.parse(macro_file)
	root = xmldoc.getroot()

	for child in root:
		name = get_item_name(child)
		macro = get_item_macro(child)
		if name.endswith("."):
			macros[name] = macro

def get_macro_of_obj(obj):
	global macros
	if obj.startswith("Device."):
		obj = "InternetGateway" + obj
	if not obj.startswith("InternetGatewayDevice."):
		obj = "InternetGatewayDevice." + obj
	if not obj.endswith("."):
		obj = obj + "."
	if obj in macros:
		return macros[obj]
	if obj.endswith(".1."):
		return ""
	obj = obj + "1."
	if obj in macros:
		return macros[obj]
	return ""

def is_matched(line, idx, reportFile):
	global file_name

	line = line.strip()
	for symbol in call_symbols:
		pos = line.find(symbol)
		if -1 == pos:
			continue

		pos += len(symbol)
		line = line[pos:]
		line = line.strip()

		if not line.startswith("("):
			continue
		line = line[1:]
		line = line.strip()

		if not line.startswith("\""):
			continue
		line = line[1:]
		line = line.strip()

		pos = line.find("\"")
		if -1 == pos:
			continue

		args = line[:pos]
		reportFile.write("<item><name>" + symbol + "</name><file>" + file_name + "</file><line>" + str(idx) + "</line><arg>" + args + "</arg></item>\n")
		return True

	return False

def subst_symbols(line, idx, reportFile):
	global file_name

	origLine = line
	for symbol in call_symbols:
		startPos = line.find(symbol)
		if -1 == startPos:
			continue

		line = line[startPos + len(symbol):]
		argStart = line.find("\"")
		if -1 == argStart:
			continue

		line = line[argStart + 1:]
		endPos = line.find("\"")
		if -1 == endPos:
			continue

		args = line[:endPos]
		macro = get_macro_of_obj(args)
		if "" == macro:
			print("Warning: [" + args + "] can not find MACRO")
			continue
		print(origLine[:startPos] + symbol + "Cmo(" + macro + line[endPos + 1:])
		reportFile.write(origLine[:startPos] + symbol + "Cmo(" + macro + line[endPos + 1:])
		return True

	reportFile.write(origLine)
	return False

# Load macro file
with open(sys.argv[1], "r") as f:
		content = f.readlines()

file_name = sys.argv[1]

bConvert = False
if "convert" == sys.argv[2]:
	reportFile = open(sys.argv[1], "w")
	print("Open macro file " + sys.argv[3])
	load_macros(sys.argv[3])
	bConvert = True
else:
	# Write to report file
	reportFile = open(sys.argv[3], "a")

# Get macros
idx = 0
for line in content:
	idx = idx + 1
	if bConvert:
		subst_symbols(line, idx, reportFile)
	else:
		is_matched(line, idx, reportFile)

reportFile.close()
