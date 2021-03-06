#ifndef SPARK_APP_HPP
#define SPARK_APP_HPP

#include "EditorState.hpp"

class SparkApp
{

private:
	// Variables
	StateData m_stateData;
	sf::RenderWindow* m_window;
	sf::Event m_sfEvents;
	sf::Clock m_clock;
	sf::Image m_icon;
	std::stack<State*> m_states;

	std::map<std::string, int> m_suppKeys;

	// GFX Settings
	Json::Value m_gfxConfig;
	sf::ContextSettings m_gfxContext;
	
	// Style Settings
	Json::Value m_styleConfig;
	sf::Font m_mainFont;

	// DeltaTime
	float m_deltatime;

	// Initialization
	void initFonts();
	void initStyles();
	void initIcon();
	void initWindow();
	void initKeys();
	void initStateData();
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
