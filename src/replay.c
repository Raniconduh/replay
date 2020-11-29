#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// #include "dungeon.h"

#define arrlen(array) sizeof(array) / sizeof(array[0])

// TODO : Add inventory
// TODO : Add shop screen maybe


int makeCharacter();
int mainMenu();
int dungeon();
int shop();
int invMenu();


const char CLRLINE[] = "\033[2K\r";
const char RESET[] = "\033[0;0m";
const char BLACK[] = "\033[0;30m";
const char RED[] = "\033[0;31m";
const char GREEN[] = "\033[0;32m";
const char YELLOW[] = "\033[0;33m";
const char BLUE[] = "\033[0;34m";
const char PURPLE[] = "\033[0;35m";
const char CYAN[] = "\033[0;36m";
const char WHITE[] = "\033[0;37m";
const char CLEAR[] = "\033[H\033[2J";

const char HBLACK[] = "\033[0;40m";
const char HRED[] = "\033[0;41m";
const char HGREEN[] = "\033[0;42m";
const char HYELLOW[] = "\033[0;43m";
const char HBLUE[] = "\033[0;44m";
const char HPURPLE[] = "\033[0;45m";
const char HCYAN[] = "\033[0;46m";
const char HWHITE[] = "\033[0;47m";

// All enemies available to fight

// All sizes that enemies can be (WIP)
// char* sizes[] = {"Tiny", "Small", "Normal Sized", "Large", "Enormously Big"};

char * enemies[] = {
	"Witch", "Goblin", "Ogre", "Vampire", "Ghoul", "Wolf",
	"Mummy", "Zombie", "Ghost", "Troll", "Giant", "Scorpion", 
	"Warlock", "Ant Colony", "Necromancer", "Centipede"
};

// character information (name, class, health, attack, coins)
typedef struct {
	// user chosen username
	char * name;
	// user chosen class (Warrior, Mage, or Archer)
	char * class;
	
	// Available health if healthbar was full (does not change)
	size_t totalHealth;

	// Current health (does change)
	size_t health;
	size_t damage;
	size_t coins;
} playerCharacter;
playerCharacter character;


// Names of possible items
char * inventoryOptions[] = {"Health Potion", "Rotting Flesh"};
// Amount of items in inventory 
// (index corresponds to inventoryOptions)
size_t inventoryCount[] = {0, 0};


int main() {
	makeCharacter();

	character.coins = 25;

	mainMenu();
	return 0;
}


int invMenu() {
	printf("%s%sInventory\t%s%s%s the %s%s%s\n",
			CLEAR, GREEN,
			YELLOW, character.name, PURPLE,
			YELLOW, character.class, RESET);
	printf("%s%ld%s Coins, %s%ld/%ld%s HP%s\n\n",
			YELLOW, character.coins, PURPLE,
			YELLOW, character.health, character.totalHealth, PURPLE,
			RESET);

	for (int i = 0; i < arrlen(inventoryOptions); i++) {
		printf("%s: ", inventoryOptions[i]);
		printf("%s%ld%s\n", YELLOW, inventoryCount[i], RESET);
	}

	printf("\n%sPress Enter to Continue%s\n", YELLOW, RESET);

	while (getchar() != '\n');
	getchar();

	return 0;
};


int shop() {
	printf("%sShop\n", CLEAR);
	exit(17);
	return 0;
}


