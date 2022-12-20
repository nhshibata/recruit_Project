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

#define NOMINMAX

//--- �C���N���[�h��
#include <windows.h>
#include <windef.h>
#include <d3d11.h>

#include <cinttypes>
#include <stdio.h>
#include <map>
#include <CoreSystem/Singleton.h>
#include <CoreSystem/property.h>
#include <CoreSystem/Util/define.h>


using MySpace::System::CSingleton;

//--- �N���X��`
class Application : public CSingleton<Application>
{
	friend class CSingleton<Application>; // Singleton �ł̃C���X�^���X�쐬�͋���
private:
	//--- �����o�ϐ�
	HWND					m_hWnd;						// Window�n���h��
	HINSTANCE				m_hInst;					// �C���X�^���X�n���h��
	std::map<std::string, void*>	m_aSystems;

private:
	//--- �����o�֐�
	Application() = default;							// �R���X�g���N�^
	~Application() = default;							// �f�X�g���N�^

public:
	bool Init(HINSTANCE h_cpInstance);					// �V�X�e���L����
	
	void Uninit();										// �V�X�e��������
	unsigned long MainLoop();							// ���C�����[�v

	//--- �Q�b�^�[�E�Z�b�^�[
	inline HWND GetHWnd() { return m_hWnd; };			// �E�B���h�E�n���h��
	inline HINSTANCE GetHInst() { return m_hInst; };	// �C���X�^���X�n���h��
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	// *@�V�X�e���ւ̐��߲���ɂ��ǉ�
	template <class T>
	T* AddSystem()
	{
		std::string name = typeid(T).name();
		if (m_aSystems.count(name))
		{
			return m_aSystems[name];
		}

		m_aSystems[name] = new T;
		return m_aSystems[name];
	}

	// *@�^�w��ɂ���߲���̎擾
	template <class T>
	T* GetSystem()
	{
		std::string name = typeid(T).name();
		if (!m_aSystems.count(name))
			return nullptr;
		T* ret = static_cast<T*>(m_aSystems[name]);
		return ret;
	}
};

#endif // !__APPLICATION_H__