#define arrlen(array) sizeof(array) / sizeof(array[0])


// user struct
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



int makeCharacter();
int mainMenu();
int dungeon();
int shop();
int invMenu();
void printHeader(char * menuName);
int startMenu();

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

// Sizes of enemies (Tiny, Small, Normal Sized, Large, Enormously big)
char* sizes[] = {"Tiny", "Small", "Normal Sized", "Large", "Enormously Big"};

// All possible enemies
char * enemies[] = {
 	"Witch", "Goblin", "Ogre", "Vampire", "Ghoul", "Wolf",
	"Mummy", "Zombie", "Ghost", "Troll", "Giant", "Scorpion", 
	"Warlock", "Ant Colony", "Necromancer", "Centipede"
};

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
char * inventoryOptions[] = {"Health Potion", "Flesh"};
// amount in inventory (Health Potion, Flesh)
size_t inventoryCount[] = {2, 0};

