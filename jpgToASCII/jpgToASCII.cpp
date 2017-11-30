// jpgToASCII.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <conio.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

int LowerResolution(sf::Image image,int x, int y, int maxX, int maxY) // low-pass filter
{ 
	auto divideBy{ 9 };
	const auto masc{ 1 };
	auto result{ 0 };

	for (int i = y - 1; i <= y + 1; i++)
	{
		for (int j = x - 1; j <= x + 1; j++)
		{
			if (j >= 0 && j < maxX && i >= 0 && i < maxY)
			{
				result += static_cast<int>(image.getPixel(j, i).r)*masc;
			}
			else { divideBy--; }
		}
	}

	result /= divideBy;
	return result;
}

int MedianResolution(sf::Image image, int x, int y, int maxX, int maxY) // median filter
{
	auto result{ 0 };
	vector<int> toSort;
	for (int i=y-1;i<=y+1;i++)
	{
		for (int j = x - 1; j <= x + 1; j++)
		{
			if (j >= 0 && j < maxX && i >= 0 && i < maxY)
			{
				toSort.push_back(static_cast<int>(image.getPixel(j, i).r));
			}
		}
	}

	sort(toSort.begin(),toSort.end());
	auto vsize{ toSort.size() };
	result = toSort[vsize / 2 + (vsize % 2 != 0 && vsize > 1)];

	return result;
}

char ConvertToASCII(const int color)
{
	if (color < 25)
	{
		return '@';
	}
	else if (color < 50)
	{
		return '#';
	}
	else if (color < 75)
	{
		return '$';
	}
	else if (color < 100)
	{
		return '%';
	}
	else if (color < 125)
	{
		return '=';
	}
	else if (color < 150)
	{
		return '+';
	}
	else if (color < 175)
	{
		return '|';
	}
	else if (color < 200)
	{
		return ':';
	}
	else if (color < 225)
	{
		return '.';
	}
	else
	{
		return ' ';
	}

}
	

int main()
{
	
#pragma region //inicjalization

	string file;
	cout << "enter path to the file (C:\\photo\\photo.jpg)\n";
	cin >> file;

	sf::Image image;
	
	while (true){
		if (image.loadFromFile(file))
		{
			cout << "correctly loaded file\n"; break;
		}
		else
		{
			cout << "enter correct path to the file\n";
			cin >> file;
		}
	}

	const auto y = static_cast<int>(image.getSize().y); //static cast for disable warning C4018
	const auto x = static_cast<int>(image.getSize().x);

#pragma endregion

#pragma region //converting to black and white

	auto mixedColours = 0;
	for (auto i = 0; i < y; i++)
	{
		for (auto j = 0; j < x; j++)
		{
			mixedColours = (image.getPixel(j, i).r + image.getPixel(j, i).g + image.getPixel(j, i).b) / 3;
			image.setPixel(j, i, sf::Color (mixedColours, mixedColours, mixedColours));
		}
	}

#pragma endregion


	///img resize is under development
	//image.saveToFile("tmp.jpg");
	//cv::Mat srcImage = cv::imread("tmp.jpg", CV_LOAD_IMAGE_COLOR);
	//cv::Mat dstImage;
	//cv::resize(srcImage, dstImage, cv::Size(100,100), 0, 0);
	//cv::imwrite("tmp1.jpg", dstImage);
	//image.loadFromFile("tmp2.jpg");
	//remove("tmp.jpg");
	//remove("tmp2.jpg");


#pragma region //lowering quality
	sf::Image newimage(image);

	char selection{ NULL };
	cout << "Which filter do you want to use? \n"
	"L - low-pass filter\n"
	"M - median filter\n"
	"C - no filter (clean)\n";
	cin >> selection;

	switch (selection)
	{
	case 'L':
	case 'l':
		cout << "you have choosed low-pass filter\ncomputing data...\n";
		for (auto i = 0; i < y; i++)
		{
			for (auto j = 0; j < x; j++)
			{
				mixedColours = LowerResolution(image, j, i, x-1, y-1);
				newimage.setPixel(j, i, sf::Color(mixedColours, mixedColours, mixedColours));
			}
		}
		break;
	case 'M':
	case 'm':
	default:
		cout << "you have choosed median filter\ncomputing data...\n";
		for (auto i = 0; i < y; i++)
		{
			for (auto j = 0; j < x; j++)
			{
				mixedColours = MedianResolution(image, j, i, x, y);
				newimage.setPixel(j, i, sf::Color(mixedColours, mixedColours, mixedColours));
			}
		}
		break;
	case 'C': break;
	}
	cout << "filtering ended successfully\n";
#pragma endregion

#pragma region //save result
	string resultPath;
	string resultASCII;

	for (auto i = 0; i < y; i++)
	{
		for (auto j = 0; j < x; j++)
		{
			resultASCII += ConvertToASCII(static_cast<int>(newimage.getPixel(j, i).r));
		}
		resultASCII += '\n';
	}

	while (true) {
		cout << "input name for ASCII file\n";
		cin >> resultPath;
		resultPath += ".txt";
		ofstream outputFile(resultPath);
		if (outputFile) { outputFile << resultASCII; break; }
		else { cout << "wrong file name\n"; }
	}
	cout << "file with ASCII characters have been saved\n";
#pragma endregion


#pragma region //for debugging
	//sf::Texture texture;
	//texture.loadFromImage(newimage);
	//sf::Sprite sprite;
	//sprite.setTexture(texture);
	//sf::RenderWindow window({ 1024,640 },"DEBUG");
	//window.setFramerateLimit(30);
	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}
	//	window.clear();
	//	window.draw(sprite);
	//	window.display();
	//}
#pragma endregion

	cout << "Press Enter to exit";
	_getch();
    return 0;
}

