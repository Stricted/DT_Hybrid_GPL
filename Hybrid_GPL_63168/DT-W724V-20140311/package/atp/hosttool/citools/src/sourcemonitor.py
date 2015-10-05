import sys
import json
import os
import stat
import os.path
import atpcommon

info = atpcommon.get_who_am_i()

#print("echo 'sourcemonitor " + info[0] + " " + os.path.join(sys.argv[1], sys.argv[3]+".smp") + " " + sys.argv[2] + " " + sys.argv[4] + " " + sys.argv[3] + "' | netcat " + info[1] + " 10000")
os.system("echo 'sourcemonitor " + info[0] + " " + os.path.join(sys.argv[1], sys.argv[3]+".smp") + " " + sys.argv[2] + " " + sys.argv[4] + " " + sys.argv[3] + "' | netcat " + info[1] + " 10000")
