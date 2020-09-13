#include "imaging.h"
#include "..//minimax/minimax.h"
#include <algorithm>

void ChessGame::run()
{
	while (this->window.isOpen())
	{
		handleInput();
		draw();
	}
}

void ChessGame::draw()
{
	this->window.clear(sf::Color::Color(200,200,200));
	for (int i = 0; i < this->board.size(); i++)
	{
		for (int j = 0; j < this->board[i].size(); j++)
		{
			board[i][j].draw(this->window,this->textureManager);
		}
	}
	// Draw the captured pieces
	drawCapturedPieces();
	this->gui.draw();
	this->window.display();
}

void ChessGame::promotePawn(chess::Tile* pawn)
{
	//create new window like in chess.com
	sf::RenderWindow promoteWindow;
	promoteWindow.create(sf::VideoMode(400, 100), "", sf::Style::None);
	//create the 4 possible imagingTiles
	std::array<imagingTile, 4> promotions;
	std::array<chess::Tile, 4> tiles;
	int counter = 0;
	for (int i=0;i<4;i++)
	{
		sf::Color c = counter % 2 == 0 ? this->style.blackTileColor : this->style.whiteTileColor;
		imagingTile tile;
		chess::Tile t(pawn->color, 0);
		tiles[i] = t;
		tile.rect = sf::RectangleShape(sf::Vector2f(this->tileWidth, this->tileWidth));
		tile.sprite = sf::RectangleShape(sf::Vector2f(this->tileWidth, this->tileWidth));
		tile.rect.setFillColor(c);
		tile.defaultColor = c;
		tile.tile = &tiles[i];
		tile.pos = Position(i * tileWidth, 0);
		tile.rect.setPosition(sf::Vector2f(i*tileWidth,0));
		tile.sprite.setPosition(sf::Vector2f(i*tileWidth,0));
		promotions[i] = tile;
		++counter;
	}
	promotions[0].tile->pawn = 5;
	promotions[1].tile->pawn = 2;
	promotions[2].tile->pawn = 4;
	promotions[3].tile->pawn = 3;
	while (promoteWindow.isOpen())
	{
		sf::Event event;
		while (promoteWindow.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				int x = event.mouseButton.x / this->tileWidth;
				pawn->pawn = tiles[x].pawn;
				promoteWindow.close();
			}
		}
		promoteWindow.clear(sf::Color::White);
		for (imagingTile tile : promotions)
		{
			tile.draw(promoteWindow, this->textureManager);
		}
		promoteWindow.display();
	}
}

void ChessGame::handleInput()
{
	sf::Event event;
	while(this->window.pollEvent(event))
	{
		this->gui.handleEvent(event);
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
		case sf::Event::MouseButtonPressed:
		{
			int x = ((event.mouseButton.x-widthOffset) / (double) windowWidth) * 8;
			int y = (event.mouseButton.y / (double) windowHeight) * 8;
			std::cout << "Tile[" << x << "][" << y << "]" << " was pressed" << std::endl;
			if (x > -1 && x<8 && y>-1 && y < 8)
			{
				
				if(board[x][y].state == 2) // clicked on a possible move
				{
					Position pos = getFocusedTile();
					historyManager.addMove({ pos,{x,y},this->chessboard.getTiles()[x][y] });

					auto captured = chess::move(this->chessboard.getTiles(), pos, Position(x, y));
					if (captured.pawn != 0)
					{
						this->chessboard.addCapturedPiece(captured);
					}

					if (this->chessboard.getTiles()[x][y].pawn == 1 && (y == 0 || y == 7))
					{
						promotePawn(&this->chessboard.getTiles()[x][y]);
					}
					removeFocus();
					chess::switchTurns(this->chessboard);
					// check for checkmate
					this->chessboard.setCheckMate(chess::checkCheckMate(this->chessboard.getTiles(), this->chessboard.getTurn()));
					if(this->chessboard.checkMate)
					{
						std::string checkmate = this->chessboard.turn ? "White" : "Black";
						std::cout << "CheckMate " << checkmate << std::endl;
					}
				}
				else if (board[x][y].tile->pawn != 0)
				{
					removeFocus();
					board[x][y].state = 1;
					auto moves = getMoves(this->chessboard.getTiles(),this->chessboard.getTurn(),Position(x,y));
					chess::removeCheck(moves, this->chessboard.getTiles(), Position(x, y));
					for (Position pos : moves)
					{
						this->board[pos.x][pos.y].state = 2;
					}
				}
			}
		}
		default:
			break;
		}
	
	}
}

