#!/bin/bash
svn info > svninfo.txt
svninfo=$(sed -n '5p' svninfo.txt)
rm -rf svninfo.txt
svninfo=${svninfo##* } 
echo The last change svn version: $svninfo
echo $svninfo > ../../svnversion.txt
