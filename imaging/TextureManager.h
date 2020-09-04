#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "..//game/chess.h"
#include "SFML/Graphics.hpp"

class TextureManager
{
public:
	sf::Texture pawn_black;
	sf::Texture rook_black;
	sf::Texture knight_black;
	sf::Texture bishop_black;
	sf::Texture king_black;
	sf::Texture queen_black;
	sf::Texture pawn_white;
	sf::Texture rook_white;
	sf::Texture knight_white;
	sf::Texture bishop_white;
	sf::Texture king_white;
	sf::Texture queen_white;
	TextureManager()
	{
		this->pawn_black.loadFromFile("sprites/pawn_b.png");
		this->rook_black.loadFromFile("sprites/rook_b.png");
		this->knight_black.loadFromFile("sprites/knight_b.png");
		this->bishop_black.loadFromFile("sprites/bishop_b.png");
		this->king_black.loadFromFile("sprites/king_b.png");
		this->queen_black.loadFromFile("sprites/queen_b.png");
		this->pawn_white.loadFromFile("sprites/pawn_w.png");
		this->rook_white.loadFromFile("sprites/rook_w.png");
		this->knight_white.loadFromFile("sprites/knight_w.png");
		this->bishop_white.loadFromFile("sprites/bishop_w.png");
		this->king_white.loadFromFile("sprites/king_w.png");
		this->queen_white.loadFromFile("sprites/queen_w.png");
	}

	sf::Texture* getTexture(int pawn, int color)
	{
		switch (pawn)
		{
		case 1:
		{
			return color ? &this->pawn_white : &this->pawn_black;
		}
		case 2:
		{
			return color ? &this->rook_white : &this->rook_black;
		}
		case 3:
		{
			return color ? &this->knight_white : &this->knight_black;
		}
		case 4:
		{
			return color ? &this->bishop_white : &this->bishop_black;
		}
		case 5:
		{
			return color ? &this->queen_white : &this->queen_black;
		}
		case 6:
		{
			return color ? &this->king_white : &this->king_black;
		}
		default:
			break;
		}
	}
};

#endif // !TEXTUREMANAGER_H

