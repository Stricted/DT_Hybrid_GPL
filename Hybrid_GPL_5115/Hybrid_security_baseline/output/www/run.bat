@echo off
ECHO ɾ���������ļ�
if exist .\language  (rename .\language language1)
if not exist .\language (md language) 
xcopy .\language1\lang_en_us.js .\language\ /E /Q /Y
xcopy .\language1\lang_es_es.js .\language\ /E /Q /Y
xcopy .\language1\lang_fr_fr.js .\language\ /E /Q /Y
xcopy .\language1\lang_ar_sa.js .\language\ /E /Q /Y
rd /S /Q .\language1
ECHO ɾ��tools�ļ�
IF EXIST .\tools ( rd /s/q .\tools )
ECHO ɾ��.svn�ļ�
for /r %%p in (.) do ( if exist %%p/.svn ( rd /s/q "%%p/.svn" ) )
PAUSE
