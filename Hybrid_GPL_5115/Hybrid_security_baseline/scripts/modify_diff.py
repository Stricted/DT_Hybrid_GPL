#! /usr/bin/env python
#coding=utf-8
import sys
import string
import os

def get_svn_info():
	os.system('svn info > svnurl')
	with open('svnurl') as f:
		for line in f:
			index= line.find('URL: http://')
			if -1 != index:
				line = line[index+len('URL: '):]
				url = line.strip()
			else:
				index = line.find('Revision:')
				if -1 != index:
					line = line[index+len('Revision:'):]
					revision = line.strip()
	return url+" "+revision
	
def diff_modify(filename):
	svninfo = get_svn_info().split(' ')
	url = svninfo[0]
	revision = svninfo[1]
	with open(filename) as f:
		for line in f:
			if '' == line:
				break
			line = line.strip()
			if 0 == len(line):
				continue
			line.replace('\\','/')
			index = line.rfind('/')
			if -1 == index:
				dir = ''
			else:
				dir = line[:index]
			newCmd = 'mkdir -p zzModifyDiffzz/before/'+dir
			os.system(newCmd)
			newCmd = 'mkdir -p zzModifyDiffzz/after/'+dir
			os.system(newCmd)
			newCmd = 'svn export -r '+revision + ' '+url+'/'+line+' zzModifyDiffzz/before/'+line
			print newCmd
			os.system(newCmd)	
			newCmd = 'cp '+line+' zzModifyDiffzz/after/'+line
			os.system(newCmd)

if __name__ == '__main__':
	diff_modify(sys.argv[1])