int dungeon() {
	size_t damageDeviation[] = {-2, -2, -1, -1, 0, 1, 1, 2, 2};
	char * userInput = malloc(3);

	srand(time(NULL));
	typedef struct {
		size_t health;

		// Minimum possible health (changes in future versions)
		size_t minHealth;
		
		// Maximum possible health (changes in future versions)
		size_t maxHealth;
		
		// The amount of health if none is lost
		size_t totalHealth;


		size_t damage;

		// Minimum possible damage
		size_t minDamage;
		
		// Maximum possible damage
		size_t maxDamage;


		char * name;
	} badGuy;
	badGuy enemy;

	// should change based on enemy size
	enemy.minHealth = 10;
	enemy.maxHealth = 50;

	enemy.minDamage = 5;
	enemy.maxDamage = 15;

	// random in range: (rand() % (max - min + 1)) + min
	enemy.health = (rand() % (
				enemy.maxHealth - enemy.minHealth + 1)) + enemy.minHealth;
	enemy.totalHealth = enemy.health;

	enemy.damage = (rand() % (
				enemy.maxDamage - enemy.minDamage + 1)) + enemy.minDamage;

	enemy.name = enemies[rand() % (sizeof(enemies) / sizeof(enemies[0]))];

    printf("%s%sDungeon\t%s%s%s the %s%s%s\n",
			CLEAR, GREEN,
			YELLOW, character.name, PURPLE,
			YELLOW, character.class, RESET);
	printf("%s%ld%s Coins, %s%ld/%ld%s HP%s\n\n",
			YELLOW, character.coins, PURPLE,
			YELLOW, character.health, character.totalHealth, PURPLE,
			RESET);

	// If enemy name begins with a vowel use 'an' instead of 'a'
	if (enemy.name[0] == 'A' ||
			enemy.name[0] == 'E' || 
			enemy.name[0] == 'I' || 
			enemy.name[0] == 'O' || 
			enemy.name[0] == 'U') {

		printf("%sYou have enountered an %s%s%s with %s%ld%s Health and %s%ld%s Attack Damage%s",
				PURPLE,
				YELLOW, enemy.name, PURPLE,
				YELLOW, enemy.health, PURPLE,
				YELLOW, enemy.damage, PURPLE,
				RESET);
	} else {
		printf("%sYou have enountered a %s%s%s with %s%ld%s Health and %s%ld%s Attack Damage%s",
				PURPLE,
				YELLOW, enemy.name, PURPLE,
				YELLOW, enemy.health, PURPLE,
				YELLOW, enemy.damage, PURPLE,
				RESET);
	}

	fflush(stdout);

	sleep(3);

	while ((long long)character.health > 0 && (long long)enemy.health > 0) {
input:
		printf("%s%sDungeon\t%s%s%s the %s%s%s\n",
				CLEAR, GREEN,
				YELLOW, character.name, PURPLE,
				YELLOW, character.class, RESET);
		printf("%s%ld%s Coins, %s%ld/%ld%s HP%s\n\n",
				YELLOW, character.coins, PURPLE,
				YELLOW, character.health, character.totalHealth, PURPLE,
				RESET);

		// Clear line and write over it with enemy stats
		printf("%s%s%s  %ld/%ld%s HP  %s%ld%s Attack Damage%s\n\n", 
				CLRLINE, YELLOW, enemy.name, 
				enemy.health, enemy.totalHealth, PURPLE,
				YELLOW, enemy.damage, PURPLE, 
				RESET);

		// Scan for user input
		printf("%s  1. Attack%s\n\n", CYAN, RESET);
		printf("%s>>>%s ", YELLOW, RESET);

		scanf("%s", userInput);
		fflush(stdin);

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

			printf("%s%sYou defeated the enemy.%s\n", CLEAR, PURPLE, RESET);
			sleep(1);
			
			printf("%sYou have been awarded %s%d%s Coins.%s\n", 
					PURPLE,
					YELLOW, awardedCoins, PURPLE,
					RESET);
			
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
		printf("%s%sMenu\t%s%s%s the %s%s%s\n",
				CLEAR, GREEN, 
				YELLOW, character.name, PURPLE, 
				YELLOW, character.class, RESET);
		printf("%s%ld%s Coins, %s%ld/%ld%s HP%s\n\n",
				YELLOW, character.coins, PURPLE,
				YELLOW, character.health, character.totalHealth, PURPLE, 
				RESET);

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

			invMenu();
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
	    fflush(stdin);
	    sleep(1);
	    makeCharacter();
	}

	fflush(stdin);

	// User chooses a class (Warrior, Mage, or Archer)
	printf("%s", CLEAR);
	printf("%sWelcome to Replay%s\n", GREEN, RESET);
	printf("%sPick Your class:%s\n\n", PURPLE, RESET);
	
	printf("%s  1. Warrior\n", CYAN);
	printf("  2. Mage\n");
	printf("  3. Archer%s\n\n", RESET);
	
	printf("%s>>>%s ", YELLOW, RESET);
	
	scanf("%s", userInput);
	fflush(stdin);

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
		character.damage = 20;

	} else if (!strcmp(userInput, "3")) {
		character.class = "Archer";
		character.health = 75;
		character.totalHealth = 75;
		character.damage = 15;

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
	
	fflush(stdin);

	// If user enters the wrong thing and wouuld like to retry
	if (!strncmp(userInput, "n", 1) || !strncmp(userInput, "N", 1)) {
		printf("%s%sOk. Retrying...%s\n", CLEAR, RED, RESET);

		sleep(1);
		makeCharacter();
	}
	
	free(userInput);

	return 0;
}

