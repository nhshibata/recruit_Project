//=========================================================
// [drawSystem.h]
//---------------------------------------------------------
// 作成:2023/03/??
// 更新:2023/03/15 シェーダー情報を構造体に変更
//---------------------------------------------------------
// 派生クラス
// 影描画や通常描画、基本部分の定義、実装
//=========================================================

//--- インクルードガード
#ifndef __DRAW_SYSTEM_BASE_H__
#define __DRAW_SYSTEM_BASE_H__

//--- インクルード部
#include <GameSystem/Manager/mapSystemBase.h>
#include <GraphicsSystem/DirectX/depthShadow.h>
#include <GraphicsSystem/Shader/instancingData.h>

#include <DirectXMath.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
		class CPolygonRenderer;
	}
	namespace Graphics
	{
		class CMesh;
	}
}

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CRenderer;
		using MySpace::Graphics::CMesh;

		//--- クラス定義
		class CDrawSystemBase : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
			friend class CRenderer;
			friend class CPolygonRenderer;
		protected:
#pragma region Struct
			//--- 構造体定義
			// IDをキャッシュする
			struct ST3DData
			{
				std::vector<int> aID;
			};
			// 継承し、描画のためのﾎﾟｲﾝﾀを保持
			// 処理の関係で、nullptrやdeleteの心配はしない
			struct STMeshData : public ST3DData
			{
				CMesh* pMesh;
			};

			// ゲームオブジェクトから渡される描画情報格納
			// シェーダー切替に対応するため
			// mapのKeyとして使う
			struct STDrawInfo
			{
				std::string strName;
				std::string strPixel;
				std::string strVertex;

				// *@コンストラクタ
				STDrawInfo() 
				{}
				// *@コンストラクタ
				STDrawInfo(std::string name, std::string ps, std::string vs)
					:strName(name),strPixel(ps), strVertex(vs)
				{}

				// *@演算子のオーバーロード
				// *@mapは挿入位置を参照するために比較するため、作成しなければならない
				bool operator <(const STDrawInfo& other)const
				{
					if (strName != other.strName)
						return strName < other.strName;
					else if (strPixel != other.strPixel)
						return strPixel < other.strPixel;
					else
						return strVertex < other.strVertex;
				}
				// *@いずれかが存在しないエラー
				bool IsError()const
				{
					return (strName.empty() | strPixel.empty() | strVertex.empty());
				}
			};
#pragma endregion

			//--- エイリアス
			using PolygonRenderWeakList = std::vector<std::weak_ptr<CPolygonRenderer>>;
			using InstancingMap = std::map<STDrawInfo, ST3DData>;
			using InstancingMeshMap = std::map<STDrawInfo, STMeshData>;

		protected:
			//--- メンバ変数
			bool m_bIsSortNecessary;							// 整列実行フラグ
			PolygonRenderWeakList m_aPolygonList;				// 管理しているmapをソートした結果を入れる
			InstancingMap m_aInstancingModelMap;				// インスタンシング描画格納用
			InstancingMeshMap m_aInstancingMeshMap;				// インスタンシング描画格納用
			std::unique_ptr<Graphics::CDepthShadow> m_pDepthShadow;	// 深度書き込み用

#if BUILD_MODE
			// 確認用変数
			int m_nSkipCnt;
			int m_nDrawCnt;
			int m_nInstancingCnt;
			bool m_bFrustum;
			bool m_bShadowView;
			std::map<std::string, std::vector<CMesh*>> m_aDebugMeshMap;		// インスタンシング描画格納用
#endif // _DEBUG

		protected:
			//--- メンバ関数
			// *@ポリゴン整列
			void Sort();
			// *@整列ﾌﾗｸﾞ
			inline void SortOn() { m_bIsSortNecessary = true; }
			
			// *@3Dインスタンシング影描画
			void Draw3DShadow();
			// *@3Dインスタンシング描画
			virtual void Draw3D();
			// *@3Dオブジェクトの描画確認
			virtual void CheckRenderedObjectsIn3D();

		protected:
			// *@描画依頼時に連結した文字列を分割
			// *@0:ﾃｸｽﾁｬやﾓﾃﾞﾙ名 1:pixel 2:vertex
			const STDrawInfo TextSplitToNamePSVS(const std::string name);

		public:
			CDrawSystemBase();
			virtual ~CDrawSystemBase();

			void Init();
			virtual void Update();

			// *@登録 override
			_NODISCARD int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				return ret;
			}

			// *@登録
			_NODISCARD int PolygonRegist(std::weak_ptr<CPolygonRenderer> render);

			// *@破棄 overlide
			bool ExecutSystem(const int idx);

			// *@インスタンシング描画のために情報を格納する
			void SetInstanchingModel(std::string name, std::string ps, std::string vs, const int id);

			// *@インスタンシング描画のために情報を格納する
			void SetInstanchingMesh(std::string name, std::string ps, std::string vs, const int id, CMesh* mesh);

			// *@所持リスト
			_NODISCARD std::vector<std::weak_ptr<CRenderer>> GetList();

#if BUILD_MODE
			virtual void ImGuiDebug();
			// *@インスタンシング描画のために情報を格納する
			void SetDebugMesh(std::string name, CMesh* mesh);
			void ReleaseDebugMesh(const CMesh* mesh);
#endif // BUILD_MODE

		};

	}
}

#endif