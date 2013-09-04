Panama 4.2 release notice
-------------------------
A random .plan? How banal. Panama.

 Well, kids, here it is: the spunky panama version 4.2. It does a number of
things that panama 2.4 didn't manage. And it uses much less system overhead,
especially when running as a daemon; so your sysadmins should like it better,
as well.
 Unlike spunky panama version 3.4, daemon mode does not spawn evil child
processes that refuse to die. Finger logging option is also available and
a semantic error has been corrected. My apologies for the half-dazed
release of 4.1.

Usage:
------

 Panama can be made to run in one of two modes: 'single run' or
'daemon mode'.

Single Run Mode
---------------
	In single run mode (with the -d option NOT set), panama simply looks
in <directory> and chooses a random file from this directory. It then writes
this file to the output file (default is the .plan in your home directory). 

	Thus one can set up a directory of .plan candidates which will be
chosen from whenever panama is executed.

	A main advantage of panama over other .plan randomizers is that the
candidate files need be in no special format and need have no special names-
new files can simply be 'thrown' into the directory and files no longer
desired can simply be removed with no effect on panama's run in either
mode.

	If the -c option is set, the .plan file written to will take on the
permissions next specified (see the chmod command).

	If -f or -h is set, the file will be added to the footer or header of
the file, respectively. -o allows you to specify to write the output to a file
other than .plan (eg: a .signature or .project file).
 
	In single run mode, panama can be called in resource or script files
(eg: .login, .mailrc, etc.) to change the outpput file; or it can be set up
as a cron job (see the man pages on crontab for your system's usage) to change
the plan at specified times (e.g.: once an hour).


Daemon Mode
-----------
	A more interesting application of panama is launching it as a daemon
that will change the output file any time something opens it to read it.
Your .plan and .project will change when you are fingered, and every time
you are fingered panama will change the files. Your .signatures will change
with every letter. This mode also has the advantage over running as a cron
job of not changing the output file when it is not necessary.

	To launch panama into daemon mode, specify the -d option. Take note of
the process ID number, as you will need this to kill panama at a later date.
Remember also to remove the output file after you kill panama running in
daemon mode ("rm .plan"), as your plan has actually been converted to a
named pipe. If someone tries to read your .plan after you have killed panama,
their console will hang. Also note that panama must be killed with the -9
option, as it ignores hangup and interrupt signals to avoid dieing on
logout and nasty consequences of interrupts being sent during a read.

	The -f and -h files you specify when you select daemon mode will be
optional.

	If the files are not there, they will simply not be added to your .plan.
If they are, they will. Changing them or deleting them will not phaze panama in
daemon mode. Should you want to change the names of the files that will be
used as footer or header files, however; you will need to kill and restart
panama.

	The -l option allows you to keep tabs upon just who's been fingering
you. The file will appear when you get fingered and will continue to grow
and log each time you are fingered. You may delete the file at any time and
this will "clear" the log for you so it doesn't become cluttered.

	File permissions in daemon mode will only be attributed to the output
when panama is launched. Hence, permissions on the output file can be changed
without needing to kill or restart panama.

Acknowledgements
----------------
Special thanks go out to the following:

	Joe Zbiciak for an introduction to named pipes.
	Jeff Fuller for clerical help, motivation, and beta-testing.
	Jim Federline and 'Drew Kalbrener for beta-testing.
	Samantha Thomas for fingering me incessently for test purposes.


 The program is (c)1993, but can be reproduced or used at will, so long as
the code remains unmodified and this notice is included with the code (and
so long as I don't hate you, in which case, write your own code).
 Please also feel free to make the minor system-spesific modifications that
will allow panama to compile on your system (i.e.: the #define for the ls
command must indicate a bsd compatible version of ls).

 I have had some suggestions that panama be made shareware, and so we'll
go with that (yeah, like any of you will send me anything for panama). You
can send a dollar or a cookie or whatever you might think random .plan files
are worth to you to:

	Jared "Jake" Gage
	690 17th Ave NW,
	New Brighton, MN 55112.
	(convention is to write the "k" in Jake backwards)

 This semi-impoverished college student would appreciate any donations (even
the cookie- it would be nice to see that someone actually liked this program).
If you include your email address, I'll put you on a mailing list to recieve
all updates to panama, as well as any other useless code I might churn out
for a bit.

 That's about it from the trenches.


						-Jake
						(I was The Deathmon)
						deathmon@sunrayce.solar.umn.edu
