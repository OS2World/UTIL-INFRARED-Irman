coptions=/Gd+ /Gm+ /C+
debug=/Ti+
libs=rexx.lib
objs=irmand.obj irman.obj irdata.obj timer.obj rexxfunc.obj xcpthandler.obj messages.obj

.cpp.obj:
	$(CC) $(debug) $(coptions) %s
        
irmand.exe: $(objs)
	$(CC) $(debug) $(libs) $**
	rc16 irmand
	dllrname irmand.exe cpprmi36=irmrmi36
        copy /b irmand.exe prog

messages: en\*.txt nl\*.txt
	mkmsgf en\irm.txt en\irm.msg
	mkmsgf nl\irm.txt nl\irm.msg
        copy /b en\irm.msg prog

sourcepkg: *.cpp *.hpp *.rc makefile *.ico
	nmake /i clean
	copy *.* src

wpi:
	wic irmand -a 1 -cprog *.* 2 -cdoc *.* 3 -csrc *.* 4 -cen *.* 5 -cnl *.* -s irman.wis
        
all: irmand.exe messages sourcepkg wpi

clean:
	del *.obj *~ *.res nl\*~ en\*~ *.res *.exe *.wpi
