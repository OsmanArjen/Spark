#include "KeybindHandler.hpp"

// Static members
using KeyTyp = sf::Keyboard::Key;
std::map<std::string, sf::Keyboard::Key> sp::KeybindHandler::m_keyStringMap{
	{"A",          KeyTyp(0)}, {"Period",    KeyTyp(50)}, {"Num0",      KeyTyp(26)}, {"Numpad1",   KeyTyp(76)}, 
	{"B",          KeyTyp(1)}, {"Quote",     KeyTyp(51)}, {"Num1",      KeyTyp(27)}, {"Numpad2",   KeyTyp(77)}, 
	{"C",          KeyTyp(2)}, {"Slash",     KeyTyp(52)}, {"Num2",      KeyTyp(28)}, {"Numpad3",   KeyTyp(78)}, 
	{"D",          KeyTyp(3)}, {"Backslash", KeyTyp(53)}, {"Num3",      KeyTyp(29)}, {"Numpad4",   KeyTyp(79)}, 
	{"E",          KeyTyp(4)}, {"Tilde",     KeyTyp(54)}, {"Num4",      KeyTyp(30)}, {"Numpad5",   KeyTyp(80)}, 
	{"F",          KeyTyp(5)}, {"Equal",     KeyTyp(55)}, {"Num5",      KeyTyp(31)}, {"Numpad6",   KeyTyp(81)}, 
	{"G",          KeyTyp(6)}, {"Hyphen",    KeyTyp(56)}, {"Num6",      KeyTyp(32)}, {"Numpad7",   KeyTyp(82)}, 
	{"H",          KeyTyp(7)}, {"Space",     KeyTyp(57)}, {"Num7",      KeyTyp(33)}, {"Numpad8",   KeyTyp(83)}, 
	{"I",          KeyTyp(8)}, {"Enter",     KeyTyp(58)}, {"Num8",      KeyTyp(34)}, {"Numpad9",   KeyTyp(84)}, 
	{"J",          KeyTyp(9)}, {"Backspace", KeyTyp(59)}, {"Num9",      KeyTyp(35)}, {"F1",        KeyTyp(85)}, 
	{"K",         KeyTyp(10)}, {"Tab",       KeyTyp(60)}, {"Escape",    KeyTyp(36)}, {"F2",        KeyTyp(86)}, 
	{"L",         KeyTyp(11)}, {"PageUp",    KeyTyp(61)}, {"LControl",  KeyTyp(37)}, {"F3",        KeyTyp(87)}, 
	{"M",         KeyTyp(12)}, {"PageDown",  KeyTyp(62)}, {"LShift",    KeyTyp(38)}, {"F4",        KeyTyp(88)}, 
	{"N",         KeyTyp(13)}, {"End",       KeyTyp(63)}, {"LAlt",      KeyTyp(39)}, {"F5",        KeyTyp(89)}, 
	{"O",         KeyTyp(14)}, {"Home",      KeyTyp(64)}, {"LSystem",   KeyTyp(40)}, {"F6",        KeyTyp(90)}, 
	{"P",         KeyTyp(15)}, {"Insert",    KeyTyp(65)}, {"RControl",  KeyTyp(41)}, {"F7",        KeyTyp(91)}, 
	{"Q",         KeyTyp(16)}, {"Delete",    KeyTyp(66)}, {"RShift",    KeyTyp(42)}, {"F8",        KeyTyp(92)}, 
	{"R",         KeyTyp(17)}, {"Add",       KeyTyp(67)}, {"RAlt",      KeyTyp(43)}, {"F9",        KeyTyp(93)}, 
	{"S",         KeyTyp(18)}, {"Subtract",  KeyTyp(68)}, {"RSystem",   KeyTyp(44)}, {"F10",       KeyTyp(94)}, 
	{"T",         KeyTyp(19)}, {"Multiply",  KeyTyp(69)}, {"Menu",      KeyTyp(45)}, {"F11",       KeyTyp(95)}, 
	{"U",         KeyTyp(20)}, {"Divide",    KeyTyp(70)}, {"LBracket",  KeyTyp(46)}, {"F12",       KeyTyp(96)}, 
	{"V",         KeyTyp(21)}, {"Left",      KeyTyp(71)}, {"RBracket",  KeyTyp(47)}, {"F13",       KeyTyp(97)}, 
	{"W",         KeyTyp(22)}, {"Right",     KeyTyp(72)}, {"Semicolon", KeyTyp(48)}, {"F14",       KeyTyp(98)}, 
	{"X",         KeyTyp(23)}, {"Up",        KeyTyp(73)}, {"Comma",     KeyTyp(49)}, {"F15",       KeyTyp(99)},
	{"Pause",     KeyTyp(100)}
};

