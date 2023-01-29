// 参考
//https://tositeru.github.io/ImasaraDX11/part/tessellation
#if 0

cbuffer Param : register(b0)
{
    float cbEdgeFactor;
    float cbInsideFactor;
};

struct VS_CONTROL_POINT_OUTPUT
{
    float3 pos : POSITION;
};

struct HS_CONTROL_POINT_OUTPUT
{
    float3 pos : POSITION;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[4] : SV_TessFactor;
    float InsideTessFactor[2] : SV_InsideTessFactor;
};

//#define NUM_CONTROL_POINTS 4

// パッチ定数関数
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

	[unroll]
    for (uint i = 0; i < NUM_CONTROL_POINTS; ++i)
    {
        Output.EdgeTessFactor[i] = cbEdgeFactor;
    }
    Output.InsideTessFactor[0] = Output.InsideTessFactor[1] = cbInsideFactor;

	[branch]
    if (false)
    {
        float4 roundedEdgeFactors;
        float2 roundedInsideFactor;
        float2 unroundedInsideFactor;
        ProcessQuadTessFactorsAvg(cbEdgeFactor, cbInsideFactor, roundedEdgeFactors, roundedInsideFactor, unroundedInsideFactor);
		//ProcessQuadTessFactorsMax(cbEdgeFactor, cbInsideFactor, roundedEdgeFactors, roundedInsideFactor, unroundedInsideFactor);
		//ProcessQuadTessFactorsMin(cbEdgeFactor, cbInsideFactor, roundedEdgeFactors, roundedInsideFactor, unroundedInsideFactor);
		//Process2DQuadTessFactorsAvg(cbEdgeFactor, cbInsideFactor, roundedEdgeFactors, roundedInsideFactor, unroundedInsideFactor);
		//Process2DQuadTessFactorsMax(cbEdgeFactor, cbInsideFactor, roundedEdgeFactors, roundedInsideFactor, unroundedInsideFactor);
		//Process2DQuadTessFactorsMin(cbEdgeFactor, cbInsideFactor, roundedEdgeFactors, roundedInsideFactor, unroundedInsideFactor);

        Output.EdgeTessFactor[0] = roundedEdgeFactors.x;
        Output.EdgeTessFactor[1] = roundedEdgeFactors.y;
        Output.EdgeTessFactor[2] = roundedEdgeFactors.z;
        Output.EdgeTessFactor[3] = roundedEdgeFactors.w;
        Output.InsideTessFactor[0] = roundedInsideFactor.x;
        Output.InsideTessFactor[1] = roundedInsideFactor.y;
    }

    return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
[maxtessfactor(16.f)]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> inputPatch,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONTROL_POINT_OUTPUT Output;
    Output.pos = inputPatch[i].pos;
    return Output;
}

#else
/////////////
// GLOBALS //
/////////////
cbuffer TessellationBuffer
{
    float tessellationAmount;
    float3 padding;
};

//////////////
// TYPEDEFS //
//////////////
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct HullInputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};


#define NUM_CONTROL_POINTS 4

////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
ConstantOutputType ColorPatchConstantFunction(InputPatch<HullInputType, NUM_CONTROL_POINTS> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;


	// Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;

	// Set the tessellation factor for tessallating inside the triangle.
    output.inside = tessellationAmount;

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Hull Shader
////////////////////////////////////////////////////////////////////////////////
[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ColorPatchConstantFunction")]
[maxtessfactor(16.f)]
//HullOutputType ColorHullShader(InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
HullOutputType main(InputPatch<HullInputType, NUM_CONTROL_POINTS> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutputType output;

	// Set the position for this control point as the output position.
    output.position = patch[pointId].position;

	// Set the input color as the output color.
    output.color = patch[pointId].color;

    return output;
}
#endif