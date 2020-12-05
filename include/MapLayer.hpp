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
	sf::FloatRect mapRect;
	sf::Vector2f  gridSize;
	std::vector<Tile> tileRects;
	bool visible;

	// For organize layer group
	MapLayer* baseLayer;
	int queueIndex;
	MapLayer* subLayer;

	// Initializer Function for tileRects
	void initLayerTileRects();
public:
	// Constructor
	MapLayer(const sf::FloatRect& map_rect, 
			 const sf::Vector2f& grid_size,
			 int queue_indx,
			 MapLayer* base_layer,
			 MapLayer* sub_layer);
	
	// Functions
	const int& 	getQueueIndex()  const;
	const bool& getVisible() const;
	std::vector<Tile>& getTileRects();
	MapLayer* getBaseLayer();
	MapLayer* getSubLayer();
	void setQueueIndex(int index);
	void setVisible(bool visible);
	void setTileRects(const std::vector<sp::Tile>& tile_rects);
	void setBaseLayer(MapLayer* base_layer);
	void setSubLayer(MapLayer* sub_layer);

	void update(float mousePos);
	void render(sf::RenderTarget& surface);
};
} // sp
#endif // MAP_LAYER_HPP
