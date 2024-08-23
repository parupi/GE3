#pragma once
#include "DirectXManager.h"

// スプライト共通部
class SpriteManager
{
public:
	void Initialize(DirectXManager* directXManager);


private:

	DirectXManager* dxManager = nullptr;

};