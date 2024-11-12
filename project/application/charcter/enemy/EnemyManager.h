#pragma once
#include <Enemy.h>
#include <list>
class EnemyManager
{
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();

private:
	std::list<Enemy*> enemies_;
	float stageTimer = 0.0f;
	int popNum = 0;

public:
	std::list<Enemy*> GetEnemy() { return enemies_; }
};

