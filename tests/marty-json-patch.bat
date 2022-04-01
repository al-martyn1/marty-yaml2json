@rem SELF_PATH
@set SP=%~dp0
@rem ..\tests\marty-json-diff-01.txt 
@rem call ..\run_test.bat marty-yaml2json %SP%\marty-json-diff-02.txt >..\tests\marty-json-diff-02.log.txt

@call %SP%\marty-json-diff.bat

@call ..\run_test.bat marty-json-patch -j -d:4 %SP%\marty-json-diff-01.txt %SP%\marty-json-diff.json.dif %SP%\marty-json-patch-02.log.json
@call ..\run_test.bat marty-json-patch -y -d:4 %SP%\marty-json-diff-01.txt %SP%\marty-json-diff.yaml.dif %SP%\marty-json-patch-02.log.yaml

@call ..\run_test.bat marty-json-patch -j -d:4 %SP%\marty-yaml-diff-01.txt %SP%\marty-yaml-diff.json.dif %SP%\marty-yaml-patch-02.log.json
@call ..\run_test.bat marty-json-patch -y -d:4 %SP%\marty-yaml-diff-01.txt %SP%\marty-yaml-diff.yaml.dif %SP%\marty-yaml-patch-02.log.yaml


@rem Org files:
@rem marty-json-diff-01.txt
@rem marty-yaml-diff-01.txt

@rem "New" files:
@rem marty-json-diff-02.txt
@rem marty-yaml-diff-02.txt


@rem Diff files:

@rem marty-json-diff.json.dif
@rem marty-json-diff.yaml.dif
                        
@rem marty-yaml-diff.json.dif
@rem marty-yaml-diff.yaml.dif


@rem Результаты патча

@rem marty-json-patch-02.log.json
@rem marty-json-patch-02.log.yaml
                            
@rem marty-yaml-patch-02.log.json
@rem marty-yaml-patch-02.log.yaml


@rem start meld  marty-json-diff-02.txt  marty-json-patch-02.log.json
@rem start meld  marty-yaml-diff-02.txt  marty-yaml-patch-02.log.yaml

