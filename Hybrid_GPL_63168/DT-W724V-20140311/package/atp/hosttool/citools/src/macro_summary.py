import sys
import json
import os
import stat
import os.path

import xml.etree.ElementTree as etree
from xml.etree.ElementTree import tostring

def get_item_macro(item):
	tmp = item.findall("name")[0]
	return tmp.text

def get_item_package(item):
	tmp = item.findall("package")[0]
	return tmp.text

def get_item_count(item):
	tmp = item.findall("count")[0]
	return tmp.text

def get_item_used(item):
	tmp = item.findall("used")[0]
	return tmp.text

def load_db(file_name):
	xmldoc = etree.parse(file_name)
	root = xmldoc.getroot()
	packages = {}
	macros = {}
	for child in root:
		name = get_item_macro(child)
		package = get_item_package(child)
		count = int(get_item_count(child))
		used = int(get_item_used(child))
		if name in macros:
			if -1 == macros[name]["impacts"].find(package+","):
				macros[name]["impacts"] += package+","
				macros[name]["impact_module_cnt"] += 1
			if used:
				macros[name]["usedby_cnt"] += count
				if -1 == macros[name]["usedby_packages"].find(package+","):
					macros[name]["usedby_packages"] += package+","
					macros[name]["usedby_module_cnt"] += 1
		else:
			macros[name] = {"impact_module_cnt":1, "impacts":package+","}
			if used:
				macros[name]["usedby_cnt"] = count
				macros[name]["usedby_packages"] = package+","
				macros[name]["usedby_module_cnt"] = 1
			else:
				macros[name]["usedby_cnt"] = 0
				macros[name]["usedby_packages"] = ""
				macros[name]["usedby_module_cnt"] = 0
		if package in packages:
			packages[package]["impactby_macro_cnt"] += 1
			packages[package]["impactby_macros"] += name+","
			if used:
				packages[package]["used_cnt"] += count
				if -1 == packages[package]["used_macros"].find(name+","):
					packages[package]["used_macros"] += name+","
					packages[package]["used_macro_cnt"] += 1
		else:
			if used:
				packages[package] = {"impactby_macro_cnt":1, "impactby_macros":name+",", "used_cnt": count, "used_macros":name+",", "used_macro_cnt":1}
			else:
				packages[package] = {"impactby_macro_cnt":1, "impactby_macros":name+",", "used_cnt": 0, "used_macros":"", "used_macro_cnt":0}
	return packages, macros

packages, macros = load_db(sys.argv[1])
#print(len(packages))
#print(len(macros))

reportFile = open("package_macro_report.xml", "w")
reportFile.write("<packages>\n")
for package in packages:
	reportFile.write("<package><name>" + package + "</name><used_macro_cnt>" + str(packages[package]["used_macro_cnt"]) +
				"</used_macro_cnt><used_cnt>"+str(packages[package]["used_cnt"])+
				"</used_cnt><impactby_macro_cnt>"+str(packages[package]["impactby_macro_cnt"])+
				"</impactby_macro_cnt></package>\n")
reportFile.write("</packages>\n")
reportFile.close()

reportFile = open("macro_count_report.xml", "w")
reportFile.write("<macros>\n")
for package in macros:
	reportFile.write("<macro><name>" + package + "</name><usedby_module_cnt>"+str(macros[package]["usedby_module_cnt"])+
		"</usedby_module_cnt><usedby_cnt>"+str(macros[package]["usedby_cnt"])+
		"</usedby_cnt><impact_module_cnt>"+str(macros[package]["impact_module_cnt"])+
		"</impact_module_cnt><usedby_packages>"+str(macros[package]["usedby_packages"])+
		"</usedby_packages></macro>\n")
reportFile.write("</macros>\n")
reportFile.close()
