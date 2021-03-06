#ifndef MAP_HPP
#define MAP_HPP
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> 
#include <vector>
#include "MapLayer.hpp"

namespace sp
{
class Map
{
private:
	sf::Vector2f m_size;
	sf::Vector2f m_mapGrid;
	sf::Vector2f m_gridSize;
	sf::Color m_gridColor, m_outlineColor;
	std::vector<MapLayer*> m_layers;
	MapLayer* m_currLayer;
	sf::VertexArray m_outlines, m_gridlines;
	bool m_preview;
public:
	// Initializer Functions
	void initOutlines();
	void initGridlines();
	// Constructor/Destructor
	Map(const sf::Vector2f& map_grid, 
		const sf::Vector2f& grid_size,
		const sf::Color& grid_color, 
		const sf::Color& outline_color);
	~Map();

	// Getter/Setter methods
	MapLayer* getCurrentLayer();
	MapLayer* getLayer(std::size_t index);
	const bool& getPreview();
	void setPreview(const bool& preview);
	void newLayer();
	void setCurrentLayer(std::size_t index);
	void nextLayer();
	void prevLayer();
	// Functions
	void sortLayersByQueue();
	void updateLayersQueue();
	void render(sf::RenderTarget* surface);
};
} // sp
#endif // MAP_HPP
