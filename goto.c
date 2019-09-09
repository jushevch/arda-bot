/*

ArdaBot by Yuri Shevchenko, 2019
Автоматическая прокачка персонажей MUD-сервера arda.pp.ru

goto.c - маршруты к зонам

*/


#include "main.h"


extern void pushTask( int );


// Маршрут к зоне

bool task_goto( const char *path, const char *check, const int area )
{
	if ( bot.gotoSent ) {
		if ( !strstr( buff.remote, check ) )
			return false;
		pushTask( area );
		bot.gotoSent = false;
		return true;
	}
	
	strcpy( buff.aiComm, path );
	bot.gotoSent = true;
	return false;
}


bool task_gotoHole( void )
{
	const char *path = "ю\nз\nю\nю\nю\nв\nс\n";
	
	const char *check = "Вход в заброшенную нору.";
	
	if ( task_goto( path, check, TASK_EXPLORE_HOLE ) )
		return true;
	return false;
}


bool task_gotoForsaken( void )
{
	const char *path = "ю\nз\nю\nю\nю\nв\nв\nю\nг открой\nю\nю\nв\nв\nв\nв\nв\nв\nвойти\n";
	
	const char *check = "Пустой зал \'Забытого приюта\'.";

	if ( task_goto( path, check, TASK_EXPLORE_FORSAKEN ) )
		return true;
	return false;
}


bool task_gotoMoat( void )
{
	const char *path = "ю\nз\nз\nг открой\nз\nвн\n";
	
	const char *check = "Под насыпью.";

	if ( task_goto( path, check, TASK_EXPLORE_MOAT ) )
		return true;
	return false;
}


bool task_gotoManor( void )
{
	const char *path = "ю\nз\nю\nю\nю\nв\nв\nю\nг открой\nю\nю\n"
	"в\nв\nс\nс\nвойти\nс\nвв\nоткрыть север\nс\nоткрыть север\nс\n";

	const char *check = "Фойе.";

	if ( task_goto( path, check, TASK_EXPLORE_MANOR ) )
		return true;
	return false;
}


bool task_gotoTeds( void )
{
	const char *order = "с\nс\nс\nс\nв\nю\nв\nю\nю\nв\nв\nс\nсз\nсз\nз\nс\nс\nс\nс\n"
	"открыть север\nс\n";

	const char *bree = "ю\nз\nз\nг открой\nз\nз\nс\nз\nз\nз\nз\nз\nз\nз\nз\nз\nз\n"
	"ю\nз\nз\nю\nз\nс\nсз\nсз\nз\nс\nс\nс\nс\nоткрыть север\nс\n";

	const char *check = "Огромный вход.";
	
	const char *path = bot.level > BREE_LEVEL ? order : bree;

	if ( task_goto( path, check, TASK_EXPLORE_TEDS ) )
		return true;
	return false;
}


bool task_gotoNettle( void )
{
	const char *order = "с\nс\nс\nс\nв\nв\nю\nю\nю\nв\nв\nв\nс\nв\nв\nс\nв\nв\nв\n"
	"в\nв\nв\nв\nв\nв\nв\nю\nв\nпостучать ворота\nв\nв\nв\nс\nлечить усталости\n" /* bree */
	"ю\nз\nю\nю\nю\nв\nв\nю\nг открой\nю\nю\nв\nв\nс\nв\nв\nв\nв\nю\n"
	"в\nв\nв\nв\nв\nв\nв\nв\n";
	
	const char *bree = "ю\nз\nю\nю\nю\nв\nв\nю\nговорить открой\n"
	"ю\nю\nв\nв\nс\nв\nв\nв\nв\nю\nв\nв\nв\nв\nв\nв\nв\nв\n";
	
	const char *check = "В самом центре зарослей.";
	
	const char *path = bot.level > BREE_LEVEL ? order : bree;
	
	if ( task_goto( path, check, TASK_EXPLORE_NETTLE ) )
		return true;
	return false;
}


bool task_gotoBywater( void )
{
	const char *order = "с\nс\nс\nс\nв\nю\nв\nю\nю\nв\nв\nс\nсз\nсз\nоткрыть север\nс\n";

	const char *bree = "ю\nз\nз\nг открой\nз\nз\nс\nз\nз\nз\nз\nз\nз\nз\nз\nз\nз\n"
	"ю\nз\nз\nю\nз\nс\nсз\nсз\nоткрыть север\nс\n";

	const char *check = "Плохо освещеннаЯ прихожаЯ.";
	
	const char *path = bot.level > BREE_LEVEL ? order : bree;

	if ( task_goto( path, check, TASK_EXPLORE_BYWATER ) )
		return true;
	return false;
}


