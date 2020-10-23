#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "State.hpp"
#include "MapLayer.hpp"

class EditorState: public State
{
private:
	// Variables
	sf::IntRect mapRect;
	sf::Vector2i gridSize;
	sf::Font font;
	sf::Color fillColor;
	sf::View view;
	sf::RectangleShape mapShape;
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	float zoomScale;
	float defOutlineThicc;
	

	// Initializer Functions
	void initView();
	void initTextures();
	void initKeybinds();
	void initFonts();

	void createMapSurface(sf::IntRect& mapRect);
public:
	// Constructors/Destructors
	EditorState(StateData* state_data, sf::IntRect map_rect, sf::Vector2i grid_size, sf::Color fill_color);
	~EditorState();

	// Handle SFML Events
	void handleEvents(sf::Event& events);

	// MosePos Converters


	// Update Functions
	void updateInput(const float& dt);
	void update(const float& dt);

	// Render Functions
	void render(sf::RenderTarget* surface = nullptr);
};

#endif // EDITOR_STATE_HPP
