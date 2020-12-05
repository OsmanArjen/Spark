#ifndef MAP_HPP
#define MAP_HPP
#include <iostream>
// SFML CORE
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
// Data structures
#include <vector>
// Math
#include <algorithm> 
// User defined
#include "MapLayer.hpp"
class Map
{
private:
	sf::FloatRect rect;
	sf::Vector2f gridSize;
	sf::Color gridColor, outlineColor;
	std::vector<sp::MapLayer*> layers;
	sp::MapLayer* currLayer;
	sf::VertexArray outlines, gridlines;
public:
	// Initializer Functions
	void initOutlines();
	void initGridlines();
	// Constructor/Destructor
	Map() = default;
	Map(const sf::FloatRect& map_rect, 
		const sf::Vector2f& grid_size, 
		const sf::Color& grid_color, 
		const sf::Color& outline_color);
	~Map();

	// Getter/Setter methods
	sp::MapLayer* getCurrentLayer();
	sp::MapLayer* getLayer(std::size_t index);
	void newLayer();
	void setCurrentLayer(std::size_t index);
	void nextLayer();
	void prevLayer();
	// Functions
	void sortLayersByQueue();
	void updateLayersQueue();
	void update();
	void render(sf::RenderTarget& surface);
};

#endif // MAP_HPP