bool task_gotoForest( void )
{
	const char *path = "с\nс\nс\nс\nв\nв\nю\nю\nю\nв\nв\nв\nс\nв\nв\nс\nв\nв\nв\n"
	"ю\nю\nв\nоткрыть восток\nв\n";

	const char *check = "Перед Заплотом.";
	
	if ( task_goto( path, check, TASK_EXPLORE_FOREST ) )
		return true;
	return false;
}


bool task_gotoMaggots( void )
{
	const char *path = "с\nс\nс\nс\nв\nв\nю\nю\nю\nю\nю\nюв\nв\nв\nв\nв\nв\nвойти\n";

	const char *check = "УтрамбованнаЯ дорожка.";

	if ( task_goto( path, check, TASK_EXPLORE_MAGGOTS ) )
		return true;
	return false;
}


bool task_gotoSouth( void )
{
	const char *path = "с\nс\nс\nс\nв\nв\nю\nю\nю\nв\nв\nв\nс\nв\nв\nс\nв\nв\nв\n"
	"в\nв\nв\nв\nв\nв\nв\nю\nв\nпостучать ворота\nв\nв\nв\nс\nлечить усталости\n" /* bree */
	"ю\nз\nю\nю\nю\nв\nв\nю\nг открой\nю\nю\nю\n";
	
	const char *check = "У подножиЯ горы.";
	
	if ( task_goto( path, check, TASK_EXPLORE_SOUTH ) )
		return true;
	return false;
}


bool task_gotoAlley( void )
{
	const char *path = "с\nс\nс\nс\nв\nв\nю\nю\nю\nв\nв\nв\nс\nв\nв\nс\nв\nв\nв\nв\n"
	"с\nс\nс\nс\n";
	
	const char *check = "Начало длинной аллеи.";
	
	if ( task_goto( path, check, TASK_EXPLORE_ALLEY ) )
		return true;
	return false;
}


bool task_gotoBandits( void )
{
	const char *path = "с\nс\nс\nс\nв\nв\nю\nю\nю\nз\nз\nз\n";
	
	const char *check = "СломаннаЯ повозка.";
	
	if ( task_goto( path, check, TASK_EXPLORE_BANDITS ) )
		return true;
	return false;
}


bool task_gotoBeorn( void )
{
	const char *path = "смотреть\n";
	
	const char *check = "У огромного цветка.";
	
	if ( task_goto( path, check, TASK_EXPLORE_BEORN ) )
		return true;
	return false;
}

bool task_gotoSteading( void )
{
	const char *path = "с\nс\nс\nс\nв\nв\nв\nв\nв\nв\nв\nю\nю\nз\nз\nс\nв\n";
	
	const char *check = "ПодъезднаЯ дорога.";
	
	if ( task_goto( path, check, TASK_EXPLORE_STEADING ) )
		return true;
	return false;
}


bool task_gotoHavens( void )
{
	const char *path = "смотреть\n";
	
	const char *check = "Перед воротами.";
	
	if ( task_goto( path, check, TASK_EXPLORE_HAVENS ) )
		return true;
	return false;
}


bool task_gotoDwarven( void )
{
	const char *path = "с\nс\nс\nс\nс\nз\nз\nз\nз\nю\nю\nз\nз\nз\nз\nз\nз\nю\nю\nю\nю\nз\nю\n"
	"з\nз\nз\nю\nю\nв\nв\nг гэндальф\nв\nю\nв\nю\nв\nв\nсв\nсв\nсв\n";
	
	const char *check = "Окраина заброшенного подземного города гномов.";
	
	if ( task_goto( path, check, TASK_EXPLORE_DWARVEN ) )
		return true;
	return false;
}


bool task_gotoUmbar( void )
{
	return true;
}


bool task_gotoLich( void )
{
	return true;
}


bool task_gotoLossoth( void )
{
	return true;
}


bool task_gotoSpiders( void )
{
	return true;
}


bool task_gotoCannibals( void )
{
	return true;
}


bool task_gotoPleasure( void )
{
	return true;
}


bool task_gotoSwamp( void )
{
	return true;
}


bool task_gotoFornost( void )
{
	return true;
}

