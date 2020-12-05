#ifndef SPARK_APP_HPP
#define SPARK_APP_HPP

#include "EditorState.hpp"

class SparkApp
{

private:
	// Variables
	StateData stateData;
	sf::RenderWindow* window;
	sf::Event sfEvents;
	sf::Clock clock;

	std::stack<State*> states;

	std::map<std::string, int> suppKeys;

	// GFX Settings
	Json::Value gfxConfig;
	sf::ContextSettings gfxContext;
	
	// Style Settings
	Json::Value styleConfig;

	// DeltaTime
	float dt;

	// Initialization
	void initWindow();
	void initKeys();
	void initStateData();
	void initColorStyles();
	void initStates();



public:
	// Constructors/Destructors

	SparkApp();
	~SparkApp();

	

	// Main loop functions
	/* ------------------- */

	// Update
	void updateDeltaTime();
	void updateSFMLEvents();
	void update();

	// Render
	void render();

	// Core
	void run();

};
#endif // SPARK_APP_HPP
