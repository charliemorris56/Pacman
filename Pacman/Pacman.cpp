#include "Pacman.h"
#include "Player.h"
#include "Enemy.h"
#include "Menu.h"
#include "MovingEnemy.h"

#include <sstream>
#include <time.h>
#include <iostream>


Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	srand(time(NULL));

	GetMunchieCount();

	//Initialise member variable Pacman
	_pacman = new Player();
	_pacman->currentFrameTime = 0;
	_pacman->direction = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->speed = 0.1f;
	_pacman->frameTime = 250;
	_pacman->score = 0;
	_pacman->dead = false;
	_pacman->win = false;

	//Initialise member variable Munchie
	_munchie = new Enemy[munchuieCount];
	for (int i = 0; i < munchuieCount; i++)
	{
		//_munchie[i] = new Enemy();
		_munchie[i].currentFrameTime = 0;
		_munchie[i].frame = rand() % 1;
		_munchie[i].frameTime = rand() % 500 + 50;
	}	

	//Initialise member variable Cherry
	_cherry = new Enemy();
	_cherry->currentFrameTime = 0;
	_cherry->frame = rand() % 1;
	_cherry->frameTime = rand() % 500 + 50;

	//Initialise member variable Menu
	_menu = new Menu();

	//Initialise  member variable Ghost
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = rand() % 4;
		_ghosts[i]->speed = 0.2f;
	}

	//_munchieFrameCount = 0;
	//_scorePacman2 = 0;
	/*_scorePacman = 0;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;*/
	//_munchieCurrentFrameTime = 0;
	_pause = false;
	_pKeyDown = false;
	_rKeyDown = false;
	_start = true;

	_pop = new SoundEffect();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//delete _pacmanTexture;
	//delete _pacmanSourceRect;
	//delete _pacman2Texture;
	//delete _pacman2SourceRect;
	//delete _munchieBlueTexture;
	//delete _munchieInvertedTexture;
	//delete _munchieRect;
	//delete _munchiePosition;

	delete _pacman->position;
	delete _pacman->sourceRect;
	delete _pacman->texture;
	delete _pacman;

	for (int i = 0; i < munchuieCount; i++)
	{
		delete _munchie[i].position;
		delete _munchie[i].sourceRect;
	}
	delete _munchie[0].texture;
	delete[] _munchie;

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->position;
		delete _ghosts[i]->sourceRect;
	}
	delete _ghosts[0]->texture;
	delete[] _ghosts;

	delete _cherry->position;
	delete _cherry->sourceRect;
	delete _cherry->texture;
	delete _cherry;

	delete _menu->position;
	delete _menu->sourceRect;
	delete _menu->texture;
	delete _menu;

	delete _pop;
}

void Pacman::LoadContent()
{
	// Load Pacman
	/*_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(50.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32); //The first two variables are related to the current stage of the texture, play with this next time*/

	//Load Pacman
	_pacman->position = new Vector2(50.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);

	// Load Pacman2
	/*_pacman2Texture = new Texture2D();
	_pacman2Texture->Load("Textures/Pacman2.tga", false);
	_pacman2Position = new Vector2(150.0f, 550.0f);
	_pacman2SourceRect = new Rect(0.0f, 0.0f, 32, 32);*/

	// Load Munchie
	/*_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.png", true);
	//_munchieInvertedTexture = new Texture2D();
	//_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(0.0f, 0.0f, 12, 12);
	_munchiePosition = new Vector2(150, 150);*/

	//Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", true);

	for (int i = 0; i < munchuieCount; i++)
	{
		_munchie[i].position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_munchie[i].sourceRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchie[i].texture = munchieTex;
	}

	//Load Ghost
	Texture2D* ghostTex = new Texture2D();
	ghostTex->Load("Textures/GhostBlue.png", false);

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
		_ghosts[i]->texture = ghostTex;
	}

	//Load Cherry
	_cherry->position = new Vector2(-50.0f, -50.0f); //Loads off screen
	_cherry->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", true);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set menu paramters
	/*_menuBackgroud = new Texture2D();
	_menuBackgroud->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);*/

	//Set menu Paramters
	_menu->position = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_menu->sourceRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->texture = new Texture2D();
	_menu->texture->Load("Textures/Transparency.png", false);

	//Load sounds
	_pop->Load("Sounds/pop.wav");
}

