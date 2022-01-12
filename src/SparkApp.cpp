#include "SparkApp.hpp"

// Initializer Functions
void SparkApp::initStyles()
{

}

void SparkApp::initIcon()
{
   m_icon.loadFromFile("res/icon.png");
}

void SparkApp::initWindow()
{
	// Read config
	std::ifstream file("config/graphics.json", std::ifstream::binary);

	Json::CharReaderBuilder builder;
	std::string errs;

	if ( !Json::parseFromStream(builder, file, &m_gfxConfig, &errs) ) // Error while parsing JSON file
	{
		std::cerr << errs << std::endl; 
	}
	
	file.close();
	
	// Process the config
	m_gfxContext.antialiasingLevel = m_gfxConfig["antialiasing_level"].asInt();

	if (m_gfxConfig["fullscreen"].asBool())
		m_window = new sf::RenderWindow(
			sf::VideoMode(m_gfxConfig["width"].asInt(),m_gfxConfig["height"].asInt()), 
			"- Spark", // Title
			sf::Style::Fullscreen,
			m_gfxContext);
			
	else
		m_window = new sf::RenderWindow(
			sf::VideoMode(m_gfxConfig["width"].asInt(),m_gfxConfig["height"].asInt()), 
			"- Spark", // Title
			sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close,
			m_gfxContext);

	m_window->setFramerateLimit(m_gfxConfig["framerate_limit"].asInt());
	m_window->setVerticalSyncEnabled(m_gfxConfig["vertical_sync"].asBool());
	m_window->setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
}

void SparkApp::initMapEditor()
{
	m_mapEditor = new MapEditor(m_window, {100, 40}, {32, 32}, sf::Color(169,169,169), sf::Color::White);
}

// Constructors/Destructors
SparkApp::SparkApp()
{
	// Initilazer functions
	initStyles();
	initIcon();
	initWindow();
	initMapEditor();
}

SparkApp::~SparkApp()
{

	delete m_window;
	delete m_mapEditor;
}

// Main loop functions
void SparkApp::update()
{
	// Update deltatime
	//------------------
	m_deltatime = m_clock.restart().asSeconds();

	// Update events
	//------------------
	while (m_window->pollEvent(m_sfEvents))
	{
		if (m_sfEvents.type == sf::Event::Closed)
			m_window->close();

		m_mapEditor->handleEvents(m_sfEvents);
		
	}

	// Update map editor
	//------------------
	if (m_window->hasFocus())
	{
		m_mapEditor->update(m_deltatime);	
	}

}

void SparkApp::render()
{
	m_window->clear(sf::Color(51,51,51));

	// Render map editor
	m_mapEditor->render();

	m_window->display();
}

void SparkApp::run()
{
	while(m_window->isOpen())
	{
		update();
		render();
	}
}

