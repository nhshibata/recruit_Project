//=========================================================
// [vertex.h] 
// 作成:2022/07/25
//---------------------------------------------------------
// 構造体定義
//=========================================================

//--- インクルードガード
#ifndef __VERTEX__H__
#define __VERTEX__H__

//--- インクルード部
#include <DirectXTex.h>

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// 頂点座標
	DirectX::XMFLOAT4 diffuse;	// 拡散反射光
	DirectX::XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_2D;

// 頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 反射光 / テクスチャ座標 )
typedef struct {
	DirectX::XMFLOAT3 vtx;		// 頂点座標
	DirectX::XMFLOAT3 nor;		// 法線ベクトル
	DirectX::XMFLOAT4 diffuse;	// 拡散反射光
	DirectX::XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_3D;


#endif // !__VERTEX__H__
