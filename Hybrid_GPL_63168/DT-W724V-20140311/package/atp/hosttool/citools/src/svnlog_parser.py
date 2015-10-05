#! /usr/bin/env python
# -*- coding=utf-8 -*-

from xml.etree.ElementTree import ElementTree
from xml.etree.ElementTree import Element
import sys
import io
import os
import codecs
import StringIO
import re

patterns = {"DTS": u"【问题单号】",
			"Description": u"【问题描述】", "Modification": u"【修改说明】", "TestSuggestion": u"【测试建议】",
			"Modifier": u"【修 改 人】", "Reviewer": u"【检 视 人】"}

file_list_patterns = ["Modified :", "Added :", "Deleted :"]

dts = {}

def match_one_pattern(line, pattern):
	if not line.startswith(pattern):
		return ""
	line = line[len(pattern):]
	return line.strip()

def add_collumn(commit, col_name, col_value):
	subItem = Element(col_name)
	subItem.text = col_value
	commitItem.append(subItem)

def get_log_entry(node, name):
	items = node.findall(name)
	if len(items) > 0:
		return items[0].text
	return ""

def get_pattern_value(lines, pattern):
	#print("\n\nGet " + pattern)
	for line in lines:
		line = line.strip()
		#print(line)
		res = match_one_pattern(line, pattern)
		if "" != res:
			return res
	return ""

def add_dts(res, node):
	global dts
	revision = node.attrib["revision"]
	items = re.split('/| |\||,', res)
	for item in items:
		item = item.strip()
		item = item.upper()
		if not item.startswith("DTS"):
			continue
		subItems = item.split(u'、')
		for subItem in subItems:
			subItem = subItem.strip()
			if subItem in dts:
				dts[subItem]["count"] += 1
				dts[subItem]["revision"] += "," + revision
			else:
				dts[subItem] = {"count": 1, "revision": revision}

def parse_atp_messages(mode, commit, msg):
	#print("\n\n=========================")
	for key in patterns:
		s = StringIO.StringIO(msg)
		res = get_pattern_value(s, patterns[key])
		#print(key + "-->" + res)
		add_collumn(commit, key, res)
		if key == "DTS" and "" != res:
			add_dts(res, node)

def get_paths(node):
	items = node.findall("paths")
	if 0 == len(items):
		return ""
	#print("\n\n===========================")
	res = ""
	for sub_path in items[0]:
		res += sub_path.attrib["action"] + " " + sub_path.attrib["kind"] + " " + sub_path.text + "\n"
	#print(res)
	return res

def report_dts(dts):
	f = codecs.open('DTS_report.xml', "wb", "utf-8-sig")
	f.write("<DTS>\n")
	for item in dts:
		f.write("<item><name>" + item + "</name><count>" + str(dts[item]["count"]) + "</count><revisions>" + dts[item]["revision"] + "</revisions></item>\n")
	f.write("</DTS>")
	f.close()

if len(sys.argv) < 2:
	print("Usage: " + sys.argv[0] + " svn_commit_log_file")
	print('Log format: svn log --xml -v -r {`date +"%Y-%m-%d" -d "1 month ago"`}:{`date +"%Y-%m-%d"`}')
	sys.exit(1)

try:
	tree = ElementTree()
	xmldoc = tree.parse(sys.argv[1])
except:
	print("Parse svn log [" + sys.argv[1] + "] failed.")
	sys.exit(1)

newTree = ElementTree()
newRoot = Element('Commits')
newTree._setroot(newRoot)
for node in xmldoc:
	# Create new node
	commitItem = Element('commit')

	# Add Revision
	add_collumn(commitItem, "Revision", node.attrib["revision"])

	# Add Commitor
	add_collumn(commitItem, "Commitor", get_log_entry(node, "author"))

	# Add Date
	add_collumn(commitItem, "Date", get_log_entry(node, "date"))

	add_collumn(commitItem, "FileList", get_paths(node))

	# Parse ATP message
	parse_atp_messages(node, commitItem, get_log_entry(node, "msg"))

	# Add new node to root
	newRoot.append(commitItem)

newTree.write('commit_report.xml', "UTF-8")

#print(dts)
report_dts(dts)
