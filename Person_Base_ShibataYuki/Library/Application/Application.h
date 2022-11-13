//=========================================================
// [Application.cpp]
// �Q�[�����s�׽
//------------------------
// �쐬:2022/05/24
// �X�V:2022/11/12 FixedUpdate����
// 
//=========================================================


//--- �C���N���[�h�K�[�h
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//--- �C���N���[�h��
#define NOMINMAX
#include <windows.h>
#include <windef.h>
#include <d3d11.h>

#include <cinttypes>
#include <stdio.h>
#include <map>
#include <CoreSystem/Singleton.h>
#include <CoreSystem/property.h>

//#pragma comment(lib, "winmm.lib")	// winmm.lib���g�p����

namespace MySpace
{
	namespace System
	{
		class CSystemManager;
	}
}
using MySpace::System::CSingleton;

//--- �N���X��`
class Application : public CSingleton<Application>
{
	friend class CSingleton<Application>; // Singleton �ł̃C���X�^���X�쐬�͋���
private:
	//--- �����o�ϐ�
	HWND					m_hWnd;						// Window�n���h��
	HINSTANCE				m_hInst;					// �C���X�^���X�n���h��
	uint32_t				m_SystemCounter;			// �V�X�e���J�E���^
	FILE*					m_fp;						// �f�o�b�O�p�R���\�[��

private:
	//--- �����o�֐�
	Application() { };									// �R���X�g���N�^
	~Application() {};									// �f�X�g���N�^

	//Application(const Application&);					// �R�s�[
	//Application& operator = (const Application&) {}		// =

public:

	bool Init(HINSTANCE h_cpInstance);					// �V�X�e���L����
	
	void Uninit();										// �V�X�e��������
	unsigned long MainLoop();							// ���C�����[�v

	// �����o�擾�֐�
	inline HWND GetHWnd() { return m_hWnd; };			// �E�B���h�E�n���h��
	inline HINSTANCE GetHInst() { return m_hInst; };	// �C���X�^���X�n���h��
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

};

#endif // !__APPLICATION_H__