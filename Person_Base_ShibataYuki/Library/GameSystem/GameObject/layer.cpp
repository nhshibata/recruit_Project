//=========================================================
// [layer.h]
//---------------------------------------------------------
//�쐬:2022/04/19
//---------------------------------------------------------
// ���C���[�׽ : ���i�׽
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/layer.h>

namespace MySpace
{
	namespace Game
	{
		//--- �ÓI�����o�ϐ�
		std::map<int, std::string> CLayer::m_aNoList = {
				{ int(E_Layer::SKY),	NAME_TO(E_Layer::SKY) },
				{ int(E_Layer::BG),		NAME_TO(E_Layer::BG) },
				{ int(E_Layer::DEFAULT),NAME_TO(E_Layer::DEFAULT) },
				{ int(E_Layer::MODEL),	NAME_TO(E_Layer::MODEL) },
				{ int(E_Layer::UI),		NAME_TO(E_Layer::UI) },
				{ int(E_Layer::FOG),	NAME_TO(E_Layer::FOG) },
		};

	}
}