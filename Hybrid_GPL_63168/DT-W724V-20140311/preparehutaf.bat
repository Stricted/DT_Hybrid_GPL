@echo off

set ftpactions=ftptemp.txt

set ftpserver=10.176.120.19
set ftpusername=123
set ftppasswd=123

set firstdir=atp
set seconddir=hg658b

if not "%1"=="" set firstdir=%1
if not "%2"=="" set seconddir=%2


echo open %ftpserver%>%ftpactions%

echo user %ftpusername%>>%ftpactions%
echo %ftppasswd%>>%ftpactions%

echo mkdir %firstdir%>>%ftpactions%
echo cd %firstdir%>>%ftpactions%
echo mkdir %seconddir%>>%ftpactions%
echo cd %seconddir%>>%ftpactions%
echo del main.bin>>%ftpactions%
echo put output/images/hutaf/main.bin>>%ftpactions%

echo bye>>%ftpactions%
ftp -n -s:%ftpactions%

del %ftpactions%

