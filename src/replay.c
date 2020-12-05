#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#include "replay.h"

#define arrlen(array) sizeof(array) / sizeof(array[0])

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


int shop() {
shopLabel:
	printHeader("Shop");

	// Options for shop menu
	printf("%s  1. Health Potion - %s10%s Coins\n",
			CYAN, YELLOW, CYAN);
	printf("  2. Sell Flesh - %s5%s Coins Each\n",
			YELLOW, CYAN);
	printf("  3. Exit%s\n\n", RESET);

	printf("%s>>>%s ", YELLOW, RESET);

	char * userInput = malloc(3);
	scanf("%s", userInput);
	
	// If user chooses 1st input
	if (!strncmp(userInput, "1", 1)) {
		// If user has enough coins
		if (character.coins >= 10) {
			printf("%s%sSuccessfully bought 1 Health Potion.\n", 
					CLEAR, PURPLE);

			sleep(1);

			printf("You have been deducted %s10%s Coins%s\n", 
					YELLOW, PURPLE, RESET);

			character.coins -= 10;
			// Inventory item 0 is health potions
			inventoryCount[0]++;
			
			sleep(1);

		// If user does not have enough coins
		} else {
			printf("%s%sYou do not have enough coins for that.%s\n", 
					CLEAR, PURPLE, RESET);
			
			sleep(1);
		}

	// If user chooses 2nd option
	} else if (!strncmp(userInput, "2", 1)) {
		// If user has enough flesh (more than 0)
		if (inventoryCount[1] > 0) {
			printf("%s%sSuccessfully sold 1 Flesh.\n",
					CLEAR, PURPLE);

			sleep(1);

			printf("You have gained %s5%s Coins%s\n",
					YELLOW, PURPLE, RESET);

			// Inventory item 1 is Flesh
			inventoryCount[1]--;
			character.coins += 5;

			sleep(1);

		// If user does not have any flesh to sell
		} else {
			printf("%s%sYou have no Flesh to sell.%s\n",
					CLEAR, PURPLE, RESET);
			sleep(1);
		}

	// If user chooses 3rd option
	} else if (!strncmp(userInput, "3", 1)) {
		// quit immediatly
		return 0;
	} else {
		printf("%s%sError: Invalid Input. Retrying...%s\n",
				CLEAR, RED, RESET);
		
		sleep(1);
		shop();
	}

	free(userInput);

	goto shopLabel;

	return 0;
}


