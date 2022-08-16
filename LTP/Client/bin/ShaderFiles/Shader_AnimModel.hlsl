
#include "Shader_Define.hpp" 


texture2D			g_NoiseTexture;
texture2D			g_DepthTexture;


cbuffer AttechMatrix
{
	matrix g_AttechMatrix;
};


cbuffer DeltaTime
{
	float			g_fDeltaTime = 0;
	float			g_fVisualValue  = 2.f;

};



//cbuffer MtrlDesc
//{
//	float4		g_vMtrlAmbient = float4(0.4f, 0.4f, 0.4f, 1.f);
//	float4		g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);
//};


struct VS_IN
{
	float3		vModelDataPosition : POSITION;
	float3		vModelDataNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;	
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
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
VS_OUT_SHADOW VS_Shadow_NoWeightW(VS_IN In)
{

	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;

	matrix			matWV, matWVP;


	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	

	vector		vLocalPosition = mul(vector(In.vModelDataPosition, 1.f), BoneMatrix);


	Out.vPosition = mul(vLocalPosition, g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;

	Out.vTexUV = In.vTexUV;

	return Out;
};
VS_OUT_SHADOW VS_Shadow_Attached(VS_IN In)
{
	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;

	matrix			matWV, matWVP;


	float		fWeightX = 1.f - (In.vBlendWeight.y + In.vBlendWeight.z + In.vBlendWeight.w);

	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	vector		vLocalPosition = mul(vector(In.vModelDataPosition, 1.f), BoneMatrix);
	vector		vLocalNormal = mul(vector(In.vModelDataNormal, 0.f), BoneMatrix);

	matrix			WorldMatrix = g_AttechMatrix;

	Out.vPosition = mul(vLocalPosition, WorldMatrix);
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


	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
	
	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vLocalPosition = mul(vector(In.vModelDataPosition, 1.f), BoneMatrix);
	vector		vLocalNormal = mul(vector(In.vModelDataNormal, 0.f), BoneMatrix);


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vLocalPosition, matWVP);
	Out.vNormal = normalize(mul(vector(vLocalNormal.xyz, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vLocalPosition, g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));

	return Out;
}

VS_OUT VS_MAIN_NOWEIGHTW(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;


	float		fWeightX = 1.f - (In.vBlendWeight.y + In.vBlendWeight.z + In.vBlendWeight.w);

	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	vector		vLocalPosition = mul(vector(In.vModelDataPosition, 1.f), BoneMatrix);
	vector		vLocalNormal = mul(vector(In.vModelDataNormal, 0.f), BoneMatrix);


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vLocalPosition, matWVP);
	//Out.vNormal = 1;
	Out.vNormal = normalize(mul(vector(vLocalNormal.xyz, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vLocalPosition, g_WorldMatrix);
	Out.vProjPos = Out.vPosition;


	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));
	return Out;
}

VS_OUT VS_MAIN_ATTACHEDNOWEIGHTW(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;


	float		fWeightX = 1.f - (In.vBlendWeight.y + In.vBlendWeight.z + In.vBlendWeight.w);

	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	vector		vLocalPosition = mul(vector(In.vModelDataPosition, 1.f), BoneMatrix);
	vector		vLocalNormal = mul(vector(In.vModelDataNormal, 0.f), BoneMatrix);

	matrix			WorldMatrix = g_AttechMatrix;

	matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(vLocalPosition.xyz , 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(vLocalNormal.xyz, 0.f), WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vLocalPosition, WorldMatrix);
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
struct PS_OUT_NOLIGHT
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


	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);

	return Out;
}

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	//
	if (vDiffuse.a < 0.1f)
		discard;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	NormalWorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, NormalWorldMatrix);

	
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

	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz,0);
	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_MotionTrail(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	NormalWorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, NormalWorldMatrix);


	Out.vDiffuse = vector(0,0,0,0.001f);
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = 1.f;
	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_Test(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	if (vDiffuse.a < 0.1f)
		discard;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	NormalWorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, NormalWorldMatrix);


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

	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_MAIN_DEFAULT_Dissolve(PS_IN In)
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


	vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - g_fDissolveValue;

	if (NoiseDesc.r < 0)
		discard;

	if (NoiseDesc.r < 0.15 && g_fDissolveValue > 0 && g_fDissolveValue < 1)
	{
		vector		BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);

		Out.vDiffuse = BurnRampDesc;
		//Out.vEmissive = max((max(BurnRampDesc.r, BurnRampDesc.g), BurnRampDesc.b) - 0.15f, 0);
		Out.vEmissive = vector(1.f,0.5f,1.f,1.f);
		//o.Emission = tex2D(_BurnRamp, float2(test *(1 / _BurnSize), 0));
		//o.Albedo *= o.Emission;
	}


	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
	vNormal = mul(vNormal, WorldMatrix);


	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
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

PS_OUT PS_MotionTrail_NoEmissive(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	NormalWorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, NormalWorldMatrix);


	Out.vDiffuse = vector(g_vLimLight.xyz, 0.0001f);
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = 0.f;
	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_MAIN_SoftEffect(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	//
	if (vDiffuse.a < 0.1f)
		discard;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	NormalWorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, NormalWorldMatrix);


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


	float2		vAlphaUV = In.vProjPos.xy / In.vProjPos.w;
	vAlphaUV.x = vAlphaUV.x * 0.5f + 0.5f;
	vAlphaUV.y = vAlphaUV.y * -0.5f + 0.5f;
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vAlphaUV);
	float		fViewZ = vDepthDesc.x * FarDist;

	Out.vDiffuse.a = saturate((fViewZ - In.vProjPos.w) / In.vProjPos.w * g_fVisualValue);

	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;


	Out.vNormal.a =	Out.vSpecular.a =	Out.vEmissive.a =	Out.vDepth.a =	Out.vWorldPosition.a  = Out.vDiffuse.a;

	return Out;
}

technique11		DefaultTechnique
{
	pass Shadow		//0
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Shadow_NoWeightW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow();
	}
	pass Shadow_AttachedBone		//1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Shadow_Attached();
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
	pass NOWEIGHTW		//3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AttachedWeapon //4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHEDNOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass ForMotionTrailBuffer//5
	{
		SetBlendState(AlphaBlendingJustDiffuse, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MotionTrail();
	}



	pass Test		//6
	{
		SetBlendState(TestBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Test();
	}


	pass Dissolve_NoWeightW		//7
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT_Dissolve();
	}
	pass Dissolve_Weight		//8
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT_Dissolve();
	}
	pass Dissolve_AttachedWeapon //9
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHEDNOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT_Dissolve();
	}
	pass Dissolve_Shadow_NoWeightW		//10
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Shadow_NoWeightW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow_Dissolve();
	}
	pass Dissolve_Shadow_Weight		//11
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Shadow_NoWeightW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow_Dissolve();
	}	
	pass Dissolve_Shadow_AttachedWeapon //12
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Shadow_Attached();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow_Dissolve();
	}

	pass Player_NOWEIGHTW_CullNone		//13
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass Test2		//14
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHEDNOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Test();
	}
	pass ForMotionTrailBuffer_NoEmi//15
	{
		SetBlendState(AlphaBlendingJustDiffuse, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MotionTrail_NoEmissive();
	}

	pass SoftEffect		//16
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_NOWEIGHTW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SoftEffect();
	}
}