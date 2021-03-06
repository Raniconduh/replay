#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>

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
		// if file name is not . nor .. nor any dotfile
		if (dirContents->d_name[0] != '.') {
			// add save file name to saveFiles array at index saves
			saveFiles[saves] = dirContents->d_name;
			// increment saves to show number of save files found
			saves++;

			// reallocate saveFiles to be able to store more saves
			saveFiles = realloc(saveFiles, sizeof(char *) * (saves + 35));
		}
	}

	if (saves > 0)
		return 0;

	return 1;
}


int loadSave(char * saveName, playerCharacter * character) {
	// actual saveFile itself
	xmlDoc * saveFile = NULL;
	// create nodes necessary for parsing
	xmlNode *root, *first_child, *node;
	
	// save the path of save file into `file` variable
	char * filePath = calloc((strlen(getenv("HOME"))
				+ strlen(saveName) + 35) * sizeof(char), 1);
	strcpy(filePath, getenv("HOME"));
	strcat(filePath, "/.config/replay/");
	strcat(filePath, saveName);

	character->savePath = calloc(sizeof(char) * (strlen(filePath) + 35), 1);
	strcpy(character->savePath, filePath);

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
		
		else if (strcmp((char *)node->name, "text")) {
			// if string is "i[0-9]", then it is an inventory index save
			if ((char)node->name[0] == 'i' && isdigit((char)node->name[1])) {

				char * newIndex = malloc(strlen((char *)node->name) * sizeof(char));
				strcpy(newIndex, (char *)node->name);
				// strip the letter 'i' from the index of the inventory item
				memmove(&newIndex[0], &newIndex[1], strlen(newIndex));
				// convert string index to a number
				size_t invIndex = strtoull(newIndex, NULL, 10);
				free(newIndex);
				// count of the item at inventory index invIndex
				size_t invCount = strtoull((char *)xmlNodeGetContent(node), NULL, 10);
				// load item count
				inventoryCount[invIndex] = invCount;
			}
		}
	}

	xmlFreeDoc(saveFile);
	xmlCleanupParser();
	free(filePath);

	return 0;
}


int startMenu(playerCharacter * character) {
startMenuLabel:
	printf("%s%sWelcome to Replay!%s\n\n", CLEAR, GREEN, RESET);

	printf("%s  1. Load Save\n", CYAN);
	printf("  2. Create New Save\n");
	printf("  q. Quit Game%s\n\n", RESET);

	printf("%s>>>%s ", YELLOW, RESET);
	char * userInput = malloc(sizeof(char) * 10);
	scanf("%s", userInput);

	// Load an existing save
	if (userInput[0] == '1') {
		// get $HOME environment variable
		char * home = malloc(sizeof(char) * (strlen(getenv("HOME"))
					+ strlen("/.config/replay") + 35));
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
				
				free(home);
				free(saveFiles);
				free(userInput);
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

			// exit option
			printf("%s  q. Exit%s\n\n", CYAN, RESET);

			printf("%s>>>%s ", YELLOW, RESET);
			scanf("%s", userInput);
			
			// if user chooses exit option
			if (userInput[0] == 'q') {
				free(home);
				free(saveFiles);
				free(userInput);
				closedir(dir);

				goto startMenuLabel;
			}

			intInput = strtoull(userInput, NULL, 10);
			// invalid input entered
			if (intInput == 0 || intInput > saves) {
				printf("%s%sError: Invalid Input. Retrying...%s\n",
						CLEAR, RED, RESET);
				sleep(1);
				
				free(userInput);
				free(home);
				free(saveFiles);
				closedir(dir);

				goto startMenuLabel;
			}

			if (!loadSave(saveFiles[intInput - 1], character)) {
				free(userInput);
				free(saveFiles);
				free(home);
				closedir(dir);

				return 0;
			}
			
			free(userInput);
			free(home);
			free(saveFiles);
			closedir(dir);

			goto startMenuLabel;


		// Directory does not exist
		} else if (ENOENT == errno) {
			printf("%s%sNo directory found.%s\n", CLEAR, RED, RESET);
			sleep(1);
			
			free(userInput);
			free(home);
			free(saveFiles);

			goto startMenuLabel;
		// opendir failed
		} else {
			printf("%s%sProblem searching for saves.%s\n", CLEAR, RED, RESET);
			sleep(1);
			
			free(userInput);
			free(home);
			free(saveFiles);

			goto startMenuLabel;
		}

	// Make a new save
	} else if (userInput[0] == '2') {
		free(userInput);

		return 1;
	} else if (userInput[0] == 'q') {
		free(userInput);

		exit(0);
	} else {
		free(userInput);

		goto startMenuLabel;
	}

	free(userInput);

	return 0;
}


