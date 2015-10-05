import sys
import json
import os
import stat
import os.path

def get_who_am_i():
    result = os.popen("who am i").read()
    segs = result.split()
    name = segs[0]
    ip = segs[-1]
    ip = ip.strip()
    ip = ip[1:-1]
    return [name, ip]

info = get_who_am_i()

#print("echo 'upgrade " + sys.argv[1] + " " + info[0] + "' | netcat " + info[1] + " 10000")
os.system("echo 'open_xml " + info[0] + " " + sys.argv[1] + "' | netcat " + info[1] + " 10000")
