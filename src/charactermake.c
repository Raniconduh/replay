#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "replay.h"

int makeCharacter(playerCharacter * character) {
makeCharacterLabel:
	// INITIALIZE POINTERS
	character->name = malloc(20);
	character->class = malloc(10);

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


	// If user enters the wrong thing and wouuld like to retry
	if (!strncmp(userInput, "n", 1) || !strncmp(userInput, "N", 1)) {
		printf("%s%sOk. Retrying...%s\n", CLEAR, RED, RESET);

		sleep(1);
		goto makeCharacterLabel;
	}
	
	free(userInput);

	return 0;
}

