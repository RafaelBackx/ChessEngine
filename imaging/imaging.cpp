#include "imaging.h"
#include "..//minimax/minimax.h"

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
	//draw undo and redo buttons
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
		sf::Color c = counter % 2 == 0 ? this->black_color : this->white_color;
		imagingTile tile;
		chess::Tile t(pawn->color, 0);
		tiles[i] = t;
		tile.rect = sf::RectangleShape(sf::Vector2f(this->tileWidth, this->tileWidth));
		tile.rect.setFillColor(c);
		tile.defaultColor = c;
		tile.tile = &tiles[i];
		tile.pos = Position(i * tileWidth, 0);
		tile.rect.setPosition(sf::Vector2f(i*tileWidth,0));
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
				std::cout << x << std::endl;
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

					if (this->historyIndex < this->history.size())
					{
						chess::Move move = { pos, Position(x,y), this->chessboard.getTiles()[x][y] };
						this->history[this->historyIndex++] = move;
					}
					else if (this->historyIndex == this->history.size())
					{
						this->history.push_back({ pos, Position(x,y), this->chessboard.getTiles()[x][y] });
						this->historyIndex++;
					}
					else
					{
						std::cout << "something wrong with history index" << std::endl;
					}

					chess::move(this->chessboard.getTiles(), pos, Position(x, y));
					if (this->chessboard.getTiles()[x][y].pawn == 1 && (y == 0 || y == 7))
					{
						std::cout << "Promoting pawn" << std::endl;
						promotePawn(&this->chessboard.getTiles()[x][y]);
					}
					removeFocus();
					chess::switchTurns(this->chessboard);
					std::cout << "turn: " << this->chessboard.getTurn() << std::endl;
					// check for checkmate
					//std::cout << "Checkmate ? ";
					//this->chessboard.setCheckMate(chess::checkCheckMate(this->chessboard.getTiles(), this->chessboard.getTurn()));
					//auto bestMove = getBestMove(this->chessboard,3  ,true);
					//std::cout << "Best move: " << " from " << bestMove.first << " to " << bestMove.second << std::endl;
					//chess::move(this->chessboard.getTiles(), bestMove.first, bestMove.second);
					//chess::switchTurns(this->chessboard);
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

	//this->board[1][2].tile->pawn = 1;
	//this->board[1][2].tile->color = 1;

	//this->board[5][7].tile->pawn = 1;
	//this->board[5][7].tile->color = 1;

	//this->board[2][2].tile->pawn = 1;
	//this->board[2][2].tile->color = 0;

	//this->board[7][1].tile->pawn = 6;
	//this->board[7][1].tile->color = 0;

	//this->board[7][6].tile->pawn = 6;
	//this->board[7][6].tile->color = 1;

	////this->board[4][0].tile->pawn = 3;
	////this->board[4][0].tile->color = 0;

	////this->board[5][2].tile->pawn = 1;
	////this->board[5][2].tile->color = 1;

	////this->board[7][7].tile->pawn = 1;
	////this->board[7][7].tile->color = 1;

	////this->board[4][3].tile->pawn = 6;
	////this->board[4][3].tile->color = 1;

	////this->board[0][7].tile->pawn = 6;
	////this->board[0][7].tile->color = 0;


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
	this->rect.setTexture(tManager.getTexture(this->tile->pawn, this->tile->color));
	window.draw(this->rect);
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

ChessGame::ChessGame() 
{
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
			if(this->historyIndex > 0)
			{
				this->historyIndex--;
				chess::Move move = this->history[this->historyIndex];
				chess::move(this->chessboard.getTiles(), move.to, move.from);
				if (move.capturedPiece.pawn != 0)
				{
					this->chessboard.getTiles()[move.to.x][move.to.y].color = move.capturedPiece.color;
					this->chessboard.getTiles()[move.to.x][move.to.y].pawn = move.capturedPiece.pawn;
				}
				chess::switchTurns(this->chessboard);
			}
		});
	redo->connect("Clicked", [&]()
		{
			if (this->historyIndex < this->history.size())
			{
				chess::Move move = this->history[this->historyIndex++];
				chess::move(this->chessboard.getTiles(), move.from, move.to);
			}
		});
	undo->getRenderer()->setTexture(tgui::Texture("sprites/undo.png"));
	redo->getRenderer()->setTexture(tgui::Texture("sprites/redo.png"));
	window.clear(); // clear the window of any previous placed widgets
	// setup a chess game window with some space left for both players to potentially add a timer and show what pieces where captured
	// lets keep a space of 200 pixels on both sides
	window.create(sf::VideoMode(windowWidth+widthOffset*2, windowHeight), "Chess");
	sf::Color c = black_color;
	int colorCounter = 1;
	for (int i = 0; i < this->board.size(); i++)
	{
		for (int j = 0; j < this->board[i].size(); j++)
		{
			c = (i + j) % 2 == 0 ? this->black_color : this->white_color;
			board[i][j].rect = sf::RectangleShape(sf::Vector2f(tileWidth, tileWidth));
			board[i][j].rect.setFillColor(c);
			board[i][j].defaultColor = c;
			board[i][j].rect.setPosition((i * tileWidth)+widthOffset, j * tileWidth);
			board[i][j].pos = Position(i, j);
			board[i][j].tile = &this->chessboard.getTiles()[i][j];
		}
	}
	setupPawns();

}