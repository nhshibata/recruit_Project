// éQçl
//https://tositeru.github.io/ImasaraDX11/part/tessellation

#if 1

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
struct HullInputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

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

#define NUM_CONTROL_POINTS 1

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
[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(1)]
[patchconstantfunc("ColorPatchConstantFunction")]

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
