:: circle ci windows build script.
::
:: More or less the same as appveyor. However, each cci run
:: command runs in a separate interpreter, so variables are
:: not carried over from one run: to next. Hence, we need a
:: top level script with a single context
::
:: Furthermore, there is quoting problems in .circleci/config.yml making
:: it hard to use paths when calling scripts. This script works around
:: also this.
::
@echo off

PATH C:\Program Files\Cmake\bin;%PATH%
cd %~dp0..
call ci\appveyor.bat
