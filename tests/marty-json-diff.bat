@rem SELF_PATH
@set SP=%~dp0
@rem ..\tests\marty-json-diff-01.txt 
@call ..\run_test.bat marty-yaml2json %SP%\marty-json-diff-02.txt >..\tests\marty-json-diff-02.log.txt


@call ..\run_test.bat marty-json-diff -j %SP%\marty-json-diff-01.txt %SP%\marty-json-diff-02.txt %SP%\marty-json-diff.json.dif
@call ..\run_test.bat marty-json-diff -y %SP%\marty-json-diff-01.txt %SP%\marty-json-diff-02.txt %SP%\marty-json-diff.yaml.dif

@call ..\run_test.bat marty-json-diff -j %SP%\marty-yaml-diff-01.txt %SP%\marty-yaml-diff-02.txt %SP%\marty-yaml-diff.json.dif
@call ..\run_test.bat marty-json-diff -y %SP%\marty-yaml-diff-01.txt %SP%\marty-yaml-diff-02.txt %SP%\marty-yaml-diff.yaml.dif

