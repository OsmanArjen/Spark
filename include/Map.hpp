#ifndef MAP_HPP
#define MAP_HPP
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
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
	sf::VertexArray m_outlines, m_gridlines;
	std::vector<MapLayer*> m_layers;
	MapLayer* m_currLayer;
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
	MapLayer* getCurrentLayer() const;
	MapLayer* getLayer(std::size_t index) const;
	const bool& getPreview() const;
	void setPreview(const bool& preview);
	void setCurrentLayer(std::size_t index);
	// Layer functions
	void newLayer();
	void nextLayer();
	void prevLayer();
	void sortLayersByQueue();
	void updateLayersQueue();

	// Render
	void render(sf::RenderTarget* surface);
};
} // sp
#endif // MAP_HPP
