import os

def get_who_am_i():
    result = os.popen("who am i").read()
    segs = result.split()
    name = segs[0]
    ip = segs[-1]
    ip = ip.strip()
    ip = ip[1:-1]
    return [name, ip]
