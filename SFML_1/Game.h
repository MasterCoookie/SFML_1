#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


class Game {
private:
	//window variables
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode VM;

	//gameobjects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//mousepos
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;
	sf::Text uiText;

	//Game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawTimerMax;
	unsigned int maxEnemies;
	bool mouseHeld;


	void initVariables();
	void initWindow();
	void initEnemies();
	void initText();
public:
	Game();
	~Game();

	const bool running() const;
	const bool getEndGame() const;

	void SpawnEnemy();
	void UpdateEnemies();
	void UpdateText();
	void Update();
	void PollEvents();
	void UpdateMousePositions();
	void RenderEnemies(sf::RenderTarget& target);
	void RenderText(sf::RenderTarget& target);
	void Render();
};

