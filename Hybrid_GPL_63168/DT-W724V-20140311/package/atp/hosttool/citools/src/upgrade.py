import sys
import json
import os
import stat
import os.path
import atpcommon

info = atpcommon.get_who_am_i()

#print("echo 'upgrade " + sys.argv[1] + " " + info[0] + "' | netcat " + info[1] + " 10000")
os.system("echo 'upgrade " + sys.argv[1] + " " + info[0] + "' | netcat " + info[1] + " 10000")
