@rem SELF_PATH
@set SP=%~dp0

@call ..\run_test.bat marty-yaml2json %SP%\test001_001.txt                 2 >marty-yaml2json_001_1_log.json
@call ..\run_test.bat marty-yaml2json %SP%\test001_002.txt                 2 >marty-yaml2json_002_1_log.json
@call ..\run_test.bat marty-yaml2json %SP%\test001_003.txt                 2 >marty-yaml2json_003_1_log.json

@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_001_1_log.json  2 >marty-yaml2json_001_2_log.json
@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_002_1_log.json  2 >marty-yaml2json_002_2_log.json
@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_003_1_log.json  2 >marty-yaml2json_003_2_log.json

@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_001_2_log.json -1 >marty-yaml2json_001_3_log.json
@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_002_2_log.json -1 >marty-yaml2json_002_3_log.json
@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_003_2_log.json -1 >marty-yaml2json_003_3_log.json

@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_001_3_log.json -1 >marty-yaml2json_001_4_log.json
@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_002_3_log.json -1 >marty-yaml2json_002_4_log.json
@call ..\run_test.bat marty-yaml2json %SP%\marty-yaml2json_003_3_log.json -1 >marty-yaml2json_003_4_log.json


