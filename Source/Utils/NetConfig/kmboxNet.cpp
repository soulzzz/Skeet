#include "kmboxNet.h"

SOCKET sockClientfd = 1;

namespace
{
	int ReturnIfConnected()
	{
		return sockClientfd > 0 ? success : err_creat_socket;
	}
}

int kmNet_init(char* ip, char* port, char* mac)
{
	(void)ip;
	(void)port;
	(void)mac;
	sockClientfd = 1;
	return success;
}

int kmNet_mouse_move(short x, short y)
{
	(void)x;
	(void)y;
	return ReturnIfConnected();
}

int kmNet_mouse_left(int isdown)
{
	(void)isdown;
	return ReturnIfConnected();
}

int kmNet_mouse_right(int isdown)
{
	(void)isdown;
	return ReturnIfConnected();
}

int kmNet_mouse_middle(int isdown)
{
	(void)isdown;
	return ReturnIfConnected();
}

int kmNet_mouse_wheel(int wheel)
{
	(void)wheel;
	return ReturnIfConnected();
}

int kmNet_mouse_all(int button, int x, int y, int wheel)
{
	(void)button;
	(void)x;
	(void)y;
	(void)wheel;
	return ReturnIfConnected();
}

int kmNet_mouse_move_auto(int x, int y, int time_ms)
{
	(void)x;
	(void)y;
	(void)time_ms;
	return ReturnIfConnected();
}

int kmNet_mouse_move_beizer(int x, int y, int ms, int x1, int y1, int x2, int y2)
{
	(void)x;
	(void)y;
	(void)ms;
	(void)x1;
	(void)y1;
	(void)x2;
	(void)y2;
	return ReturnIfConnected();
}

int kmNet_keydown(int vkey)
{
	(void)vkey;
	return ReturnIfConnected();
}

int kmNet_keyup(int vkey)
{
	(void)vkey;
	return ReturnIfConnected();
}

int kmNet_reboot(void)
{
	sockClientfd = -1;
	return success;
}

int kmNet_debug(short port, char enable)
{
	(void)port;
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_monitor(char enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_monitor_mouse_left()
{
	return ReturnIfConnected();
}

int kmNet_monitor_mouse_middle()
{
	return ReturnIfConnected();
}

int kmNet_monitor_mouse_right()
{
	return ReturnIfConnected();
}

int kmNet_monitor_mouse_side1()
{
	return ReturnIfConnected();
}

int kmNet_monitor_mouse_side2()
{
	return ReturnIfConnected();
}

int kmNet_monitor_keyboard(unsigned char vk_key)
{
	(void)vk_key;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_left(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_right(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_middle(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_side1(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_side2(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_x(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_y(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_mouse_wheel(int enable)
{
	(void)enable;
	return ReturnIfConnected();
}

int kmNet_mask_keyboard(short vkey)
{
	(void)vkey;
	return ReturnIfConnected();
}

int kmNet_unmask_all()
{
	return ReturnIfConnected();
}

int kmNet_setconfig(char* ip, unsigned short port)
{
	(void)ip;
	(void)port;
	return ReturnIfConnected();
}

int kmNet_lcd_color(unsigned short rgb565)
{
	(void)rgb565;
	return ReturnIfConnected();
}

int kmNet_lcd_picture_bottom(unsigned char* buff_128_80)
{
	(void)buff_128_80;
	return ReturnIfConnected();
}

int kmNet_lcd_picture(unsigned char* buff_128_160)
{
	(void)buff_128_160;
	return ReturnIfConnected();
}
