// FIX: layer sisteminde düzenlemeler veya hatalar olacak onları düzelt!
#include "MapLayer.hpp"

sp::MapLayer sp::createMapLayer(const sf::Vector2f& map_grid, const sf::Vector2f& grid_size, int queue_idx,
		   		MapLayer* base_layer, MapLayer* sub_layer)
{
	sp::MapLayer layer;
	layer.mapGrid  = map_grid;
	layer.gridSize = grid_size;
	layer.visible  = true;
	layer.baseLayer  = base_layer;
	layer.queueIndex = queue_idx; 
	layer.subLayer   = sub_layer;
	for (int y = 0; y < layer.mapGrid.y; y++)
	{
		layer.tileRects.push_back(std::vector<Tile>());
		for (int x = 0; x < layer.mapGrid.x; x++)
		{
			std::vector<Tile>& TileRow{layer.tileRects.back()};
			TileRow.push_back({ {x * layer.gridSize.x, y * layer.gridSize.y}, layer.gridSize, {x,y} });
		}
	}
}


void sp::renderMapLayer(sf::RenderTarget* surface, const sp::MapLayer& layer)
{
	if(layer.visible)
	{	
		for(std::vector<Tile>& tilerow : layer.tileRects)
		{	
			for(Tile& tile : tilerow)
			{
				tile.render(surface);
			}
		}
	}
}
