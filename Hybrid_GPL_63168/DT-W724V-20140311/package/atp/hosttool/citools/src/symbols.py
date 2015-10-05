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

def get_who_am_i():
    result = os.popen("who am i").read()
    segs = result.split()
    name = segs[0]
    ip = segs[-1]
    ip = ip.strip()
    ip = ip[1:-1]
    return [name, ip]

def which(program):
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    return None

def focus_text(text, screen):
	screen.cprint(15, 0, text)
	screen.reset()

def err_text(text, screen):
	screen.cprint(15, 0, text)
	screen.reset()

def get_module_name_from_lib(libname):
	return str(libname.replace(".so", "").replace("lib", ""))

def get_library_id_by_name(name, conn):
	sqlcmd = 'select id from modules where name = "%s"' % (name)
	conn.execute(sqlcmd)
	for row in conn:
		return row[0]
	return -1

def get_module_callers(library_id, conn):
	sqlcmd = 'select binary from depends where library_id=%d' % library_id
	conn.execute(sqlcmd)
	res = {}
	for row in conn:
		res[get_module_name_from_lib(row[0])] = 0
	return res

def get_module_depends(library_id, conn):
	sqlcmd = 'select library from depends where binary_id=%d' % library_id
	conn.execute(sqlcmd)
	res = {}
	for row in conn:
		res[get_module_name_from_lib(row[0])] = 0
	return res

# select name from symbols where parent_id=32 and undef=1 and name in (select name from symbols where undef=0)
def get_undefined_symbols(library_id, conn):
	sqlcmd = 'select name from symbols where parent_id=%d and undef=1 and name in (select name from symbols where undef=0)' % library_id
	conn.execute(sqlcmd)
	res = []
	for row in conn:
		func_name = str(row[0])
		res.append(func_name)
	return res

# select * from symbols where parent_id=32 and undef=0
def get_provided_symbols(library_id, conn):
	sqlcmd = 'select name from symbols where parent_id=%d and undef=0' % library_id
	conn.execute(sqlcmd)
	res = []
	for row in conn:
		func_name = str(row[0])
		if "_init" == func_name or "_fini" == func_name:
			continue
		res.append(func_name)
	return res

def get_symbol_called_by(symbol_name, conn):
	sqlcmd = 'select name from modules where id in (select parent_id from symbols where undef=1 and name="%s")' % symbol_name
	conn.execute(sqlcmd)
	res = []
	for row in conn:
		res.append(get_module_name_from_lib(row[0]))
	return res

def get_symbol_provided_by(symbol_name, conn):
	sqlcmd = 'select name from modules where id in (select parent_id from symbols where undef=0 and name="%s")' % symbol_name
	conn.execute(sqlcmd)
	res = []
	for row in conn:
		res.append(get_module_name_from_lib(row[0]))
	return res

def get_call_count(call_id, lib_id):
	pass

def write_mutual_call_sub_graph(callers, depends, warnings, f):
	if 0 == len(warnings):
		return

	f.write("subgraph both_call {\nfontcolor=blue\nnode [shape=Mrecord];\n")
	f.write('struct1 [label="')
	idx = 0
	for item in warnings:
		f.write('<both_call_' + item + '> ' + item)
		idx += 1
		if idx < len(warnings):
			f.write(" | ")
	f.write('"];\n')

	f.write(sys.argv[2] + ' [style=filled, fillcolor=red]\n')

	f.write('struct2 [label="')
	idx = 0
	for item in warnings:
		f.write('<both_depend_' + item + '> ' + item)
		idx += 1
		if idx < len(warnings):
			f.write(" | ")
	f.write('"];\n')

	for item in warnings:
		f.write("struct1:both_call_" + item + " -> " + sys.argv[2] + ' [label="' + str(warnings[item]["call"]) + '"];\n')
		f.write(sys.argv[2] + " -> " + "struct2:both_depend_" + item + ' [label="' + str(warnings[item]["depend"]) + '"];\n')
	f.write('label = "Mutual calls";}\n')

def write_single_call_sub_graph(callers, depends, warnings, f):
	f.write("subgraph both_call {\nfontcolor=blue\nnode [shape=Mrecord];\n")
	idx = 0
	if (len(callers) - len(warnings)) > 0:
		f.write('struct3 [label="')
		for item in callers:
			if item in warnings:
				continue
			f.write('<s_call_' + item + '> ' + item)
			idx += 1
			if idx < (len(callers) - len(warnings)):
				f.write(" | ")
		f.write('"];\n')

	f.write(sys.argv[2] + '2 [style=filled, fillcolor=blue, label="' + sys.argv[2] + '"]\n')

	if (len(depends) - len(warnings)) > 0:
		f.write('struct4 [label="')
		idx = 0
		for item in depends:
			if item in warnings:
				continue
			f.write('<s_depend_' + item + '> ' + item)
			idx += 1
			if idx < (len(depends) - len(warnings)):
				f.write(" | ")
		f.write('"];\n')

	for item in callers:
		if item in warnings:
			continue
		f.write("struct3:s_call_" + item + " -> " + sys.argv[2] + '2 [label="' + str(callers[item]) + '"];\n')
	for item in depends:
		if item in warnings:
			continue
		f.write(sys.argv[2] + "2 -> " + "struct4:s_depend_" + item + ' [label="' + str(depends[item]) + '"];\n')
	f.write('label = "Single calls";}\n')

