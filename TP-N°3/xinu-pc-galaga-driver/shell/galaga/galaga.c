
#include "titlescreen.h"
#include "playerImage.h"
#include "enemy.h"
#include "boss.h"
#include "gameover.h"
#include "shoot.h"

extern unsigned char tecla_actual;
typedef unsigned short u16;
#define RGB(r, g, b) (r | (g << 5) | (b << 10))
// #define REG_DISPCNT *(u16 *)0x4000000
#define extern videoBuffer
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define WHITE RGB(31, 31, 31)
#define BLACK RGB(0, 0, 0)

/*
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
*/
// #define BUTTONS *(volatile unsigned int *)0x4000130
#define BUTTON_CERRAR 0x2d
#define BUTTON_A 0x24
#define BUTTON_B 0x25
#define BUTTON_SELECT 0x03
#define BUTTON_START 0x2c
#define BUTTON_RIGHT 0x1f
#define BUTTON_LEFT 0x1e
#define BUTTON_UP 'w'
#define BUTTON_DOWN 's'
#define BUTTON_R '1'
#define BUTTON_L '2'
#define KEY_DOWN_NOW(key) (tecla_actual == key)

// variable definitions
#define playerspeed 2
#define enemyspeed 1
#define fastXSpeed 3
#define fastYSpeed 2

void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16 *image);
void delay_galaga();
void waitForVBlank();

// helpers
void initialize();
void drawEnemies();
void endGame();
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY);

void gestorControl(void);
void puntajeYVida(void);
int pid_puntajeYVida;
int pid_control;
int pid_juego;

// objects
struct Players
{
	volatile u16 playerX;
	volatile u16 playerY;
};
struct Enemy
{
	volatile u16 enemyX;
	volatile u16 enemyY;
};
struct FastEnemy
{
	volatile u16 fastX;
	volatile u16 fastY;
};

