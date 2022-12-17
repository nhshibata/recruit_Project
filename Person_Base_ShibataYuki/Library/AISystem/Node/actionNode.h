//========================================================
// [actionNode.h]
//------------------
// �쐬:2022/12/04
//------------------
// ���s�m�[�h
// �q�͎��ĂȂ�
//========================================================

//--- �C���N���[�h��
#include <AISystem/Node/aiNode.h>

#ifndef __ACTION_NODE_H__
#define __ACTION_NODE_H__

namespace AI
{
	class CActionNode : public CAINode
	{
	public:
		// *@�m�[�h���s�֐�
		virtual CAINode::EStatus Execute(CAISystem* sys) = 0;
	};
}

#endif // !__ACTION_NODE_H__
