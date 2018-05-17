for /r . %%a in (*.cpp;*.c) do %astyle% --style=ansi --pad=oper --unpad=paren -s4 -n "%%a"
