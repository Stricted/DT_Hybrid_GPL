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
import requests
import json

#urlbase = "http://10.167.100.171:3000"
urlbase = "http://rnd-ams.huawei.com"

payload = {}
headers = {'content-Type': 'application/json; charset=UTF-8'}

patterns = {"DTS": u"【问题单号】",
			"Modifier": u"【修 改 人】", "Reviewer": u"【检 视 人】"}

file_list_patterns = ["Modified :", "Added :", "Deleted :"]

dts = {"dts": "", "svnlog": {}, "svnpath": {}}

def match_one_pattern(line, pattern):
	pos = line.find(pattern)
	if pos < 0:
		return ""
	line = line[pos + len(pattern):]
	return line.strip()

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

def parse_atp_messages(item, msg):
	#print("\n\n=========================")
	dts = ""
	for key in patterns:
		s = StringIO.StringIO(msg)
		res = get_pattern_value(s, patterns[key])
		#print(key + "-->" + res)
		if key == "DTS":
			dts = res
			#add_dts(res, node)
		elif key == "Modifier":
			item["modifier"] = res
		elif key == "Reviewer":
			item["reviewer"] = res
	item["description"] = msg
	return dts

def get_paths(node):
	res = []
	items = node.findall("paths")
	if 0 == len(items):
		return res
	for sub_path in items[0]:
		res.append({"action" : sub_path.attrib["action"], "kind": sub_path.attrib["kind"], "path": sub_path.text})
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

for node in xmldoc:
	item = {}

	# Add Revision
	item["revision"] = node.attrib["revision"]

	# Add Commitor
	item["commitor"] = get_log_entry(node, "author")

	# Add Date
	item["created_at"] = get_log_entry(node, "date")
	item["updated_at"] = item["created_at"]

	# Get file list
	payload["svnpath"] = get_paths(node)

	# Parse ATP message
	payload["dts"] = parse_atp_messages(item, get_log_entry(node, "msg"))
	payload["svnlog"] = item
	print(item["revision"] + "-->" + payload["dts"])

	#print(json.dumps(payload))
	r = requests.post(urlbase + "/svn_logs/addlog", data=json.dumps(payload), headers=headers)

	#break
