#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "replay.h"

int shop(playerCharacter * character) {
	do { //always go to top
		printHeader("Shop");

		// Options for shop menu
		printf("%s  1. Health Potion - %s10%s Coins\n",
				CYAN, YELLOW, CYAN);
		printf("  2. Sell Flesh - %s5%s Coins Each\n",
				YELLOW, CYAN);
		printf("  3. Exit%s\n\n", RESET);

		printf("%s>>>%s ", YELLOW, RESET);

		char * userInput = malloc(sizeof(char) * 10);
		scanf("%s", userInput);
		
		// If user chooses 1st input
		if (!strncmp(userInput, "1", 1)) {
			// If user has enough coins
			if ((long long)character->coins >= 10) {
				printf("%s%sSuccessfully bought 1 Health Potion.\n", 
						CLEAR, PURPLE);

				sleep(1);

				printf("You have been deducted %s10%s Coins%s\n", 
						YELLOW, PURPLE, RESET);

				character->coins -= 10;
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
				character->coins += 5;

				sleep(1);

			// If user does not have any flesh to sell
			} else {
				printf("%s%sYou have no Flesh to sell.%s\n",
						CLEAR, PURPLE, RESET);
				sleep(1);
			}

		// If user chooses 3rd option
		} else if (!strncmp(userInput, "3", 1)) {
			free(userInput);

			// quit immediatly
			return 0;
		} else {
			printf("%s%sError: Invalid Input. Retrying...%s\n",
					CLEAR, RED, RESET);
			
			sleep(1);
		}

		free(userInput);

	} while (1); // infinite loop better than goto

	return 0;

}
