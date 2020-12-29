#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "replay.h"


/* makes save file if non-existant and dumps save data to it
 * overwrite option specifies wether or not to overwrite an existing save
 * if overwrite is true i.e. 1, then don't overwrite
 * if overwrite is false i.e. 0, then overwrite */
int makeSave(char * saveName, playerCharacter * character, int overwrite);


// inital make character screen (select character stats n stuff)
int makeCharacter(playerCharacter * character) {
makeCharacterLabel:
	// INITIALIZE POINTERS
	character->name = calloc(sizeof(char) * 20, 1);
	character->class = calloc(sizeof(char) * 10, 1);

	// user enters their username
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sEnter your name:%s ", PURPLE, RESET);
	scanf("%s", character->name);
	
	// if user enters name thats too long (more than 19 character)
	if (strlen(character->name) > 19) {
	    printf("%s%sError: name too long. Retrying...%s\n", 
		    CLEAR, RED, RESET);

	    sleep(1);

		free(character->name);
		free(character->class);

	    goto makeCharacterLabel;
	}

	// User chooses a class (Warrior, Mage, or Archer)
	printf("%s", CLEAR);
	printf("%sWelcome to Replay%s\n", GREEN, RESET);
	printf("%sPick Your class:%s\n\n", PURPLE, RESET);
	
	// character class input choices / options
	printf("%s  1. Warrior\n", CYAN);
	printf("  2. Mage\n");
	printf("  3. Archer%s\n\n", RESET);
	
	char * userInput = malloc(sizeof(char) * 5);
	printf("%s>>>%s ", YELLOW, RESET);
	scanf("%s", userInput);

	// Compare user input with all available options
	// warrior option is chosen
	if (!strcmp(userInput, "1")) {
		character->class = "Warrior";
		character->health = 100;
		character->totalHealth = 100;
		character->damage = 15;

	// mage option is chosen
	} else if (!strcmp(userInput, "2")) {
		character->class = "Mage";
		character->health = 50;
		character->totalHealth = 50;
		character->damage = 25;

	// archer option is chosen
	} else if (!strcmp(userInput, "3")) {
		character->class = "Archer";
		character->health = 75;
		character->totalHealth = 75;
		character->damage = 20;

	// invalid input is chosen
	} else {
		printf("%s%sError: Invalid input. Retrying...%s\n", CLEAR, RED, RESET);

		sleep(1);
		free(userInput);
		free(character->name);
		free(character->class);
		goto makeCharacterLabel;
	}
	
	character->coins = 15;

	// Asks for confirmation on user choices of class and username
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sYou are %s%s%s the %s%s%s\n", 
			PURPLE, 
			YELLOW, character->name, PURPLE, 
			YELLOW, character->class, RESET);
	printf("%sIs that correct?%s\n", PURPLE, RESET);

	printf("%s[Y/n]:%s ", YELLOW, RESET);
	scanf("%s", userInput);


	// If user enters no and wouuld like to retry
	if (!strncmp(userInput, "n", 1) || !strncmp(userInput, "N", 1)) {
		printf("%s%sOk. Retrying...%s\n", CLEAR, RED, RESET);

		sleep(1);
	
		free(userInput);
		free(character->name);
		free(character->class);

		goto makeCharacterLabel;
	}
	
saveInputLabel:
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sEnter Save Name (no spaces):%s\n\n", PURPLE, RESET);

	char * saveInput = malloc(sizeof(char) * 15);
	printf("%s>>>%s ", YELLOW, RESET);
	scanf("%s", saveInput);

	if (strlen(saveInput) > 14) {
		printf("%s%sError: Save name too long. Retrying...%s\n",
				CLEAR, RED, RESET);
		
		sleep(1);
		free(saveInput);
		goto saveInputLabel;
	}

	// if user selected save file already exists
	if (makeSave(saveInput, character, 1)) {
		printf("%s%sError: This file already exists.%s\n", CLEAR, RED, RESET);
		printf("%sWould you like to delete and overwrite the save file?%s\n\n",
				PURPLE, RESET);
		
		printf("%s[Y/n] :%s ", YELLOW, RESET);
		scanf("%s", userInput);

		if (!strncmp(userInput, "y", 1) || !strncmp(userInput, "Y", 1)) {
			makeSave(saveInput, character, 0);	
		} else {
			free(saveInput);

			goto saveInputLabel;
		}
	}

	free(saveInput);
	free(userInput);

	return 0;
}


// create a save file and write to it
int makeSave(char * saveName, playerCharacter * character, int overwrite) {
	// make sure save directory exists
	system("mkdir -p ~/.config/replay 2> /dev/null");
	
	// copy exact path of save file into savePath variable
	char * home = getenv("HOME");
	char * savePath = calloc((strlen(home) + strlen(saveName) + strlen(".save") + 35) * sizeof(char), 1);
	strcpy(savePath, home);
	strcat(savePath, "/.config/replay/");
	strcat(savePath, saveName);
	strcat(savePath, ".save");

	// store the save path globally in case it is needed elsewhere
	character->savePath = calloc(sizeof(char) * (strlen(savePath) + 35), 1);
	strcpy(character->savePath, savePath);

	// if save file exists and overwrite is 1 (dont overwrite)
	if (access(savePath, F_OK) == 0 && overwrite) {
		free(savePath);
		free(character->savePath);

		return 1;
	}

	// buffer for what to dump into save file
	char * toSave = calloc((200 + strlen(character->name)
				+ strlen(character->class)
				+ (sizeof(long long) * 2)) * sizeof(char), 1);

	// copy character data into save buffer
	sprintf(toSave, "<?xml version=\"1.0\"?><character><name>%s</name>\
			<class>%s</class><totalHealth>%ld</totalHealth>\
			<health>%ld</health><damage>%ld</damage><coins>%ld</coins></character>",
			character->name, character->class, character->totalHealth,
			character->health, character->damage, character->coins);
	
	// open and write save buffer to actual save file
	FILE * saveFile = fopen(savePath, "w+");
	fputs(toSave, saveFile);
	fflush(saveFile);
	fclose(saveFile);
	
	free(toSave);
	free(savePath);
	return 0;
}


