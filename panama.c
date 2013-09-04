/*	A random plan? How banal. Panama.				*/
/*	Version 4.5							*/
/*		Jared "Jake" Gage (I was The Deathmon)			*/
/*	v2.3	11/3/93		(c)1993					*/
/*	v2.4	11/9/93		(c)1993					*/
/*	v3.0	11/17/93	(c)1993					*/
/*	v3.3	11/18/93	(c)1993					*/
/*	v3.4	11/19/93	(c)1993					*/
/*	v4.0	12/11/93	(c)1993					*/
/*	v4.2	12/14/93	(c)1993					*/
/*	v4.3	1/10/94		(c)1994					*/
/*	v4.5	1/13/94		(c)1994					*/
#include<math.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#define	MAXFIELD	256
#define LSPATH		"/bin/ls"	/* ls must be BSD compatible.	*/
#define MKNOD		"/etc/mknod"	/* mknod is often out of bin.	*/
#define DEFAULTFILE	".plan"		/* Default output filename.	*/





/* 	Function USAGE: They done it wrong.				*/
void	usage()
{
printf("Usage: panama [-c <perms>] [-d [-l <file>]] [-f|h|o <file>] <directory>\n");
printf("\t-c File permissions to set on the output file.\n");
printf("\t-d Launch program as a pseudo-daemon.\n");
printf("\t-l File in which to log finger attempts.\n");
printf("\t-f Footer file to add to the end of random file.\n");
printf("\t-h Header file to add to the beginning of random file.\n");
printf("\t-o Output filename.\n");

exit(1);
}			/* End function USAGE.		*/




/*	Function REMOVE_TEMP: Rid ourselves of that pesky file.		*/
void	remove_temp(home_directory)
{
char	command_string[MAXFIELD+20];

strcpy(command_string, home_directory);
strcat(command_string, "/panama.temp");
unlink(command_string);
}			/* End function REMOVE_TEMP.	*/




/*	Function ANALIZE_ARGS: Analize command line arguments.		*/
int	analize_args(argc, argv, access_string, plan_directory, header_file,
		footer_file, log_file, home_directory, output_file)
int	argc;
char	*argv[],
	*access_string[],
	*plan_directory[],
	*header_file[],
	*footer_file[],
	*log_file[],
	*home_directory[],
	*output_file[];
{
char	*ptr,
	temp_string[MAXFIELD];
int	count, daemon_flag, test;

strcpy(access_string, "");
strcpy(plan_directory, "");
strcpy(header_file, "");
strcpy(footer_file, "");
strcpy(log_file, "");
strcpy(output_file, "");
daemon_flag = 0;

for(count=1; (count+1)<=argc; count=count+2)
{
if( (count+1)==argc )
	{
	/* Is arg really an accessable dir?	*/
	strcpy(temp_string, argv[count]);
	strcat(temp_string, "/.");
	if ( !(test = access(temp_string, X_OK)) )
		strcpy(plan_directory, argv[count]);
	if ( test )
		{
		printf("panama: Could not access %s\n", argv[count]);
		exit(1);
		}
	}

if( !strcmp(argv[count], "-d") )	/* Daemon mode switch.	*/
	{
	daemon_flag = 1;
	count--;
	}

if( !strcmp(argv[count], "-h") )	/* Header file arg.	*/
	{
	/* Accept arg now, test validity later.	*/
	strcpy(header_file, argv[count+1]);
	}

if( !strcmp(argv[count], "-f") )	/* Footer file arg.	*/
	{
	/* Accept arg now, test validity later.	*/
	strcpy(footer_file, argv[count+1]);
	}

if( !strcmp(argv[count], "-o") )	/* Output file arg.	*/
	{
	/* Let the shell handle if arg is an unwriteable file.	*/
	strcpy(output_file, argv[count+1]);
	}

if( !strcmp(argv[count], "-c") )	/* Chmod perms arg.	*/
	{
	/* Get it.				*/
	strcpy(access_string, argv[count+1]);
	}

if( !strcmp(argv[count], "-l") )
	{
	/* Get it.				*/
	strcpy(log_file, argv[count+1]);
	}

}

/*	Do they want to log when daemon_mode is not set?		*/

if( strcmp(log_file, "") && (daemon_flag == 0) )
	{
	printf("panama: Finger logging only availiable in daemon_mode.\n");
	exit(1);
	}

/*	Is there a plan_directory assigned?				*/

if( !strcmp(plan_directory, "") )
	usage();

/*	Pull home_directory from the environment variables.		*/

if ( ptr = getenv("HOME") )
	strcpy(home_directory, ptr);
if ( !(ptr = getenv("HOME")) )
	{printf("Couldn't find your home directory.");
	 exit(1);}

/*	Assign default output_file if option -o not set.		*/

if ( !strcmp(output_file, "") )
	{
	strcpy(output_file, home_directory);
	strcat(output_file, "/");
	strcat(output_file, DEFAULTFILE);
	}
return(daemon_flag);
}			/* End function ANALIZE_ARGS.	*/





