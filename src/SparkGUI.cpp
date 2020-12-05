#include "../include/SparkGUI.hpp"

// Button widget

// Constructor/Destructor
sp::Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size,
				   const std::string& text, sf::Font& font,
				   unsigned int charSize,
				   const sp::Style& idle_style,
				   const sp::Style& hover_style,
				   const sp::Style& active_style)
	:Bstate(ButtonState::IDLE)


{
	// Initialize styles
	this->setIdleStyle(idle_style);
	this->setIdleStyle(idle_style);
	this->setIdleStyle(idle_style);

	// Initialize shape
	this->shape.setPosition(pos);
	this->shape.setSize(size);
	this->shape.setFillColor(this->idleStyle.backgroundColor);
	this->shape.setOutlineThickness(this->idleStyle.outlineThickness);
	this->shape.setOutlineColor(this->idleStyle.outlineColor);

	// Initialize text
	this->text.setFont(font);	
	this->setCharSize(charSize);			
	this->text.setFillColor(this->idleStyle.foregroundColor);
	this->setText(text);			
	this->text.setPosition(					
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y
	);										
}

sp::Button::~Button()
{
	
}

// Accessors
bool sp::Button::isPressed() const
{
	return (this->Bstate == ButtonState::ACTIVE);
}

const sf::Vector2f& sp::Button::getPosition() const
{
	return this->shape.getPosition();
}

const sf::Vector2f& sp::Button::getSize() const
{
	return this->shape.getSize();
}

const std::string sp::Button::getText() const
{
	return this->text.getString();
}

const sf::Font* sp::Button::getFont() const
{
	return this->text.getFont();
}

unsigned int sp::Button::getCharSize() const
{
	return this->text.getCharacterSize();
}

const sp::Style& sp::Button::getIdleStyle() const
{
	return this->idleStyle;
}

const sp::Style& sp::Button::getHoverStyle() const
{
	return this->hoverStyle;
}

const sp::Style& sp::Button::getActiveStyle() const
{
	return this->activeStyle;
}

// Modifiers
void sp::Button::setPosition(const sf::Vector2f& pos)
{
	this->shape.setPosition(pos);
	this->text.setPosition(					
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y
	);	
}

void sp::Button::setSize(const sf::Vector2f& size)
{
	this->shape.setSize(size);
	this->text.setPosition(					
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y
	);	
}

void sp::Button::setText(const std::string& text)
{
	this->text.setString(text);
}

void sp::Button::setFont(const sf::Font& font)
{
	this->text.setFont(font);
}

void sp::Button::setCharSize(unsigned int charSize)
{
	this->text.setCharacterSize(charSize);
}

void sp::Button::setIdleStyle(const sp::Style& style)
{
	this->idleStyle = style;
}

void sp::Button::setHoverStyle(const sp::Style& style)
{
	this->hoverStyle = style;
}

void sp::Button::setActiveStyle(const sp::Style& style)
{
	this->activeStyle = style;
}

void sp::Button::setIdleColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline)
{
	this->idleStyle.backgroundColor = background;
	this->idleStyle.foregroundColor = foreground;
	this->idleStyle.outlineColor    = outline;
}

void sp::Button::setHoverColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline)
{
	this->hoverStyle.backgroundColor = background;
	this->hoverStyle.foregroundColor = foreground;
	this->hoverStyle.outlineColor    = outline;
}

void sp::Button::setActiveColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline)
{
	this->activeStyle.backgroundColor = background;
	this->activeStyle.foregroundColor = foreground;
	this->activeStyle.outlineColor    = outline;
}

void sp::Button::setOutlineThickness(const float& idle, const float& hover, const float& active)
{
	this->idleStyle.outlineThickness   = idle;
	this->hoverStyle.outlineThickness  = hover;
	this->activeStyle.outlineThickness = active;
}

// Functions
void sp::Button::update(const sf::Vector2i& mousePosWindow)
{
	// IDLE
	this->Bstate = ButtonState::IDLE;

	if(this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
	{
		// HOVER
		this->Bstate = ButtonState::HOVER;

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// ACTIVE
			this->Bstate = ButtonState::ACTIVE;
		}
	}

	switch(this->Bstate)
	{
	case ButtonState::IDLE:
		// Set button to IDLE
		this->shape.setFillColor(this->idleStyle.backgroundColor);
		this->text.setFillColor(this->idleStyle.foregroundColor);
		this->shape.setOutlineColor(this->activeStyle.outlineColor);
		this->shape.setOutlineThickness(this->idleStyle.outlineThickness);
		break;
	case ButtonState::HOVER:
		// Set button to HOVER
		this->shape.setFillColor(this->hoverStyle.backgroundColor);
		this->text.setFillColor(this->hoverStyle.foregroundColor);
		this->shape.setOutlineColor(this->activeStyle.outlineColor);
		this->shape.setOutlineThickness(this->hoverStyle.outlineThickness);
		break;
	case ButtonState::ACTIVE:
		// Set button to ACTIVE
		this->shape.setFillColor(this->activeStyle.backgroundColor);
		this->text.setFillColor(this->activeStyle.foregroundColor);
		this->shape.setOutlineColor(this->activeStyle.outlineColor);
		this->shape.setOutlineThickness(this->activeStyle.outlineThickness);
		break;
	}

}

void sp::Button::render(sf::RenderTarget& surface)
{
	surface.draw(this->shape);
	surface.draw(this->text);
}
