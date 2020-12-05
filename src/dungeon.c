#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "replay.h"


int dungeon(playerCharacter * character) {
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

	while ((long long)character->health > 0 && (long long)enemy.health > 0) {
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
			size_t totalDamage = character->damage + attackChance;

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
					if (character->health < character->totalHealth - 15) {
						printf("%s%sYou used a Health Potion and gained %s15%s health.%s\n",
								CLEAR, PURPLE,
								YELLOW, PURPLE,
								RESET);
						
						inventoryCount[0]--;
						character->health += 15;
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

					character->health -= excuseDamage;
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
			character->coins += awardedCoins;
			
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
			character->health -= totalDamage;
			printf("%sCritical! The enemy dealt %s%ld%s Damage to You.%s\n", 
					RED, 
					YELLOW, totalDamage, RED,
					RESET);
		
		// if deviation is not 2 nor 0
		} else {
			character->health -= totalDamage;
			printf("%sThe enemy dealt %s%ld%s Damage to you.%s\n", 
					RED, YELLOW, totalDamage, RED,
				   	RESET);
		}
		fflush(stdout);
		sleep(2);

	}

	return 0;
}
