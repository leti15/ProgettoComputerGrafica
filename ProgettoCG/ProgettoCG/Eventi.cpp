#include "Eventi.h"

void keyboardPressedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		press_a = true;
		break;

	case 'd':
		press_d = true;
		break;

	case 'w':
		press_w = true;
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
}

void keyboardReleasedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		press_a = false;
		break;

	case 'd':
		press_d = false;
		break;

	case 'w':
		press_w = false;
		break;

	default:
		break;
	}
}

void update(int a)
{
	bool move = false;

	if (press_a)
	{
		accelerazione += 0.1;
		posx -= accelerazione;
		if (posx <= 80) {
			posx = 81;
		}
		move = true;
	}

	if (press_d)
	{
		accelerazione += 0.1;
		posx += accelerazione;
		if (posx >= width - 80) {
			posx = width - 81;
		}

		velocita += 0.01;
		move = true;
	}

	if (press_w)
	{
		accelerazione_verticale += 0.5;
		posy += accelerazione_verticale;

		move = true;
	}

	if (!move ) {	
		if (accelerazione_verticale > -0.4) { accelerazione_verticale -= 0.2; }
		if (accelerazione_verticale < -0.4) { accelerazione_verticale += 0.1; }
		posy += accelerazione_verticale;
	}


	//contengo l'aquilone
	if (posy >= (height * 0.9 - 40))
	{
		posy = height * 0.9 - 41;
		accelerazione -= 0.2;
	}
	if (posy <= (height * 0.1 + 40))
	{
		posy = height * 0.1 + 41;
		accelerazione -= 0.2;
	}
	if (posx >= width - 80) {	posx = width - 81; }


	//aggiorno offser pale eoliche
	for (int i = 0; i <= nPale; i++) {
		offsets[i] -= velocita;
		if (offsets[i] <= 0)
			offsets[i] = width - offsets[i];
	}

	//aggiorno e creo nuvole
	for (int i = 0; i < tot_nuvole; i++) {
		//se sono da iniziallizzare 
		if (posNuvole[i].x == -1 && posNuvole[i].y == -1) {
			posNuvole[i].x = width;
			posNuvole[i].y = height * (rand() % 100 + 1) / 100;
		}

		posNuvole[i].x -= velocita;

		//se avviene collisione tra aquilone e nuvola, sparisce
		if (((posx + 80 >= posNuvole[i].x + 2 && posx + 80 <= posNuvole[i].x + 152) || (posx - 80 >= posNuvole[i].x + 2 && posx - 80 <= posNuvole[i].x + 152)) && ((posy + 40 >= posNuvole[i].y + 12 && posy + 40 <= posNuvole[i].y + 50) || (posy - 40 >= posNuvole[i].y + 12 && posy - 40 <= posNuvole[i].y + 50))) {
			posNuvole[i].x = -300;
			accelerazione -= 0.2;
			if (accelerazione < 0) { accelerazione = 0; }
		}

		//in caso le nuvole vadano fuori dalla viewport metto in disuso i loro puntatori
		if (posNuvole[i].x <= -150) {
			posNuvole[i].x = -1;
			posNuvole[i].y = -1;
		}
	}

	//aggiorno posizione del sole e delle pale 
	posSole += 0.2;
	angolo += 5;

	glutPostRedisplay();
	glutTimerFunc(24, update, 0);
}
