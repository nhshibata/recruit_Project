
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
// �h���C���V�F�[�_�̏o��
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

// �n���V�F�[�_����̏o�͐���_
struct HullOutputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

// �o�͒萔�ް�
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

#define NUM_CONTROL_POINTS 3
//#define NUM_CONTROL_POINTS 4
////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]

//PixelInputType ColorDomainShader(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
PixelInputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, NUM_CONTROL_POINTS> patch)
{
    float3 vertexPosition;
    PixelInputType output;
    
 	// Determine the position of the new vertex.
    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    
	// Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Send the input color into the pixel shader.
    output.color = patch[0].color;

    return output;
}