#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "State.hpp"
#include "MapLayer.hpp"
#include "SparkGUI.hpp"
#include "Map.hpp"

class EditorState: public State
{
private:
	// Variables
	sf::FloatRect mapRect;
	sf::Vector2f gridSize;
	sf::Font font;
	sf::View view;
	std::vector<float> zoomFactors;
	float zoomScale;

	// Our map
	Map map; 

	// Selection
	sf::RectangleShape cursorSelection;
	std::vector<sp::Tile*> selectedTiles;

	// GUI
	sf::Text zoomText;

	// Initializer Functions
	void initView();
	void initTextures();
	void initKeybinds();
	void initFonts();
	void initMap();
	void initTexts();

	// Event Functions
	void panningEvent(sf::Event& events);
	void zoomingEvent(sf::Event& events);
	void selectionEvent(sf::Event& events);
public:
	// Constructors/Destructors
	EditorState(StateData* state_data, 
		const sf::FloatRect& map_rect, 
		const sf::Vector2f& grid_size, 
		const sf::Color& grid_color, 
		const sf::Color& outline_color);
	~EditorState();

	// Handle SFML Events
	void handleEvents(sf::Event& events);

	// Update Functions
	void updateInput(const float& dt);
	void update(const float& dt);

	// Render Functions
	void render();
};

#endif // EDITOR_STATE_HPP
