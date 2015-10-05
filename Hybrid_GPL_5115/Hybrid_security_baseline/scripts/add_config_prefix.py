#! /usr/bin/env python
#coding=utf-8
import sys
import string

def add_content_prefix(filename):
    found = False
    file = open(filename)
    ret = ""
    while 1:
        lines = file.readlines(100000)
        if not lines:
            break
        for line in lines:
            if line.startswith("# "):
                if -1 != line.find(" is not set"):
                    if line.startswith("# CONFIG_"):
                        ret += line
                    else:
                        ret += "# CONFIG_" + line[2:]
                else:
                    ret += line
            elif line.startswith("#"):
                ret += line
            elif "" == string.strip(line):
                ret += line
            elif line.startswith("CONFIG_"):
                ret += line
            else:
                ret += "CONFIG_" + line

    file.close
    return ret

def add_token_if_not_exist(filename):
	bFound = False
	file = open(filename)
	while 1:
		lines = file.readlines(100000)
		if not lines:
			break
		for line in lines:
			if line.startswith("# ATP Linux Kernel Extension"):
				bFound = True
				break
		if bFound:
			break
	file.close

	if bFound:
		return

	file = open(filename, "ab")
	file.write("\n#\n# ATP Linux Kernel Extension\n")
	file.close

if __name__ == '__main__':
	if "add_config" == sys.argv[1]:
		atp_config = add_content_prefix(sys.argv[2])
		file = open(sys.argv[2], "w")
		file.write(atp_config)
		file.close
	else:
		add_token_if_not_exist(sys.argv[1])

