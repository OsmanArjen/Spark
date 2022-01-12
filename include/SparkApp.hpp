#ifndef SPARK_APP_HPP
#define SPARK_APP_HPP

#include "MapEditor.hpp"

class SparkApp
{

private:
	// Variables
	sf::RenderWindow* m_window;
	sf::Event m_sfEvents;
	sf::Clock m_clock;
	sf::Image m_icon;
	MapEditor* m_mapEditor;

	// GFX Settings
	Json::Value m_gfxConfig;
	sf::ContextSettings m_gfxContext;
	
	// Style Settings
	Json::Value m_styleConfig;

	// DeltaTime
	float m_deltatime;

	// Initialization
	void initStyles();
	void initIcon();
	void initWindow();
	void initMapEditor();
public:
	// Constructors/Destructors
	SparkApp();
	~SparkApp();

	// Main loop functions
	/* ------------------- */
	
	// Update
	void update();

	// Render
	void render();

	// Core
	void run();

};
#endif // SPARK_APP_HPP
