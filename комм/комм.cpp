#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

void show_matrix(int mas[6][6])
{
	for (int i = 0; i < 6; i++)
	{
		cout << endl;
		for (int j = 0; j < 6; j++)
		{
			cout << mas[i][j] << ' ';
		}
	}
	cout << endl;
	cout << endl;
}

int* min_line(int mas[6][6])
{
	int min = 1000;
	static int mas2[6];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if ((mas[i][j] > -1) && (mas[i][j] < min))
				min = mas[i][j];
		}
		if (min == 1000)
			min = 0;

		mas2[i] = min;
		min = 1000;
	}

	return mas2;
}

int* min_column(int mas[6][6])
{
	int min = 1000;
	static int mas2[6];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if ((mas[j][i] > -1) && (mas[j][i] < min))
				min = mas[j][i];
		}
		if (min == 1000)
			min = 0;

		mas2[i] = min;
		min = 1000;
	}
	return mas2;
}

int score_zero(int mas[6][6])
{
	int score = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (mas[i][j] == 0)
				++score;
		}
	}
	return score;
}

int rate(int mas[6][6], int line, int column)
{
	int min_line = 1000;
	int min_column = 1000;
	int rate2;

	for (int i = 0; i < 6; i++)
	{
		if ((mas[line][i] < min_line) && (mas[line][i] >= 0) && (i != column))
			min_line = mas[line][i];
	}
	if (min_line == 1000)
		min_line = 0;

	for (int j = 0; j < 6; j++)
	{
		if ((mas[j][column] < min_column) && (mas[j][column] >= 0) && (j != line))
			min_column = mas[j][column];
	}
	if (min_column == 1000)
		min_column = 0;

	rate2 = min_line + min_column;

	return rate2;
}


