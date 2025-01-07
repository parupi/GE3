#pragma once
#include <memory>
#include <charcter/enemy/Enemy.h>
class EnemyManager
{
public:
	void Initialize(Player* player);

	void Update();

	void Draw();


private:
	std::array<std::unique_ptr<Enemy>, 10> enemy_;

	Player* player_;

	uint32_t deadNum_ = 0;

public:
	Enemy* GetEnemy(int i) { return enemy_[i].get(); }

	uint32_t GetDeadNum() { return deadNum_; }
	void SetDeadNum(uint32_t deadNum) { deadNum_ = deadNum; }
};