void ChessGame::setupPawns()
{
	// test setup

	/*this->board[4][7].tile->pawn = 6;
	this->board[4][7].tile->color = 1;

	this->board[3][7].tile->pawn = 5;
	this->board[3][7].tile->color = 1;

	this->board[4][6].tile->pawn = 1;
	this->board[4][6].tile->color = 1;

	this->board[3][6].tile->pawn = 1;
	this->board[3][6].tile->color = 1;

	this->board[5][6].tile->pawn = 1;
	this->board[5][6].tile->color = 1;

	this->board[5][7].tile->pawn = 1;
	this->board[5][7].tile->color = 1;

	this->board[0][5].tile->pawn = 3;
	this->board[0][5].tile->color = 0;

	this->board[2][5].tile->pawn = 5;
	this->board[2][5].tile->color = 0;

	this->board[7][7].tile->pawn = 1;
	this->board[7][7].tile->color = 1;*/


	// real setup
	for (int i = 0; i < 8; i++) this->board[i][0].tile->color = 0;
	this->board[0][0].tile->pawn = 2;
	this->board[1][0].tile->pawn = 3;
	this->board[2][0].tile->pawn = 4;
	this->board[4][0].tile->pawn = 6;
	this->board[3][0].tile->pawn = 5;
	this->board[5][0].tile->pawn = 4;
	this->board[6][0].tile->pawn = 3;
	this->board[7][0].tile->pawn = 2;
	
	for (int i = 0; i < 8; i++)
	{
		board[i][1].tile->pawn = 1;
		board[i][1].tile->color = 0;
	}

	for (int i = 0; i < 8; i++) this->board[i][7].tile->color = 1;
	this->board[0][7].tile->pawn = 2;
	this->board[1][7].tile->pawn = 3;
	this->board[2][7].tile->pawn = 4;
	this->board[4][7].tile->pawn = 6;
	this->board[3][7].tile->pawn = 5;
	this->board[5][7].tile->pawn = 4;
	this->board[6][7].tile->pawn = 3;
	this->board[7][7].tile->pawn = 2;

	for (int i = 0; i < 8; i++)
	{
		board[i][6].tile->pawn = 1;
		board[i][6].tile->color = 1;
	}
}

void imagingTile::draw(sf::RenderWindow& window, TextureManager& tManager)
{
	if (state == 0)
	{
		this->rect.setFillColor(defaultColor);
	}
	else if (state == 1)
	{
		this->rect.setFillColor(focusColor);
	}
	else if (state == 2)
	{
		this->rect.setFillColor(possibleMoveColor);
	}
	this->sprite.setTexture(tManager.getTexture(this->tile->pawn, this->tile->color));
	window.draw(this->rect);
	if (this->tile->pawn != 0)
		window.draw(this->sprite);
}

void ChessGame::removeFocus()
{
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		{
			this->board[i][j].state = 0;
		}
	}
}

Position ChessGame::getFocusedTile()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].state == 1)
			{
				return board[i][j].pos;
			}
		}
	}
}

ChessGame::ChessGame(StyleManager style) 
{
	this->style = style;
	undo = tgui::Button::create();
	redo = tgui::Button::create();
	gui.add(undo);
	gui.add(redo);
	undo->setPosition({ 925,25 });
	redo->setPosition({ 925,100 });
	undo->setSize({ 50,50 });
	redo->setSize({ 50,50 });
	undo->connect("Clicked", [&]()
		{
			removeFocus();
			this->historyManager.undo(this->chessboard);
		});
	redo->connect("Clicked", [&]()
		{
			removeFocus();
			this->historyManager.redo(this->chessboard);
		});
	undo->getRenderer()->setTexture(tgui::Texture("sprites/undo.png"));
	redo->getRenderer()->setTexture(tgui::Texture("sprites/redo.png"));
	window.clear();
	window.create(sf::VideoMode(windowWidth+widthOffset*2, windowHeight), "Chess");
	sf::Color c = this->style.blackTileColor;
	int colorCounter = 1;
	for (int i = 0; i < this->board.size(); i++)
	{
		for (int j = 0; j < this->board[i].size(); j++)
		{
			c = (i + j) % 2 == 0 ? this->style.blackTileColor : this->style.whiteTileColor;
			board[i][j].rect = sf::RectangleShape(sf::Vector2f(tileWidth, tileWidth));
			board[i][j].sprite = sf::RectangleShape(sf::Vector2f(tileWidth, tileWidth));
			board[i][j].rect.setFillColor(c);
			board[i][j].defaultColor = c;
			board[i][j].rect.setPosition((i * tileWidth)+widthOffset, j * tileWidth);
			board[i][j].sprite.setPosition((i * tileWidth)+widthOffset, j * tileWidth);
			board[i][j].pos = Position(i, j);
			board[i][j].tile = &this->chessboard.getTiles()[i][j];
		}
	}
	setupPawns();
}

