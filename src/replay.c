#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "replay.h"


// TODO : add parsing of save data files
// TODO : add creation of save data files

const char * CLRLINE = "\033[2K\r";
const char * RESET   = "\033[0;0m";
const char * BLACK   = "\033[0;30m";
const char * RED     = "\033[0;31m";
const char * GREEN   = "\033[0;32m";
const char * YELLOW  = "\033[0;33m";
const char * BLUE    = "\033[0;34m";
const char * PURPLE  = "\033[0;35m";
const char * CYAN    = "\033[0;36m";
const char * CLEAR   = "\033[H\033[2J";


char * sizes[] = {
	"Tiny", "Small", "Normal Sized", "Large", "Enormously Big"
};

char * enemies[] = {
	"Witch",   "Goblin",    "Ogre",         "Vampire", "Ghoul", "Wolf",
	"Mummy",   "Zombie",    "Ghost",        "Troll",   "Giant", "Scorpion", 
	"Warlock", "Ant Colony", "Necromancer", "Centipede"
};


// character information (name, class, health, attack, coins)
playerCharacter character;

// Amount of items in inventory 
char * inventoryOptions[] = {"Health Potion", "Flesh"};
// (index corresponds to inventoryOptions)
size_t inventoryCount[] = {2, 0};

char * healerNames[] = {"Marcita", "Lovely", "Essence",
	"Precious", "Rihana", "Heather", 
	"Lily", "Chrysanthemum", "Crystal"};


int main() {
	if (startMenu(&character)) {
		makeCharacter(&character);
	}

	mainMenu();
	return 0;
}


void printHeader(char * menuName) {
	printf("%s%s%s\t%s%s%s the %s%s%s\n",
			CLEAR, GREEN, menuName,
			YELLOW, character.name, PURPLE,
			YELLOW, character.class, RESET);
	printf("%s%ld%s Coins  %s%ld/%ld%s HP  %s%ld%s Attack Damage%s\n\n",
			YELLOW, character.coins, PURPLE,
			YELLOW, character.health, character.totalHealth, PURPLE,
			YELLOW, character.damage, PURPLE,
			RESET);
}


int mainMenu() {
	while (1) {
		// Header
		printHeader("Menu");

		// Options
		printf("  %s1. Enter Dungeon\n", CYAN);
		printf("  2. Enter Shop\n");
		printf("  3. Enter Inventory\n\n");

		printf("  4. Save Game\n");
		printf("  5. Quit Game%s\n\n", RESET);

		// User input line
		printf("%s>>>%s ", YELLOW, RESET);
		char * userInput = malloc(sizeof(char) * 3);
		fgets(userInput, 2, stdin);

		if (!strncmp(userInput, "1", 1)) {
			free(userInput);
			dungeon(&character);

		} else if (!strncmp(userInput, "2", 1)) {
			free(userInput);
			shop(&character);

		} else if (!strncmp(userInput, "3", 1)) {
			free(userInput);
			invMenu(&character);

		} else if (!strncmp(userInput, "4", 1)) {
			free(userInput);
			saveGame(&character);
			printf("%s%sSaving...%s\n", CLEAR, PURPLE, RESET);
			sleep(1);

		} else if (!strncmp(userInput, "5", 1)) {
			free(userInput);
			exit(0);

		} else {
			free(userInput);
		}
	}

	return 0;
}


int saveGame(playerCharacter * character) {
	char * toSave = calloc((200 + strlen(character->name)
				+ strlen(character->class)
				+ sizeof(long long) * 2
				+ arrlen(inventoryCount) * 5)
				* sizeof(char), sizeof(char));
	sprintf(toSave, "<?xml version=\"1.0\"?><character><name>%s</name><class>%s</class><totalHealth>%ld</totalHealth><health>%ld</health><damage>%ld</damage><coins>%ld</coins>",
			character->name, character->class, character->totalHealth,
			character->health, character->damage, character->coins);

	// buffer for individual inventory indexes
	char * buf = malloc(sizeof(char) * 10);

	// save inventory specs block
	for (size_t i = 0; i < arrlen(inventoryCount); i++) {
		// <iindex>count</iindex>
		// i is necessary as xml tags cannot start with numbers
		sprintf(buf, "<i%ld>%ld</i%ld>", i, inventoryCount[i], i);
		strcat(toSave, buf);
	}

	free(buf);

	strcat(toSave, "</character>");

	FILE * saveFile = fopen(character->savePath, "w+");
	fputs(toSave, saveFile);
	fflush(saveFile);
	fclose(saveFile);

	free(toSave);

	return 0;
}

