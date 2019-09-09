/*

ArdaBot by Yuri Shevchenko, 2019
Автоматическая прокачка персонажей MUD-сервера arda.pp.ru

main.c - генеральный цикл, сокеты

*/


#include "main.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <locale.h>


#define ARDA_IP "79.98.143.118" // arda.pp.ru
#define ARDA_PORT "4000"
#define LOCALHOST_PORT "4000"


struct sockData {
	SOCKET local;
	SOCKET remote;
};


struct sockData sock;
struct buffData buff;
struct botData bot;


void init_winsock( void );
SOCKET get_local_socket( void );
SOCKET get_remote_socket( void );
void shutdown_sockets( void );

void main_loop( void );
bool read_from( SOCKET, char* );
void write_to( SOCKET, char* );

void switch_bot_state( void );
void reset_bot( void );

extern void ai( void );


// Старт программы

int main( void )
{
	setlocale( LC_ALL, "RUS" );
	
	printf( "\nArdaBot by Yuri Shevchenko, 2019\n\n" );

	init_winsock();
	
	sock.local = get_local_socket();
	sock.remote = get_remote_socket();
	
	main_loop();
	
	shutdown_sockets();

	printf( "Normal termination.\n" );
	
	return 0;
}


// Генеральный цикл

void main_loop( void )
{
	printf( "Exchanging data . . .\n" );

	fd_set inSet, outSet;
	
	while ( 1 ) {
		
		// Подготовка наборов

		FD_ZERO( &inSet );
		FD_ZERO( &outSet );

		FD_SET( sock.local, &inSet );
		FD_SET( sock.remote, &inSet );

		FD_SET( sock.local, &outSet );
		FD_SET( sock.remote, &outSet );

		// Анализ состояния сокетов
		
		if ( buff.remote[ 0 ] == '\0' && buff.local[ 0 ] == '\0' )

			if ( select( 0, &inSet, NULL, NULL, NULL ) == SOCKET_ERROR ) {
				printf( "select() failed: %i\n", WSAGetLastError() );
				exit( 1 );
			}
		
		static TIMEVAL noLag;
		
		if ( select( 0, &inSet, &outSet, NULL, &noLag ) == SOCKET_ERROR ) {
			printf( "select() failed: %i\n", WSAGetLastError() );
			exit( 1 );
		}
		
		// Получение данных
		
		if ( FD_ISSET( sock.remote, &inSet ) )
			if ( !read_from( sock.remote, buff.remote ) )
				return;
		
		if ( FD_ISSET( sock.local, &inSet ) )
			if ( !read_from( sock.local, buff.local ) )
				return;

		// Отправка мад-клиенту данных Арды, запуск ИИ

		if ( FD_ISSET( sock.local, &outSet ) && buff.remote[ 0 ] != '\0' ) {

			write_to( sock.local, buff.remote );

			if ( bot.on )
				ai();

			buff.remote[ 0 ] = '\0';
		}
		
		// Отправка Арде данных мад-клиента и команд ИИ
		// В бот-режиме игнорируются все команды мад-клиента кроме "бот"
		
		if ( FD_ISSET( sock.remote, &outSet ) ) {
			
			if ( buff.local[ 0 ] != '\0' ) {

				if ( strstr( buff.local, "**бот" ) )
					switch_bot_state();
				else if ( !bot.on )
					write_to( sock.remote, buff.local );

				buff.local[ 0 ] = '\0';
			}
			
			if ( bot.on && buff.aiComm[ 0 ] != '\0' ) {
				write_to( sock.remote, buff.aiComm );
				buff.aiComm[ 0 ] = '\0';
			}
		}
	}
}


// Переключатель состояния бот-режима

