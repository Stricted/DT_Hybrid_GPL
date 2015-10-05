#! /usr/bin/env python
#coding=utf-8
import sys
import string
import os
import os.path
import time

tmp=['/var','/tmp','/mnt']

def is_static(path):
	for x in tmp:
		if path.startswith(x):
			return 0
	return 1
	
def create_device_table(name,offset,uid,files,top_dir):
	fpStatic = open(top_dir+'/output/devicetable/device_table_'+name+'.static','w')
	fpDynamic = open(top_dir+'/output/devicetable/device_table_'+name+'.dynamic','w')
	fpStatic.write('\r\n');
	fpDynamic.write('\r\n');
	
	uid = str(int(uid)+int(offset))
	for x in files.split(';'):
		info = x.split(' ')
		path = info[0]
		type = info[1]
		mode = info[2]
		fp = fpDynamic
		if is_static(path) == 1:
			fp = fpStatic	
		fp.write(path+'	'+type+'				'+ mode +'	'+ uid + '	'+ uid + '	-	-	-	-	-\r\n')
	fpStatic.close()
	fpDynamic.close()

if __name__ == '__main__':
	create_device_table(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],sys.argv[5])