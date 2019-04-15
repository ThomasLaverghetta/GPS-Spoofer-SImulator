#include "Keyboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

#include <vector>

struct termios oldSettings, newSettings;
struct timeval tv;

char lastKeyPressed = 0;


void Keyboard_Init()
{
	// User input settings	
	tcgetattr(fileno(stdin), &oldSettings);
	newSettings = oldSettings;
	newSettings.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(fileno(stdin), TCSANOW, &newSettings);
}

void Keyboard_Update(unsigned int update_seconds, unsigned int update_microseconds)
{
	fd_set set;

	tv.tv_sec = update_seconds;
	tv.tv_usec = update_microseconds;

	FD_ZERO(&set);
	FD_SET(fileno(stdin), &set);

	int res = select(fileno(stdin)+1, &set, NULL, NULL, &tv);

	if(res > 0)
	{
		char c;
		read(fileno(stdin), &c, 1);

		lastKeyPressed = c;
	}
	else
	{
		lastKeyPressed = 0;
	}

}

char Keyboard_GetLastKey()
{
	return lastKeyPressed;
}

void Keyboard_Cleanup()
{
	tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
}