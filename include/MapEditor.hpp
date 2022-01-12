#ifndef MAP_EDITOR_HPP
#define MAP_EDITOR_HPP

// SFML CORE
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


// Json CORE
#include <json/json.h>


// Stream headers
#include <fstream>
#include <iostream>

// Data structors
#include <map>
#include <vector>

// Math
#include <algorithm>

// Other headers
#include "TextureSelector.hpp"
#include "KeybindHandler.hpp"
#include "SparkGUI.hpp"
#include "Map.hpp"

class MapEditor
{
private:
	// Type alias
	using RectShape = sf::RectangleShape;

	// Window
	sf::RenderWindow* m_window;

	// Map grid related
	sf::Vector2f m_mapGrid;
	sf::Vector2f m_gridSize;

	// View related
	sf::View m_guiView;
	sf::View m_mapView;
	sf::Vector2f m_defViewSize;
	std::vector<float> m_zoomFactors;
	float m_zoomScale;

	// Resources
	sp::KeybindHandler m_keybinds;
	std::map<std::string, sf::Texture> m_textures;
	sf::Font m_font;

	// Our map
	sp::Map m_map;

	// Our texture selector
	TextureSelector* m_texselector; 

	// Selection
	RectShape m_mapSelection;
	RectShape m_cursorSelection;
	std::vector<sp::Tile*> m_selectedTiles;

	// GUI
	sf::Text   m_zoomText;
	sf::Text   m_tileText;
	
	// Event States
	bool m_panningState, m_selectionState;
	
	// Initializer Functions
	void initView();
	void initTextures();
	void initKeybinds();
	void initFont();
	void initMap();
	void initGUI();

	// Event Functions
	void resizeEvent(sf::Event& events);
	void panningEvent(sf::Event& events);
	void zoomingEvent(sf::Event& events);
	void selectionEvent(sf::Event& events);

	// Update Functions
	void updateInput(const float& dt);
	void updateGUI();

public:
	// Constructors/Destructors
	MapEditor(sf::RenderWindow*   window, 
	          const sf::Vector2f& map_grid, 
	          const sf::Vector2f& grid_size, 
	          const sf::Color&    grid_color, 
	          const sf::Color&    outline_color);
	// TODO: Constructor for map files
	~MapEditor();

	// Handle SFML Events
	void handleEvents(sf::Event& events);

	// Update
	void update(const float& dt);

	// Render
	void render();
};

#endif // MAP_EDITOR_HPP
