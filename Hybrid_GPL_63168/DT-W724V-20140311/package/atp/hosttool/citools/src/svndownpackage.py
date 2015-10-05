import sys
import json
import os
import stat
import os.path

def get_svn_baseurl(path):
    result = os.popen("svn info " + path).read()
    for line in result.split("\n"):
    	line = line.strip()
    	if line.startswith("URL:"):
    		line = line[4:]
    		line = line.strip()
    		return line
    return ""

#print('==================')
#print(sys.argv[1])
#print(sys.argv[2])

revision = sys.argv[2][3:]

os.system("svn co -q " + get_svn_baseurl(sys.argv[3]) + "/" + sys.argv[1] + " -r " + revision)
