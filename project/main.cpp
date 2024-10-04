#include "MyGameTitle.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	MyGameTitle game;

	game.Initialize();

	while (true) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (game.IsEndRequest()) {
			// ゲームループを抜ける
			break;
		}

		game.Update();

		game.Draw();
	}
	game.Finalize();


	return 0;
}
