#include "MyGameTitle.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GuchisFramework* game = new MyGameTitle();

	game->Run();

	delete game;

	return 0;
}