/*	Function TEST_ARGS: Test the immediate validity of args for
	single run application (not running in daemon mode).		*/
void	test_args(header_file, footer_file)
char	*header_file,
	*footer_file;
{
if ( (access(header_file, R_OK)) && (strcmp(header_file, "")) )
	{
	printf("panama: Could not access %s\n", header_file);
	exit(1);
	}
if ( (access(footer_file, R_OK)) && (strcmp(footer_file, "")) )
	{
	printf("panama: Could not access %s\n", footer_file);
	exit(1);
	}
}			/* End function TEST_ARGS.	*/





/*	Function PLAN_SELECT: Go get a plan_file.			*/
void	plan_select(home_directory, plan_directory, plan_name)
{
char	command_string[MAXFIELD*7],
	max_string[MAXFIELD];
FILE	*input_file_pointer;
int	count, file_number, max, random_value;

/*	Create a file with the first entry as the number of files in the
	PLANDIR directory, and subsequent entries as the names of those
	files.								*/

strcpy(command_string,LSPATH);
strcat(command_string," -1 ");
strcat(command_string,plan_directory);
strcat(command_string," | wc -w > ");
strcat(command_string,home_directory);
strcat(command_string,"/panama.temp");
strcat(command_string," ; ");

strcat(command_string,LSPATH);
strcat(command_string," -1 ");
strcat(command_string,plan_directory);
strcat(command_string," >> ");
strcat(command_string,home_directory);
strcat(command_string,"/panama.temp");
system(command_string);

/*	Read in the first entry, to be used as the max plan number.	*/

strcpy(command_string,home_directory);
strcat(command_string,"/panama.temp");
input_file_pointer = fopen(command_string, "r");
fscanf(input_file_pointer, "%s", max_string);
max = atoi(max_string);

/*	Duz he have a file?						*/

if(max < 1)	/* No file? Set plan_name to null.	*/
	strcpy(plan_name, "");

/*	Pick a card, any card.						*/

srandom( (long)time(0) );
random_value=random(2);
file_number = random_value-((int)(random_value/max))*max+1;

/*	Read in the filename, babe.					*/

for (count = 1; count <= file_number; count++) fscanf(input_file_pointer, "%s",
							plan_name);
fclose(input_file_pointer);
remove_temp(home_directory);

}			/* End function PLAN_SELECT.	*/





/*	Function REPLACE_PLAN: Replaces the new .plan file for single
	run application (not running in daemon_mode).			*/
void	replace_plan(access_string, footer_file, header_file, home_directory,
			plan_directory, plan_name, output_file)
{
char	command_string[MAXFIELD*8];

/*	Put the selected file into .plan.				*/

strcpy(command_string,"cat ");	/* If you can't access cat, fuck you.	*/
if ( strcmp(header_file, "") )
	{strcat(command_string,header_file);
	 strcat(command_string," ");} 
if ( strcmp(plan_name, "") )
	{
	strcat(command_string,plan_directory);
	strcat(command_string,"/");
	strcat(command_string,plan_name);
	strcat(command_string," ");
	}
else if ( !strcmp(plan_name, "") )
	{
	strcat(command_string, "/dev/null ");
	}
if ( strcmp(footer_file, "") )
	{strcat(command_string, footer_file);
	 strcat(command_string, " ");}
strcat(command_string,"> ");
strcat(command_string,output_file);
strcat(command_string," ; ");

/*	Set file permissions.						*/

if( strcmp(access_string, "") )
	{
	strcpy(command_string, "chmod ");
	strcat(command_string, access_string);
	strcat(command_string, " ");
	strcat(command_string, output_file);
	}

system(command_string);

}			/* End function REPLACE_PLAN.	*/





/*	Function DAEMON_MODE: Launches the process as a pseudo-daemon
	to continue running in the background.				*/
void	daemon_mode(access_string, footer_file, header_file, log_file,
			home_directory, plan_directory, plan_name, output_file)