int dungeon() {
	size_t damageDeviation[] = {-2, -2, -1, -1, 0, 1, 1, 2, 2};
	char * userInput = malloc(3);

	srand(time(NULL));
	
	badGuy enemy;

	// all enemy sizes: Tiny, Small, Normal Sized, Large, Enormously Big
	enemy.size = sizes[rand() % arrlen(sizes)];

	if (!strcmp(enemy.size, "Tiny")) {
		enemy.minHealth = 5;
		enemy.maxHealth = 15;

		enemy.minDamage = 5;
		enemy.maxDamage = 10;

	} else if (!strcmp(enemy.size, "Small")) {
		enemy.minHealth = 10;
		enemy.maxHealth = 25;
		
		enemy.minDamage = 10;
		enemy.maxDamage = 20;

	} else if (!strcmp(enemy.size, "Normal Sized")) {
		enemy.minHealth = 20;
		enemy.maxHealth = 35;

		enemy.minDamage = 15;
		enemy.maxDamage = 25;
		
	} else if (!strcmp(enemy.size, "Large")) {
		enemy.minHealth = 30;
		enemy.maxHealth = 45;
		
		enemy.minDamage = 20;
		enemy.maxDamage = 35;

	// Enormously big
	} else {
		enemy.minHealth = 30;
		enemy.maxHealth = 50;

		enemy.minDamage = 35;
		enemy.maxDamage = 45;

	}

	// random in range: (rand() % (max - min + 1)) + min
	enemy.health = (rand() % (
				enemy.maxHealth - enemy.minHealth + 1)) + enemy.minHealth;
	enemy.totalHealth = enemy.health;

	enemy.damage = (rand() % (
				enemy.maxDamage - enemy.minDamage + 1)) + enemy.minDamage;

	enemy.name = enemies[rand() % (sizeof(enemies) / sizeof(enemies[0]))];

    printHeader("Dungeon");

	// If enemy name begins with a vowel use 'an' instead of 'a'
	if (!strcmp(enemy.size, "Enormously Big")) {
		printf("%sYou have enountered an %sEnormously Big %s%s with %s%ld%s Health and %s%ld%s Attack Damage%s",
				PURPLE, YELLOW,
				enemy.name, PURPLE,
				YELLOW, enemy.health, PURPLE,
				YELLOW, enemy.damage, PURPLE,
				RESET);
	} else {
		printf("%sYou have enountered a %s%s %s%s with %s%ld%s Health and %s%ld%s Attack Damage%s",
				PURPLE,
				YELLOW, enemy.size, enemy.name, PURPLE,
				YELLOW, enemy.health, PURPLE,
				YELLOW, enemy.damage, PURPLE,
				RESET);
	}

	fflush(stdout);

	sleep(3);

	while ((long long)character.health > 0 && (long long)enemy.health > 0) {
input:
		printHeader("Dungeon");

		// Clear line and write over it with enemy stats
		printf("%s%s%s %s  %ld/%ld%s HP  %s%ld%s Attack Damage%s\n\n",
				CLRLINE, YELLOW, enemy.size, enemy.name,
				enemy.health, enemy.totalHealth, PURPLE,
				YELLOW, enemy.damage, PURPLE,
				RESET);

		// Scan for user input
		printf("%s  1. Attack\n", CYAN);
		printf("  2. Use Item\n");
		printf("  3. Run Away%s\n\n", RESET);
		printf("%s>>>%s ", YELLOW, RESET);

		scanf("%s", userInput);

		// User turn and User enter attack
		if (!strncmp(userInput, "1", 1)) {
			// User damage deviation value
			size_t attackChance = rand() % 
				(sizeof(damageDeviation) / sizeof(damageDeviation[0]));

			// Damage dealt by user + deviation value
			size_t totalDamage = character.damage + attackChance;

			// If deviation value is 0, turn is missed
			if (attackChance == 0) {
				printf("%s%sYou %smissed%s the enemy!%s\n", 
						CLEAR, PURPLE, 
						YELLOW, PURPLE, 
						RESET);

			// Deviation value of 2, user deals critical damage
			} else if (attackChance == 2) {
				enemy.health -= totalDamage;
				printf("%s%sCritical!%s You attacked for %s%ld%s Damage.%s\n", 
						CLEAR, YELLOW, 
						PURPLE, 
						YELLOW, totalDamage, PURPLE,
						RESET);

			// Normal damage is dealt (deviation is not 2 nor 0)
			} else {
				enemy.health -= totalDamage;
				printf("%s%sYou attacked for %s%ld%s Damage.%s\n", 
						CLEAR, PURPLE,
						YELLOW, totalDamage, PURPLE,
						RESET);
			}
		// If use item menu is chosen
		}  else if (!strncmp(userInput, "2", 1)) {
useItemInput:
			printHeader("Dungeon - Use Item");

			// options
			printf("%s  1. Health Potion: %s%ld%s\n",
					CYAN,
					YELLOW, inventoryCount[0], RESET);
			printf("%s  2. Exit%s\n\n", CYAN, RESET);
			
			printf("%s>>>%s ", YELLOW, RESET);
			scanf("%s", userInput);

			// if option 1 is chosen
			if (!strncmp(userInput, "1", 1)) {
				// if item 1 is available in the inventory (greater than 0)
				if (inventoryCount[0] >= 1) {
					if (character.health < character.totalHealth - 15) {
						printf("%s%sYou used a Health Potion and gained %s15%s health.%s\n",
								CLEAR, PURPLE,
								YELLOW, PURPLE,
								RESET);
						
						inventoryCount[0]--;
						character.health += 15;
						sleep(1);
					} else {
						printf("%s%sYou cannot use this item. Health is too high%s\n",
						CLEAR, PURPLE, RESET);
						sleep(1);
						goto useItemInput;
					}
				// if item 1 is not available in the inventory (less than 1)
				} else {
					printf("%s%sYou do not have any Health Potions.%s\n",
							CLEAR, PURPLE,
							RESET);
					sleep(1);
					goto input;
				}
			} else if (!strncmp(userInput, "2", 1)) {
				goto input;
			} else {
				printf("%s%sError: Invalid input. Retrying...%s\n", CLEAR, RED, RESET);
				sleep(1);
				goto useItemInput;
			}

		// if runaway option is chosen
		} else if (!strncmp(userInput, "3", 1)) {
			// If run away is succesful
			if (rand() % 3 == 0) {
				printf("%s%sYou successfully ran away.%s\n",
						CLEAR, PURPLE, RESET);
				sleep(1);
				
				return 0;

			// if runaway fails
			} else {
				// Index 0 is good excuses, index 1 is bad
				char excuses[2][2][100] = {
					{
						"You forgot how to run! Did not escape.",
						"Your enemy has slowed you. Did not escape."
					},
					{
						"You have been poisoned by your enemy. Did not escape",
						"You tripped on a sharp rock. Did not escape."
					}
				};
				
				// good excuse; index 0
				if (rand() % 2 == 0) {
					char * excuse = excuses[0][rand() % 2];
					printf("%s%s%s%s\n",
							CLEAR, PURPLE, excuse, RESET);
					sleep(1);
				// bad excuse; index 1
				} else {
					char * excuse = excuses[1][rand() % 2];
					
					int minExcuseDamage = 2;
					int maxExcuseDamage = 6;

					int excuseDamage = (rand() % (maxExcuseDamage - minExcuseDamage + 1)) + minExcuseDamage;

					printf("%s%s%s%s\n",
							CLEAR, PURPLE, excuse, RESET);
					sleep(1);
					printf("%sYou lost %s%d%s Health%s\n",
							PURPLE,
							YELLOW, excuseDamage, PURPLE,
							RESET);

					character.health -= excuseDamage;
					sleep(1);

				}
			}
		} else {
			printf("%s%sError: Invalid input. Retrying...%s\n", CLEAR, RED, RESET);
			sleep(1);
			goto input;
		}
		fflush(stdout);
		sleep(1);


		// If enemy is defeated
		if ((long long)enemy.health < 1) {
			int minCoins = 5;
			int maxCoins = 15;
			// Amount of coins earned after battle
			int awardedCoins = (rand() % (maxCoins - minCoins)) + minCoins;
			character.coins += awardedCoins;
			
			int gainFlesh = rand() % 5;

			printf("%s%sYou defeated the enemy.%s\n", CLEAR, PURPLE, RESET);
			sleep(1);
			
			printf("%sYou have been awarded %s%d%s Coins.%s\n", 
					PURPLE,
					YELLOW, awardedCoins, PURPLE,
					RESET);
			if (gainFlesh == 0 || gainFlesh == 1 || gainFlesh == 2) {
				sleep(1);

				printf("%sYou have also gained %s1%s Flesh%s\n",
						PURPLE, YELLOW, PURPLE, RESET);
				
				inventoryCount[1]++;
			}
			
			fflush(stdout);
			sleep(2);
			break;
		}

		// Enemy turn
		size_t attackChance = rand() % 
				(sizeof(damageDeviation) / sizeof(damageDeviation[0]));
		size_t totalDamage = enemy.damage + attackChance;

		// If deviation is 0, user dodged enemy
		if (attackChance == 0) {
			printf("%sYou %sdodged%s the enemy!%s\n", PURPLE, 
					YELLOW, PURPLE,
					RESET);

		// if deviation is 2, enemy deals critical damage to the user
		} else if (attackChance == 2) {
			character.health -= totalDamage;
			printf("%sCritical! The enemy dealt %s%ld%s Damage to You.%s\n", 
					RED, 
					YELLOW, totalDamage, RED,
					RESET);
		
		// if deviation is not 2 nor 0
		} else {
			character.health -= totalDamage;
			printf("%sThe enemy dealt %s%ld%s Damage to you.%s\n", 
					RED, YELLOW, totalDamage, RED,
				   	RESET);
		}
		fflush(stdout);
		sleep(2);

	}

	return 0;
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

			dungeon();
		} else if (!strncmp(userInput, "2", 1)) {
			free(userInput);

			shop();
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

