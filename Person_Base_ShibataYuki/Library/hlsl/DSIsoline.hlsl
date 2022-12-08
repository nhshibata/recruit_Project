
/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
// ドメインシェーダの出力
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

// ハルシェーダからの出力制御点
struct HullOutputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

// 出力定数ﾃﾞｰﾀ
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

#define NUM_CONTROL_POINTS 2

////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("isoline")]

//PixelInputType ColorDomainShader(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
PixelInputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> patch)
{
    float4 vertexPosition = float4(0,0,0,0);
    PixelInputType output;

    //適当に位置をずらしている
	//実際にはパッチ定数関数の出力値を元に処理を行う形になると思う。
    vertexPosition = float4(lerp(patch[0].position, patch[1].position, uvwCoord.x), 1);
    vertexPosition.x += 0.5f * uvwCoord.y;
	//上に凸な曲線を描くためにyの位置をずらしている。
    const float RATE = abs(uvwCoord.x - 0.5f) * 2.f;
    const float PI = 3.14159265f;
    vertexPosition.y = cos(RATE * 0.5f * PI);
    
	// Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Send the input color into the pixel shader.
    output.color = patch[0].color;

    return output;
}