int shoots[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int curr_shot = 0;
#define N_SHOOTS 10
#define N_ENEMIES 9

int shotCollision[10] = {0};
int enemyCollision[10] = {0};
void input()
{
	while (1)
	{
		/*
		open
		read(tecla_actual)
		close*/
	}
}
// proceso 1
int galaga(void)
{

	// easy enemy wave set setup
	struct Enemy easyEnemies[9];
	for (int a = 0; a < 9; a++)
	{
		easyEnemies[a].enemyX = (28 * a);
		easyEnemies[a].enemyY = 0;
	}
	easyEnemies[1].enemyX = 240;
	easyEnemies[4].enemyX = 240;
	easyEnemies[8].enemyX = 240;
	// difficult enemies setup
	struct Enemy hardEnemies[9];
	for (int a = 0; a < 9; a++)
	{
		hardEnemies[a].enemyX = (28 * a);
		hardEnemies[a].enemyY = 160;
	}
	hardEnemies[3].enemyX = 240;
	hardEnemies[6].enemyX = 240;
	// player setup
	struct Players player;
	player.playerX = 120;
	player.playerY = 136;
	// fast enemy "boss" setup
	struct FastEnemy fast;
	fast.fastX = 0;
	fast.fastY = 30;

	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	// initalize title screen
	print_text_on_vga(10, 20, "GALAGA ");
	drawImage3(0, 0, 240, 160, titlescreen);
	while (1)
	{
		if (KEY_DOWN_NOW(BUTTON_START))
		{
			break;
		}
	}
	// start black screen for drawing
	for (int i = 0; i < 240; i++)
	{
		for (int j = 0; j < 160; j++)
		{
			setPixel(i, j, BLACK);
		}
	}

	while (1)
	{

		if (KEY_DOWN_NOW(BUTTON_CERRAR))
		{
			send(pid_control, 1);
		}

		// go back to title screen if select button is pressed
		if (KEY_DOWN_NOW(BUTTON_SELECT))
		{
			// initialize();
			galaga();
		}
		// player shots
		if (KEY_DOWN_NOW(BUTTON_A))
		{
			if (shoots[curr_shot] == 0)
			{
				shoots[curr_shot] = 136 * 240 + player.playerX + 9; /* 24 widht player */
				curr_shot++;
				if (curr_shot >= N_SHOOTS)
					curr_shot = 0;
			};
		}
		// player movement input
		if (KEY_DOWN_NOW(BUTTON_LEFT) && (player.playerX > 0))
		{
			player.playerX -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_RIGHT) && (player.playerX <= 216))
		{
			player.playerX += playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_UP) && (player.playerY > 25))
		{
			player.playerY -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN) && (player.playerY <= 136))
		{
			player.playerY += playerspeed;
		}
		waitForVBlank();
		sleepms(50);
		// draw player
		drawImage3(player.playerX, player.playerY, 24, 24, playerImage);
		drawHollowRect(player.playerX - 1, player.playerY - 1, 26, 26, BLACK);
		drawHollowRect(player.playerX - 2, player.playerY - 2, 28, 28, BLACK);
		// draw easy enemies with downward movement
		for (int a = 0; a < 9; a++)
		{
			easyEnemies[a].enemyY += enemyspeed;
			drawImage3(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, enemy);
			if (collision(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 15, 15, player.playerX, player.playerY))
			{
				// Restar una vida al jugador
				if (!enemyCollision[a])
				{
					send(pid_puntajeYVida, 1);
					enemyCollision[a] = 1;
				}
			}
			if (easyEnemies[a].enemyY >= 160)
			{
				easyEnemies[a].enemyY = 0;
			}
		}

		// draw shots
		for (int i = 0; i < N_SHOOTS; i++)
		{
			if (shoots[i] != 0)
			{
				drawRect((shoots[i] % 240), (shoots[i] / 240) + 4, 5, 5, BLACK);
				drawImage3((shoots[i] % 240), (shoots[i] / 240), 5, 5, shoot);
				shoots[i] = shoots[i] - 240 * 4;
				if (shoots[i] <= 0)
					shoots[i] = 0;
			}

			// check hits of shoots
			for (int j = 0; j < 9; j++)
			{
				if (collision(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 15, 15, shoots[i] % 240, shoots[i] / 240))
				{
					drawRect(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 20, 20, BLACK);
					drawRect((shoots[i] % 240), (shoots[i] / 240) + 4, 5, 5, BLACK);
					easyEnemies[j].enemyY = 0;
					if (shoots[i] != 0)
					{
						shoots[i] = 0;
						send(pid_puntajeYVida, 2);
					}
				}
			}
		}
	}
	return 0;
}
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY)
{
	// check if bottom right corner of enemy is within player
	if (((enemyX + enemyWidth) > playerX) && ((enemyY + enemyHeight) > playerY) && ((enemyX + enemyWidth) < (playerX + 24)) && ((enemyY + enemyWidth) < (playerY + 24)))
	{
		return 1;
	}
	// check bottom left corner of enemy
	if ((enemyX < (playerX + 24)) && (enemyX > playerX) && ((enemyY + enemyHeight) > playerY) && ((enemyY + enemyHeight) < (playerY + 24)))
	{
		return 1;
	}
	// check top left corner
	if ((enemyX < (playerX + 24)) && (enemyX > playerX) && (enemyY > playerY) && (enemyY < (playerY + 24)))
	{
		return 1;
	}
	// check top right corner
	if (((enemyX + enemyWidth) < (playerX + 24)) && ((enemyX + enemyWidth) > playerX) && (enemyY > playerY) && (enemyY < (playerY + 24)))
	{
		return 1;
	}
	return 0;
}

// proceso 3
void gestorControl(void)
{
	int mensaje;
	pid_juego = create(galaga, 1024, 20, "Proceso juego", 0);
	resume(pid_juego);

	pid_puntajeYVida = create(puntajeYVida, 1024, 20, "Proceso puntaje y vida", 0);
	resume(pid_puntajeYVida);

	pid_control = getpid();

	mensaje = receive();
	if (mensaje == 1)
	{
		endGame();
		kill(pid_juego);
		kill(pid_puntajeYVida);
		kill(getpid());
	}
}

// proceso 2
void puntajeYVida()
{
	int vidas = 3;
	int puntaje = 0;
	int mensaje;
	char cadena1[30];
	char cadena2[30];
	// Mostrar vidas y puntaje inicialmente
	while (1)
	{
		sprintf(cadena1, "Vidas: %d", vidas);
		print_text_on_vga(300, 100, cadena1);
		sprintf(cadena2, "Puntaje: %d", puntaje);
		print_text_on_vga(300, 120, cadena2);

		mensaje = receive();

		if (mensaje == 1)
		{

			vidas--;
			if (vidas <= 0)
			{
				send(pid_control, 1);
			}
		}
		else if (mensaje == 2)
		{
			puntaje += 100;
		}
	}
}

void endGame()
{
	// start Game Over State
	drawImage3(0, 0, 240, 160, gameover);
	drawHollowRect(0, 0, 240, 160, WHITE);
}
