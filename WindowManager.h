#pragma once
#include <Windows.h>
class WindowManager
{
public:
	// �N���C�A���g�̈�̃T�C�Y
	static const int kClientWidth = 1280;
	static const int kClientHeight = 720;

public: // �����o�ϐ�
	// ������
	void Initialize();
	// �X�V
	void Update();

private:
	// Window�֘A
	HWND hwnd_ = nullptr;   // �E�B���h�E�n���h��
	WNDCLASSEX wndClass_{}; // �E�B���h�E�N���X
};