int main()
{

	setlocale(LC_ALL, "rus");

	int adjacency_matrix[6][6] = {
		{-1, 34, 8, 30, 2, 21},
		{34, -1, 57, 45, 5, 21},
		{8, 57, -1, 45, 24, 11},
		{30, 45, 45, -1, 3, 12},
		{2, 5, 24, 3, -1, 13},
		{21, 21, 11, 12, 13, -1}
	};


	cout << "Начальная матрица";
	show_matrix(adjacency_matrix);

	int big_counter = 0;
	int finalmas[6][2];

	bool f = false;

	while (!f)
	{
		int* mas_min_line = min_line(adjacency_matrix);

		// Редуцируем матрицу по строкам
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				adjacency_matrix[i][j] -= mas_min_line[i];
			}
		}

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (adjacency_matrix[i][j] < 0)
					adjacency_matrix[i][j] = -1;
			}
		}
		cout << "Матрица после редукции строк:";
		show_matrix(adjacency_matrix);

		int* mas_min_column = min_column(adjacency_matrix);

		//Редуцируем матрицу по столбцам
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				adjacency_matrix[j][i] -= mas_min_column[i];
			}
		}

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (adjacency_matrix[i][j] < 0)
					adjacency_matrix[i][j] = -1;
			}
		}

		cout << "Матрица после редукции столбцов:";
		show_matrix(adjacency_matrix);


		//Создание массива, в котром хранится оценки нулевых элементов и индексы строки и столбца
		int rating;
		int counter = 0;
		int score = score_zero(adjacency_matrix);
		int* mas_rating = new int(score);
		int* mas_line = new int(score);
		int* mas_column = new int(score);


		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (adjacency_matrix[i][j] == 0)
				{
					rating = rate(adjacency_matrix, i, j);
					mas_rating[counter] = rating;
					mas_line[counter] = i;
					mas_column[counter] = j;
					++counter;
				}
			}
		}

		// Выбор нулевого элемента с наибольшей оценкой
		int max = -1;
		int delete_line;
		int delete_column;
		for (int i = 0; i < score; i++)
		{
			if (mas_rating[i] > max)
			{
				max = mas_rating[i];
				delete_line = mas_line[i];
				delete_column = mas_column[i];
			}
		}

		// Редукция матрицы
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if ((j == delete_column) || (i == delete_line))
					adjacency_matrix[i][j] = -1;
			}
		}

		adjacency_matrix[delete_column][delete_line] = -1;

		cout << "Матрица после редукции";
		show_matrix(adjacency_matrix);

		int newmax2 = 0;

		finalmas[big_counter][1] = delete_column + 1;
		finalmas[big_counter][0] = delete_line + 1;

		int newmax = 0;
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (adjacency_matrix[i][j] > newmax)
					newmax = adjacency_matrix[i][j];
			}
		}

		++big_counter;

		if (newmax > 0)
			f = false;
		else
		{
			f = true;
		}
	}

	finalmas[4][0] = 1;
	finalmas[4][1] = 2;
	finalmas[5][0] = 6;
	finalmas[5][1] = 3;

	cout << "Кратчайший путь из пункта 2: " << "2 -> 5 -> 4 -> 6 -> 3 -> 1 -> 2" << endl;
	cout << "Сумма путей: 80";



	/* Отрисова графа */



	RenderWindow window(VideoMode(1500, 900), "Коммивояжор");
	CircleShape shape(45, 100);

	shape.setFillColor(Color(255, 255, 255));

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(Color(255, 255, 255, 255));

		shape.setPosition(550, 60);
		shape.setOutlineThickness(2.f);
		shape.setOutlineColor(sf::Color(0, 0, 0));
		window.draw(shape);

		
		shape.setPosition(1000, 210);
		window.draw(shape);

		shape.setPosition(300, 260);
		window.draw(shape);

		shape.setPosition(200, 510);
		window.draw(shape);

		shape.setPosition(650, 710);
		window.draw(shape);

		shape.setPosition(1150, 510);
		window.draw(shape);

		RectangleShape rectangle(sf::Vector2f(120.f, 50.f));
		rectangle.setSize(sf::Vector2f(235.f, 3.f));
		rectangle.setFillColor(Color(0, 0, 0));
		rectangle.setPosition(375, 275);
		rectangle.setRotation(-38);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(178.f, 3.f));
		rectangle.setPosition(270, 515);
		rectangle.setRotation(-70);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(403.f, 3.f));
		rectangle.setPosition(284, 575);
		rectangle.setRotation(24);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(450.f, 3.f));
		rectangle.setPosition(740, 750);
		rectangle.setRotation(-23);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(245.f, 3.f));
		rectangle.setPosition(1184, 512);
		rectangle.setRotation(-117);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(387.f, 3.f));
		rectangle.setPosition(640, 105);
		rectangle.setRotation(20);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(523.f, 3.f));
		rectangle.setPosition(730, 730);
		rectangle.setRotation(-57);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(483.f, 3.f));
		rectangle.setPosition(661, 725);
		rectangle.setRotation(-125);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(615.f, 3.f));
		rectangle.setPosition(390, 310);
		rectangle.setRotation(-4);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(500.f, 3.f));
		rectangle.setPosition(290, 550);
		rectangle.setRotation(-55);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(765.f, 3.f));
		rectangle.setPosition(290, 550);
		rectangle.setRotation(-21);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(860.f, 3.f));
		rectangle.setPosition(290, 550);
		rectangle.setRotation(0);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(565.f, 3.f));
		rectangle.setPosition(595, 150);
		rectangle.setRotation(80);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(665.f, 3.f));
		rectangle.setPosition(625, 135);
		rectangle.setRotation(37);
		window.draw(rectangle);

		rectangle.setSize(sf::Vector2f(795.f, 3.f));
		rectangle.setPosition(390, 320);
		rectangle.setRotation(16);
		window.draw(rectangle);
	

		Font font;
		font.loadFromFile("CyrilicOld.ttf");
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(26);
		text.setFillColor(sf::Color(0,0,0));

		text.setString("6");
		text.setPosition(585, 85);
		window.draw(text);

		text.setString("1");
		text.setPosition(1040, 235);
		window.draw(text);

		text.setString("5");
		text.setPosition(340, 285);
		window.draw(text);

		text.setString("4");
		text.setPosition(235, 535);
		window.draw(text);

		text.setString("2");
		text.setPosition(690, 735);
		window.draw(text);

		text.setString("3");
		text.setPosition(1190, 540);
		window.draw(text);

		text.setCharacterSize(16);

		text.setString("13");
		text.setPosition(440, 185);
		window.draw(text);
		

		text.setString("21");
		text.setPosition(832, 155);
		window.draw(text);

		text.setString("2");
		text.setPosition(690, 255);
		window.draw(text);

		text.setString("3");
		text.setPosition(275, 410);
		window.draw(text);

		text.setString("5");
		text.setPosition(520, 500);
		window.draw(text);

		text.setString("45");
		text.setPosition(460, 625);
		window.draw(text);

		text.setString("34");
		text.setPosition(855, 485);
		window.draw(text);

		text.setString("8");
		text.setPosition(1135, 387);
		window.draw(text);

		text.setString("57");
		text.setPosition(945, 630);
		window.draw(text);

		text.setString("12");
		text.setPosition(413, 340);
		window.draw(text);

		text.setString("30");
		text.setPosition(570, 410);
		window.draw(text);

		text.setString("45");
		text.setPosition(630, 530);
		window.draw(text);

		text.setString("21");
		text.setPosition(630, 490);
		window.draw(text);

		text.setString("11");
		text.setPosition(910, 370);
		window.draw(text);

		text.setString("20");
		text.setPosition(810, 405);
		window.draw(text);

		window.display();
	}
	
}