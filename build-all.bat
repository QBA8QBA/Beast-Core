@ECHO OFF

IF NOT EXIST "bin\" (
    echo "Bin does not exist. Creating now..."
    MKDIR "bin\"
    echo "Bin created successfully."
) ELSE (
    echo "Bin detected."
)
echo "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#"
ECHO "Building everything..."
PUSHD bin

echo "Clearing Bin..."
rm -rf *
echo "Bin cleared."

echo "#-#-#-#-#-#-#-#-#-#-#-CMAKE_&&_MAKE-#-#-#-#-#-#-#-#-#-#-#"
cmake -G "Unix Makefiles" ..
make
echo "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#"

IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && pause && exit)
ECHO "Built successfully."
pause