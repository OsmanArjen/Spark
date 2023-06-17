#ifndef MAP_LAYER_HPP
#define MAP_LAYER_HPP
#include <vector>
#include "Tile.hpp"

namespace sp
{
struct MapLayer
{
	using tile_rects_t = std::vector<std::vector<Tile>>;
	sf::Vector2f mapGrid;
	sf::Vector2f gridSize;
	tile_rects_t tileRects;
	bool visible;

	MapLayer* baseLayer;
	int queueIndex;
	MapLayer* subLayer;	
};

void renderMapLayer(sf::RenderTarget* surface, const MapLayer& layer);
MapLayer createMapLayer(const sf::Vector2f& map_grid, const sf::Vector2f& grid_size,int queue_indx, 
			MapLayer* base_layer = nullptr,MapLayer* sub_layer  = nullptr);

} // sp
#endif // MAP_LAYER_HPP
