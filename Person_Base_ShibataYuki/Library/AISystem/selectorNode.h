//========================================================
// [SequencerNode.h]
//------------------
// �쐬:2022/12/02
//------------------
// �q�m�[�h�̂����ꂩ�����s����
// ���s�ł���Ώ������I����
//========================================================

#ifndef __SELECTOR_NODE_H__
#define __SELECTOR_NODE_H__

//--- �C���N���[�h��
#include "parentNode.h"

namespace AI
{
	class CSelectorNode : public CParentNode
	{
	private:

	public:

		// *@�m�[�h���s�֐�
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

#endif // !__SELECTOR_NODE_H__