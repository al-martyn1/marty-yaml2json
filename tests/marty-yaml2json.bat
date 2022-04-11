@rem SELF_PATH
@set SP=%~dp0

@call ..\run_test.bat marty-yaml2json -d:4 %SP%\test001_001.txt                  >marty-yaml2json_001_1_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\test001_002.txt                  >marty-yaml2json_002_1_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\test001_003.txt                  >marty-yaml2json_003_1_log.json

@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_001_1_log.json   >marty-yaml2json_001_2_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_002_1_log.json   >marty-yaml2json_002_2_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_003_1_log.json   >marty-yaml2json_003_2_log.json

@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_001_2_log.json   >marty-yaml2json_001_3_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_002_2_log.json   >marty-yaml2json_002_3_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_003_2_log.json   >marty-yaml2json_003_3_log.json

@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_001_3_log.json   >marty-yaml2json_001_4_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_002_3_log.json   >marty-yaml2json_002_4_log.json
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_003_3_log.json   >marty-yaml2json_003_4_log.json


@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_description.yaml     >marty-json2yaml_description.json
@call ..\run_test.bat marty-json2yaml          <marty-json2yaml_description.json     >marty-yaml2json_description-new.yaml
@call ..\run_test.bat marty-yaml2json -d:4 %SP%\marty-yaml2json_description-new.yaml >marty-json2yaml_description-new.json
@call ..\run_test.bat marty-json2yaml          <marty-json2yaml_description-new.json >marty-yaml2json_description2-new.yaml



