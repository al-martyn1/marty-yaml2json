@call ..\run_test.bat test002 001 %* >test002_001_1_log.json
@call ..\run_test.bat test002 002 %* >test002_002_1_log.json
@call ..\run_test.bat test002 003 %* >test002_003_1_log.json
@call ..\run_test.bat test002 004 %* >test002_004_1_log.json
@call ..\run_test.bat test002 005 %* >test002_005_1_log.json

@rem JSON is valid YAML
@rem So transmorm JSON->JSON must generate the identical data
@call ..\run_test.bat test002 001_1_log.json %* >test002_001_2_log.json
@call ..\run_test.bat test002 002_1_log.json %* >test002_002_2_log.json
@call ..\run_test.bat test002 003_1_log.json %* >test002_003_2_log.json
@call ..\run_test.bat test002 004_1_log.json %* >test002_004_2_log.json
@call ..\run_test.bat test002 005_1_log.json %* >test002_005_2_log.json

