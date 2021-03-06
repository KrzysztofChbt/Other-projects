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

int main()
{
	
#pragma region //inicjalization
	string file;
	cout << "enter path to the file (C:\\photo\\photo.jpg)\n";
	cin >> file;
	jpgToASCII image(file);

#pragma endregion

	image.ToBlackAndWhite();
	image.Resize();
	image.Filter();
	image.Save();

	
#pragma region //for debugging
	////it show .jpg image in separate window after filtering etc...
	//sf::Texture texture;
	//texture.loadFromImage(image.GetImage());
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
