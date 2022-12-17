//=========================================================
// [polygon.h] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __POLYGON_H__
#define __POLYGON_H__

//--- インクルード部
#include <GraphicsSystem/Render/vertex.h>
#include <CoreSystem/Math/MyMath.h>

#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数

namespace MySpace
{
	namespace Graphics
	{
		using namespace MySpace::MyMath;

		class CPolygon
		{
		private:
			//--- メンバ変数
			static ID3D11ShaderResourceView* m_pTexture;	// テクスチャへのポインタ

			static VERTEX_2D m_vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

			static Vector3 m_vPos;							// ポリゴンの移動量
			static Vector3 m_vAngle;						// ポリゴンの回転量
			static Vector3 m_vScale;						// ポリゴンのサイズ
			static Color m_vColor;						// ポリゴンの頂点カラー
			static bool m_bInvalidate;						// 頂点データ更新フラグ

			static XMFLOAT2 m_vPosTexFrame;					// UV座標
			static XMFLOAT2 m_vSizeTexFrame;				// テクスチャ抽出サイズ

			static ID3D11Buffer* m_pConstantBuffer;			// 定数バッファ
			static ID3D11Buffer* m_pVertexBuffer;			// 頂点バッファ
			static ID3D11SamplerState* m_pSamplerState;		// テクスチャ サンプラ
			static ID3D11VertexShader* m_pVertexShader;		// 頂点シェーダ
			static ID3D11InputLayout* m_pInputLayout;		// 頂点フォーマット
			static ID3D11PixelShader* m_pPixelShader;		// ピクセルシェーダ

			static XMFLOAT4X4 m_mProj;						// 射影変換行列
			static XMFLOAT4X4 m_mView;						// ビュー変換行列
			static XMFLOAT4X4 m_mWorld;						// ワールド変換行列
			static XMFLOAT4X4 m_mTex;						// テクスチャ変換行列

		private:

			//--- メンバ関数
			static HRESULT MakeVertex(ID3D11Device* pDevice);
			static void SetVertex(void);

		public:
			static HRESULT Init(ID3D11Device* pDevice);
			static void Fin();
			static void Draw(ID3D11DeviceContext* pDeviceContext);

			// *@テクスチャの設定
			static inline void SetTexture(ID3D11ShaderResourceView* pTexture)
			{
				m_pTexture = pTexture;
				m_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
			}
			// *@表示座標の設定
			static inline  void SetPos(float fX, float fY) { m_vPos.x = fX; m_vPos.y = fY; }
			// *@表示座標の設定
			static inline  void SetPos(Vector2 pos) { m_vPos.x = pos.x; m_vPos.y = pos.y; }

			// *@表示サイズの設定
			static inline void SetSize(float fScaleX, float fScaleY) { m_vScale.x = fScaleX; m_vScale.y = fScaleY; }
			// *@表示サイズの設定
			static inline void SetSize(Vector2 size) { m_vScale.x = size.x; m_vScale.y = size.y; }

			// *@表示角度の設定(単位:度)
			static inline void SetAngle(float fAngle) { m_vAngle.z = fAngle; }

			// *@左上テクスチャ座標の設定 (0.0≦fU＜1.0, 0.0≦fV＜1.0)
			static inline void SetUV(float fU, float fV) { m_vPosTexFrame.x = fU; m_vPosTexFrame.y = fV; }
			static inline void SetUV(XMFLOAT2 uv) { m_vPosTexFrame = uv; }

			// *@テクスチャフレームサイズの設定 (0.0＜fWidth≦1.0, 0.0＜fHeight≦1.0)
			static inline void SetFrameSize(float fWidth, float fHeight) { m_vSizeTexFrame.x = fWidth; m_vSizeTexFrame.y = fHeight; }
			// *@テクスチャフレームサイズの設定 (0.0＜fWidth≦1.0, 0.0＜fHeight≦1.0)
			static inline void SetFrameSize(XMFLOAT2 size) { m_vSizeTexFrame = size; }

			// *@頂点カラーの設定
			static inline void SetColor(float fRed, float fGreen, float fBlue) { SetColor(XMFLOAT3(fRed, fGreen, fBlue)); }
			// *@頂点カラーの設定
			static inline void SetColor(XMFLOAT3 vColor)
			{
				if (m_vColor != vColor)
				{
					m_vColor = vColor;
					m_bInvalidate = true;
				}
			}

			// *@不透明度の設定
			static inline void SetAlpha(float fAlpha)
			{
				if (fAlpha == m_vColor.a)
					return;
				m_vColor.a = fAlpha;
				m_bInvalidate = true;
			}

			// *@頂点カラーの設定
			static inline void SetColor(float fRed, float fGreen, float fBlue, float fAlpha)
			{
				SetColor(XMFLOAT4(fRed, fGreen, fBlue, fAlpha));
			}
			// *@頂点カラーの設定
			static inline void SetColor(XMFLOAT4 vColor)
			{
				if (m_vColor != vColor)
				{
					m_vColor = vColor;
					m_bInvalidate = true;
				}
			}

		};

	}
}

#endif // !__POLYGON_H__
