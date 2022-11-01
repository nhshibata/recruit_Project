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

#define NUM_CONTROL_POINTS 1

////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]

//PixelInputType ColorDomainShader(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
PixelInputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> patch)
{
    float4 vertexPosition;
    PixelInputType output;
    
    static const float3 offsets[] =
    {
        { 0.5f, 0.5f, 0.f },
        { -0.5f, 0.5f, 0.f },
        { 0.0f, -0.5f, 0.f },
    };
    
 	// Determine the position of the new vertex.
    vertexPosition = float4(offsets[0] * uvwCoord.x + offsets[1] * uvwCoord.y + offsets[2] * uvwCoord.z, 1);
    //vertexPosition = float4(uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position, 1);
    
	// Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Send the input color into the pixel shader.
    output.color = patch[0].color;

    return output;
}