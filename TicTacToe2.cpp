#include<iostream>
#include<vector>
#include<iomanip>
#include<fstream>
#include<string>
#include<ctime>
//#include<Windows.h>//Sleep(ms)

using namespace std;
class Check_input
{

public:
	void input_integer(istream& cin, int& integer)
	{
		string str;
		do
		{
			cin >> str;
			bool wrong = 0;
			for (unsigned int i = 0; i < str.length(); i++)
			{
				if (str[i] < 48 || str[i]>57)//0-9
					wrong = 1;
			}
			if (!wrong)
			{
				integer = stoi(str);//or atoi()?
				return;
			}
			else
			{
				cout << "error!please input integer:";
				cin.clear();
				cin.sync();//防止他输入多个数字
			}
		} while (true);
	}
	void input_char(istream& cin, char& ch)
	{

		string str;
		do
		{
			cin >> str;
			bool wrong = 0;
			if (str.length() == 1)
			{
				ch = str[0];
				return;
			}
			else
			{
				cout << "error!please input char:";
				cin.clear();
				cin.sync();//防止他输入字符串
			}
		} while (true);
	}
	void input_string(istream& cin, int& integer, bool& s)//save
	{
		string str;
		do{
		cin >> str;
		if (str == "save")
		{
			s = 1;
			return;
		}
		else //将string转换为integer
		{
			bool wrong = 0;
			for (unsigned int i = 0; i < str.length(); i++)
			{
				if (str[i] < 48 || str[i]>57)//0-9
					wrong = 1;
			}
			if (!wrong)
			{
				integer = stoi(str);//or atoi()?
				return;
			}
			else
			{
				cout << "Sorry! I don't understand, please input again:";
				cin.clear();
				cin.sync();
			}
		}
		} while (true);
	}
};
class TicTacToe{
private:
	Check_input tool;//check the illegal input
	int playerNum;
	int rows, cols;//多少行多少列
	int winSequence;
	bool square=0;//square rule
	int step = 0;//用来判断哪一个玩家下
	bool s = 0;//save
	double duration;//use second as unit
	vector<vector<char> > pos;//二维vector
	vector<char> player;//26 players MAX
public:
	TicTacToe()
	{
		player = { 'X', 'O', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
			'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'Y', 'Z' };
	}
	void gameStart()
	{
		cout << "Welcome to Xinyao's TicTacToe!" << endl
			<< "Please follow the instructions to setup the game." << endl;
			
		//判断有没有存档，如果有就load()

		ifstream infile("TicTacToe.txt");
		if (resume())
			if (infile)
				load(infile);
			else
			{
				cout << "Sorry, there isn't a saved game!" << endl;
				setGame();
			}
		else
			setGame();

		infile.close();
		cout << endl << "Now! Everything is set, let's enjoy the game!" << endl;
		runGame();

	}
	void setGame()
	{
		cout << "How many players are playing(2 to 26):";
		do
		{
			tool.input_integer(cin, playerNum);
			if (playerNum < 2 || playerNum>26)
				cout << "Sorry! Please input a proper number(2 to 26):";
			else
				break;
		} while (true);

		cout << "How large the board you want?(3 to 999)" << endl << "rows:";
		do
		{
			tool.input_integer(cin, rows);
			if (rows < 3 || rows>999)
				cout << "Sorry! Please input a proper number(3 to 999):";
			else
				break;
		} while (true);

		cout << "cols:";
		do
		{
			tool.input_integer(cin, cols);
			if (cols < 3 || cols>999)
				cout << "Sorry! Please input a proper number(3 to 999):";
			else
				break;
		} while (true);

		cout << "What win sequence count do you want:";
		do
		{
			tool.input_integer(cin, winSequence);
			if (winSequence < 3)
				cout << "Please input a number bigger than 2:";
			else if (winSequence > rows&&winSequence > cols)
				cout << "Sorry!The number you input is bigger than the edge." << endl << "Please input a proper number:";
			else if (winSequence > (rows*cols / playerNum + 1 - ((rows*cols%playerNum) == 0)))
			{
				cout << "Sorry!Too many players! You have to reset the game." << endl;
				setGame();
				return;
			}
			else
				break;
		} while (true);

		do
		{
			cout << "Do you want to add the square rule, in which the square must be filled in(Y/N):";
			char a;
			tool.input_char(cin, a);
			if (a == 'y' || a == 'Y')
			{
				square = 1;
				break;
			}
			else if (a == 'n' || a == 'N')
			{
				square = 0;
				break;
			}
			else
				cout << "Sorry! I don't understand!" << endl;
		} while (true);


		posResize();//clear all the position

		ofstream outfile("TicTacToe.txt");
		saveSetup(outfile);
		outfile.close();
	}
	void runGame()
	{
			display();//format
			//int r, c;
			while (gameover() == 0 && !s)//check
			{
				setpos();//在这里每一步都存起来
				step++;
				display();
			}//step--;
			if (s)//save
			{
				cout << "The game has been saved, see you next time!" << endl;
				return;
			}
			if (gameover() == -1)
				cout << "\n" << "      draw!!!!" << "\n\n";
			else
				cout << "\n" << "congratulation to player " << player[gameover() - 1] << "!\n\n";

			remove("TicTacToe.txt");
			//这里再问问玩家要不要保存？
	}//这里就是整个程序的结束
	void setpos()//每一步都存下来
	{
		int r, c;
		clock_t start, finish;
		cout << "Player " << player[step % playerNum] << "'s turn. You can input \"save\" anytime you want." << endl;
		cout << "row: ";

		start = clock();

		tool.input_string(cin, r, s);
		if (s)
			return;
		cout << "col: ";
		tool.input_string(cin, c, s);

		finish = clock();

		if (s)
			return;
		if (r > rows || r < 1 || c > cols || c < 1)
		{
			cout << endl << "error! try again!" << endl << endl;
			display();
			setpos();
			return;
		}
		else if (pos[r - 1][c - 1] != ' ')
		{
			cout << endl << "error! try again!" << endl << endl;
			display();
			setpos();
			return;
		}
		else
			pos[r - 1][c - 1] = player[step % playerNum];

		duration = (double)(finish - start) / CLOCKS_PER_SEC;

		ofstream outfile("TicTacToe.txt", ios::app);
		outfile << r - 1 << " " << c - 1 << " " << duration << endl;
		outfile.close();
	}
	void display()
	{
		int interval;//通过多少有行来判断空多少格，比如150行，三位数，需要空三格
		if (rows < 10)
			interval = 1;
		else if (rows < 100)
			interval = 2;
		else
			interval = 3;

		cout << setw(interval) << " ";    //the first line
		for (int j = 1; j <= cols; j++)
		{
			if (j < 10)
				cout << " " << j << "  ";
			else if (j < 100)
				cout << " " << j << " ";
			else
				cout << j << " ";
		}
		cout << endl;

		for (int i = 0; i < rows; i++)//rest lines
		{
			cout << setw(interval) << i + 1;
			for (int j = 0; j < cols; j++)
			{
				cout << " " << pos[i][j] << " ";
				if (j < cols - 1)
					cout << "|";
				else
					cout << endl;
			}
			if (i < rows - 1)
			{
				cout << setw(interval) << " ";
				for (int j = 0; j < cols; j++)
				{
					if (j < cols - 1)
						cout << "---+";
					else
						cout << "---" << endl;
				}
			}
		}
	}
	int gameover()//0 continue, else return winner number, consider draw(-1)
	{             
		//分四种情况讨论：横的，竖的，斜的，矩形，还有打平
		for (int w = 0; w < playerNum; w++)//遍历每个玩家
		{
			if (winSequence <= cols)//check rows
			{
				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < cols - winSequence + 1; j++)//一行之内
					{
						int num = 0;
						for (int p = j; p < j + winSequence; p++)
						{
							if (pos[i][p] != player[w])
								break;
							else
								num++;
						}
						if (num == winSequence)
							return w + 1;
					}
				}
			}
			if (winSequence <= rows)//check cols
			{
				for (int j = 0; j < cols; j++)
				{
					for (int i = 0; i < rows - winSequence + 1; i++)
					{
						int num = 0;
						for (int p = i; p < i + winSequence; p++)
						{
							if (pos[p][j] != player[w])
								break;
							else
								num++;
						}
						if (num == winSequence)
							return w + 1;
					}
				}
			}
			if (winSequence <= cols && winSequence <= rows)//check diagonal
			{
				for (int i = 0; i < rows - winSequence + 1; i++)//check "\"
				{
					for (int j = 0; j < cols - winSequence + 1; j++)
					{
						int num = 0;
						for (int p = 0; p <  winSequence; p++)
						{
							if (pos[i+p][j+p] != player[w])
								break;
							else
								num++;
						}
						if (num == winSequence)
							return w + 1;
					}
				}
				for (int i = 0; i < rows - winSequence + 1; i++)//check "/"
				{
					for (int j = winSequence - 1; j < cols; j++)
					{
						int num = 0;
						for (int p = 0; p < winSequence; p++)
						{
							if (pos[i + p][j - p] != player[w])
								break;
							else
								num++;
						}
						if (num == winSequence)
							return w + 1;
					}
				}
			}
			if (square)//check square
			{
				if (winSequence - 1 <= cols && winSequence - 1 <= rows)
				{
					for (int i = 0; i < rows - winSequence + 2; i++)
					{
						for (int j = 0; j < cols - winSequence + 2; j++)
						{
							int num = 0;
							for (int p = i; p < i + winSequence - 1; p++)
							{
								for (int q = j; q < j + winSequence - 1; q++)
								{
									if (pos[p][q] != player[w])
										goto outside;
									else
										num++;
								}
							}
							if (num == (winSequence - 1)*(winSequence - 1))
								return w + 1;
						outside:;
						}
					}
				}
			}
			
		}
		//没有人赢，要不就打平要不就继续
		for (int i = 0; i < rows; i++)//check draw
		{
			for (int j = 0; j < cols; j++)
			{
				if (pos[i][j] == 32)
				{
					return 0;//继续游戏
				}
			}
		}
		return -1;//打平
	}
	void saveSetup(ofstream& outfile)
	{
		outfile << playerNum << " " << rows << " " << cols << " " << winSequence << " " << square << endl;
	}
	void load(ifstream& infile)
	{
		infile >> playerNum >> rows >> cols >> winSequence >> square;
		int i, j;
		double dur;//duration of each step
		int count = 0;
		while (infile >> ws && !infile.eof())
		{
			infile >> i >> j >> dur;
			count++;
		}
		infile.close();
		infile.open("TicTacToe.txt");
		infile >> playerNum >> rows >> cols >> winSequence >> square;
		posResize();//clear all the position
		for (int p = 0; p < count; p++)//load display
		{
			infile >> i >> j >> dur;
			pos[i][j] = player[p% playerNum];
			cout << endl << "Player " << player[p% playerNum] << "'s step, it takes " << dur << " seconds, please wait......" << endl;
			breakTime(dur);
			display();
		}
		step = count;
	}
	bool resume()
	{
		char a;
		do
		{
			cout << "Would you like to resume a saved game?(Y/N):";
			tool.input_char(cin, a);
			if (a == 'y' || a == 'Y')
				return 1;
			else if (a == 'n' || a == 'N')
				return 0;
			else
				cout << "Sorry! I don't understand, please input again(Y/N):";
		} while (true);
	}
	void posResize()
	{
		pos.resize(rows, vector<char>(cols));
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				pos[i][j] = 32;
	}
	void breakTime(double dur)
	{
		clock_t start;
		start = clock();
		while ((double)(clock() - start) / CLOCKS_PER_SEC < dur);
	}
};


int main()
{
	TicTacToe tic;
	tic.gameStart();
	return 0;
}
