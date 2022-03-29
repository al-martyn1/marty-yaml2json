@rem SELF_PATH
@set SP=%~dp0
@rem ..\tests\marty-json-diff-01.txt 
call ..\run_test.bat marty-yaml2json %SP%\marty-json-diff-02.txt >..\tests\marty-json-diff-02.log.txt
call ..\run_test.bat marty-json-diff %SP%\marty-json-diff-01.txt %SP%\marty-json-diff-02.txt %SP%\marty-json-diff-res.dif

