@echo on
@rem tkToolの中身を3dsMaxにコピー！！！！

@set CURRENT_DIR=%~dp0

@rem maxScriptをコピー
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2015\scripts\"
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2016\scripts\"
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2017\scripts\"
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2018\scripts\"



@popd
@echo セットアップ終了

@pause