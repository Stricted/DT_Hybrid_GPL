#! /usr/bin/env python
#coding=utf-8

import string
import re
import sys
import os
import glob
import getopt
import subprocess
import sqlite3
from stat import *
from colorconsole import terminal

import xml.etree.ElementTree as etree
from xml.etree.ElementTree import tostring

#Average_Complexity
#Complexity_of_Most_Complex_Function
def load_monitor_report(file_name):
	xmldoc = etree.parse(file_name)
	root = xmldoc.getroot()
	tmp = root.findall("StatementAvgComplexity")
	if len(tmp) > 0:
		tmp = tmp[0]
	else:
		tmp = root.findall("Average_Complexity")[0]
	avg = tmp.text
	tmp = root.findall("Complexity_of_Most_Complex_Function")[0]
	max = tmp.text
	return (float(avg), int(max))

def get_item_macro(item):
	tmp = item.findall("name")[0]
	return tmp.text

def get_item_count(item):
	tmp = item.findall("count")[0]
	return tmp.text

def get_item_used(item):
	tmp = item.findall("used")[0]
	return tmp.text

def load_macro_report(file_name):
	xmldoc = etree.parse(file_name)
	root = xmldoc.getroot()
	macros = {}
	for child in root:
		name = get_item_macro(child)
		count = int(get_item_count(child))
		used = int(get_item_used(child))
		if used:
			macros[name] = count
	return macros

def check_new_macros(old, new, screen):
	for item in new:
		if item not in old:
			if new[item] > 3:
				screen.cprint(15, 0, "[Failed], " + item + " used " + str(new[item]) + " times, only 3 times allowed.")
				screen.reset()
				print("")
				return
	screen.cprint(15, 0, "[Passed]")
	screen.reset()
	print("")
	return

def check_total_macros(old, new, screen):
	oldSum = 0
	for item in old:
		oldSum += old[item]
	newSum = 0
	for item in new:
		newSum += new[item]
	if len(old) > 0 and newSum > oldSum:
		screen.cprint(15, 0, "[Failed], " + "increased from " + str(oldSum) + " to " + str(newSum) + ".")
	else:
		screen.cprint(15, 0, "[Passed], " + "decreased from " + str(oldSum) + " to " + str(newSum) + ".")
	print("")
	screen.reset()
	return

def check_existing_complexities(old, new, screen):
	sys.stdout.write("Check avarage complexity: ")
	if float(new[0]) > float(old[0]):
		screen.cprint(15, 0, "[Failed], " + "increased from " + str(old[0]) + " to " + str(new[0]) + ".")
	else:
		screen.cprint(15, 0, "[Passed], " + "decreased from " + str(old[0]) + " to " + str(new[0]) + ".")
	print("")
	screen.reset()

	sys.stdout.write("Check maximum complexity: ")
	if new[1] > old[1]:
		screen.cprint(15, 0, "[Failed], " + "increased from " + str(old[1]) + " to " + str(new[1]) + ".")
	else:
		screen.cprint(15, 0, "[Passed], " + "decreased from " + str(old[1]) + " to " + str(new[1]) + ".")
	print("")
	screen.reset()

def check_new_complexities(new, screen):
	sys.stdout.write("Check avarage complexity: ")
	if float(new[0]) > 7.0:
		screen.cprint(15, 0, "[Failed], " + "avg complexity is " + str(new[0]) + ", only 7 is allowed.")
	else:
		screen.cprint(15, 0, "[Passed], " + "current avg complexity is " + str(new[0]) + ".")
	print("")
	screen.reset()

	sys.stdout.write("Check maximum complexity: ")
	if new[1] > 20:
		screen.cprint(15, 0, "[Failed], " + "max complexity is " + str(new[1]) + ", only 20 is allowed.")
	else:
		screen.cprint(15, 0, "[Passed], " + "current max complexity is " + str(new[1]) + ".")
	print("")
	screen.reset()

screen = terminal.get_terminal()

newMonitor = load_monitor_report(sys.argv[2])
newMacros = load_macro_report(sys.argv[3])

print("\n\n=========Souce code check summary================")
oldMacros = {}
if os.path.exists(sys.argv[5]):
	oldMacros = load_macro_report(sys.argv[5])

sys.stdout.write("Check new macros: ")
check_new_macros(oldMacros, newMacros, screen)
sys.stdout.write("Check totally used macros: ")
check_total_macros(oldMacros, newMacros, screen)

oldMonitor = {}
if os.path.exists(sys.argv[4]):
	oldMonitor = load_monitor_report(sys.argv[4])
	check_existing_complexities(oldMonitor, newMonitor, screen)
else:
	check_new_complexities(newMonitor, screen)
