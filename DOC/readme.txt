Irman Daemon for OS/2
Version 1.0
Copyright 2002 D.J. van Enckevort
This program is released under the GNU Public License version 2

1. Installation
This program comes as a WarpIN installation package. You will need
WarpIN to install this package. WarpIN can be found at
http://www.xworkplace.org or http://www.netlabs.org.

2. Configuration
Normally the Irman Daemon does not need any special parameters. It
automatically detects the presence of an Irman device and tie itself to
its COM port. To speed up this process you can specify the number of
the COM port on the commandline. OS/2 supports up to 16 COM ports
without special drivers, and so does the Irman Daemon.

When the Irman Daemon is running you will see messages about unhandled
events on your screen when you press a button on your remote control.
Please copy the event string 12 character string of the event you want
to attach an event to, you will need this information to configure the
Irman Daemon.

For each event the Irman Daemon starts a small REXX script called
irman.rx, which must be in the same directory as the Irman Daemon. 
This is a sample of such a script:

/* Sample Irman Rexx procedure */

PARSE ARG IrData

/* CHANGES START HERE */

p.1.name='Open config.sys'
p.1.prog='e.exe'
p.1.args='c:\config.sys'
p.1.irdata='06a000000000'

/* p.0 must equal the total number of records set up */
p.0=1

/* DO NOT EDIT ANYTHING BELOW THIS LINE */

DO i=1 TO p.0
    /* Compare *exact* match */
    IF (p.i.irdata==IrData) THEN
    DO
        /* Start program in background */
        CALL StartProgram p.i.prog, p.i.args
        SAY "STARTED: " || p.i.name
        RETURN 0
    END
END
SAY "UNHANDLED EVENT: " || IrData
RETURN 1

In the script you can use ANY valid REXX command, but you cannot call
back to CMD.EXE so you cannot use any executable directly. The Irman
Daemon however provides a REXX function to start a program in another
session. The syntax of this function is as follows:

CALL IrStartProgram executable, parameters, environment, options

executable	the name of the executable (including the full path if
                it is not found in the path environment)
parameters	any parameters you want to pass to the program
enviroment	any environment variables you want to set for the
                program
options		controls how the program is started. Options can be a
                combination of the following:
                B - execute in the background
                I - inherit the environment from the daemon
                P/V/v/O/o - Program mode: PM, DOS Fullscreen, DOS window,
                            OS/2 Fullscreen or OS/2 Window. If more than one
                            is specified the precedence is as follows: oOvVP
                            with OS/2 Window (o) having the highest precedence

If you just want to start programs you can use the sample script almost
untouched. The only thing you need to do is extend the REXX structured
variables tree at the beginning with your own events.


3. Translation
All messages for the program are stored in a standard OS/2 message file
irm.msg. The source of this file is included with the NLS support
packages. If you have the IBM Message File Utility MKMSGF you can
translate this file to your own language. The MKMSGF utility is
included in the OS/2 Toolkit, and ships with most C/C++ compilers. The
toolkit is available as part of the IBM Developer Toolbox, or as part
of IBM OS/2 Warp 4 Convenience Pack, IBM OS/2 Warp Server for
e-Business Convenience Pack, or SSI eComStation.

4. Credits
Credits have to go in the first place to Joachim Benjamins who
stimulated me to write this package in the first place. Other people
who helped me with various parts of the project are:

* Christian van Enckevort for pointers on C++
* Jos van de Ven for pointers on C++ Classes
* Dink for giving me valuable input on COM port setup on OS/2 
* The people at Evation for providing documentation for the interface

