#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

main() {
	char pid2[10];
	char dirName [25] = "curtisjo.rooms.";
	snprintf(pid2, 10,"%d",(int)getpid());
	strcat (dirName, pid2); 
	printf("%s\n",dirName);
	mkdir(dirName, 0755);
}