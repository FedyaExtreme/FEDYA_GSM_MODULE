@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


"C:\Program Files\IAR Systems\Embedded Workbench 6.0\common\bin\cspybat" "C:\Program Files\IAR Systems\Embedded Workbench 6.0\avr\bin\avrproc.dll" "C:\Program Files\IAR Systems\Embedded Workbench 6.0\avr\bin\avrsim.dll"  %1 --plugin "C:\Program Files\IAR Systems\Embedded Workbench 6.0\avr\bin\avrlibsupportbat.dll" --backend -B "--cpu=m8" "--enhanced_core" "-p" "C:\Program Files\IAR Systems\Embedded Workbench 6.0\avr\Config\iom8.ddf" "--eeprom_size" "512" "-d" "sim" 