void switch_bot_state( void )
{
	if ( bot.on ) {
		bot.on = false;
		strcpy( buff.local, "приглашение " );
		strcat( buff.local, bot.charPrompt );
		strcat( buff.local, "\nбприглашение ");
		strcat( buff.local, bot.charFPrompt );
		strcat( buff.local, "\n" );
		write_to( sock.remote, buff.local );
	}
	else {
		bot.on = true;
		reset_bot();
		if ( strstr( buff.local, "нора" ) )
			bot.area = AREA_HOLE;
		else if ( strstr( buff.local, "забытый" ) )
			bot.area = AREA_FORSAKEN;
		else if ( strstr( buff.local, "ров" ) )
			bot.area = AREA_MOAT;
		else if ( strstr( buff.local, "особняк" ) )
			bot.area = AREA_MANOR;
		else if ( strstr( buff.local, "тэд" ) )
			bot.area = AREA_TEDS;
		else if ( strstr( buff.local, "крапива" ) )
			bot.area = AREA_NETTLE;
		else if ( strstr( buff.local, "уводье" ) )
			bot.area = AREA_BYWATER;
		else if ( strstr( buff.local, "древлепуща" ) )
			bot.area = AREA_FOREST;
		else if ( strstr( buff.local, "мэггот" ) )
			bot.area = AREA_MAGGOTS;
		else if ( strstr( buff.local, "увалы" ) )
			bot.area = AREA_SOUTH;
		else if ( strstr( buff.local, "аллея" ) )
			bot.area = AREA_ALLEY;
		else if ( strstr( buff.local, "бандиты" ) )
			bot.area = AREA_BANDITS;
		else if ( strstr( buff.local, "беорн" ) )
			bot.area = AREA_BEORN;
		else if ( strstr( buff.local, "хутор" ) )
			bot.area = AREA_STEADING;
		else if ( strstr( buff.local, "гавани" ) )
			bot.area = AREA_HAVENS;
		else if ( strstr( buff.local, "алчные" ) )
			bot.area = AREA_DWARVEN;
		else if ( strstr( buff.local, "умбар" ) )
			bot.area = AREA_UMBAR;
		else if ( strstr( buff.local, "башня" ) )
			bot.area = AREA_LICH;
		else if ( strstr( buff.local, "лоссоты" ) )
			bot.area = AREA_LOSSOTH;
		else if ( strstr( buff.local, "паучье" ) )
			bot.area = AREA_SPIDERS;
		else if ( strstr( buff.local, "людоеды" ) )
			bot.area = AREA_CANNIBALS;
		else if ( strstr( buff.local, "радость" ) )
			bot.area = AREA_PLEASURE;
		else if ( strstr( buff.local, "болото" ) )
			bot.area = AREA_SWAMP;
		else if ( strstr( buff.local, "форност" ) )
			bot.area = AREA_FORNOST;
		else
			bot.on = false;
		write_to( sock.remote, "\n" );
	}
	
	return;
}


void reset_bot( void )
{
	bot.step = 0;
	bot.task = 0;
	bot.taskStackPoint = 0;
	
	bot.getLevelSent = false;
	bot.gotoSent = false;
	bot.lookSent = false;
	bot.killSent = false;
	bot.castSent = false;
	
	bot.charPrompt[ 0 ] = '\0';
	bot.charFPrompt[ 0 ] = '\0';
	
	buff.aiComm[ 0 ] = '\0';
	
	return;
}


// Получение данных от сокета

bool read_from( SOCKET sck, char *buf )
{
	// Текущий уровень буфера
	
	int iStart = strlen( buf );
	
	if ( iStart >= MAX_BUFF ) {
		printf( "Buffer overflow.\n" );
		return false;
	}
	
	// Получение строки
	
	int nRead = recv( sck, buf + iStart, MAX_BUFF - iStart, 0 );
	
	if ( nRead > 0 )
		iStart += nRead;
	
	else if ( nRead == 0 ) {
		printf( "Socket closed from outside.\n" );
		return false;
	}

	else {
		printf( "recv() failed: %i\n", WSAGetLastError() );
		return false;
	}

	buf[ iStart ] = '\0';

	return true;
}


// Отправка данных на сокет

