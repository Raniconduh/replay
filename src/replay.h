#define arrlen(array) sizeof(array) / sizeof(array[0])


// user struct
struct pc {
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

	char * savePath;
};
typedef struct pc playerCharacter;


int makeCharacter(playerCharacter * character);
int mainMenu();
int dungeon(playerCharacter * character);
int shop(playerCharacter * character);
int invMenu(playerCharacter * character);
void printHeader(char * menuName);
int startMenu(playerCharacter * character);

int saveGame(playerCharacter * character);


extern const char * CLRLINE;
extern const char * RESET;
extern const char * BLACK;
extern const char * RED;
extern const char * GREEN;
extern const char * YELLOW;
extern const char * BLUE;
extern const char * PURPLE;
extern const char * CYAN;
extern const char * CLEAR;


// enemy struct
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
		char * size;
} badGuy;


// all possible items (Health Potion, Flesh)
extern char * inventoryOptions[2];
// amount in inventory (Health Potion, Flesh)
extern size_t inventoryCount[2];

extern char * sizes[5];
extern char * enemies[16];

