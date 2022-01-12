#ifndef MAP_ACTIONS_HPP
#define MAP_ACTIONS_HPP
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Map.hpp"
class MapActions
{
public:
	// Constructor
	MapActions(sf::RenderTarget* target, sp::Map* map, sf::View* mapView);

	// Action Functions
	void panningAction(sf::Event& events);
	void zoomingAction(sf::Event& events);
	void selectionAction(sf::Event& events);	
};
#endif