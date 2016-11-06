#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct Room {
	char roomName[15];
	int numCons;
	int conCount;
	const char * connections[6];
	char type[12];
};

const char * makeDir(char * dirName){
	char pid[10];
	
	// Get the process ID
	snprintf(pid, 10,"%d",(int)getpid());
	// Add the process ID to the directory name
	strcat (dirName, pid); 
	
	printf("%s\n",dirName);
	// Make the directory
	mkdir(dirName, 0755);
	// Return the directory name so that files can me put in it
	return dirName;
}

struct Room rooms[7];

main() {
	
	char dirName[50] = "curtisjo.rooms."; // Name for directory and files
	char fileName[50];
	int consCount, n, j, k, i;
	int conNum;
	int totCons;
	int roomType = 0; // Flag for room type. 0 = START_ROOM, 1 = MID_ROOM, 2 = END_ROOM.
	char startRoom[15];
	char priorRoom[15];
	int fileCount = 0;
	FILE * room1;
	FILE * room2;
	int lineCount = 1; // Count for lines in file
	int addCon = 1; // Flag to identify connection should be added
	const char * startCons[6];
	char line[256]; // Line read from file
	char potentialCon[15];
	
	makeDir(dirName);
	
	// Create Array of Total Room Names
	const char *roomNames[10];
	roomNames[0] = "Dungeon";
	roomNames[1] = "Cellar";
	roomNames[2] = "Attic";
	roomNames[3] = "Kitchen";
	roomNames[4] = "Library";
	roomNames[5] = "Laboratory";
	roomNames[6] = "Aviary";
	roomNames[7] = "Conservatory";
	roomNames[8] = "Tower";
	roomNames[9] = "Bedroom";
	
	// Array of Connection Room Names
	const char *conRoom[6];
	
	// Initializes random number generator 
	srand(time(NULL));
	n = rand() % 10;
	
	// Select Start room
	strcpy (startRoom, roomNames[n]);
	
	// Create room structs
	i = 0;
	int conFlag; 
	int	selfFlag;
	while(i < 7){
		conFlag = 1;
		n = rand() % 10;
		for(k=0; k < i; k++) {
			if (strcmp(rooms[k].roomName, roomNames[n]) == 0){
					conFlag = 0;
			}
		}
		if (conFlag == 1) {
			strcpy (rooms[i].roomName, roomNames[n]);
			printf("Room %d %s \n", i+1,rooms[i].roomName);
			if (i == 0)
				strcpy(rooms[i].type, "START_ROOM");
			else if (i == 6)
				strcpy(rooms[i].type, "END_ROOM");
			else
				strcpy(rooms[i].type, "Mid_ROOM");
			i++;
		}
		rooms[i].conCount = 0;
	}
	
	//Add connections
	for(i = 0; i < 7; i++){
		printf("Type %s\n", rooms[i].type);
		j = rand() % 4 + 3;
		if(j > rooms[i].conCount) {
			rooms[i].numCons = j;
			printf("%s number of connections: %d\n",rooms[i].roomName, rooms[i].numCons);
			while (rooms[i].conCount < rooms[i].numCons){
			selfFlag = 1;
			conFlag = 1;
			n = rand() % 7;
			if (strcmp(rooms[i].roomName, rooms[n].roomName) == 0){
					selfFlag = 0;
			}
			if (selfFlag == 1) {
				for (k = 0; k < rooms[i].conCount ; k++){
					if (strcmp(rooms[i].connections[k],rooms[n].roomName) == 0)
						conFlag = 0;
				}
				if (conFlag == 1){
					//if (rooms[n].conCount + 1 <= rooms[n].numCons){
					rooms[i].connections[rooms[i].conCount] = rooms[n].roomName;
					rooms[n].connections[rooms[n].conCount] = rooms[i].roomName;
					printf("%s Connection %d: %s\n",rooms[i].roomName, rooms[i].conCount +1, rooms[i].connections[rooms[i].conCount]);
					printf("%s Connection %d: %s\n",rooms[n].roomName, rooms[n].conCount +1, rooms[n].connections[rooms[n].conCount]);
					rooms[n].conCount ++;
					rooms[i].conCount ++;
					}
				}
			}
		}
	}
	
	// Write structs to files
	strcat (dirName, "/");
	for (i = 0; i < 7; i++){
		strcpy (fileName, dirName);
		strcat (fileName, rooms[i].roomName);
		room1 = fopen(fileName, "w+");
		fprintf(room1, "ROOM NAME: %s \n", rooms[i].roomName);
		for(k = 0; k < rooms[i].conCount; k++){
			fprintf(room1, "%s %d%s %s \n", "CONNECTION", k+1, ":", rooms[i].connections[k]);
		}
		fprintf(room1, "ROOM TYPE: %s\n", rooms[i].type);
		fclose(room1);
	}
		
	
	return 0;
}
/*


	// Use the conRoom array to build the additional files 
	for(k=0;k<6;k++){
		strcpy (fileName, dirName);
		strcat (fileName, conRoom[k]);
		room1 = fopen(fileName, "w+");
		fileCount++;
		printf ("New file %s fileCount: %d\n", fileName, fileCount);
		fprintf(room1, "%s %s \n", "ROOM NAME:", conRoom[k]);
		fclose(room1);
	}
	
	// Add connections to start room
	strcpy (fileName, dirName);
	strcat (fileName, startRoom);
	room1 = fopen(fileName, "w+");
	// Get random number of connections between 3 and 6
	totCons = rand() % 4 + 3;
	printf("Number of connections: %d\n",totCons);
	
	//randomly select rooms for connections to START_ROOM
	consCount=0;
	while (lineCount < 7) {
		fgets(line, sizeof line, room1);// read a line 
		if (lineCount > 1) {
			while(consCount<totCons){
				addCon = 1;
				j = rand() % 6;
				// Check if connection matches existing connection
					for(k=0;k<consCount;k++){
						if (strcmp(startCons[k], conRoom[j]) == 0){
							addCon = 0;
						}
					}
					if (addCon == 1) {
						startCons[consCount] = conRoom[j];
						printf("%s %d%s %s \n", "CONNECTION", consCount+1, ":", conRoom[j]);
						fprintf(room1, "%s %d%s %s \n", "CONNECTION", consCount+1, ":", conRoom[j]);
						consCount++;
					}	
				//}	
			}				
		}
	lineCount++;	
	}
	// Add the last line of the file that says what the room type is
	//addRoomType(roomType, room1);
	
	// Close file
	fclose(room1);
	
	return 0;
}*/