#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include "replay.h"

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


