//=========================================================
// [startUp.cpp]
//---------------------------------------------------------
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/04/19 �R���|�[�l���g�A�Q�[���I�u�W�F�N�g�A�V�[���Bunity���Q�l��tag��layer���`�B
// �X�V:2022/04/20 cpp�̎����Bcollision�Atransform�Acursor�Afps�쐬
// �X�V:2022/04/21 Factory�׽�Aplayer�׽�̎���
// 			!�ۑ�! �I�u�W�F�N�g�̃g�����X�t�H�[���̌��ߕ�(���W�A�p�x�A�傫��)�▼�O�A�ׂ����R���|�[�l���g�̒ǉ����ǂ��������邩
// 		�`��͂��邪�A���̑��̍X�V���������Ȃ��I�u�W�F�N�g�͂ǂ����邩?�X���[���̊e�X�V�͂ǂ����삷�邩?
// �X�V:2022/04/22 �����C���B
// �X�V:2022/04/24 �����蔻��X�V�B
//			!�ۑ�! UI�֘A�̕`�揇�̌��ߕ��B���݂�Component�ł͂Ȃ��A�h���׽DrawComponent���`���A�`�揇�����߂�ϐ���݂���B
// �X�V:2022/06/03 �ǉ��͂��Ă������̂́A�L�^�͎c���Ă��Ȃ��B���Ƃ�����������?
//			�\�z: UI�͂�͂�UI������`��˗���\��������@�������B�|�C���^�Ȃǂ�n�����C���[���ŕ��т����B
// �X�V:2022/06/07 �^�O�ɔY�ݒ��B�񋓑̂ł͂Ȃ�������Ƃ��Ďg���ꍇ�A�ǂ������ׂ���?
//			�|�C���^���X�}�[�g�|�C���^�ɕύX�B���L����|�C���^�Ǝ�Q�Ƃ̃|�C���^���ǂ������ׂ���?
//			���N���X�𕡐��쐬
// �X�V:2022/06/08 ��̃X�}�[�g�|�C���^�ɕύX�B����ڰĸ׽�ɂ��Ȃ�܂����i�G���[���킩��ɂ����i�p��?
//			�������Bstd::list<std::weak_ptr<*>>��std::find�̑g�ݍ��킹�����������悤�B���|�C���^�̔�r�ŉ���
// �X�V:2022/06/09 ���r���[������OnCollision�I���B
// �X�V:2022/06/10 
// �X�V:2022/06/20 �E�B���h�E���� 
// �X�V:2022/08/20 ���݂��v���o���B�Ȃ񂩐F�X�ǉ�
//				�v���W�F�N�g�𕡐��ɕ����B
// �X�V:2022/08/23 �ۑ萧��J�n�B��i�𐧍삵�ďo���G���[��s�ւ��΍�
// �X�V:2022/08/25 Unity�̃V�[���J�ڎ��̌Ăяo���֐�(�f���Q�[�g?)����
//				std::funcction,std::bind��p�����֐��|�C���^����сA�ϒ�����ڰĈ��������g���čČ�
// �X�V:2022/08/28�`8/31 UE�Ղ��R�������Ƃŋx��
// �X�V:2022/09/01 ����ĊJ�B���J�f�̕����\��
// �X�V:2022/11/01 �C���^�[���Œ�~���Ă����̂ōĊJ�B
// �X�V:2022/12/07 �`��V�X�e���ⓖ���蔻��V�X�e�����P������
//					NavMesh��AISystem(Behavior)���ڐA
// �X�V:2022/12/08 ����}�ɔ�������C4743�icpp���̃o�C�g�T�C�Y�Ⴂ�j�x��������
//					���z�֐��̐錾�Ǝ����𓯎��ɍs���Ă����̂�����?�炵���Bcpp��h�ł킯�邱�Ƃŉ���
// 
//
// ���e:�R���|�[�l���g�w�����x�[�X�Ƃ����v���O�����B
// ��������Ɍl����̃Q�[���𐧍삷��B
// 
//=========================================================

#define NOMINMAX

//--- �C���N���[�h��
#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <memory>
#include <time.h>
#include <tchar.h>

#include <Application/main.h>
#include <GameSystem/Manager/sceneManager.h>
//#include <gameCentipedeMarch.h>

// ���C�u�����Q��
// �v���p�e�B�Ŏw�肷�邩�A�����Ŏw�肷�邩�c�Y�݂ǂ���
#pragma region LIBRARY

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex")
#else
#pragma comment(lib, "DirectXTex")
#endif // _DEBUG

#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")

#pragma comment(lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")	

//#pragma comment(lib, "Application")
//#pragma comment(lib, "CoreSystem")
//#pragma comment(lib, "DebugSystem")
//#pragma comment(lib, "GameSystem")
//#pragma comment(lib, "GraphicsSystem")
#pragma endregion

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	using namespace MySpace::SceneManager;

	// *@�V�[���쐬���ɌĂяo���׽
	// *@�ÓI�ȃV�[���쐬�̏ꍇ�A���I�ȍ쐬�̓t�@�C���������݂Ɠǂݍ��݂��s��
	// *@�ʏ�̊֐��ł������悤��
	class CMyScene
	{
	public:
		CMyScene()
		{
		}
		void* Load(CScene* scene, int mode)
		{
#if 0
			//auto obj = scene->GetObjManager()->CreateGameObject();
			//obj->AddComponent<CGameCentipedeMarch>();
#endif // 0
			scene->SetSceneName("BuildeScene");
			scene->CreateEmptyScene();
			return nullptr;
		}
	};

	CMyScene my;
	// �V�[���ǂݍ��ݎ��Ăяo���֐���ݒ�
	CSceneManager::Get().SceneLoaded<CMyScene>(&CMyScene::Load, &my);
	
	// �J�n
	// �E�B���h�E�̐����Ȃǂ��s��
	StartUp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// �Q�[�����[�v
	MainLoop();

	// �I��
	ShutDown();

	return 0;
}