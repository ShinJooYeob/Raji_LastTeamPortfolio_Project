
#include "Shader_Define.hpp" 



cbuffer LightPosition
{
	float4 g_vLightPosition;
};

cbuffer AttechMatrix
{
	matrix g_AttechMatrix;
};

float4 g_vColor = 0;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;	
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
	float2		vTexUV : TEXCOORD0;
	float4		vClipPosition : TEXCOORD1;
};

VS_OUT_SHADOW VS_Shadow(VS_IN In)
{

	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;


	Out.vPosition = mul(vector(In.vPosition, 1), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	return Out;
};
VS_OUT_SHADOW VS_Shadow_AttachedBone(VS_IN In)
{

	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;


	Out.vPosition = mul(vector(In.vPosition, 1), g_AttechMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	return Out;
};




VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
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

VS_OUT VS_MAIN_ATTACHBONE(VS_IN In)
{


	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matrix			WorldMatrix = g_AttechMatrix;

	matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), WorldMatrix);

	Out.vProjPos = Out.vPosition;

	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), WorldMatrix));
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
	float2		vTexUV : TEXCOORD0;
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

	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vDiffuse;


	if (vEmissiveDesc.a > 0)
	{
		Out.vEmissive.xyz = min(g_fEmissive.xyz * length(vEmissiveDesc.xyz), 1.f);
		if (length(Out.vEmissive.xyz) > 0)
			Out.vDiffuse += length(Out.vEmissive.xyz) * pow(g_vLimLight, 1.f / 2.2f);
	}
	else
	{
		Out.vEmissive = vector(g_fEmissive.xyz, 1);
	}

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;
	return Out;
}
PS_OUT PS_MAIN_ZTESTALLMOST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);


	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vDiffuse;


	if (vEmissiveDesc.a > 0)
	{
		Out.vEmissive.xyz = min(g_fEmissive.xyz * length(vEmissiveDesc.xyz), 1.f);
		if (length(Out.vEmissive.xyz) > 0)
			Out.vDiffuse += length(Out.vEmissive.xyz) * pow(g_vLimLight, 1.f / 2.2f);
	}
	else
	{
		Out.vEmissive = vector(g_fEmissive.xyz, 1);
	}

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);


	if (vDiffuse.a < 1)
		discard;

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);

	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_MAIN_SKYBOX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = 1;

	Out.vNormal = vector(0, 0, 0, 1);
	Out.vDepth = vector(1, 0, 0, 1);
	Out.vSpecular = vector(0, 0, 1, 0);
	Out.vWorldPosition = vector(1, 0, 0, 1);
	Out.vEmissive = vector(g_fEmissive.xyz, 1);
	Out.vLimLight = vector(0.f, 0.f, 0.f, 0.f);

	return Out;
}
PS_OUT PS_MAIN_SKYBOX_ForFog(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_vColor;
	Out.vDiffuse.a = 1;

	Out.vNormal = vector(0, 0, 0, 1);
	Out.vDepth = vector(1, 0, 0, 1);
	Out.vSpecular = vector(0, 0, 1, 0);
	Out.vWorldPosition = vector(1, 0, 0, 1);
	Out.vEmissive = vector(g_fEmissive.xyz, 1);
	Out.vLimLight = vector(0.f, 0.f, 0.f, 0.f);

	return Out;
}
PS_OUT PS_MAIN_DEFAULT_Dissolve(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (vDiffuse.a < 0.1f)
		discard;

	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vDiffuse;


	if (vEmissiveDesc.a > 0)
	{
		Out.vEmissive.xyz = min(g_fEmissive.xyz * length(vEmissiveDesc.xyz), 1.f);
		if (length(Out.vEmissive.xyz) > 0)
			Out.vDiffuse += length(Out.vEmissive.xyz) * pow(g_vLimLight, 1.f / 2.2f);
	}
	else
	{
		Out.vEmissive = vector(g_fEmissive.xyz, 1);
	}



	vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - g_fDissolveValue;

	if (NoiseDesc.r < 0)
		discard;

	if (NoiseDesc.r < 0.15 && g_fDissolveValue > 0 && g_fDissolveValue < 1)
	{
		vector		BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);

		Out.vDiffuse = BurnRampDesc;
		//Out.vEmissive = max((max(BurnRampDesc.r, BurnRampDesc.g), BurnRampDesc.b) - 0.15f, 0);
		Out.vEmissive = vector(1.f, 0.5f, 1.f, 1.f);
		//o.Emission = tex2D(_BurnRamp, float2(test *(1 / _BurnSize), 0));
		//o.Albedo *= o.Emission;
	}

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
	vNormal = mul(vNormal, WorldMatrix);


	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;

	return Out;
}

PS_OUT_SHADOW PS_Shadow_Dissolve(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;



	vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - g_fDissolveValue;

	if (NoiseDesc.r < 0)
		discard;


	float Depth = In.vClipPosition.z / In.vClipPosition.w;
	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);


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
	pass Shadow_AttachedBone		//1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Shadow_AttachedBone();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow();
	}
	pass Default		//2
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}	
	pass AllMost_Discard		//3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ZTESTALLMOST();
	}
	pass CullModeNone		//4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AllMost_Discard_CullModeNone		//5
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ZTESTALLMOST();
	}


	pass SkyBox		//6
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SKYBOX();
	}

	pass AttachBone		//7
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHBONE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AttachBone_ccw		//8
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHBONE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	pass SkyBox_ForFog		//9
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SKYBOX_ForFog();
	}


	pass Dissolve_Default //10
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT_Dissolve();
	}
	pass Dissolve_Attached //11
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHBONE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT_Dissolve();
	}
	pass Dissolve_Shadow_Default //12
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Shadow();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow_Dissolve();
	}
	pass Dissolve_Shadow_Attached //13
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Shadow_AttachedBone();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow_Dissolve();
	}

	
}