# Connect to database
conn = sqlite3.connect(sys.argv[1])
c = conn.cursor()

# Get module name
moduleName = "lib"+sys.argv[2]+".so"

# Get library_id 
lib_id = get_library_id_by_name(moduleName, c)

callers = get_module_callers(lib_id, c)
depends = get_module_depends(lib_id, c)

# Report provided symbols
should_be_static_cnt = 0
provided = get_provided_symbols(lib_id, c)
report = os.path.join(sys.argv[3], sys.argv[2] + "_symbols.xml")
f = open(report, "wb")
f.write('<?xml version="1.0" encoding="UTF-8"?>\n<symbols>\n')
for item in provided:
	called_by = get_symbol_called_by(item, c)
	if 0 == len(called_by):
		should_be_static_cnt += 1
	for modo in called_by:
		if modo in callers:
			callers[modo] += 1
		else:
			callers[modo] = 1
	f.write("<item><name>" + item + "</name>\n" + "<refcnt>" + str(len(called_by)) + "</refcnt>\n" +  "<called_by>" + ",".join(called_by) + "</called_by></item>\n")
f.write('</symbols>\n')
f.close()

# Report undefined symbols
report = os.path.join(sys.argv[3], sys.argv[2] + "_undefined_symbols.xml")
f = open(report, "wb")
f.write('<?xml version="1.0" encoding="UTF-8"?>\n<symbols>\n')
used = get_undefined_symbols(lib_id, c)
for item in used:
	provided_module = get_symbol_provided_by(item, c)
	for modo in provided_module:
		if modo in depends:
			depends[modo] += 1
		else:
			depends[modo] = 1
	f.write("<item><name>" + item + "</name>\n" + "<provided_by>" + ",".join(provided_module) + "</provided_by></item>\n")
f.write('</symbols>\n')
f.close()

warnings = {}
for item in callers:
	if item in depends:
		warnings[item] = {"call":callers[item], "depend":depends[item]}

if os.path.exists(sys.argv[4]):
	print("Export symbol summaries of [" + sys.argv[2] + "] now ...")
	with open(sys.argv[4], "a") as f:
		f.write("<item><name>"+ sys.argv[2] + "</name><symbol_cnt>" + str(len(provided)) + "</symbol_cnt><undefined_cnt>" + str(len(used)) + "</undefined_cnt><should_be_static>" + str(should_be_static_cnt) +
			"</should_be_static><used_by>" + str(len(callers)) + "</used_by><depends_on>" + str(len(depends)) + "</depends_on><mutual_cnt>" +
			str(len(warnings)) + "</mutual_cnt><mutuals>" + ",".join(warnings) + "</mutuals></item>\n")
	# Close connection to database
	c.close()
	conn.close()
	sys.exit(0)


screen = terminal.get_terminal()

print("============================Dependence report for [" + sys.argv[2] + "]")
sys.stdout.write("Public functions count: ")
focus_text(str(len(provided)), screen)
sys.stdout.write("; Used by modules count: ")
focus_text(str(len(callers)), screen)
if should_be_static_cnt > 0:
	sys.stdout.write("; Not used public functions count:")
	err_text(str(should_be_static_cnt), screen)
sys.stdout.write("\nCalled functions count: ")
focus_text(str(len(used)), screen)
sys.stdout.write("; Depends on moduls count: ")
focus_text(str(len(depends)), screen)
if len(warnings) > 0:
	sys.stdout.write("\nMutual called modules: ")
	focus_text(str(len(warnings)), screen)
	sys.stdout.write(": " + ",".join(warnings))

print("\n\nDeatailed report files can be found here:")
print(os.path.join(sys.argv[3], sys.argv[2]) + "_symbols.xml")
print(os.path.join(sys.argv[3], sys.argv[2]) + "_undefined_symbols.xml")

graph_text = os.path.join(sys.argv[3], sys.argv[2] + "_graph.txt")
f = open(graph_text, "wb")
f.write("digraph g{\n")
write_mutual_call_sub_graph(callers, depends, warnings, f)
write_single_call_sub_graph(callers, depends, warnings, f)
f.write("}\n")
f.close()
os.system("dot " + graph_text + " -Tpng -o" + os.path.join(sys.argv[3], sys.argv[2]) + ".png")
info = get_who_am_i()
os.system("echo 'open_file " + info[0] + " " + os.path.join(sys.argv[3], sys.argv[2]) + ".png' | netcat " + info[1] + " 10000")

# Close connection to database
c.close()
conn.close()