char	header_file[],
	footer_file[],
	log_file[],
	output_file[];
{
char	command_string[MAXFIELD*4],
	file_iden[MAXFIELD],
	IP_string[20],
	plan_file[MAXFIELD*2],
	script_file[MAXFIELD+12],
	*ptr;
FILE	*input_file_pointer,
	*script_file_pointer,
	*plan_file_pointer;
int	c, count, new_kill_script, process_ID;


/*	This is ground control to Major Tom...				*/

if ( (process_ID = fork()) > 0 )
	{
	/*	Make the script that will clean up panama when it is
		no longer desired.					*/

	sprintf(script_file, "%s/.killpanama%i", home_directory, process_ID);
	script_file_pointer = fopen(script_file, "w");

	fprintf(script_file_pointer, "# %s is being controlled by this processs.\n", output_file);	
	fprintf(script_file_pointer, "kill -9 %i\n", process_ID);
	fprintf(script_file_pointer, "rm %s\n", output_file);
	fprintf(script_file_pointer, "rm %s\n", script_file);

	fclose(script_file_pointer);

	strcpy(command_string, "chmod 744 ");
	strcat(command_string, script_file);
	system(command_string);

	/*	Tell the people about it.	*/
	printf("Panama version 4.5 (c)1994 Jared \"Jake\" Gage.\n");
	printf("\tLaunched under PID %i.\n", process_ID);
	printf("\tType \".killpanama%i\" in home dir to halt.\n" ,process_ID);
	if( strcmp(log_file, "") )
		printf("\tLog file set to %s\n", log_file);
	exit(0);
	}
else if ( process_ID < 0 )
	{
	printf("panama: Could not complete fork()\n");
	exit(1);
	}

/*	Here they come to snuff the process. No, he ain't gunna dieie.	*/

for(count=1; count<32; count++)
	signal(count, SIG_IGN);

/*	Wipe out the old output_file (.plan) and replace it with a
	named pipe.							*/

unlink(output_file);			/* Make sure .plan is gone.	*/
strcpy(command_string, MKNOD);		/* Create named pipe file.	*/
strcat(command_string, " ");
strcat(command_string, output_file);
strcat(command_string, " p");
if ( strcmp(access_string, "") )	/* Set file permissions.	*/
	{
	strcat(command_string, " ; chmod ");
	strcat(command_string, access_string);
	strcat(command_string, " ");
	strcat(command_string, output_file);
	}
system(command_string);

while (1)
{

/*	Open the pipe for a write, which freezes the program until
	something, somewhere tries to read that file.			*/

plan_file_pointer = fopen(output_file, "w");

/*	Write the header file to the pipe.				*/

if ( strcmp(header_file, "") )
	{
	input_file_pointer = fopen(header_file, "r");
	if ( input_file_pointer != NULL )
		{
		while ( (c=getc(input_file_pointer)) != EOF )
			putc(c, plan_file_pointer);
		fclose(input_file_pointer);
		}
	}

/*	Select a plan file and write that to the pipe.			*/

plan_select(home_directory, plan_directory, plan_name);

strcpy(plan_file, plan_directory);
strcat(plan_file, "/");
strcat(plan_file, plan_name);

if ( strcmp(plan_name, "") )
	{
	input_file_pointer = fopen(plan_file, "r");
	while ( (c=getc(input_file_pointer)) != EOF )
		putc(c, plan_file_pointer);
	fclose(input_file_pointer);
	}

/*	Write the footer file to the pipe.				*/

if ( strcmp(footer_file, "") )
	{
	input_file_pointer = fopen(footer_file, "r");
	if ( input_file_pointer != NULL )
		{
		while ( (c=getc(input_file_pointer)) != EOF )
			putc(c, plan_file_pointer);
		fclose(input_file_pointer);
		}
	}

/*	Update the log file while we still have a hold on that pipe.	*/

if ( strcmp(log_file, "") )
	{
	strcpy(command_string, "date >> ");
	strcat(command_string, log_file);
	strcat(command_string, " ; ps -au | grep finger | grep -v grep | grep -v \">>\" | head -1 | awk '{ printf \"%s\\t\\t");
	strcat(command_string, plan_file);
	strcat(command_string, "\\n\",$1 }' >> ");
	strcat(command_string, log_file);
	strcat(command_string, " ; netstat -n | grep \"79 \" | grep ESTA | head -1 | awk '{ printf \"%s\\t");
	strcat(command_string, plan_file);
	strcat(command_string, "\\n\",$5 }' >> ");
	strcat(command_string, log_file);
	system(command_string);
	}

/*      Break (close) the pipe, which just sends an EOF to the pipe.
        Any program reading from the .plan file should see this as the
        end of the file and go on with it's life.                       */

fclose(plan_file_pointer);

sleep(1);	/*	Wait for it.	*/

}	/*	End of while(1). Go up and wait again.	*/

}			/* End function DAEMON_MODE.	*/





main(argc, argv)
int	argc;		/* Mess with it not! This arcane code assures	*/
char	*argv[];	/* compilition on arcane Suns.			*/
{
char	access_string[MAXFIELD],
	footer_file[MAXFIELD],
	header_file[MAXFIELD],
	home_directory[MAXFIELD],
	log_file[MAXFIELD],
	output_file[MAXFIELD],
	plan_name[MAXFIELD],
	plan_directory[MAXFIELD+2];
int	daemon_flag;

daemon_flag = analize_args(argc, argv, access_string, plan_directory,
	header_file, footer_file, log_file, home_directory, output_file);

if ( daemon_flag == 0 )		/* Single excecution (non-daemon mode). */
	{
	test_args(header_file, footer_file);
	plan_select(home_directory, plan_directory, plan_name);
	replace_plan(access_string, footer_file, header_file, home_directory,
		plan_directory, plan_name, output_file);
	exit(0);
	}

daemon_mode(access_string, footer_file, header_file, log_file,
		home_directory, plan_directory, plan_name, output_file);
}			/* End function MAIN.		*/