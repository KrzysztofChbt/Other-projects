/*
Copyright 2017 Krzysztof Chobot

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include "stdafx.h"
#include "jpgToASCII.h"


using namespace std;

void jpgToASCII::SetDimensions()
{
	y = image.getSize().y;
	x = image.getSize().x;
}
char jpgToASCII::ConvertToASCII(const int color)
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


void jpgToASCII::Resize(int maxX, int maxY, const int receivedX, const int receivedY)
{
	auto divideBy{ 4 };
	auto result{ 0 };
	const auto startY = maxY; // needed for deleting

	int **grid = new int*[maxY];
	for (int i = 0; i < maxY; i++)
	{
		grid[i] = new int[maxX];
	}

	for (auto y = 0; y < maxY; y++)
	{
		for (auto x = 0; x < maxX; x++) // dimensions of image
		{
			grid[y][x] = 0;
		}
	}

	for (auto y = 0; y < maxY; y++)
	{
		for (auto x = 0; x < maxX; x++) // dimensions of image
		{
			grid[y][x] = image.getPixel(x, y).r;
		}
	}


	while (maxX >= receivedX || maxY >= receivedY)
	{
		for (auto y = 0; y<maxY; y += 2)
		{
			for (auto x = 0; x<maxX; x += 2) // dimensions of image
			{
				for (auto i = y; i <= y + 1; i++)
				{
					for (auto j = x; j <= x + 1; j++) //getting 2x2 boxes of pixels
					{
						if (j < maxX && i < maxY)
						{
							result += grid[y][x];

						}
						else { divideBy--; }
					}
				}
				if (divideBy != 0) { result /= divideBy; }
				grid[y / 2][x / 2] = result;
				result = 0;
			}
		}

		//cout << "again.";
		maxX /= 2;
		maxY /= 2;
	}


	sf::Image newImage;
	newImage.create(maxX, maxY, sf::Color(0, 0, 0));

	for (auto y = 0; y < maxY; y++)
	{
		for (auto x = 0; x < maxX; x++) // dimensions of image
		{
			newImage.setPixel(x, y, sf::Color(grid[y][x], grid[y][x], grid[y][x]));
		}
	}

	newImage.saveToFile("tmp.jpg");


	for (auto i = 0; i<startY; i++)
	{
		delete[] grid[i];
	}
	delete[] grid;
}


int jpgToASCII::LowerResolution(int x, int y)
{
	//const auto masc{ 1 };
	auto divideBy{ 9 };

	auto result{ 0 };

	for (auto i = y - 1; i <= y + 1; i++)
	{
		for (auto j = x - 1; j <= x + 1; j++)
		{
			if (j >= 0 && j < this->x-1 && i >= 0 && i < this->y-1)
			{
				result += static_cast<int>(image.getPixel(j, i).r);// *masc (in low pass filter it is always 1, so we skip it)
			}
			else { divideBy--; }
		}
	}

	result /= divideBy;
	return result;
}

int jpgToASCII::MedianResolution(int x, int y)
{
	auto result{ 0 };
	vector<int> toSort;
	for (auto i = y - 1; i <= y + 1; i++)
	{
		for (auto j = x - 1; j <= x + 1; j++)
		{
			if (j >= 0 && j < this->x && i >= 0 && i < this->y)
			{
				toSort.push_back(static_cast<int>(image.getPixel(j, i).r));
			}
		}
	}

	sort(toSort.begin(), toSort.end());
	auto vsize{ toSort.size() };
	result = toSort[vsize / 2 + (vsize % 2 != 0 && vsize > 1)];

	return result;
}

jpgToASCII::jpgToASCII(std::string file)
{
	while (true) {
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
	SetDimensions();
}

jpgToASCII::~jpgToASCII()
{
}

void jpgToASCII::ToBlackAndWhite()
{
	auto mixedColours = 0;
	for (auto i = 0; i < y; i++)
	{
		for (auto j = 0; j < x; j++)
		{
			mixedColours = (image.getPixel(j, i).r + image.getPixel(j, i).g + image.getPixel(j, i).b) / 3;
			image.setPixel(j, i, sf::Color(mixedColours, mixedColours, mixedColours));
		}
	}
}


void jpgToASCII::Resize() {
	cout << "do you want to resize your image (highly recommended for bigger ones)\n"
		"Y/N\n";
	cin >> selection;
	if (selection == 'Y' || selection == 'y')
	{
		cout << "do you want to customize the resize function?\n"
			"Y/N\n";
		cin >> selection;
		if (selection == 'Y' || selection == 'y')
		{
			auto receivedX = 0, receivedY = 0;

			while (true)
			{
				cout << "Type max X dimension\n";
				if (cin >> receivedX) {
					break;
				}
				else {
					cout << "Please enter a valid X dimension" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			}
			while (true)
			{
				cout << "Type max Y dimension\n";
				if (cin >> receivedY) {
					break;
				}
				else {
					cout << "Please enter a valid Y dimension" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}

			}
			Resize(x, y, receivedX, receivedY);
		}
		else { Resize(x, y, 200, 200); }

		image.loadFromFile("tmp.jpg");
		SetDimensions();

		remove("tmp.jpg");
		cout << "your image has ben successfully resized to: " << x << "x" << y << "\n\n";
	}
}

void jpgToASCII::Filter() {
	sf::Image newimage(image);
	int mixedColours;

	cout << "Which filter do you want to use? (Be aware that might take a while)\n"
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
				mixedColours = LowerResolution(j, i);
				newimage.setPixel(j, i, sf::Color(mixedColours, mixedColours, mixedColours));
			}
		}
		image = newimage;
		cout << "filtering ended successfully\n";
		break;
	case 'M':
	case 'm':
		cout << "you have choosed median filter\ncomputing data...\n";
		for (auto i = 0; i < y; i++)
		{
			for (auto j = 0; j < x; j++)
			{
				mixedColours = MedianResolution(j, i);
				newimage.setPixel(j, i, sf::Color(mixedColours, mixedColours, mixedColours));
			}
		}
		image = newimage;
		cout << "filtering ended successfully\n";
		break;
	case 'C':
	case 'c':break;
	default: cout << "you have not choosed anything! filtering skipped."; break;
	}
}

void jpgToASCII::Save()
{
	string resultPath;
	string resultASCII;

	for (auto i = 0; i < y; i++)
	{
		for (auto j = 0; j < x; j++)
		{
			resultASCII += ConvertToASCII(static_cast<int>(image.getPixel(j, i).r));
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
}

sf::Image jpgToASCII::GetImage() { return image; }
