#if 0

uint2 random(uint stream, uint sequence)
{
    uint2 v = uint2(stream, sequence);
    uint s = 0x9E3779B9u;
	[unroll]
    for (int i = 0; i < 5; ++i)
    {
        v.x += ((v.y << 4u) + 0xA341316Cu) ^ (v.y + s) ^ ((v.y >> 5u) + 0xC8013EA4u);
        v.y += ((v.x << 4u) + 0xAD90777Du) ^ (v.x + s) ^ ((v.x >> 5u) + 0x7E95761Eu);
        s += 0x9E3779B9u;
    }
    return v;
}
float4 calColor(float2 domain)
{
    float4 color = 1;
    uint rnd_seed = domain.x * 123123123 + domain.y * 456456456;
    uint2 r = random(rnd_seed, rnd_seed << 3);
    r = random(r.x, r.y);
    color.r = r.x / (float) 0xffffffff;
    r = random(r.x, r.y);
    color.g = r.x / (float) 0xffffffff;
    r = random(r.x, r.y);
    color.b = r.x / (float) 0xffffffff;
    return color;
}

struct DS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : TEXCOORD0;
};

// 出力制御点
struct HS_CONTROL_POINT_OUTPUT
{
    float3 pos : POSITION;
};

// 出力パッチ定数データ。
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[4] : SV_TessFactor;
    float InsideTessFactor[2] : SV_InsideTessFactor;
};

//#define NUM_CONTROL_POINTS 4

[domain("quad")]
DS_OUTPUT main(HS_CONSTANT_DATA_OUTPUT input,float2 domain : SV_DomainLocation,	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
    DS_OUTPUT output;
    
	//4点間の補間 bilinear interpolation
    output.pos = float4(patch[0].pos * (1 - domain.x) * (1 - domain.y) + patch[1].pos * domain.x * (1 - domain.y) +	patch[2].pos * (1 - domain.x) * domain.y + patch[3].pos * domain.x * domain.y, 1);

    output.color = calColor(domain);
    return output;
}
#else

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

#define NUM_CONTROL_POINTS 4

////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("quad")]

//PixelInputType ColorDomainShader(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
PixelInputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> patch)
{
    float4 vertexPosition = float4(0, 0, 0, 1);
    PixelInputType output;
    
    vertexPosition = float4(
    patch[0].position * (1 - uvwCoord.x) * (1 - uvwCoord.y) +
    patch[1].position * uvwCoord.x * (1 - uvwCoord.y) +
    patch[2].position * (1 - uvwCoord.x) * uvwCoord.y +
    patch[3].position * uvwCoord.x * uvwCoord.y, 1);
    
    //4点間の補間
	// Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(vertexPosition, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Send the input color into the pixel shader.
    output.color = patch[0].color;

    return output;
}

#endif