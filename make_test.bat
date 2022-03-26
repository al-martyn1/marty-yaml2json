@rem А потом ручками надо зайти
@rem в testXXXX.vcxproj - поменять в одном месте
@rem в testXXXX.pro     - поменять в двух местах
@rem в testXXXX.cpp     - поменять поменять описание и имя тест файла (или будем брать имя теста за основу?)

@if "%1"=="" goto INVALID_NAME

@if exist src\%1.cpp goto ALREADY


@umba-subst-macros --batch -K --set=TEST_NAME:%1  ^
src\cpp.template=src\%1.cpp ^
_prj_msvc2019\vcxproj_stdcpp20.template=_prj_msvc2019\%1.vcxproj
@rem _prj_qmake\pro.template=_prj_qmake\%1.pro

@rem echo. >test_data\%1.txt

@copy doc\yaml-examples\swagger-example-tinkoff-openapi.yaml test_data\%1_001.txt
@copy doc\yaml-examples\appveyor.yml                         test_data\%1_002.txt
@copy doc\yaml-examples\travis.yml                           test_data\%1_003.txt


git add src\%1.cpp              
git add _prj_msvc2019\%1.vcxproj
@rem git add _prj_qmake\%1.pro       
git add test_data\%1.txt

@exit /b


:INVALID_NAME
@echo Test name not taken
exit /b 1

:ALREADY
@echo Test %1 already exists
exit /b 1
