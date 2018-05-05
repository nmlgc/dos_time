.autodepend
.cacheautodepend

.cpp.obj:
	$(CC) -O -G -Z -x- -o$& -c $**

time.exe: cmd.obj ticks.obj
	$(CC) -M -ls -e$& $**

clean:
	del ticks.obj
	del time.obj
	del time.exe
