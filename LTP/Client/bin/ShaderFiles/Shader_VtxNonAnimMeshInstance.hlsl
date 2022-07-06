
#include "Shader_Define.hpp"


cbuffer LightPosition
{
	float4 g_vLightPosition;
};

cbuffer AttechMatrix
{
	matrix g_AttechMatrix;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;

};
struct VS_OUT_SHADOW
{
	float4		vPosition : SV_POSITION;
	float4		vClipPosition : TEXCOORD1;
};

VS_OUT_SHADOW VS_Shadow(VS_IN In)
{

	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;


	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);


	Out.vPosition = mul(vector(In.vPosition, 1), TransformMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;
	return Out;
};



VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;
	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matWV = mul(TransformMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;


	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));


	return Out;
}





struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;
};
struct PS_IN_SHADOW
{
	float4		vPosition : SV_POSITION;
	float4		vClipPosition : TEXCOORD1;
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
struct PS_OUT_NODEFERRED
{
	vector		vDiffuse : SV_TARGET0;
};
struct PS_OUT_SHADOW
{
	vector		vDiffuse : SV_TARGET0;
};

PS_OUT_SHADOW PS_Shadow(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	float Depth = In.vClipPosition.z / In.vClipPosition.w;

	//Out.vDiffuse = float4(Depth.xxx, 1);


	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);

	return Out;
}

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = g_fEmissive;
	Out.vLimLight = g_vLimLight;
	return Out;
}
PS_OUT PS_MAIN_ZTESTALLMOST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);


	if (vDiffuse.a < 1)
		discard;

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = g_fEmissive;
	Out.vLimLight = g_vLimLight;
	return Out;
}



technique11		DefaultTechnique
{

	pass Shadow		//0
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Shadow();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow();
	}
	pass Default		//1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AllMost_Discard		//2
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ZTESTALLMOST();
	}
	pass CullModeNone		//3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AllMost_Discard_CullModeNone		//4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ZTESTALLMOST();
	}

}

