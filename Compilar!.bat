@echo off
cls
del pxdosc.lib
cls
title Compilando a Biblioteca de Funcoes C do PX-DOS(R)
cls
echo.
echo Compilando Biblioteca de Funcoes C para PX-DOS(R)
echo.
echo Versao 1.6, compativel com PX-DOS 0.9.0+
echo.
echo.
echo Copyright (C) 2013-2016 Felipe Miguel Nery Lunkes
echo Todos os direitos reservados.
echo.
echo.
maker -fcomando.dos
echo.
if exist pxdosc.lib ( GOTO SUCESSO) else (
GOTO FALHA )

:SUCESSO
title Sucesso ao compilar a Biblioteca
echo.
echo.
echo Sucesso ao compilar a Biblioteca de Funcoes C para PX-DOS(R).
echo.
echo.
echo Pronto!
goto FIM

:FALHA
title Falha ao compilar a Biblioteca
echo.
echo.
echo Falha ao compilar a Biblioteca de Funcoes C para PX-DOS(R).
echo.
echo.

:FIM

echo.
echo.
pause
echo.
exit