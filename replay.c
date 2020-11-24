#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "./include/dungeon.h"

int makeCharacter();


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
const char* enemies[] = {"Witch", "Goblin", "Ogre", "Vampire", "Ghoul", "Wolf", 
	"Mummy", "Zombie", "Ghost", "Troll", "Giant", "Scorpion", "Warlock", 
	"Ant Colony", "Necromancer", "Centipede"};

// All sizes that enemies can be (WIP)
// char* sizes[] = {"Tiny", "Small", "Normal Sized", "Large", "Enormously Big"};

// character class (warrior, mage, or archer)
typedef struct {
	char * name;
	char * class;
	
	size_t totalHealth;

	size_t health;
	size_t attackDamage;
	size_t coins;
} playerCharacter;
playerCharacter character;


int main() {
	makeCharacter();
	test();
	return 0;
}


int makeCharacter() {
	character.name = malloc(20);
	character.class = malloc(10);

	char * userInput = malloc(10);

	// user enters their name
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sEnter your name:%s ", PURPLE, RESET);
	scanf("%s", character.name);	

	printf("%s", CLEAR);
	printf("%sWelcome to Replay%s\n", GREEN, RESET);
	printf("%sPick Your class:%s\n\n", PURPLE, RESET);
	
	printf("%s  1. Warrior\n", CYAN);
	printf("  2. Mage\n");
	printf("  3. Archer%s\n\n", RESET);
	
	printf("%s>>>%s ", YELLOW, RESET);
	
	scanf("%s", userInput);

	if (!strcmp(userInput, "1")) {
		character.class = "Warrior";
		character.health = 100;
		character.totalHealth = 100;
		character.attackDamage = 15;

	} else if (!strcmp(userInput, "2")) {
		character.class = "Mage";
		character.health = 50;
		character.totalHealth = 50;
		character.attackDamage = 20;

	} else if (!strcmp(userInput, "3")) {
		character.class = "Archer";
		character.health = 75;
		character.totalHealth = 75;
		character.attackDamage = 15;

	} else {
		printf("%s%sError: Invalid input. Retrying...\n%s", CLEAR, RED, RESET);
		sleep(1);
		makeCharacter();
	}
	
	printf("%s%sWelcome to Replay%s\n", CLEAR, GREEN, RESET);
	printf("%sYou are %s%s%s the %s%s%s\n", 
			PURPLE, YELLOW, character.name, PURPLE, YELLOW, character.class, RESET);
	printf("%sIs that correct?%s\n", PURPLE, RESET);
	printf("%s[Y/n]:%s ", YELLOW, RESET);
	scanf("%s", userInput);

	if (!strncmp(userInput, "n", 1) || !strncmp(userInput, "N", 1)) {
		printf("%sOk. Retrying%s", RED, RESET);
		sleep(1);
		makeCharacter();
	}

	return 0;
}

