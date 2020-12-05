#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#include "replay.h"


// TODO : add parsing of save data files
// TODO : add creation of save data files

const char * CLRLINE = "\033[2K\r";
const char * RESET = "\033[0;0m";
const char * BLACK = "\033[0;30m";
const char * RED = "\033[0;31m";
const char * GREEN = "\033[0;32m";
const char * YELLOW = "\033[0;33m";
const char * BLUE = "\033[0;34m";
const char * PURPLE = "\033[0;35m";
const char * CYAN = "\033[0;36m";
const char * CLEAR = "\033[H\033[2J";



char* sizes[] = {"Tiny", "Small", "Normal Sized", "Large", "Enormously Big"};
char * enemies[] = {
	"Witch", "Goblin", "Ogre", "Vampire", "Ghoul", "Wolf",
	"Mummy", "Zombie", "Ghost", "Troll", "Giant", "Scorpion", 
	"Warlock", "Ant Colony", "Necromancer", "Centipede"
};


// character information (name, class, health, attack, coins)
playerCharacter character;

// Amount of items in inventory 
char * inventoryOptions[] = {"Health Potion", "Flesh"};
// (index corresponds to inventoryOptions)
size_t inventoryCount[] = {2, 0};


int main() {
	startMenu();

	makeCharacter();

	character.coins = 15;

	mainMenu();
	return 0;
}


int startMenu() {
startMenuLabel:
	printf("%s%sWelcome to Replay!%s\n\n", CLEAR, GREEN, RESET);

	printf("%s  1. Load Save\n", CYAN);
	printf("  2. Create New Save%s\n\n", RESET);

	printf("%s>>>%s ", YELLOW, RESET);
	char * userInput = malloc(3);
	scanf("%s", userInput);

	// Load an existing save
	if (!strncmp(userInput, "1", 1)) {
		char * home = getenv("HOME");
		strcat(home, "/.config/replay");

		DIR * dir = opendir(home);
		struct dirent * dirContents;
		size_t saves = 0;
		char ** saveFiles = malloc(sizeof(char) * saves + 1);

		// If directory exists
		if (dir) {
			while ((dirContents = readdir(dir)) != NULL) {
				// storing contents of directory

				// if file name is not . nor ..
				if (strcmp(dirContents->d_name, ".") && strcmp(dirContents->d_name, "..")) {
					saveFiles[saves] = dirContents->d_name;
					saves++;
					void * ptr = realloc(saveFiles, sizeof(char) * saves + 1);
				}
					
			}

			if (saves == 0) {
				printf("%s%sNo saves found.%s\n", CLEAR, PURPLE, RESET);
				sleep(1);

				closedir(dir);

				goto startMenuLabel;
			}

			printf("%s%sSelect a Save%s\n\n", CLEAR, GREEN, RESET);
		
			size_t i;
			for (i = 1; i <= saves; i++) {
				printf("%s  %ld. %s%s\n", CYAN, i, saveFiles[i-1], RESET);
			}

			printf("%s  %ld. Exit%s\n\n", CYAN, i, RESET);

			printf("%s>>>%s ", YELLOW, RESET);
			scanf("%s", userInput);
			
			char * stringi = malloc(sizeof(i));
			snprintf(stringi, sizeof(i), "%ld", i);

			if (!strncmp(userInput, stringi, 1)) {
				home = NULL;
				dir = NULL;
				dirContents = NULL;
				startMenu();
			}
			
			free(stringi);
			free(saveFiles);
			closedir(dir);

		// Directory does not exist
		} else if (ENOENT == errno) {
			printf("%s%sNo directory found.%s\n", CLEAR, RED, RESET);
			sleep(1);

		// opendir failed
		} else {
			printf("%s%sProblem searching for saves.%s\n", CLEAR, RED, RESET);
			sleep(1);
			
			goto startMenuLabel;
		}

	// Make a new save
	} else if (!strncmp(userInput, "2", 1)) {
		
	}

	free(userInput);

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
		printf("  3. Enter Inventory%s\n\n", RESET);

		// User input line
		printf("%s>>>%s ", YELLOW, RESET);
		char * userInput = malloc(3);
		scanf("%s", userInput);

		if (!strncmp(userInput, "1", 1)) {
			free(userInput);

			dungeon(&character);
		} else if (!strncmp(userInput, "2", 1)) {
			free(userInput);

			shop(&character);
		} else if (!strncmp(userInput, "3", 1)) {
			free(userInput);

			invMenu(&character);
		} else {
			free(userInput);

			printf("%s%sError: Invalid Input. Retrying...%s\n", CLEAR, RED, RESET);
			sleep(1);
		}
	}

	return 0;
}


int makeCharacter() {
	// INITIALIZE POINTERS
	character.name = malloc(20);
	character.class = malloc(10);

	char * userInput = malloc(10);

	// user enters their username
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sEnter your name:%s ", PURPLE, RESET);
	scanf("%s", character.name);
	
	// if user enters name thats too long
	if (strlen(character.name) > 19) {
	    printf("%s%sError: name too long. Retrying...%s\n", 
		    CLEAR, RED, RESET);

	    sleep(1);
	    makeCharacter();
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
		character.class = "Warrior";
		character.health = 100;
		character.totalHealth = 100;
		character.damage = 15;

	} else if (!strcmp(userInput, "2")) {
		character.class = "Mage";
		character.health = 50;
		character.totalHealth = 50;
		character.damage = 25;

	} else if (!strcmp(userInput, "3")) {
		character.class = "Archer";
		character.health = 75;
		character.totalHealth = 75;
		character.damage = 20;

	} else {
		printf("%s%sError: Invalid input. Retrying...%s\n", CLEAR, RED, RESET);
		sleep(1);
		makeCharacter();
	}
	
	// Asks for confirmation on user choices of class and username
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sYou are %s%s%s the %s%s%s\n", 
			PURPLE, 
			YELLOW, character.name, PURPLE, 
			YELLOW, character.class, RESET);
	printf("%sIs that correct?%s\n", PURPLE, RESET);
	printf("%s[Y/n]:%s ", YELLOW, RESET);
	scanf("%s", userInput);


	// If user enters the wrong thing and wouuld like to retry
	if (!strncmp(userInput, "n", 1) || !strncmp(userInput, "N", 1)) {
		printf("%s%sOk. Retrying...%s\n", CLEAR, RED, RESET);

		sleep(1);
		makeCharacter();
	}
	
	free(userInput);

	return 0;
}

