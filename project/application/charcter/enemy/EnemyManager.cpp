#include "EnemyManager.h"

void EnemyManager::Initialize()
{
}

void EnemyManager::Finalize()
{
	for (Enemy* enemy : enemies_) {
		enemy->Finalize();
		delete enemy;
	}
}

void EnemyManager::Update()
{
	stageTimer += 1.0f / 3600.0f;

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// デスフラグの立った弾を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (!enemy->GetIsAlive()) {
			delete enemy;
			return true;
		}
		return false;
	});

	if (stageTimer >= 0.1f && popNum == 0) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({50.0f, 8.0f, 5.0f * i + 5.0f});
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.2f && popNum == 1) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 65.0f, 10.0f, 5.0f * i + 20.0f});
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.3f && popNum == 2) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 90.0f, 10.0f, 10.0f * i});
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.4f && popNum == 3) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 110.0f, 10.0f, 3.0f * i + 20.0f });
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.5f && popNum == 4) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 140.0f, 10.0f, 5.0f * i });
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.6f && popNum == 5) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 160.0f, 7.0f, 5.0f * i + 5.0f});
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.7f && popNum == 6) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 180.0f, 8.0f, 5.0f * i - 2.0f});
			enemies_.push_back(newEnemy);
		}
	}
	if (stageTimer >= 0.8f && popNum == 7) {
		popNum += 1;
		for (int i = 0; i < 3; i++) {
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize({ 200.0f, 10.0f, 5.0f * i - 10});
			enemies_.push_back(newEnemy);
		}
	}
}

void EnemyManager::Draw()
{
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
}
