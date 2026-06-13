#pragma once

#include <Winsock2.h>
#include <stdio.h>

#pragma warning(disable : 4996)

extern SOCKET sockClientfd;

enum
{
	err_creat_socket = -9000,
	err_net_version,
	err_net_tx,
	err_net_rx_timeout,
	err_net_cmd,
	err_net_pts,
	success = 0,
	usb_dev_tx_timeout,
};

int kmNet_init(char* ip, char* port, char* mac);
int kmNet_mouse_move(short x, short y);
int kmNet_mouse_left(int isdown);
int kmNet_mouse_right(int isdown);
int kmNet_mouse_middle(int isdown);
int kmNet_mouse_wheel(int wheel);
int kmNet_mouse_all(int button, int x, int y, int wheel);
int kmNet_mouse_move_auto(int x, int y, int time_ms);
int kmNet_mouse_move_beizer(int x, int y, int ms, int x1, int y1, int x2, int y2);
int kmNet_keydown(int vkey);
int kmNet_keyup(int vkey);
int kmNet_reboot(void);
int kmNet_debug(short port, char enable);
int kmNet_monitor(char enable);
int kmNet_monitor_mouse_left();
int kmNet_monitor_mouse_middle();
int kmNet_monitor_mouse_right();
int kmNet_monitor_mouse_side1();
int kmNet_monitor_mouse_side2();
int kmNet_monitor_keyboard(unsigned char vk_key);
int kmNet_mask_mouse_left(int enable);
int kmNet_mask_mouse_right(int enable);
int kmNet_mask_mouse_middle(int enable);
int kmNet_mask_mouse_side1(int enable);
int kmNet_mask_mouse_side2(int enable);
int kmNet_mask_mouse_x(int enable);
int kmNet_mask_mouse_y(int enable);
int kmNet_mask_mouse_wheel(int enable);
int kmNet_mask_keyboard(short vkey);
int kmNet_unmask_all();
int kmNet_setconfig(char* ip, unsigned short port);
int kmNet_lcd_color(unsigned short rgb565);
int kmNet_lcd_picture_bottom(unsigned char* buff_128_80);
int kmNet_lcd_picture(unsigned char* buff_128_160);
