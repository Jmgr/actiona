all: main.obj
        cl /Fe../runner.exe main.c /Os /link /nologo /SUBSYSTEM:WINDOWS /NODEFAULTLIB /ENTRY:wmainCRTStartup libcmt.lib User32.lib Shell32.lib Kernel32.lib