void Pacman::Update(int elapsedTime)
{	
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	
	//Game start
	GameStart(keyboardState, Input::Keys::SPACE);

	if (!_start)
	{
		//Game pause
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_pause && !_pacman->dead && !_pacman->win)
		{
			//Pacman's movement key check and movement implementation
			Input(elapsedTime, keyboardState, mouseState);

			//Animation update for Pacman
			UpdatePacman(elapsedTime);

			//Checks if Pacman is trying to disappear off of the sides of the play area
			CheckViewportCollision();

			//Animation update for Munchie
			UpdateMunchie(elapsedTime);

			//Animation update for Cherry
			UpdateCherry(elapsedTime);

			//Ghost movement
			UpdateGhost(elapsedTime);

			//Pacman ghost collision
			CheckGhostCollisions();

			//Pacman Munchie collision
			CheckMunchieCollisions();
			
		

			//Pacman2
			/*
			// Checks if L key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::L))
				_pacman2Position->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

			// Checks if J key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::J))
				_pacman2Position->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

			// Checks if I key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::I))
				_pacman2Position->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

			// Checks if K key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::K))
				_pacman2Position->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

			// Checks if Pacman is trying to disappear
			if (_pacman2Position->X + _pacman2SourceRect->Width > Graphics::GetViewportWidth())  //1024 is the game width
			{
				// Pacman hit right wall - reset his position
				//_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;

				//Warp to the left
				_pacman2Position->X = 0;
			}

			if (_pacman2Position->X < 0)  //1024 is the game width
			{
				// Pacman hit left wall - reset his position
				//_pacmanPosition->X = 0;

				//Warp to the right
				_pacman2Position->X = Graphics::GetViewportWidth() - _pacman2SourceRect->Width;
			}

			if (_pacman2Position->Y + _pacman2SourceRect->Height > Graphics::GetViewportHeight())  //1024 is the game width
			{
				// Pacman hit bottom wall - reset his position
				//_pacmanPosition->Y = 768 - _pacmanSourceRect->Height;

				//Warp to the top
				_pacman2Position->Y = 0;
			}

			if (_pacman2Position->Y < 0)  //1024 is the game width
			{
				// Pacman hit top wall - reset his position
				//_pacmanPosition->Y = 0;

				//Warp to the bottom
				_pacman2Position->Y = Graphics::GetViewportHeight() - _pacman2SourceRect->Height;
			}

			//Touching the food
			if ((((_pacman2Position->X - _munchieRect->X) < 5) && ((_pacman2Position->X - _munchieRect->X) > -25)) && (((_pacman2Position->Y - _munchieRect->Y) < 5) && ((_pacman2Position->Y - _munchieRect->Y) > -25)))
			{

				_munchiePosition->X = rand() % (Graphics::GetViewportWidth() - _pacmanSourceRect->Width);
				_munchiePosition->Y = rand() % (Graphics::GetViewportHeight() - _pacmanSourceRect->Height);

				_scorePacman2 += 1;
			}*/
		}

		/*else if (_pacman->win)
		{
			for (int i = 0; i < munchuieCount; i++)
			{
				_munchie[i].currentFrameTime = 0;
				_munchie[i].frame = rand() % 1;
				_munchie[i].frameTime = rand() % 500 + 50;
			}

			_cherry->currentFrameTime = 0;
			_cherry->frame = rand() % 1;
			_cherry->frameTime = rand() % 500 + 50;

			//Initialise  member variable Ghost
			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				_ghosts[i]->direction = rand() % 4;
				_ghosts[i]->speed = 0.2f;
			}

			_pacman->win = false;
		}*/
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y << "\nPacman Score : " << _pacman->score;// << " Pacman 2 : " << _scorePacman2;
	//stream << "Pacman : " << _pacman->score << "\nPacman 2 : " << _scorePacman2;

	SpriteBatch::BeginDraw(); // Starts Drawing

	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}

	SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect); // Draws Cheery
	//SpriteBatch::Draw(_pacman2Texture, _pacman2Position, _pacman2SourceRect); // Draws Pacman2

	for (int i = 0; i < munchuieCount; i++)
	{
		SpriteBatch::Draw(_munchie[i].texture, _munchie[i].position, _munchie[i].sourceRect);
	}

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	}
	
	//SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	   
	if (_pause && !_pacman->dead && !_pacman->win)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED\nWASD to move\nShift to sprint\nR to spawn in cherry, or left click";

		SpriteBatch::Draw(_menu->texture, _menu->sourceRect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->position, Color::Red);
	}

	if (_start)
	{
		std::stringstream menuStream;
		menuStream << "PRESS SPACE TO START\nPress P for controls";

		SpriteBatch::Draw(_menu->texture, _menu->sourceRect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->position, Color::Blue);
	}

	if (_pacman->dead)
	{
		std::stringstream menuStream;
		menuStream << "DEAD";

		SpriteBatch::Draw(_menu->texture, _menu->sourceRect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->position, Color::Yellow);
	}

	if (_pacman->win)
	{
		std::stringstream menuStream;
		menuStream << "YOU WIN";

		SpriteBatch::Draw(_menu->texture, _menu->sourceRect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->position, Color::Magenta);
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	//Pacman
	float pacmanSpeed = elapsedTime * _pacman->speed * _pacman->speedMultiplier;

	//Checks if D jey is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 0;
	}

	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 2;
	}

	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 3;
	}

	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 1;
	}

	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		_pacman->speedMultiplier = 2.0f;
	}

	else
	{
		_pacman->speedMultiplier = 1.0f;
	}

	//Cherry
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_cherry->position->X = mouseState->X;
		_cherry->position->Y = mouseState->Y;
	}

	if (state->IsKeyDown(Input::Keys::R) && !_rKeyDown)
	{		
		_rKeyDown = true;
		_cherry->position->X = rand() % Graphics::GetViewportWidth();
		_cherry->position->Y = rand() % Graphics::GetViewportHeight();	
	}

	if (state->IsKeyUp(Input::Keys::R))
	{
		_rKeyDown = false;
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(pauseKey) && !_pKeyDown)
	{
		_pKeyDown = true;
		_pause = !_pause;
	}

	if (state->IsKeyUp(pauseKey))
	{
		_pKeyDown = false;
	}
}

