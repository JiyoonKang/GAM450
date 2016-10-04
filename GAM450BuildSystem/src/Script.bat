
for /R %%f in (.\*.cpp) do (
copy /B %%f temp.file
copy /B CopyrightP1.mtx temp2.file
echo %%~nxf >> temp2.file
copy /B temp2.file+CopyrightP2.mtx+temp.file %%f
del temp.file
del temp2.file
)
for /R %%f in (.\*.h) do (
copy /B %%f temp.file
copy /B CopyrightP1.mtx temp2.file
echo %%~nxf >> temp2.file
copy /B temp2.file+CopyrightP2.mtx+temp.file %%f
del temp.file
del temp2.file
)