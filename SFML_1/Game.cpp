#include "Game.h"

Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	if (!this->font.loadFromFile("./Fonts/PaletteMosaic-Regular.ttf")) {
		std::cout << "ERROR::GAME::CONSTRUCTOR: Failed to load font!" << std::endl;
	}
	this->initText();
}

Game::~Game() {
	delete this->window;
}

const bool Game::running() const { return this->window->isOpen(); }

void Game::Update() {
	this->PollEvents();

	if (!this->endGame) {
		this->UpdateMousePositions();

		this->UpdateText();

		this->UpdateEnemies();
	}

	if (this->health <= 0) this->endGame = true;

	//update mouse pos
	//relative to screen
	//std::cout << "Mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl;
	// relative to game window
	//std::cout << "Mouse pos: " << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << std::endl;
}

void Game::Render() {
	this->window->clear();
	//draw shit
	this->RenderEnemies(*this->window);
	this->RenderText(*this->window);
	//this->window->draw(this->enemy);

	this->window->display();

}

void Game::initVariables() {
	this->window = nullptr;

	//init game logic
	this->points = 0;
	this->enemySpawTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
	this->health = 10;
	this->endGame = false;
}

void Game::PollEvents() {
	while (this->window->pollEvent(this->ev)) {
		if (this->ev.type == sf::Event::Closed) {
			this->window->close();
		} else if (this->ev.type == sf::Event::KeyPressed) {
			if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
			}
		}
	}
}

void Game::UpdateMousePositions() {
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::initWindow() {
	this->VM.height = 600;
	this->VM.width = 800;
	this->window = new sf::RenderWindow(this->VM, "Gejm 1", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initEnemies() {
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setOutlineColor(sf::Color::Green);
	this->enemy.setOutlineThickness(2.f);
}

void Game::SpawnEnemy() {
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	//Randomize enemy type
	int type = rand() % 4;
	if (type == 0) {
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::Magenta);
	} else if(type == 1) {
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
	} else if(type == 2) {
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
	} else if(type == 3) {
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
	} else {
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Green);
	}

	

	this->enemies.push_back(enemy);
}

void Game::UpdateEnemies() {
	//timer update
	if (this->enemies.size() < this->maxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawTimerMax) {
			this->SpawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else {
			this->enemySpawnTimer += 1.f;
		}
	}
	
	// remove out of window enemies and decrease hp
	for (size_t i = 0; i < this->enemies.size(); i++) {
		this->enemies[i].move(0.f, 2.5f);
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health--;
			std::cout << "HP: " << this->health << std::endl;
		}
	}

	// killing stuff
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (this->mouseHeld == false) {
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					deleted = true;
					if (this->enemies[i].getFillColor() == sf::Color::Magenta) {
						this->points += 10;
					} else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
						this->points += 7;
					} else if (this->enemies[i].getFillColor() == sf::Color::Cyan) {
						this->points += 5;
					} else if (this->enemies[i].getFillColor() == sf::Color::Red) {
						this->points += 3;
					} else {
						this->points++;
					}

					this->enemies.erase(this->enemies.begin() + i);

					// gain points
					
					std::cout << "Points: " << this->points << std::endl;
				}
			}
		}
	} else {
		this->mouseHeld = false;
	}
}

void Game::RenderEnemies(sf::RenderTarget& target) {
	for (auto& enmy : enemies) {
		target.draw(enmy);
	}
}

const bool Game::getEndGame() const {
	return this->endGame;
}

void Game::initText() {
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::UpdateText() {
	std::stringstream ss;
	ss << "Points: " << this->points << "\n HP: " << this->health;
	this->uiText.setString(ss.str());
}

void Game::RenderText(sf::RenderTarget& target) {
	target.draw(uiText);
}