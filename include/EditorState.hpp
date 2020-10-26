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
	sf::View view;
	sf::VertexArray mapOutlines, mapGridlines;
	std::vector<float> zoomFactors;
	float zoomScale;

	// Initializer Functions
	void initView();
	void initTextures();
	void initKeybinds();
	void initFonts();
	void initMapOutlines();
	void initMapGridlines();
public:
	// Constructors/Destructors
	EditorState(StateData* state_data, sf::IntRect map_rect, sf::Vector2i grid_size);
	~EditorState();

	// Handle SFML Events
	void handleEvents(sf::Event& events);

	// Update Functions
	void updateInput(const float& dt);
	void update(const float& dt);

	// Render Functions
	void renderGrid(sf::RenderTarget* surface);
	void render(sf::RenderTarget* surface = nullptr);
};

#endif // EDITOR_STATE_HPP
