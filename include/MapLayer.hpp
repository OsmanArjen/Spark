#ifndef MAP_LAYER_HPP
#define MAP_LAYER_HPP
#include <vector>
#include "Tile.hpp"

namespace sp
{
struct MapLayer
{

	// Variables
	sf::Vector2f  mapGrid;
	sf::Vector2f  gridSize;
	std::vector<std::vector<Tile>> tileRects;
	bool visible;

	// For organize layer group
	MapLayer* baseLayer;
	int queueIndex;
	MapLayer* subLayer;

	// Initializer Function for tileRects
public:
	// TileRects type name
	using TileRects_t = std::vector<std::vector<Tile>>;
	// Constructor

	
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
	
};

void renderMapLayer(sf::RenderTarget* surface, const MapLayer& layer);
MapLayer createMapLayer(const sf::Vector2f& map_grid, const sf::Vector2f& grid_size,int queue_indx, 
			MapLayer* base_layer = nullptr,MapLayer* sub_layer  = nullptr);

} // sp
#endif // MAP_LAYER_HPP
