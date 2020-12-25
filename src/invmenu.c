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
	// in the inventory menu is found (it changes depending on number of
	// the items in the inventory)
	char * exitNumber = malloc(arrlen(inventoryCount) * sizeof(int));
	sprintf(exitNumber, "%ld", arrlen(inventoryCount) + 1);

	// if the exit option is chosen
	if (!strcmp(userInput, exitNumber)) {
		free(userInput);
		free(exitNumber);

		return 0;
	}
	free(exitNumber);

	// user input as an integer
	size_t intInput;
	if (!(intInput = strtoull(userInput, NULL, 10))) {
		free(userInput);
		goto invLabel;
	}

	// health potion option is chosen
	if (!strcmp(inventoryOptions[intInput - 1], "Health Potion")) {
		// if there are no health potions
		if (inventoryCount[intInput - 1] < 1) {
			printf("%s%sYou have no Health Potions.%s\n", 
					CLEAR, PURPLE, RESET);
			
		// if health is too high
		} else if (character->health > character->totalHealth - 10) {
			printf("%s%sHealth is too high. You cannot use this item%s\n",
					CLEAR, PURPLE, RESET);

		// conditions are met to use health potion
		} else {
			printf("%s%sYou used a Health Potion and gained %s15%s Health%s\n",
					CLEAR, PURPLE, YELLOW, PURPLE, RESET);

			inventoryCount[intInput - 1]--;
			character->health += 15;
			// set health to 100 if it is greater than 100
			character->health > character->totalHealth ?
				character->health = character->totalHealth:0;
		}

		sleep(1);
		free(userInput);
		goto invLabel;

	} else {
		free(userInput);

		printf("%s%sYou cannot use this item.%s\n",
				CLEAR, PURPLE, RESET);

		sleep(1);

		goto invLabel;
	}

	free(userInput);

	return 0;
}

