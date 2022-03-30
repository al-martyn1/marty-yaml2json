@rem SELF_PATH
@set SP=%~dp0
@rem ..\tests\marty-json-diff-01.txt 
@rem call ..\run_test.bat marty-yaml2json %SP%\marty-json-diff-02.txt >..\tests\marty-json-diff-02.log.txt

@call %SP%\marty-json-diff.bat

call ..\run_test.bat marty-json-patch -d:4 %SP%\marty-json-diff-01.txt %SP%\marty-json-diff-res.dif %SP%\marty-json-patch-02.log.txt

call ..\run_test.bat marty-json-patch -d:4 %SP%\marty-yaml-diff-01.txt %SP%\marty-yaml-diff-res.dif %SP%\marty-yaml-patch-02.log.txt


