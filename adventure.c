//interface.c
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

// Function to make the directory
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

//Function to run the user interface
const char * enterRoom(char * dirName,char * roomName, struct Room rooms[7]){
	int endRoom = 0; //Flag to check if the user has reached the end room
	int i;
	int validInput = 0; // Flag to check if the user has entered a valid room name
	char nextRoom[15]; 
	FILE * room2;
	char fileName[50];
	int lineCount = 0; // Count for lines in file
	char line[256]; // Line read from file
	char line2[256];
	char input[15];
	int stepsCount = 0; // Counter for the number of steps
	const char * steps[14]; //Array to keep track of the steps taken
	int tempCount = 0; // counter for each rooms connections
	const char * tempCons[6]; // Array to keep each room connections for input validation
	
	strcpy(nextRoom,roomName);
	
	while(endRoom == 0){
		if (stepsCount ==14){
			printf("YOU VISITED TOO MANY ROOMS. GAME OVER!\n");
			break;
		}
		strcpy (fileName, dirName);
		strcat (fileName, nextRoom);
		//printf("In first while %s\n",fileName);
		room2 = fopen(fileName, "r");
		if ( room2 != NULL ) {
			while (fgets(line, sizeof line, room2) != NULL){ // read a line 
				memset(input,0,strlen(input)); // Clear the input
				strncpy(input, &line[0], 19);
				// Check if user reached the END_ROOM
				if (strcmp(input,"ROOM TYPE: END_ROOM") == 0){
					printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
					printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepsCount);
					for(i=0;i<stepsCount;i++){
						printf("%s\n", steps[i]); // Prints steps taken
					}
					endRoom = 1;	
					break;
				}
			}			
			fclose(room2);
		}
		if (endRoom == 1)
			break;
		if (endRoom == 0){	
			//printf("endRoom ==0\n");
			validInput = 0;
			while(validInput == 0){
				
				room2 = fopen(fileName, "r");
				if ( room2 != NULL ) {
					lineCount = 0;
					tempCount = 0;
					while (fgets(line, sizeof line, room2) != NULL){ // read a line from file
						//For the first line of the file
						if (lineCount == 0){
							printf("CURRENT_LOCATION: ");
							strncpy(input, &line[11], 18);
							printf("%s",input);
							printf("POSSIBLE CONNECTIONS:\n");
						}
						// All other lines in the file
						if (lineCount > 0){
							memset(input,0,strlen(input)); // Clear the input
							strncpy(input, line, 10);
						// Check if line lists connection
							if (strcmp(input,"CONNECTION") == 0){
								memset(input,0,strlen(input)); // Clear the input
								strncpy(input, &line[14], 13);
								tempCons[tempCount] = *input;
								printf("%s",tempCons[tempCount]);	
								tempCount++;
								printf("%s",input);	
								memset(input,0,strlen(input)); // Clear the input
							}
						}
						lineCount++;
					}
					printf("\n");
					printf("WHERE TO? >");	
					printf("%d\n",tempCount);
					scanf("%s", nextRoom);
					for (i=0;i<tempCount;i++){
						printf("%s\n",tempCons[i]);
						if(strcmp(nextRoom,tempCons[i]) == 0){
							validInput =1;
							steps[stepsCount] = tempCons[i];
							stepsCount++;
						}
					}
					if(validInput == 0)
						printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n"); 
				}
				fclose(room2);
			}
						
		}
		
	}
}

struct Room rooms[7];

main() {
	
	char dirName[50] = "curtisjo.rooms."; // Name for directory and files
	char fileName[50];
	int n, j, k, i;
	FILE * room1;
	
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
	roomNames[7] = "Falconry";
	roomNames[8] = "Tower";
	roomNames[9] = "Bedroom";
	
	// Array of Connection Room Names
	const char *conRoom[6];
	
	// Initializes random number generator 
	srand(time(NULL));
	n = rand() % 10;
	
	// Create room structs
	i = 0;
	int conFlag; 	// Flag to check if connection alread exists
	int	selfFlag; 	// Flag to check if potential connection is room itself
	while(i < 7){
		conFlag = 1;
		n = rand() % 10;
		for(k=0; k < i; k++) {
			if (strcmp(rooms[k].roomName, roomNames[n]) == 0){
					conFlag = 0;
			}
		}
		if (conFlag == 1) {
			// Add room Name
			strcpy (rooms[i].roomName, roomNames[n]);
			printf("Room %d %s \n", i+1,rooms[i].roomName);
			// Add Room Type
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
		j = rand() % 4 + 3;
		if(j > rooms[i].conCount) {
			rooms[i].numCons = j;
			while (rooms[i].conCount < rooms[i].numCons){
			selfFlag = 1;
			conFlag = 1;
			n = rand() % 7;
			// Check that connection is not the room itself
			if (strcmp(rooms[i].roomName, rooms[n].roomName) == 0){
					selfFlag = 0;
			}
			if (selfFlag == 1) {
				for (k = 0; k < rooms[i].conCount ; k++){
					if (strcmp(rooms[i].connections[k],rooms[n].roomName) == 0)
						conFlag = 0;
				}
				if (conFlag == 1){
					// Add two way connection
					rooms[i].connections[rooms[i].conCount] = rooms[n].roomName;
					rooms[n].connections[rooms[n].conCount] = rooms[i].roomName;
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

	enterRoom(dirName, rooms[0].roomName, rooms);
	return 0;
}
