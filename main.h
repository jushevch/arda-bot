/*

ArdaBot by Yuri Shevchenko, 2019
Автоматическая прокачка персонажей MUD-сервера arda.pp.ru

main.h - общий заголовочный файл

*/


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


#define MAX_TASKS 10
#define MAX_STRLEN 80


struct botData {

	bool on;

	bool fight;

	bool getLevelSent;
	bool gotoSent;
	bool lookSent;
	bool killSent;
	bool castSent;

	int task;
	int taskStack[ MAX_TASKS ];
	int taskStackPoint;

	int level;
	int area;
	int step;
	int killMob;

	int hp;
	int hpMax;
	int mana;
	int manaMax;
	int move;
	int moveMax;

	char charPrompt[ MAX_STRLEN ];
	char charFPrompt[ MAX_STRLEN ];
};


#define MAX_BUFF 4096


struct buffData {
	char local[ MAX_BUFF ];
	char remote[ MAX_BUFF ];
	char aiComm[ MAX_BUFF ];
};


extern struct botData bot;
extern struct buffData buff;


#define BREE_LEVEL 10

#define MIN_HP bot.hpMax / 3
#define MIN_MANA 30
#define MIN_MOVE 30


typedef enum {
	
	AREA_HOLE, AREA_FORSAKEN, AREA_MOAT, AREA_MANOR, 
	AREA_TEDS, AREA_NETTLE, AREA_BYWATER, AREA_FOREST, 
	AREA_MAGGOTS, AREA_SOUTH, AREA_ALLEY, AREA_BANDITS, 
	AREA_BEORN, AREA_STEADING, AREA_HAVENS, AREA_DWARVEN, 
	AREA_UMBAR, AREA_LICH, AREA_LOSSOTH, AREA_SPIDERS, 
	AREA_CANNIBALS, AREA_PLEASURE, AREA_SWAMP, AREA_FORNOST

} areaNumbers;


typedef enum {
	
	TASK_DEFAULT, 
	TASK_SET_BOT_PROMPT, TASK_SET_BOT_FPROMPT, 
	TASK_CHECK_LEVEL, TASK_DISENGAGE_BOT, 

	TASK_GOTO_HOLE, TASK_GOTO_FORSAKEN, TASK_GOTO_MOAT, TASK_GOTO_MANOR, 
	TASK_GOTO_TEDS, TASK_GOTO_NETTLE, TASK_GOTO_BYWATER, TASK_GOTO_FOREST, 
	TASK_GOTO_MAGGOTS, TASK_GOTO_SOUTH, TASK_GOTO_ALLEY, TASK_GOTO_BANDITS, 
	TASK_GOTO_BEORN, TASK_GOTO_STEADING, TASK_GOTO_HAVENS, TASK_GOTO_DWARVEN, 
	TASK_GOTO_UMBAR, TASK_GOTO_LICH, TASK_GOTO_LOSSOTH, TASK_GOTO_SPIDERS, 
	TASK_GOTO_CANNIBALS, TASK_GOTO_PLEASURE, TASK_GOTO_SWAMP, TASK_GOTO_FORNOST, 
	
	TASK_EXPLORE_HOLE, TASK_EXPLORE_FORSAKEN, TASK_EXPLORE_MOAT, TASK_EXPLORE_MANOR, 
	TASK_EXPLORE_TEDS, TASK_EXPLORE_NETTLE, TASK_EXPLORE_BYWATER, TASK_EXPLORE_FOREST, 
	TASK_EXPLORE_MAGGOTS, TASK_EXPLORE_SOUTH, TASK_EXPLORE_ALLEY, TASK_EXPLORE_BANDITS, 
	TASK_EXPLORE_BEORN, TASK_EXPLORE_STEADING, TASK_EXPLORE_HAVENS, TASK_EXPLORE_DWARVEN

} taskValues;

