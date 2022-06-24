
xcopy /y/s ..\Public\"*.*" ..\..\Reference\Inc\
xcopy /y/s ..\Bin\"*.lib" ..\..\Reference\bin\
xcopy /y/s ..\Bin\"*.hlsl" ..\..\Client\Bin\
xcopy /y/s ..\Bin\"*.dll" ..\..\Client\Bin\


xcopy /y/s ..\ThirdPartyLib\"*.lib" ..\..\Reference\bin\
xcopy /y/s ..\ThirdPartyLib\"*.dll" ..\..\Client\Bin\