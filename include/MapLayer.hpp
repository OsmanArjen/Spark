#ifndef MAP_LAYER_HPP
#define MAP_LAYER_HPP
#include <vector>
#include "Tile.hpp"

namespace sp
{
class MapLayer
{
private:
	// Variables
	sf::Vector2f  m_mapGrid;
	sf::Vector2f  m_gridSize;
	std::vector<std::vector<Tile>> m_tileRects;
	bool m_visible;

	// For organize layer group
	MapLayer* m_baseLayer;
	int m_queueIndex;
	MapLayer* m_subLayer;

	// Initializer Function for tileRects
	void initLayerTileRects();
public:
	// TileRects type name
	using TileRects_t = std::vector<std::vector<Tile>>;
	// Constructor
	MapLayer(const sf::Vector2f& map_grid, 
			 const sf::Vector2f& grid_size,
			 int queue_indx,
			 MapLayer* base_layer = nullptr,
			 MapLayer* sub_layer  = nullptr);
	
	// Functions
	const int& 	 getQueueIndex()  const;
	const bool&  getVisible() const;
	TileRects_t& getTileRects();
	MapLayer* getBaseLayer();
	MapLayer* getSubLayer();
	void setQueueIndex(int index);
	void setVisible(bool visible);
	void setBaseLayer(MapLayer* base_layer);
	void setSubLayer(MapLayer* sub_layer);
	
	//-Render
	void render(sf::RenderTarget* surface);
};
} // sp
#endif // MAP_LAYER_HPP
