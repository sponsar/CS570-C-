#include<iostream>
#include<vector>

using namespace std;

bool setpos(vector<char>& pos, int r, int c, int count)
{
	if (r > 3 || r < 1 || c > 3 || c < 1)
	{
		cout << "error! try again!" << endl;
		return 0;
	}
	int a = r * 3 - 4 + c;
	if (pos[a] != ' ')
	{
		cout << "error! try again!" << endl;
		return 0;
	}
	pos[a] = count % 2 == 1 ? 'X' : 'O';
	return 1;
}
void display(vector<char> pos)
{
	cout << "   " << 1 << "   " << 2 << "   " << 3 << endl;
	cout << 1 << "  " << pos[0] << " " << '|' << " " << pos[1] << " " << '|' << " " << pos[2] << endl;
	cout << "  ---+---+---" << endl;
	cout << 2 << "  " << pos[3] << " " << '|' << " " << pos[4] << " " << '|' << " " << pos[5] << endl;
	cout << "  ---+---+---" << endl;
	cout << 3 << "  " << pos[6] << " " << '|' << " " << pos[7] << " " << '|' << " " << pos[8] << endl;
}

int gameover(vector<char> pos)//1 is x, 2 is o, 3 = no one wins
{
	for (int i = 0; i < 7; i += 3)//row
	{
		if (pos[i] == pos[i + 1] && pos[i + 1] == pos[i + 2] && pos[i] == pos[i + 2] && pos[i] == 'X')
			return 1;
		if (pos[i] == pos[i + 1] && pos[i + 1] == pos[i + 2] && pos[i] == pos[i + 2] && pos[i] == 'O')
			return 2;
	}
	for (int j = 0; j < 3; j++)//col
	{
		if (pos[j] == pos[j + 3] && pos[j] == pos[j + 6] && pos[j + 3] == pos[j + 6] && pos[j] == 'X')
			return 1;
		if (pos[j] == pos[j + 3] && pos[j] == pos[j + 6] && pos[j + 3] == pos[j + 6] && pos[j] == 'O')
			return 2;
	}
	if (((pos[0] == pos[4] && pos[4] == pos[8] && pos[0] == pos[8]) || (pos[2] == pos[4] && pos[2] == pos[6] && pos[4] == pos[6])) && pos[4] != 32)//diagonal
	{
		if (pos[4] == 'X')
			return 1;
		else
			return 2;
	}
	for (int i = 0; i < 9; i++)
		if (pos[i] == 32)
			return 0;//still good
	return 3;//full
	
}

int main()
{
	char a;
	do
	{
		vector<char> pos{ 32, 32, 32, 32, 32, 32, 32, 32, 32 };//9 blank space
		display(pos);//format

		int row, col;
		int count = 1;//count++

		while (gameover(pos) == 0)
		{
			do
			{
				cout << "input row number: ";
				cin >> row;
				cout << "input col number: ";
				cin >> col;
			} while (!setpos(pos, row, col, count));
			count++;
			display(pos);
		}
		switch (gameover(pos))
		{
		case 1:cout << "\n" << "congratulation to player X!" << "\n\n"; break;
		case 2:cout << "\n" << "congratulation to player O!" << "\n\n"; break;
		case 3:cout << "\n" << "draw!" << "\n\n"; break;
		}
		cout << "Do you want to play again?(Y/N): ";
		cin >> a;
	} while (a == 'y' || a == 'Y');
	cout << "Bye!~" << "\n\n";
}