void Pacman::CheckViewportCollision()
{
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())  //1024 is the game width
	{
		// Pacman hit right wall - reset his position
		//_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;

		//Warp to the left
		_pacman->position->X = 0;
	}

	if (_pacman->position->X < 0)  //1024 is the game width
	{
		// Pacman hit left wall - reset his position
		//_pacmanPosition->X = 0;

		//Warp to the right
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}

	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())  //1024 is the game width
	{
		// Pacman hit bottom wall - reset his position
		//_pacmanPosition->Y = 768 - _pacmanSourceRect->Height;

		//Warp to the top
		_pacman->position->Y = 0;
	}

	if (_pacman->position->Y < 0)  //1024 is the game width
	{
		// Pacman hit top wall - reset his position
		//_pacmanPosition->Y = 0;

		//Warp to the bottom
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->currentFrameTime += elapsedTime;

	if (_pacman->currentFrameTime > _pacman->frameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}

	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	for (int i = 0; i < munchuieCount; i++)
	{
		_munchie[i].currentFrameTime += elapsedTime;

		if (_munchie[i].currentFrameTime > _munchie[i].frameTime)
		{
			_munchie[i].frame++;

			if (_munchie[i].frame >= 2)
				_munchie[i].frame = 0;

			_munchie[i].currentFrameTime = 0;
		}

		if (_munchie[i].frame == 0)
		{
			// Draws Red Munchie
			_munchie[i].sourceRect->X = 0;
		}
		else
		{
			// Draw Blue Munchie
			_munchie[i].sourceRect->X = _munchie[i].sourceRect->Width;
		}
	}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherry->currentFrameTime += elapsedTime;

	if (_cherry->currentFrameTime > _cherry->frameTime)
	{
		_cherry->frame++;

		if (_cherry->frame >= 2)
			_cherry->frame = 0;

		_cherry->currentFrameTime = 0;
	}

	_cherry->sourceRect->X = _cherry->sourceRect->Width * _cherry->frame;

}

