call basePath.bat
@echo off
set pydatas=%ktpydatas%/avatar_inittab.py
set excel1=%ktexcels%/����������/��ɫ�������Ա�.xlsx
echo on
python xlsx2py/xlsx2py.py %pydatas% %excel1%
if not defined ktall (ping -n 30 127.1>nul)
