#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "State.hpp"
#include "MapLayer.hpp"
#include "SparkGUI.hpp"
#include "Map.hpp"

class EditorState: public State
{
private:
	// Type alias
	using RectShape = sf::RectangleShape;

	// Enums
	enum class KeyBind
	{
		NEXT_LAYER,
		PREV_LAYER,
		PREVIEW
		
	}; // KeyBind

	// Variables
	StateData* m_stateData;
	sf::Vector2f m_mapGrid;
	sf::Vector2f m_gridSize;
	sf::View m_guiView;
	sf::View m_mapView;
	sf::Vector2f m_defViewSize;
	std::vector<float> m_zoomFactors;
	std::map<KeyBind, int> m_keybinds;
	std::map<const char*, sf::Texture> m_textures;
	float m_zoomScale;

	// Our map
	sp::Map m_map; 

	// Selection
	RectShape m_mapSelection;
	RectShape m_cursorSelection;
	std::vector<sp::Tile*> m_selectedTiles;

	// GUI
	sf::Text m_zoomText;
	sf::Text m_tileText;
	sp::Button m_resetButton;
	
	// Event States
	bool m_panningState, m_selectionState;
	
	// Initializer Functions
	void initView();
	void initTextures();
	void initKeybinds();
	void initMap();
	void initButtons();
	void initTexts();
	void initGUI();

	// Event Functions
	void resizeEvent(sf::Event& events);
	void panningEvent(sf::Event& events);
	void zoomingEvent(sf::Event& events);
	void selectionEvent(sf::Event& events);

	// Update Functions
	void updateGUI();
	void updateInput(const float& dt);
public:
	// Constructors/Destructors
	EditorState(StateData* state_data, 
		const sf::Vector2f& map_grid, 
		const sf::Vector2f& grid_size, 
		const sf::Color& grid_color, 
		const sf::Color& outline_color);
	~EditorState();

	// Handle SFML Events
	void handleEvents(sf::Event& events);

	// Update
	void update(const float& dt);

	// Render
	void render();
};

#endif // EDITOR_STATE_HPP
