/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, strings, and string streams
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <sstream>
#ifdef _JS
#include <emscripten.h>
#endif

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LWindow
{
	public:
		//Intializes internals
		LWindow();

		//Creates window
		bool init();

		//Handles window events
		void handleEvent( SDL_Event& e );

		//Focuses on window
		void focus();

		//Shows windows contents
		void render();

		//Deallocates internals
		void free();

		//Window dimensions
		int getWidth();
		int getHeight();

		//Window focii
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();
		bool isShown();

	private:
		//Window data
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		int mWindowID;
		int mWindowDisplayID;

		//Window dimensions
		int mWidth;
		int mHeight;

		//Window focus
		bool mMouseFocus;
		bool mKeyboardFocus;
		bool mFullScreen;
		bool mMinimized;
		bool mShown;
};

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//Our custom window
LWindow gWindow;

//Display data
int gTotalDisplays = 0;
SDL_Rect* gDisplayBounds = NULL; 

LWindow::LWindow()
{
	//Initialize non-existant window
	mWindow = NULL;
	mRenderer = NULL;

	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mShown = false;
	mWindowID = -1;
	mWindowDisplayID = -1;
	
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init()
{
	//Create window
	mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if( mWindow != NULL )
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;

		//Create renderer for window
		mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

		//try software render if hardware fails
		if( mRenderer == NULL )
		{
			SDL_Log( "Accelerated renderer could not be created! SDL Error: %s\nSwitching to software renderer", SDL_GetError() );
			mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_SOFTWARE);
		}
 
		if( mRenderer == NULL )
		{
			printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			SDL_DestroyWindow( mWindow );
			mWindow = NULL;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

			//Grab window identifiers
			mWindowID = SDL_GetWindowID( mWindow );
			mWindowDisplayID = SDL_GetWindowDisplayIndex( mWindow );

			//Flag as opened
			mShown = true;
		}
	}
	else
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}

	return mWindow != NULL && mRenderer != NULL;
}

void LWindow::handleEvent( SDL_Event& e )
{
	//Caption update flag
	bool updateCaption = false;

	//If an event was detected for this window
	if( e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID )
	{
		switch( e.window.event )
		{
			//Window moved
			case SDL_WINDOWEVENT_MOVED:
			mWindowDisplayID = SDL_GetWindowDisplayIndex( mWindow );
			updateCaption = true;
			break;

			//Window appeared
			case SDL_WINDOWEVENT_SHOWN:
			mShown = true;
			break;

			//Window disappeared
			case SDL_WINDOWEVENT_HIDDEN:
			mShown = false;
			break;

			//Get new dimensions and repaint
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent( mRenderer );
			break;

			//Repaint on expose
			case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent( mRenderer );
			break;

			//Mouse enter
			case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;
			
			//Mouse exit
			case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Keyboard focus gained
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;
			
			//Keyboard focus lost
			case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

			//Window maxized
			case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
            break;
			
			//Window restored
			case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
            break;

			//Hide on close
			case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow( mWindow );
			break;
		}
	}
	else if( e.type == SDL_KEYDOWN )
	{
		//Display change flag
		bool switchDisplay = false;

		//Cycle through displays on up/down
		switch( e.key.keysym.sym )
		{
			case SDLK_UP:
			++mWindowDisplayID;
			switchDisplay = true;
			break;

			case SDLK_DOWN:
			--mWindowDisplayID;
			switchDisplay = true;
			break;
		}

		//Display needs to be updated
		if( switchDisplay )
		{
			//Bound display index
			if( mWindowDisplayID < 0 )
			{
				mWindowDisplayID = gTotalDisplays - 1;
			}
			else if( mWindowDisplayID >= gTotalDisplays )
			{
				mWindowDisplayID = 0;
			}

			//Move window to center of next display
			SDL_SetWindowPosition( mWindow, gDisplayBounds[ mWindowDisplayID ].x + ( gDisplayBounds[ mWindowDisplayID ].w - mWidth ) / 2, gDisplayBounds[ mWindowDisplayID ].y + ( gDisplayBounds[ mWindowDisplayID ].h - mHeight ) / 2 );
			updateCaption = true;
		}
	}

	//Update window caption with new data
	if( updateCaption )
	{
		std::stringstream caption;
		caption << "SDL Tutorial - ID: " << mWindowID << " Display: " << mWindowDisplayID << " MouseFocus:" << ( ( mMouseFocus ) ? "On" : "Off" ) << " KeyboardFocus:" << ( ( mKeyboardFocus ) ? "On" : "Off" );
		SDL_SetWindowTitle( mWindow, caption.str().c_str() );
	}
}

void LWindow::focus()
{
	//Restore window if needed
	if( !mShown )
	{
		SDL_ShowWindow( mWindow );
	}

	//Move window forward
	SDL_RaiseWindow( mWindow );
}

void LWindow::render()
{
	if( !mMinimized )
	{	
		//Clear screen
		SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( mRenderer );

		//Update screen
		SDL_RenderPresent( mRenderer );
	}
}

void LWindow::free()
{
	if( mWindow != NULL )
	{
		SDL_DestroyWindow( mWindow );
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int LWindow::getWidth()
{
	return mWidth;
}

int LWindow::getHeight()
{
	return mHeight;
}

bool LWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
	return mMinimized;
}

bool LWindow::isShown()
{
	return mShown;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Get number of displays
		gTotalDisplays = SDL_GetNumVideoDisplays();
		if( gTotalDisplays < 2 )
		{
			printf( "Warning: Only one display connected!" );
		}
		
		//Get bounds of each display
		gDisplayBounds = new SDL_Rect[ gTotalDisplays ];
		for( int i = 0; i < gTotalDisplays; ++i )
		{
			SDL_GetDisplayBounds( i, &gDisplayBounds[ i ] );
		}

		//Create window
		if( !gWindow.init() )
		{
			printf( "Window could not be created!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Destroy window
	gWindow.free();

	//Deallocate bounds
	delete[] gDisplayBounds;
	gDisplayBounds = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

//Main loop flag
bool quit = false;

void loop_handler(void*)
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while( SDL_PollEvent( &e ) != 0 )
	{
		//User requests quit
		if( e.type == SDL_QUIT )
		{
			quit = true;
		}

		//Handle window events
		gWindow.handleEvent( e );
	}

	//Update window
	gWindow.render();

}
 
int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
#ifdef _JS

              	emscripten_set_main_loop_arg(loop_handler, NULL, -1, 1);
#else
	//While application is running
		while( !quit )
		{
	 	 loop_handler(NULL);	
		}
#endif

	}

	//Free resources and close SDL
	close();

	return 0;
}
