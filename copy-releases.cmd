@echo off
@pushd	%~dp0

@COPY	/Y	Release\*.exe		bin\
@IF	NOT	ERRORLEVEL		0			@PAUSE

@popd
@echo	on