void Pacman::GameStart(Input::KeyboardState* state, Input::Keys startKey)
{
	if (state->IsKeyDown(startKey))
	{
		_start = false;
	}

}
void Pacman::CheckGhostCollisions()
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		int distancePacmanGhost = sqrt((_pacman->position->X - _ghosts[i]->position->X) * (_pacman->position->X - _ghosts[i]->position->X) + (_pacman->position->Y - _ghosts[i]->position->Y) * (_pacman->position->Y - _ghosts[i]->position->Y));

		if (distancePacmanGhost < 30) //Looks at the distance between the ghost and Pacman
		{
			_pacman->dead = true;
		}
	}
}

void Pacman::CheckMunchieCollisions()
{
	for (int i = 0; i < munchuieCount; i++)
	{
		int distancePacmanMunchie = sqrt((_pacman->position->X - _munchie[i].position->X) * (_pacman->position->X - _munchie[i].position->X) + (_pacman->position->Y - _munchie[i].position->Y) * (_pacman->position->Y - _munchie[i].position->Y));

		if (distancePacmanMunchie < 20) //Looks at the distance between the ghost and Pacman
		{
			_pacman->score += 1;
			Audio::Play(_pop);
			_munchie[i].position->X = -50;
			_munchie[i].position->Y = -50;
		}
	}

	if (_pacman->score == munchuieCount)
	{
		_pacman->win = true;
	}
}

void Pacman::UpdateGhost(int elapsedTime)
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		//Update movement
		if (_ghosts[i]->direction == 0) //Moves right
		{
			_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime;
			_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width * 2;
			_ghosts[i]->sourceRect->Y = _ghosts[i]->sourceRect->Height * 2;
		}

		else if (_ghosts[i]->direction == 2) //Moves left
		{
			_ghosts[i]->position->X -= _ghosts[i]->speed * elapsedTime;
			_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width;
			_ghosts[i]->sourceRect->Y = _ghosts[i]->sourceRect->Height * 2;
		}

		else if (_ghosts[i]->direction == 3) //Moves up
		{
			_ghosts[i]->position->Y += _ghosts[i]->speed * elapsedTime;
			_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width;
			_ghosts[i]->sourceRect->Y = _ghosts[i]->sourceRect->Height;
		}

		else if(_ghosts[i]->direction == 1) //Moves down
		{
			_ghosts[i]->position->Y -= _ghosts[i]->speed * elapsedTime;
			_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width * 2;
			_ghosts[i]->sourceRect->Y = _ghosts[i]->sourceRect->Height;
		}
		
		if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width >= Graphics::GetViewportWidth()) //Hits right edge
		{
			_ghosts[i]->direction = 2;
		}

		else if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width <= 0) //Hits left edge
		{
			_ghosts[i]->direction = 0;
		}

		else if (_ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height >= Graphics::GetViewportHeight()) //Hits top edge
		{
			_ghosts[i]->direction = 1;
		}

		else if (_ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height <= 0) //Hits bottom edge
		{
			_ghosts[i]->direction = 3;
		}
	}
}

void Pacman::GetMunchieCount()
{
	std::cout << "Please enter the Munchie count-> ";
	std::cin >> munchuieCount;
}