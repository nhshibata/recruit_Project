//=========================================================
// 作成:2022/04/21
// Factoryメソッド
//=========================================================

//--- インクルードガード
#ifndef __COMPONENT_FACTORY_H__
#define __COMPONENT_FACTORY_H__

//--- インクルード部
//#include <Application/main.h>
#include <map>
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- 前方参照
		class CGameObject;
		class CComponent;
	}
}

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CComponentFactory
		{
		public:
			//--- ﾒﾝﾊﾞ関数
			CComponentFactory();
			~CComponentFactory() {};

			// *@オブジェクトにｺﾝﾎﾟｰﾈﾝﾄを追加
			// *@別クラスで登録されたｺﾝﾎﾟｰﾈﾝﾄを文字列から生成する
			static bool ObjSetComponent(CGameObject& obj, std::string name);

			// *@ｺﾝﾎﾟｰﾈﾝﾄ名全取得
			static std::vector<std::string> GetNameList();

			// *@名前からｺﾝﾎﾟｰﾈﾝﾄ取得
			//static std::shared_ptr<CComponent> CreateComponent(std::string name);

		/*	template <class T>
			static T* CreateComponent();

			template <class T>
			static std::shared_ptr<T> MakeComponent();*/
		};
	}
}
#endif // !__COMPONENT_FACTORY_H__

