#pragma once
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix4x4.h>
#include <vector>
#include <map>
#include <string>

class Model;

class Animator {
public:
	// 初期化
	void Initialize(Model* model, const std::string& directoryPath, const std::string& filename);
	// 再生
	void Update();

private: // 構造体
	template <typename tValue>
	struct Keyframe {
		float time;
		tValue value;
	};
	using KeyframeVector3 = Keyframe<Vector3>;
	using KeyframeQuaternion = Keyframe<Quaternion>;

	template<typename tValue>
	struct AnimationCurve {
		std::vector<Keyframe<tValue>> keyframes;
	};
	
	struct NodeAnimation {
		AnimationCurve<Vector3> translate;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> scale;
	};

	struct Animation {
		float duration; // アニメーション全体の尺
		std::map<std::string, NodeAnimation> nodeAnimations;
	};
private:
	// アニメーションを読む処理
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);
	// 任意の時刻を取得する (Vector3)
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	// 任意の時刻を取得する (Quaternion)
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);
private:
	// アニメーション
	Animation animation;
	// モデル
	Model* model_;
	// 現在のアニメーションタイム
	float animationTime = 0.0f;
	// アニメーション用行列
	Matrix4x4 localMatrix;

public: // ゲッター // セッター //
	Matrix4x4 GetLocalMatrix() const { return localMatrix; }

};