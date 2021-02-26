#include <iostream>
#include <time.h>
using namespace std;

enum PLAYER{HUMAN = 'X', AI = 'O', EMPTY = '_' };
#define ISVALID(x, size) ((x)>=0 && (x)<(size))
#define POINT_ITEM(a, r, c) (*((*(a + r)) + c))

typedef struct
{
	int szX;
	int szY;
	PLAYER** map;
	int towin;
} Field;

void setval(PLAYER** map, int y, int x, PLAYER state)
{
	POINT_ITEM(map, y, x) = state;
}
PLAYER getval(PLAYER** map, int y, int x)
{
	return POINT_ITEM(map, y, x);
}

bool isvalid(Field& field, int x, int y)
{
	return ISVALID(x, field.szX) && ISVALID(y, field.szY);
}
bool isempty(Field& field, int x, int y)
{
	return getval(field.map, y, x) == EMPTY;
}
int check_line(Field& field, int x, int y, int vx, int vy, int len, PLAYER c)
{
	const int endx = x + (len - 1) * vx;
	const int endy = y + (len - 1) * vy;
	if (!isvalid(field, endx, endy))
		return 0;
	for (int i = 0; i < len; i++)
		if (getval(field.map, (y + i * vy), (x + i * vx)) != c)
			return 0;
	return 1;
}

void select_field(Field& field)
{
	cout << "Choose a field size first:" << endl;
	cout << "Choose the field width: ";
	cin >> field.szX;
	cout << "Choose field height: ";
	cin >> field.szY;
	if (field.szX == field.szY || field.szX < field.szY) field.towin = field.szX;
	else if (field.szX > field.szY) field.towin = field.szY;
}

void init(Field &field)
{
	field.map = (PLAYER**)calloc(sizeof(PLAYER*), field.szY);
	for (int y = 0; y < field.szY; ++y)
	{
		*(field.map+y) = (PLAYER*)calloc(sizeof(PLAYER), field.szX);
		for (int x = 0; x < field.szX; ++x)
		{
			setval(field.map, y, x, EMPTY);
		}
	}
}

void print(Field& field)
{
	system("cls");
	cout << "-------------" << endl;
	for (int y = 0; y < field.szY; ++y)
	{
		cout << "|";
		for (int x = 0; x < field.szX; ++x)
		{
			cout << (char)getval(field.map, y, x) << "|";
		}
		cout << endl;
	}
}

void human_move(Field &field)
{
	int x, y;
	do
	{
		cout << "Choose where to put X. Column and row: ";
		cin >> x >> y;
		x--;
		y--;
	} while (!isvalid(field, x, y) || !isempty(field, x, y));
	setval(field.map, y, x, HUMAN);
}

int check_win(Field& field, PLAYER player)
{
	for (int y = 0; y < field.szY; ++y)
	{
		for (int x = 0; x < field.szX; ++x)
		{
			if (check_line(field, y, x, 1, 0, field.towin, player)) return 1; //по горизонтали
			if (check_line(field, y, x, 0, 1, field.towin, player)) return 1; // по вертикали
			if (check_line(field, y, x, 1, 1, field.towin, player)) return 1; // по диагонали 1
			if (check_line(field, y, x, 1, -1, field.towin, player)) return 1; // по диагонали 2
		}
	}
	return 0;
}

int ai_win_check(Field& field)
{
	for (int y = 0; y < field.szY; ++y)
	{
		for (int x = 0; x < field.szX; ++x)
		{
			if (isempty(field, x, y))
			{
				setval(field.map, y, x, AI);
				if (check_win(field, AI)) return 1;
				setval(field.map, y, x, EMPTY);
			}
		}
	}
	return 0;
}

int human_win_check(Field& field)
{
	for (int y = 0; y < field.szY; ++y)
	{
		for (int x = 0; x < field.szX; ++x)
		{
			if (isempty(field, x, y))
			{
				setval(field.map, y, x, HUMAN);
				if (check_win(field, AI))
				{
					setval(field.map, y, x, AI);
					return 1;
				}
				setval(field.map, y, x, EMPTY);
			}
		}
	}
	return 0;
}

void ai_move(Field& field)
{
	if (ai_win_check(field)) return;
	if (human_win_check(field)) return;
	int x, y;
	do
	{
		x = rand() % field.szX;
		y = rand() % field.szY;
	} while (!isempty(field, x, y));
	setval(field.map, y, x, AI);
}

int check_draw(Field& field, PLAYER player)
{
	for (int y = 0; y < field.szY; ++y)
	{
		for (int x = 0; x < field.szX; ++x)
		{
			if (isempty(field, x, y)) return 0;
		}
	}
	return 1;
}

int check_game(Field& field, PLAYER p, const char* win_string)
{
	if (check_win(field, p))
	{
		cout << win_string;
		return 1;
	}
	if (check_draw(field, p))
	{
		cout << "Draw" << endl;
		return 1;
	}
	return 0;
}

void tictactoe()
{
	Field field;
	while (true)
	{
		select_field(field);
		init(field);
		print(field);
		while (true)
		{
			human_move(field);
			print(field);
			if (check_game(field, HUMAN, "You win!")) break;
			ai_move(field);
			print(field);
			if (check_game(field, AI, "I win!")) break;
		}
		cout << endl;
		char answer;
		cout << "Play again? y/n ";
		cin >> answer;
		if (answer != 'y') break;
	}
}

int main()
{
	srand(time_t(0));
	tictactoe();
}