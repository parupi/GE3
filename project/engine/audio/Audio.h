#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <fstream>
#include <wrl.h>
#include <mutex>
class Audio
{
	static Audio* instance;
	static std::once_flag initInstanceFlag;

	Audio() = default;
	~Audio() = default;
	Audio(Audio&) = default;
	Audio& operator=(Audio&) = default;
public:

	// シングルトンインスタンスの取得
	static Audio* GetInstance();
private: // 構造体
	// チャンクヘッダー
	struct ChunkHeader {
		char id[4]; // チャンクごとのID
		int32_t size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; // "fmt"
		WAVEFORMATEX fmt; // 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		WAVEFORMATEX wfex; // 波形フォーマット
		BYTE* pBuffer; // バッファの先頭アドレス
		unsigned int bufferSize; // バッファのサイズ
	};

public:
	// 初期化
	void Initialize();
	// 音声読み込み
	SoundData SoundLoadWave(const char* filename);
	// 音声データ解放
	void SoundUnload(SoundData* soundData);
	// 音声の再生
	void SoundPlayWave(const SoundData& soundData);
	// 終了
	void Finalize();
private:
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};