void write_to( SOCKET sck, char *buf )
{
	int len = strlen( buf );
	int iStart, nWrite, nBlock;

	for ( iStart = 0; iStart < len; iStart += nWrite ) {

		nBlock = len - iStart;
		nWrite = send( sck, buf + iStart, nBlock, 0 );

		if ( nWrite == SOCKET_ERROR ) {
			printf( "send() failed: %d\n", WSAGetLastError() );
			exit( 1 );
		}
	}
	
	return;
}


// Инициализация сокета мад-клиента

SOCKET get_local_socket( void )
{
	printf( "Initializing local server . . .\n" );

	struct addrinfo hints, *result = NULL;

	ZeroMemory( &hints, sizeof( hints ) );
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if ( getaddrinfo( NULL, LOCALHOST_PORT, &hints, &result ) ) {
		printf( "getaddrinfo() failed: %i\n", WSAGetLastError() );
		WSACleanup();
    	exit( 1 );
	}
	
	SOCKET serSock = socket( result->ai_family, result->ai_socktype, result->ai_protocol );

	if ( serSock == INVALID_SOCKET ) {
		printf( "socket() failed: %i\n", WSAGetLastError() );
		freeaddrinfo( result );
		WSACleanup();
		exit( 1 );
	}
	
    if ( bind( serSock, result->ai_addr, ( int ) result->ai_addrlen ) == SOCKET_ERROR ) {
        printf( "bind() failed: %i\n", WSAGetLastError() );
        closesocket( serSock );
        freeaddrinfo( result );
        WSACleanup();
        exit( 1 );
    }
    
    freeaddrinfo( result );
    
    if ( listen( serSock, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "listen() failed: %i\n", WSAGetLastError() );
		closesocket( serSock );
		WSACleanup();
		exit( 1 );
	}

	printf( "Server initialized.\n" );
		
	printf( "Waiting a MUD client to connect to localhost, port 4000 . . .\n" );
	
	SOCKET locSock = accept( serSock, NULL, NULL );

	closesocket( serSock );
	
	if ( locSock == INVALID_SOCKET ) {
		printf( "accept() failed: %i\n", WSAGetLastError() );
		WSACleanup();
		exit( 1 );
	}

	printf( "Connection accepted.\n" );

	return locSock;
}


// Инициализация сокета Арды

SOCKET get_remote_socket( void )
{
	printf( "Connecting to arda.pp.ru, port 4000 . . .\n" );

	struct addrinfo hints, *result = NULL;

	ZeroMemory( &hints, sizeof( hints ) );
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;	

	if ( getaddrinfo( ARDA_IP, ARDA_PORT, &hints, &result ) ) {
    	printf( "getaddrinfo() failed: %i\n", WSAGetLastError() );
    	WSACleanup();
    	exit( 1 );
	}

	SOCKET remSock = socket( result->ai_family, result->ai_socktype, result->ai_protocol );

	if ( remSock == INVALID_SOCKET ) {
    	printf( "socket() failed: %i\n", WSAGetLastError() );
    	freeaddrinfo( result );
    	WSACleanup();
    	exit( 1 );
	}

	if ( connect( remSock, result->ai_addr, ( int ) result->ai_addrlen ) == SOCKET_ERROR ) {
    	closesocket( remSock );
    	remSock = INVALID_SOCKET;
	}

	freeaddrinfo( result );

	if ( remSock == INVALID_SOCKET ) {
    	printf( "Unable to connect.\n" );
    	WSACleanup();
    	exit( 1 );
	}
	
	printf( "Connection established.\n" );
	
	return remSock;
}


// Инициализация библиотеки сокетов

void init_winsock( void )
{
	WSADATA wsaData;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) ) {
		printf( "WSAStartup failed\n" );
		exit( 1 );
	}
	
	return;
}


// Завершение работы с сокетами

void shutdown_sockets( void )
{
	shutdown( sock.local, SD_BOTH );
	closesocket( sock.local );

	shutdown( sock.remote, SD_BOTH );
	closesocket( sock.remote );

	WSACleanup();
	
	return;
}