std::map<sf::Keyboard::Key, std::string> sp::KeybindHandler::m_keyCodeMap{
	{KeyTyp(0),          "A"}, {KeyTyp(50),    "Period"}, {KeyTyp(26),      "Num0"}, {KeyTyp(76),   "Numpad1"}, 
	{KeyTyp(1),          "B"}, {KeyTyp(51),     "Quote"}, {KeyTyp(27),      "Num1"}, {KeyTyp(77),   "Numpad2"}, 
	{KeyTyp(2),          "C"}, {KeyTyp(52),     "Slash"}, {KeyTyp(28),      "Num2"}, {KeyTyp(78),   "Numpad3"}, 
	{KeyTyp(3),          "D"}, {KeyTyp(53), "Backslash"}, {KeyTyp(29),      "Num3"}, {KeyTyp(79),   "Numpad4"}, 
	{KeyTyp(4),          "E"}, {KeyTyp(54),     "Tilde"}, {KeyTyp(30),      "Num4"}, {KeyTyp(80),   "Numpad5"}, 
	{KeyTyp(5),          "F"}, {KeyTyp(55),     "Equal"}, {KeyTyp(31),      "Num5"}, {KeyTyp(81),   "Numpad6"}, 
	{KeyTyp(6),          "G"}, {KeyTyp(56),    "Hyphen"}, {KeyTyp(32),      "Num6"}, {KeyTyp(82),   "Numpad7"}, 
	{KeyTyp(7),          "H"}, {KeyTyp(57),     "Space"}, {KeyTyp(33),      "Num7"}, {KeyTyp(83),   "Numpad8"}, 
	{KeyTyp(8),          "I"}, {KeyTyp(58),     "Enter"}, {KeyTyp(34),      "Num8"}, {KeyTyp(84),   "Numpad9"}, 
	{KeyTyp(9),          "J"}, {KeyTyp(59), "Backspace"}, {KeyTyp(35),      "Num9"}, {KeyTyp(85),        "F1"}, 
	{KeyTyp(10),         "K"}, {KeyTyp(60),       "Tab"}, {KeyTyp(36),    "Escape"}, {KeyTyp(86),        "F2"}, 
	{KeyTyp(11),         "L"}, {KeyTyp(61),    "PageUp"}, {KeyTyp(37),  "LControl"}, {KeyTyp(87),        "F3"}, 
	{KeyTyp(12),         "M"}, {KeyTyp(62),  "PageDown"}, {KeyTyp(38),    "LShift"}, {KeyTyp(88),        "F4"}, 
	{KeyTyp(13),         "N"}, {KeyTyp(63),       "End"}, {KeyTyp(39),      "LAlt"}, {KeyTyp(89),        "F5"}, 
	{KeyTyp(14),         "O"}, {KeyTyp(64),      "Home"}, {KeyTyp(40),   "LSystem"}, {KeyTyp(90),        "F6"}, 
	{KeyTyp(15),         "P"}, {KeyTyp(65),    "Insert"}, {KeyTyp(41),  "RControl"}, {KeyTyp(91),        "F7"}, 
	{KeyTyp(16),         "Q"}, {KeyTyp(66),    "Delete"}, {KeyTyp(42),    "RShift"}, {KeyTyp(92),        "F8"}, 
	{KeyTyp(17),         "R"}, {KeyTyp(67),       "Add"}, {KeyTyp(43),      "RAlt"}, {KeyTyp(93),        "F9"}, 
	{KeyTyp(18),         "S"}, {KeyTyp(68),  "Subtract"}, {KeyTyp(44),   "RSystem"}, {KeyTyp(94),       "F10"}, 
	{KeyTyp(19),         "T"}, {KeyTyp(69),  "Multiply"}, {KeyTyp(45),      "Menu"}, {KeyTyp(95),       "F11"}, 
	{KeyTyp(20),         "U"}, {KeyTyp(70),    "Divide"}, {KeyTyp(46),  "LBracket"}, {KeyTyp(96),       "F12"}, 
	{KeyTyp(21),         "V"}, {KeyTyp(71),      "Left"}, {KeyTyp(47),  "RBracket"}, {KeyTyp(97),       "F13"}, 
	{KeyTyp(22),         "W"}, {KeyTyp(72),     "Right"}, {KeyTyp(48), "Semicolon"}, {KeyTyp(98),       "F14"}, 
	{KeyTyp(23),         "X"}, {KeyTyp(73),        "Up"}, {KeyTyp(49),     "Comma"}, {KeyTyp(99),       "F15"},
	{KeyTyp(100),    "Pause"}
}; 


// Functions
void sp::KeybindHandler::bind(const std::string& bind, const std::string& key)
{
	m_keybinds[bind] = getKeyCode(key); 
}

bool sp::KeybindHandler::getPressed(const std::string& bind) const
{
	return sf::Keyboard::isKeyPressed(m_keybinds[bind]); 
} 

// Static functions 
const sf::Keyboard::Key& sp::KeybindHandler::getKeyCode(const std::string& key) const
{
	return m_keyStringMap[key];
}

const std::string& sp::KeybindHandler::getStringKey(const sf::Keyboard::Key& key) const
{
	return m_keyCodeMap[key];
}