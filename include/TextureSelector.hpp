#ifndef TEXTURE_SELECTOR_HPP
#define TEXTURE_SELECTOR_HPP

// SFML CORE
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Map
#include "Map.hpp"

// STD
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

class TextureSelector
{
private:
	struct Tileset
	{
		int indexId;
		std::string  filename;
		sf::Texture* texSource;
		sp::Map  texmap;
		std::vector<std::vector<sf::Vector2i>> textileDoc; // Vec2i: tex grid position


	};

	// Type alias
	using RectShape = sf::RectangleShape;

	// Render related
	sf::RenderWindow* m_window;
	sf::RenderTexture m_renderLayer;

	// View related
	sf::View m_mapView;
	sf::Vector2f m_defViewSize;
	std::vector<float> m_zoomFactors;
	float m_zoomScale;

	// Texture related
	std::vector<Tileset>  m_tilesets; 
	Tileset* m_currTileset;

	// Selection
	RectShape m_mapSelection;
	RectShape m_cursorSelection;
	std::vector<sp::Tile*> m_selectedTexTiles;
	std::vector<std::pair<sf::Texture*, sf::Vector2i>> m_selectedTextures;

	// Event States
	bool m_panningState, m_selectionState;

	// Functions
	void initView();
	void initTexMap();
	void initRenderTexture();

	// Event Functions
	void panningEvent(sf::Event& events);
	void zoomingEvent(sf::Event& events);
	void selectionEvent(sf::Event& events);


public:
	// Constructor/Destructor
	TextureSelector(sf::RenderWindow*   window, 
	                const sf::Color&    grid_color, 
	                const sf::Color&    outline_color);
	~TextureSelector();

	// API
	void openTileset(const std::string& filename);
	void closeTileset(const std::string& filename);
	void setCurrentTileset(int index);

	// TODO: getSelectedTextures()

	// Handle SFML Events
	void handleEvents(sf::Event& events);

	// Update
	void update(const float& dt);

	// Render
	void render();
};

#endif // TEXTURE_SELECTOR_HPP
