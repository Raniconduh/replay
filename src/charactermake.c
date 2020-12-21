#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "replay.h"

int makeSave(char * saveName, playerCharacter * character, int overwrite);


// inital make character screen (select character stats n stuff)
int makeCharacter(playerCharacter * character) {
makeCharacterLabel:
	// INITIALIZE POINTERS
	character->name = calloc(sizeof(char) * 20, 1);
	character->class = calloc(sizeof(char) * 10, 1);

	char * userInput = malloc(10);

	// user enters their username
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sEnter your name:%s ", PURPLE, RESET);
	scanf("%s", character->name);
	
	// if user enters name thats too long
	if (strlen(character->name) > 19) {
	    printf("%s%sError: name too long. Retrying...%s\n", 
		    CLEAR, RED, RESET);

	    sleep(1);
	    goto makeCharacterLabel;
	}

	// User chooses a class (Warrior, Mage, or Archer)
	printf("%s", CLEAR);
	printf("%sWelcome to Replay%s\n", GREEN, RESET);
	printf("%sPick Your class:%s\n\n", PURPLE, RESET);
	
	printf("%s  1. Warrior\n", CYAN);
	printf("  2. Mage\n");
	printf("  3. Archer%s\n\n", RESET);
	
	printf("%s>>>%s ", YELLOW, RESET);
	
	scanf("%s", userInput);

	// Compare user input with all available options
	if (!strcmp(userInput, "1")) {
		character->class = "Warrior";
		character->health = 100;
		character->totalHealth = 100;
		character->damage = 15;

	} else if (!strcmp(userInput, "2")) {
		character->class = "Mage";
		character->health = 50;
		character->totalHealth = 50;
		character->damage = 25;

	} else if (!strcmp(userInput, "3")) {
		character->class = "Archer";
		character->health = 75;
		character->totalHealth = 75;
		character->damage = 20;

	} else {
		printf("%s%sError: Invalid input. Retrying...%s\n", CLEAR, RED, RESET);
		sleep(1);
		goto makeCharacterLabel;
	}
	
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
		goto makeCharacterLabel;
	}
	
saveInputLabel:
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sEnter Save Name (no spaces):%s\n\n", PURPLE, RESET);

	char * saveInput = malloc(sizeof(char) * 20);
	printf("%s>>>%s ", YELLOW, RESET);
	scanf("%s", saveInput);

	if (strlen(saveInput) > 14) {
		printf("%s%sError: Save name too long. Retrying...%s\n",
				CLEAR, RED, RESET);
		sleep(1);
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

	char * home = getenv("HOME");
	char * savePath = calloc((strlen(home) + strlen(saveName) + strlen(".save") + 15) * sizeof(char), 1);
	strcpy(savePath, home);
	strcat(savePath, "/.config/replay/");
	strcat(savePath, saveName);
	strcat(savePath, ".save");

	character->savePath = calloc(sizeof(char) * (strlen(savePath) + 35), 1);
	strcpy(character->savePath, savePath);

	// if save file exists and overwrite is 1 (dont overwrite)
	if (access(savePath, F_OK) == 0 && overwrite) {
		return 1;
	}

	char * toSave = calloc((200 + strlen(character->name)
				+ strlen(character->class)
				+ (sizeof(long long) * 2)) * sizeof(char), 1);

	sprintf(toSave, "<?xml version=\"1.0\"?><character><name>%s</name>\
			<class>%s</class><totalHealth>%ld</totalHealth>\
			<health>%ld</health><damage>%ld</damage><coins>%ld</coins></character>",
			character->name, character->class, character->totalHealth,
			character->health, character->damage, character->coins);
	
	FILE * saveFile = fopen(savePath, "w+");
	fputs(toSave, saveFile);
	fclose(saveFile);
	fflush(saveFile);
	
	free(toSave);
	free(savePath);
	return 0;
}


