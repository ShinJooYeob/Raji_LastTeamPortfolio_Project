// Engine 말고 다른 DLL Lib 업데이트
xcopy /y/s ..\Public\"*.*" ..\..\Reference\Inc\
xcopy /y/s ..\Bin\"*.lib" ..\..\Reference\bin\
xcopy /y/s ..\Bin\"*.hlsl" ..\..\Client\Bin\
xcopy /y/s ..\Bin\"*.dll" ..\..\Client\Bin\
xcopy /y/s ..\Bin\"*.dll" ..\..\ZAssimp\Bin\


xcopy /y/s ..\ThirdPartyLib\"*.lib" ..\..\Reference\bin\
xcopy /y/s ..\ThirdPartyLib\"*.dll" ..\..\Client\Bin\
xcopy /y/s ..\ThirdPartyLib\"*.dll" ..\..\ZAssimp\Bin\
