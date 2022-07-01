
#include "Shader_Define.hpp"

cbuffer CameraDesc
{
	vector			g_vCamPosition;
};

texture2D			g_DiffuseTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

	float4		vUV_WHSize: TEXCOORD5;
	float4		vColor : TEXCOORD6;

};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight		: TEXCOORD0;
	float4		vUp			: TEXCOORD1;
	float4		vLook		: TEXCOORD2;
	float4		vUV_WHSize	: TEXCOORD3;
	float4		vColor		: TEXCOORD4;
};

VS_OUT VS_MAIN_INST(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);




	Out.vPosition = mul(vPosition, g_WorldMatrix);
	Out.fPSize = In.fPSize;

	Out.vRight = In.vRight;
	Out.vUp = In.vUp;
	Out.vLook = In.vLook;

	Out.vUV_WHSize = In.vUV_WHSize;
	Out.vColor = In.vColor;

	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight		: TEXCOORD0;
	float4		vUp			: TEXCOORD1;
	float4		vLook		: TEXCOORD2;
	float4		vUV_WHSize	: TEXCOORD3;
	float4		vColor		: TEXCOORD4;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN_INST(in point GS_IN In[1], inout TriangleStream<GS_OUT> Trianglestream)
{
	GS_OUT			Out[4];

	//float3		vLook = (g_vCamPosition - In[0].vPosition).xyz;
	//float3		vAxisY = float3(0.f, 1.f, 0.f);
	//float3		vRight = cross(vAxisY, vLook);
	//float3		vUp = cross(vLook, vRight);

	float3 vLook = normalize(In[0].vLook).xyz;
	float3 vRight = normalize(In[0].vRight).xyz;
	float3 vUp = normalize(In[0].vUp).xyz;


	float RightLength = length(In[0].vRight);
	float UpLength = length(In[0].vUp);

	matrix		matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;

	vPosition = In[0].vPosition.xyz + vRight *RightLength + vUp *UpLength;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(0.f, 0.f);
	Out[0].vColor = In[0].vColor;

	vPosition = In[0].vPosition.xyz - vRight *RightLength + vUp * UpLength;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(1.f, 0.f);
	Out[1].vColor = In[0].vColor;

	vPosition = In[0].vPosition.xyz - vRight *RightLength - vUp *UpLength;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(1.f, 1.f);
	Out[2].vColor = In[0].vColor;

	vPosition = In[0].vPosition.xyz + vRight * RightLength - vUp * UpLength;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(0.f, 1.f);
	Out[3].vColor = In[0].vColor;


	Trianglestream.Append(Out[0]);
	Trianglestream.Append(Out[1]);
	Trianglestream.Append(Out[2]);
	Trianglestream.RestartStrip();

	Trianglestream.Append(Out[0]);
	Trianglestream.Append(Out[2]);
	Trianglestream.Append(Out[3]);
	Trianglestream.RestartStrip();


}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vDepth : SV_TARGET4;
	vector		vWorldPosition : SV_TARGET5;
	vector		vLimLight : SV_TARGET6;
};

PS_OUT PS_MAIN_INST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	//Out.vColor.gb = 0.f;

	if (Out.vDiffuse.a < 0.9f)
		discard;

	Out.vDiffuse *= In.vColor;
	Out.vEmissive = g_fEmissive;
	Out.vLimLight = g_vLimLight;

	return Out;
}

technique11		DefaultTechnique
{
	pass Point
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST();
	}
}