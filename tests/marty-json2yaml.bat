@rem SELF_PATH
@set SP=%~dp0

@call ..\run_test.bat marty-json2yaml <test001_004.txt >marty-json2yaml_001.log.yaml
@call ..\run_test.bat marty-json2yaml <test003_001.txt >marty-json2yaml_002.log.yaml
@call ..\run_test.bat marty-json2yaml <amazonaws-application-autoscaling-openapi.json >amazonaws-application-autoscaling-openapi-new.yaml

