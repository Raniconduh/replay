#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include <libxml/parser.h>

#include "replay.h"

// home dir to be opened
DIR * dir;
// contents of the home dir;
// contains more than just file name and is therefore a struct
struct dirent * dirContents;
// array of all save files found
char ** saveFiles;
// number of saves found
size_t saves;
// user input as an int as opposed to string
uintmax_t intInput;


int searchSaves() {
	// assign dirContents to file being read by readdir()
	// while the file being read exists
	while ((dirContents = readdir(dir)) != NULL) {
		// if file name is not . nor ..
		if (strcmp(dirContents->d_name, ".") && strcmp(dirContents->d_name, "..")) {
			// add save file name to saveFiles array at index saves
			saveFiles[saves] = dirContents->d_name;
			// increment saves to show number of save files found
			saves++;

			// reallocate saveFiles to be able to store more saves
			saveFiles = realloc(saveFiles, sizeof(char *) * (saves + 15));
		}
	}

	if (saves > 0) {
		return 0;
	} else {
		return 1;
	}

	return 0;
}


int loadSave(char * saveName, playerCharacter * character) {
	// actual saveFile itself
	xmlDoc * saveFile = NULL;
	// create nodes necessary for parsing
	xmlNode *root, *first_child, *node;
	
	// save the path of save file into `file` variable
	char * filePath = calloc((strlen(getenv("HOME")) + strlen(saveName)) * sizeof(char *), 1);
	strcpy(filePath, getenv("HOME"));
	strcat(filePath, "/.config/replay/");
	strcat(filePath, saveName);

	saveFile = xmlReadFile(filePath, NULL, 0);

	root = xmlDocGetRootElement(saveFile);

	//printf("Root element is: %s, type %i\n", root->name, root->type);
	first_child = root->children;

	// go through xml file structure
	for (node = first_child; node; node = node->next) {
		// printf("Child is %s, type %i\n", node->name, node->type);

		if (!strcmp((char *)node->name, "class"))
			character->class = (char *)xmlNodeGetContent(node);

		else if (!strcmp((char *)node->name, "name"))
			character->name = (char *)xmlNodeGetContent(node);

		else if (!strcmp((char *)node->name, "totalHealth"))
			character->totalHealth = strtoull(
					(char*)xmlNodeGetContent(node), NULL, 10);

		else if (!strcmp((char *)node->name, "health"))
			character->health = strtoull(
					(char*)xmlNodeGetContent(node), NULL, 10);

		else if (!strcmp((char *)node->name, "damage"))
			character->damage = strtoull(
					(char*)xmlNodeGetContent(node), NULL, 10);

		else if (!strcmp((char *)node->name, "coins"))
			character->coins = strtoull(
					(char*)xmlNodeGetContent(node), NULL, 10);

	}

	xmlFreeDoc(saveFile);
	free(filePath);

	return 0;
}


int startMenu(playerCharacter * character) {
startMenuLabel:
	printf("%s%sWelcome to Replay!%s\n\n", CLEAR, GREEN, RESET);

	printf("%s  1. Load Save\n", CYAN);
	printf("  2. Create New Save%s\n\n", RESET);

	printf("%s>>>%s ", YELLOW, RESET);
	char * userInput = malloc(sizeof(char) * 5);
	scanf("%s", userInput);

	// Load an existing save
	if (!strncmp(userInput, "1", 1)) {
		// get $HOME environment variable
		char * home = calloc(sizeof(char) * (strlen(getenv("HOME")) + strlen("/.config/replay") + 10), 1);
		strcpy(home, getenv("HOME"));
		strcat(home, "/.config/replay");

		dir = opendir(home);
		saveFiles = calloc(sizeof(char) * (saves + 15 + 35), 1);
		saves = 0;

		// If directory exists
		if (dir) {
			// If no save files are found
			if (searchSaves() == 1) {
				printf("%s%sNo saves found.%s\n", CLEAR, PURPLE, RESET);
				sleep(1);

				closedir(dir);

				goto startMenuLabel;
			}

			printf("%s%sSelect a Save%s\n\n", CLEAR, GREEN, RESET);
		
			// Loop through found save files and display them with
			// numbered options (e.g.  1. save1  2. save2)
			size_t i;
			for (i = 1; i <= saves; i++) {
				printf("%s  %ld. %s%s\n", CYAN, i, saveFiles[i-1], RESET);
			}

			// print i with exit option
			// i will be 1 more than highest save option
			printf("%s  %ld. Exit%s\n\n", CYAN, i, RESET);

			printf("%s>>>%s ", YELLOW, RESET);
			scanf("%s", userInput);
			
			// convert i to string to compare user input to it
			char * stringi = malloc(sizeof(i) * sizeof(char));
			snprintf(stringi, sizeof(i), "%ld", i);

			// if user chooses exit option
			if (!strncmp(userInput, stringi, 1)) {
				free(home);
				free(stringi);
				free(saveFiles);
				closedir(dir);

				goto startMenuLabel;
			} else {
				intInput = strtoull(userInput, NULL, 10);
				// invalid input entered
				if (intInput == 0 || intInput > saves) {
					printf("%s%sError: Invalid Input. Retrying...%s\n",
							CLEAR, RED, RESET);
					sleep(1);

					free(home);
					free(stringi);
					free(saveFiles);
					closedir(dir);

					goto startMenuLabel;
				} // else {

				if (!loadSave(saveFiles[intInput - 1], character))
					return 0;

				free(home);
				free(stringi);
				free(saveFiles);
				closedir(dir);

				goto startMenuLabel;

				//}

			}

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
		free(userInput);
		return 1;
	}

	free(userInput);

	return 0;
}


