@echo off
@pushd	%~dp0

FOR	%%F	IN (
	bin\*.ini
	bin\*.dat
	bin\*.bmp
	bin\*.gif
	bin\*.rgn
	bin\*.jpg
	bin\*.png
	) DO (
echo %%F
	COPY	/Y	%%F	Debug\
	IF	NOT	ERRORLEVEL		0			@PAUSE
	COPY	/Y	%%F	Release\
	IF	NOT	ERRORLEVEL		0			@PAUSE
)

@popd
@echo	on