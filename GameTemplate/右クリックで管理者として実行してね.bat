@echo on
@rem tkTool�̒��g��3dsMax�ɃR�s�[�I�I�I�I

@set CURRENT_DIR=%~dp0

@rem maxScript���R�s�[
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2015\scripts\"
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2016\scripts\"
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2017\scripts\"
copy "%CURRENT_DIR%\tools\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2018\scripts\"



@popd
@echo �Z�b�g�A�b�v�I��

@pause