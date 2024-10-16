#pragma once
#include <function.h>
class ParticleEmitter
{
public:
	// 初期化
	void Initialize(std::string name);
	// 更新
	void Update(Vector3 Position, uint32_t count);
	// 発生
	void Emit();

private:

	struct Emitter {
		std::string name; //!< パーティクルの名前
		Transform transform; //!< エミッタのTransform
		uint32_t count; //!< 発生数
		float frequency; //!< 発生頻度
		float frequencyTime; //!< 頻度用時刻
	};

private:

	Emitter emitter{};
	float kDeltaTime = 1.0f / 60.0f;
};