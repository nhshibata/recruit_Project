//==============================================
// [effect.h]
//-----------------------
// 2021/12/13 �쐬 �ēc�E�M
//				�Ƃ肠�����`��m�F
// 2021/12/17 �X�V �������ŃG�t�F�N�g�����[�h�ł���悤�ɕύX
// 2022/07/12 �O���[�v����ō쐬����cpp,h�����C�u�������ڐA
//					
//==============================================

#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

// --- �C���N���[�h�� ---
//#include <Application/main.h>
#include <GraphicsSystem/Manager/resourceMap.h>
#include <CoreSystem/Singleton.h>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/stl.h>

#include <Effekseer/Effekseer.h>
#include <Effekseer/EffekseerRendererDX11.h>

// Release�p(DEBUG�ł͂Ȃ�)
#ifndef _DEBUG

#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")

#else
// DEBUG�p

#pragma comment(lib, "EffekseerD.lib")
#pragma comment(lib, "EffekseerRendererDX11D.lib")

#endif // !_DEBUG

#define EFFECT_PATH				u"../Data/effect/"
//#define EFFECT_PATH(name)  std::u16string("a")
//#define EFFECT_PATH3(name)  u\FORDER_DIR(Data/effect/\name)

namespace MySpace
{
	namespace Graphics
	{
		class CDXDevice;
	}
}

namespace MySpace
{
	namespace System
	{
		// �G�C���A�X
		using MySpace::Graphics::CResourceMap;

		// �N���X�̑O���Q��
		class CEffekseerParam;	
		// --- �N���X��` ---
		class CEffekseer : public CSingleton<CEffekseer>, public CResourceMap<std::u16string, Effekseer::EffectRef>
		{
			friend class CSingleton<CEffekseer>;

		private:
			// ���_�V�F�[�_�ɑ���ϊ��s��
			struct Matrix
			{
				DirectX::XMFLOAT4X4 world;
				DirectX::XMFLOAT4X4 view;
				DirectX::XMFLOAT4X4 proj;
			};
		private:
			// --- �G�C���A�X ---
			using EffectMap = std::map<std::u16string, Effekseer::EffectRef>;			// �G�t�F�N�g�̎�ޕʃf�[�^�i�[
			using EffectMapPair = std::pair<std::u16string, Effekseer::EffectRef>;	// �p�C��
			using EffectParam = std::vector<CEffekseerParam>;						// ��������G�t�F�N�g�̃p�����[�^

			// --- �����o�ϐ� ---
			//static CEffekseer* m_pInstance;
			DirectX::XMFLOAT4 m_psLight;
			DirectX::XMFLOAT4 m_psParameter;
			Matrix m_vsMatrix;

			EffekseerRendererDX11::RendererRef m_renderer;
			Effekseer::ManagerRef m_manager;
			// 
			EffectParam m_effectParam;
			int m_nTimer;

			// --- �����o�֐� ---
			void SetMatrix(Effekseer::Matrix43*, XMFLOAT4X4);
			CEffekseer();
			~CEffekseer();
		public:
			//~CEffekseer();

			void Init(ID3D11Device* device, ID3D11DeviceContext* context);
			void Uninit();
			void Update();
			void Draw();

			inline bool Exists(int handle) { return m_manager->Exists(handle);};	// false�͍Đ����Ă��Ȃ�
			bool Load(std::u16string fileName);
			bool Load(std::vector<std::string> EffectName, std::vector<std::u16string> fileName);	// 2�o�C�g�����̎擾
			int Play(std::u16string EffectName, XMFLOAT3 pos);
			int Play(std::u16string EffectName, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT4 rot = XMFLOAT4(0, 0, 0, 0));
			int Stop(int handle);
			int Move(int handle, XMFLOAT3 pos);

		/*	std::vector<std::u16string> GetNameList() {
				std::vector<std::u16string> ret;
				std::copy(m_ResourceMap.begin(),
					m_ResourceMap.end(),
					std::back_inserter(ret));
				return ret;
			}*/
		};

		// �׽��`
		// �p�����[�^�׽
		class CEffekseerParam
		{
		private:
			Effekseer::EffectRef* m_pEffect;	// �`�悷��G�t�F�N�g�̎�ރ|�C���^
			Effekseer::Handle m_handle;			// �G�t�F�N�g�̃n���h��(�}�l�W�����F������ϐ�)
			XMFLOAT3 m_vPos;					// 
			XMFLOAT4 m_vRot;					// 
			XMFLOAT3 m_vScale;					// �T�C�Y
			int m_nMaxFream;					// �`��ő厞��
			int m_nCurrentFream;				// �G�t�F�N�g�`��J�E���g
		public:
			CEffekseerParam() :m_vPos(0, 0, 0), m_vRot(0, 0, 0, 0), m_vScale(1, 1, 1)
			{};
			CEffekseerParam(Effekseer::EffectRef* effect, Effekseer::Handle handle, int time)
				:m_vPos(0, 0, 0), m_vRot(0, 0, 0, 0), m_vScale(1, 1, 1), m_pEffect(effect), m_handle(handle), m_nCurrentFream(0), m_nMaxFream(time)
			{};
			~CEffekseerParam()
			{};

			inline void SetPos(float x, float y, float z) { m_vPos = XMFLOAT3(x, y, z); };
			inline void SetPos(XMFLOAT3 pos) { m_vPos = pos; };
			inline void SetRot(XMFLOAT4 rot) { m_vRot = rot; };
			inline void SetScale(XMFLOAT3 size) { m_vScale = size; };
			inline void SetTime(int time) { m_nMaxFream = time; };
			inline void SetHandle(Effekseer::Handle handle) { m_handle = handle; };
			inline XMFLOAT3 GetPos()const { return m_vPos; };
			inline XMFLOAT4 GetRot()const { return m_vRot; };
			inline XMFLOAT3 GetScale()const { return m_vScale; };
			inline Effekseer::EffectRef* GetEffect()const { return m_pEffect; };
			inline Effekseer::Handle GetHandle()const { return m_handle; };
			inline bool IsPlay() { if (++m_nCurrentFream >= m_nMaxFream) { return false; } return true; };

		};
	}
}

#endif // !__EFFEKSEER_MANAGER_H__