void ChessGame::drawCapturedPieces()
{
	auto captured = this->chessboard.getCapturedPieces();
	int x = 0, y = 0;
	for (int k=0;k<2;k++) //for both colors
	{
		int counter = 0;
		for (int i = 1; i < 6; i++) // loop through all possible captured pieces 1-> Pawn 5-> Queen
		{
			int count = std::count_if(captured.begin(), captured.end(), [=](chess::Tile t) { return t.pawn == i && t.color == k; });
			for (int j = 0; j < count; j++)
			{
				x = counter % 2 * this->widthOffset / 2;
				y = counter / 2 * this->widthOffset / 2;
				sf::Sprite sprite;
				sf::Texture texture = *this->textureManager.getTexture(i, k);
				sprite.setTexture(texture);
				sprite.setScale(0.25, 0.25);
				sprite.setPosition(x + k*(this->windowWidth+widthOffset), this->windowHeight-y-this->widthOffset/2);
				this->window.draw(sprite);
				counter++;
			}
		}
	}
}

void ComputerChessGame::handleInput()
{
	sf::Event event;
	while (this->window.pollEvent(event))
	{
		this->gui.handleEvent(event);
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
		case sf::Event::MouseButtonPressed:
		{
			if (!this->chessboard.isCheckMate())
			{
				int x = ((event.mouseButton.x - widthOffset) / (double)windowWidth) * 8;
				int y = (event.mouseButton.y / (double)windowHeight) * 8;
				std::cout << "Tile[" << x << "][" << y << "]" << " was pressed" << std::endl;
				if (x > -1 && x<8 && y>-1 && y < 8)
				{

					if (board[x][y].state == 2) // clicked on a possible move
					{
						Position pos = getFocusedTile();
						//historyManager.addMove({ pos,{x,y},this->chessboard.getTiles()[x][y] });

						auto captured = chess::move(this->chessboard.getTiles(), pos, Position(x, y));
						if (captured.pawn != 0)
						{
							this->chessboard.addCapturedPiece(captured);
						}

						if (this->chessboard.getTiles()[x][y].pawn == 1 && (y == 0 || y == 7))
						{
							promotePawn(&this->chessboard.getTiles()[x][y]);
						}
						removeFocus();
						chess::switchTurns(this->chessboard);
						// check for checkmate
						this->chessboard.setCheckMate(chess::checkCheckMate(this->chessboard.getTiles(), this->chessboard.getTurn()));
						if (this->chessboard.checkMate)
						{
							std::string checkmate = this->chessboard.turn ? "White" : "Black";
							std::cout << "CheckMate " << checkmate << std::endl;
						}
						else
						{
							auto bestMove = getBestMove(this->chessboard, this->depth, false, INT_MIN, INT_MAX);
							std::cout << "Best move: " << " from " << bestMove.first << " to " << bestMove.second << std::endl;
							chess::move(this->chessboard.getTiles(), bestMove.first, bestMove.second);
							chess::switchTurns(this->chessboard);
						}
					}
					else if (board[x][y].tile->pawn != 0)
					{
						removeFocus();
						board[x][y].state = 1;
						auto moves = getMoves(this->chessboard.getTiles(), this->chessboard.getTurn(), Position(x, y));
						chess::removeCheck(moves, this->chessboard.getTiles(), Position(x, y));
						for (Position pos : moves)
						{
							this->board[pos.x][pos.y].state = 2;
						}
					}
				}
			}
		}
		default:
			break;
		}

	}
}