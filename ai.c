/*

ArdaBot by Yuri Shevchenko, 2019
Автоматическая прокачка персонажей MUD-сервера arda.pp.ru

ai.c - обработка стека задач

*/


#include "main.h"


// Базовый функционал ИИ

void pushTask( int ); int popTask( void );
void updatePromptData( void );
void ai( void );


// Базовые задачи ИИ

bool task_default( void ); 

bool task_setBotPrompt( void ); 
bool task_setBotFPrompt( void ); 

bool task_checkLevel( void ); 
bool task_disengageBot( void ); 


// Маршруты к зонам

extern bool task_gotoHole( void ); 
extern bool task_gotoForsaken( void ); 
extern bool task_gotoMoat( void ); 
extern bool task_gotoManor( void ); 

extern bool task_gotoTeds( void ); 
extern bool task_gotoNettle( void ); 
extern bool task_gotoBywater( void );
extern bool task_gotoForest( void ); 

extern bool task_gotoMaggots( void ); 
extern bool task_gotoSouth( void ); 
extern bool task_gotoAlley( void ); 
extern bool task_gotoBandits( void ); 

extern bool task_gotoBeorn( void ); 
extern bool task_gotoSteading( void ); 
extern bool task_gotoHavens( void );
extern bool task_gotoDwarven( void ); 

extern bool task_gotoUmbar( void ); 
extern bool task_gotoLich( void ); 
extern bool task_gotoLossoth( void ); 
extern bool task_gotoSpiders( void ); 

extern bool task_gotoCannibals( void ); 
extern bool task_gotoPleasure( void ); 
extern bool task_gotoSwamp( void ); 
extern bool task_gotoFornost( void );


// Прохождение зон

extern bool task_exploreHole( void );
extern bool task_exploreForsaken( void );
extern bool task_exploreMoat( void );
extern bool task_exploreManor( void );

extern bool task_exploreTeds( void );
extern bool task_exploreNettle( void );
extern bool task_exploreBywater( void );
extern bool task_exploreForest( void );

extern bool task_exploreMaggots( void );
extern bool task_exploreSouth( void );
extern bool task_exploreAlley( void );
extern bool task_exploreBandits( void );

extern bool task_exploreBeorn( void );
extern bool task_exploreSteading( void );
extern bool task_exploreHavens( void );
extern bool task_exploreDwarven( void );


// Массив указателей на функции задач
// true - задача выполнена, false - задача не выполнена

bool ( *taskFunction[] )( void ) = {

	task_default, 
	task_setBotPrompt, task_setBotFPrompt,
	task_checkLevel, task_disengageBot, 
	
	task_gotoHole, task_gotoForsaken, task_gotoMoat, task_gotoManor, 
	task_gotoTeds, task_gotoNettle, task_gotoBywater, task_gotoForest, 
	task_gotoMaggots, task_gotoSouth, task_gotoAlley, task_gotoBandits, 
	task_gotoBeorn, task_gotoSteading, task_gotoHavens, task_gotoDwarven, 
	task_gotoUmbar, task_gotoLich, task_gotoLossoth, task_gotoSpiders, 
	task_gotoCannibals, task_gotoPleasure, task_gotoSwamp, task_gotoFornost, 
	
	task_exploreHole, task_exploreForsaken, task_exploreMoat, task_exploreManor,
	task_exploreTeds, task_exploreNettle, task_exploreBywater, task_exploreForest, 
	task_exploreMaggots, task_exploreSouth, task_exploreAlley, task_exploreBandits, 
	task_exploreBeorn, task_exploreSteading, task_exploreHavens, task_exploreDwarven
};


// Запуск бота

bool task_default( void )
{
	pushTask( TASK_DISENGAGE_BOT );
	
	pushTask( bot.area + TASK_GOTO_HOLE );

	pushTask( TASK_CHECK_LEVEL );
	pushTask( TASK_SET_BOT_FPROMPT );
	pushTask( TASK_SET_BOT_PROMPT );

	return true;
}


// Установка приглашения и бприглашения бота

bool task_setBotPrompt( void )
{
	char *comm, *save;
	
	if ( bot.task == TASK_SET_BOT_PROMPT ) {
		comm = "приглашение # bot # %h/%H %m/%M %v/%V # %X # \n";
		save = bot.charPrompt;
	}
	else {
		comm = "бприглашение # bot # %h/%H %m/%M %v/%V # %X # fight # %c \n";
		save = bot.charFPrompt;
	}
	
	static bool commSent;
	
	if ( commSent ) {
		char *pr = strstr( buff.remote, "СтараЯ: \e[0;1;37m" );
		if ( !pr )
			return false;
		for ( int i = 0; pr[ i ] != '\0'; i++ ) {
			if ( pr[ i ] != 'm' )
				continue;
			for ( int j = ++i; pr[ i ] != '\0'; i++ ) {
				if ( pr[ i ] == '\n' || pr[ i ] == '\r' ) {
					save[ i - j ] = '\0';
					commSent = false;
					return true;
				}
				save[ i - j ] = pr[ i ];
			}
		}
	}
	
	strcpy( buff.aiComm, comm );
	commSent = true;
	return false;
}


bool task_setBotFPrompt( void )
{
	if ( task_setBotPrompt() )
		return true;
	return false;
}


// Проверка уровня персонажа

bool task_checkLevel( void )
{
	if ( bot.getLevelSent ) {
		char *info = strstr( buff.remote, "Уровень:" );
		if ( !info )
			return false;
		sscanf( info, "Уровень: %i |", &bot.level );
		bot.getLevelSent = false;
		return true;
	}
	
	strcpy( buff.aiComm, "состояние\n" );
	bot.getLevelSent = true;
	return false;	
}


// Отключить бот-режим

bool task_disengageBot( void )
{
	strcpy( buff.local, "**бот" );
	return false;
}


// Операции со стеком задач

void pushTask( int t )
{
	if ( bot.taskStackPoint >= MAX_TASKS ) {
		printf( "taskStack overflow\n");
		exit( 1 );
	}
	bot.taskStack[ bot.taskStackPoint++ ] = t;
	return;
}


int popTask( void )
{
	if ( bot.taskStackPoint == 0 )
		return TASK_DISENGAGE_BOT;
	return bot.taskStack[ --bot.taskStackPoint ];
}


// Обновление данных строки приглашения

void updatePromptData( void )
{
	switch ( bot.task ) {
		case TASK_DEFAULT:
		case TASK_SET_BOT_PROMPT:
		case TASK_SET_BOT_FPROMPT:
			return;
	}
	
	char *p = strstr( buff.remote, "# bot #" );
	if ( !p ) return;
	
	char str[ strlen( p ) + 1 ];
	strcpy( str, p );
	
	for ( int i = 0; str[ i ] != '/'; i++ )
		if ( str[ i ] == '\e' ) {
			while ( str[ i ] != 'm' )
				str[ i++ ] = ' ';
			str[ i ] = ' ';
		}
	
	sscanf( str, "# bot # %i / %i %i / %i %i / %i #", 
		&bot.hp, &bot.hpMax, &bot.mana, &bot.manaMax, &bot.move, &bot.moveMax );

	bot.fight = strstr( str, "# fight #" ) ? true : false;

	return;
}


// Главный распределитель задач

void ai( void )
{
	updatePromptData();

	while ( taskFunction[ bot.task ]() )
		bot.task = popTask();

	return;
}

