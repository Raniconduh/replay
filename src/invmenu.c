#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "replay.h"

int invMenu(playerCharacter * character) {
invLabel:
	printHeader("Inventory");

	for (size_t i = 0; i < arrlen(inventoryOptions); i++) {
		printf("%s  %ld. %s%s: ", CYAN, i + 1, RESET, inventoryOptions[i]);
		printf("%s%ld%s\n", YELLOW, inventoryCount[i], RESET);
	}

	printf("%s  %ld. Exit%s\n\n", CYAN, arrlen(inventoryOptions) + 1, RESET);

	printf("%s>>>%s ", YELLOW, RESET);
	char * userInput = malloc(sizeof(char) * 5);
	scanf("%s", userInput);

	// exitNumber is the number at which the exit option
	// in the game is found (it changes depending on number of saves)
	char * exitNumber = malloc(arrlen(inventoryCount) * sizeof(int));
	sprintf(exitNumber, "%ld", arrlen(inventoryCount) + 1);

	printf("%s  %s. Exit%s\n\n", CYAN, exitNumber, RESET);

	// if the exit option is chosen
	if (!strcmp(userInput, exitNumber)) {
		free(userInput);
		free(exitNumber);

		return 0;

	// if option 1 is chosen
	} else if (!strcmp(userInput, "1")) {

		// If there are more than 0 health potions and option 1 is chosen
		if (inventoryCount[0] > 0)  {

			// If current health is far enough away from max health
			if (character->health < character->totalHealth - 15) {
			printf("%s%sYou used %s1%s Health Potion and gained %s15%s Health.%s\n",
					CLEAR, PURPLE,
					YELLOW, PURPLE, YELLOW, PURPLE,
					RESET);
			
			inventoryCount[0]--;
			character->health += 15;
			
			sleep(1);
			
			// if current health is too close to max health
			} else {
				printf("%s%sYou cannot use this item. Health is too high%s\n",
						CLEAR, PURPLE, RESET);
				sleep(1);

			}

		// If there are not enough health potions
		} else {
			printf("%s%sYou do not have any Health Potions%s\n",
					CLEAR, PURPLE, RESET);
			sleep(1);
		}
	
		free(userInput);
		free(exitNumber);

		goto invLabel;

	// If chosen option is not a usable item or does not exist
	} else {
		free(userInput);
		free(exitNumber);

		printf("%s%sYou cannot use this item.%s\n",
				CLEAR, PURPLE, RESET);

		sleep(1);

		goto invLabel;
	}

	free(userInput);
	free(exitNumber);

	return 0;
}

