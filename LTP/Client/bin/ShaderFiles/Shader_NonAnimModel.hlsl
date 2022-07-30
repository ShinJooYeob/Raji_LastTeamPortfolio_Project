
#include "Shader_Define.hpp" 


texture2D			g_NoiseTexture;
texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;


cbuffer Distortion
{
	float3				g_vScale = float3(1, 2, 3);
	float3				g_vScrollSpeeds = float3(2.f, 3.f, 4.f);

	float g_fAppearTimer = 1.f;
	float g_fMaxTime = 2.f;
	float g_fTimer = 0;
	float2 noisingdir = float2(1, 0);


	float2 distortion1 = float2(0.1f, 0.2f);
	float2 distortion2 = float2(0.1f, 0.3f);
	float2 distortion3 = float2(0.1f, 0.1f);
	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	float g_fDistortionNoisingPushPower = 0.5f;
	float g_fAlphaTestValue = 0.1f;
};

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
	float3      vPosition : POSITION;
	float3      vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float3      vTangent : TANGENT;
};

struct VS_OUT
{
	float4      vPosition : SV_POSITION;
	float4      vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float4      vWorldPos : TEXCOORD1;
	float4      vProjPos : TEXCOORD2;
	float4      vTangent : TANGENT;
	float4      vBinormal : BINORMAL;

};
struct VS_OUT_Distortion
{
	float4      vPosition : SV_POSITION;
	float4      vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float4      vWorldPos : TEXCOORD1;
	float4      vProjPos : TEXCOORD2;
	float4      vTangent : TANGENT;
	float4      vBinormal : BINORMAL;


	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};

struct VS_OUT_SHADOW
{
	float4      vPosition : SV_POSITION;
	float2      vTexUV : TEXCOORD0;
	float4      vClipPosition : TEXCOORD1;
};

VS_OUT_SHADOW VS_Shadow(VS_IN In)
{

	VS_OUT_SHADOW         Out = (VS_OUT_SHADOW)0;


	Out.vPosition = mul(vector(In.vPosition, 1), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	return Out;
};
VS_OUT_SHADOW VS_Shadow_AttachedBone(VS_IN In)
{

	VS_OUT_SHADOW         Out = (VS_OUT_SHADOW)0;


	Out.vPosition = mul(vector(In.vPosition, 1), g_AttechMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	return Out;
};




VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT         Out = (VS_OUT)0;

	matrix         matWV, matWVP;

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


	VS_OUT         Out = (VS_OUT)0;

	matrix         matWV, matWVP;

	matrix         WorldMatrix = g_AttechMatrix;

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
VS_OUT_Distortion VS_MAIN_Distortion(VS_IN In)
{


	VS_OUT_Distortion         Out = (VS_OUT_Distortion)0;


	matrix         matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;


	Out.texCoords1 = (Out.vTexUV * g_vScale.x);
	Out.texCoords1 += noisingdir*(g_fTimer * g_vScrollSpeeds.x);

	Out.texCoords2 = (Out.vTexUV * g_vScale.y);
	Out.texCoords2 += noisingdir*(g_fTimer * g_vScrollSpeeds.y);

	Out.texCoords3 = (Out.vTexUV * g_vScale.z);
	Out.texCoords3 += noisingdir*(g_fTimer * g_vScrollSpeeds.z);




	return Out;
}




struct PS_IN
{
	float4      vPosition : SV_POSITION;
	float4      vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float4      vWorldPos : TEXCOORD1;
	float4      vProjPos : TEXCOORD2;
	float4      vTangent : TANGENT;
	float4      vBinormal : BINORMAL;
};

struct PS_IN_Distortion
{

	float4      vPosition : SV_POSITION;
	float4      vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float4      vWorldPos : TEXCOORD1;
	float4      vProjPos : TEXCOORD2;
	float4      vTangent : TANGENT;
	float4      vBinormal : BINORMAL;


	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};


struct PS_IN_SHADOW
{
	float4      vPosition : SV_POSITION;
	float2      vTexUV : TEXCOORD0;
	float4      vClipPosition : TEXCOORD1;
};




struct PS_OUT
{
	vector      vDiffuse : SV_TARGET0;
	vector      vNormal : SV_TARGET1;
	vector      vSpecular : SV_TARGET2;
	vector      vEmissive : SV_TARGET3;
	vector      vDepth : SV_TARGET4;
	vector      vWorldPosition : SV_TARGET5;
	vector      vLimLight : SV_TARGET6;
};
struct PS_OUT_NODEFERRED
{
	vector      vDiffuse : SV_TARGET0;
};
struct PS_OUT_SHADOW
{
	vector      vDiffuse : SV_TARGET0;
};

PS_OUT_SHADOW PS_Shadow(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW      Out = (PS_OUT_SHADOW)0;

	float Depth = In.vClipPosition.z / In.vClipPosition.w;

	//Out.vDiffuse = float4(Depth.xxx, 1);


	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);

	return Out;
}

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

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

	vector      vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3      vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3   WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = float4(1, 0, 0, 1);

	
	return Out;
}


PS_OUT PS_MAIN_ZTESTALLMOST(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);


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

	vector      vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3      vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3   WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);


	if (vDiffuse.a < 1)
		discard;

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);

	Out.vLimLight = g_vLimLight;
	return Out;
}



PS_OUT PS_MAIN_FlowerDiscard(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);


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

	vector      vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3      vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3   WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);


	if (vDiffuse.a < 0.1f)
		discard;

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);

	Out.vLimLight = g_vLimLight;
	return Out;
}


PS_OUT PS_MAIN_AffectedColor(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_vColor;


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

	vector      vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3      vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3   WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);


	if (vDiffuse.a < 0.1f)
		discard;

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);

	Out.vLimLight = g_vLimLight;
	return Out;
}

PS_OUT PS_MAIN_SKYBOX(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

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
	PS_OUT      Out = (PS_OUT)0;

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
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

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



	vector      NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - g_fDissolveValue;

	if (NoiseDesc.r < 0)
		discard;

	if (NoiseDesc.r < 0.15 && g_fDissolveValue > 0 && g_fDissolveValue < 1)
	{
		vector      BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);

		vDiffuse = BurnRampDesc;
		//Out.vEmissive = max((max(BurnRampDesc.r, BurnRampDesc.g), BurnRampDesc.b) - 0.15f, 0);
		Out.vEmissive = vector(1.f, 0.5f, 1.f, 1.f);
		//o.Emission = tex2D(_BurnRamp, float2(test *(1 / _BurnSize), 0));
		//o.Albedo *= o.Emission;
	}

	vector      vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	float3      vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3   WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
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
	PS_OUT_SHADOW      Out = (PS_OUT_SHADOW)0;



	vector      NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - g_fDissolveValue;

	if (NoiseDesc.r < 0)
		discard;


	float Depth = In.vClipPosition.z / In.vClipPosition.w;
	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);


	return Out;
}



PS_OUT_NODEFERRED PS_Distortion_All_DiffuseMix(PS_IN_Distortion In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;




	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	vector finalNoise = noise1 + noise2 + noise3;
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);


	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	//fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * g_vColor;
	Out.vDiffuse = fireColor;






	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + g_fTimer * g_fDistortionNoisingPushPower * noisingdir));


	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = pow(g_BackBufferTexture.Sample(ClampSampler, TargetUV),1.f/1.075f);


	//Out.vDiffuse = BackBuffer;
	Out.vDiffuse = saturate((BackBuffer * (1 - Out.vDiffuse.a) + (Out.vDiffuse.a * vector( Out.vDiffuse.xyz ,1.f)) ));
	Out.vDiffuse.a = 1.f;

	return Out;
}
PS_OUT_NODEFERRED PS_Distortion_All_DiffuseMix_AppearNDisApper(PS_IN_Distortion In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	if (g_fTimer < g_fAppearTimer)
	{


		In.vTexUV = (In.vTexUV - noisingdir * (g_fAppearTimer - g_fTimer) *(1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;
		//In.texCoords1 = saturate((In.texCoords1 - noisingdir * (g_fAppearTimer - g_fTimer)));
		//In.texCoords2 = saturate((In.texCoords2 - noisingdir * (g_fAppearTimer - g_fTimer)));
		//In.texCoords3 = saturate((In.texCoords3 - noisingdir * (g_fAppearTimer - g_fTimer)));
	}
	else if (g_fTimer > g_fMaxTime - g_fAppearTimer)
	{
		In.vTexUV = (In.vTexUV + noisingdir * (g_fTimer - (g_fMaxTime - g_fAppearTimer))* (1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;

	}

	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	vector finalNoise = noise1 + noise2 + noise3;
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);



	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	//fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * g_vColor;
	Out.vDiffuse = fireColor;



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + g_fTimer * g_fDistortionNoisingPushPower * noisingdir));


	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = pow(g_BackBufferTexture.Sample(ClampSampler, TargetUV), 1.f / 1.075f);

	//Out.vDiffuse = BackBuffer;
	Out.vDiffuse = saturate((BackBuffer * (1 - Out.vDiffuse.a) + (Out.vDiffuse.a * vector(Out.vDiffuse.xyz, 1.f))));
	Out.vDiffuse.a = 1.f;

	return Out;
}

PS_OUT PS_Noise(PS_IN_Distortion In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	vector finalNoise = noise1 + noise2 + noise3;
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);



	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	//fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * g_vColor;
	Out.vDiffuse = fireColor;


	if (Out.vDiffuse.a < g_fAlphaTestValue)discard;

	Out.vEmissive = g_fEmissive;



	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);


	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

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

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;



	Out.vDiffuse.a = 1.f;

	return Out;
}

PS_OUT PS_Noise_AppearNDisApper(PS_IN_Distortion In)
{
	PS_OUT		Out = (PS_OUT)0;


	if (g_fTimer < g_fAppearTimer)
	{
		

		In.vTexUV = (In.vTexUV - noisingdir * (g_fAppearTimer - g_fTimer) *(1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;
		//In.texCoords1 = saturate((In.texCoords1 - noisingdir * (g_fAppearTimer - g_fTimer)));
		//In.texCoords2 = saturate((In.texCoords2 - noisingdir * (g_fAppearTimer - g_fTimer)));
		//In.texCoords3 = saturate((In.texCoords3 - noisingdir * (g_fAppearTimer - g_fTimer)));
	}
	else if (g_fTimer > g_fMaxTime - g_fAppearTimer)
	{
		In.vTexUV = (In.vTexUV + noisingdir * (g_fTimer - (g_fMaxTime - g_fAppearTimer)  )* (1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;

	}

	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	vector finalNoise = noise1 + noise2 + noise3;
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);



	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	//fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * g_vColor;
	Out.vDiffuse = fireColor;

	if (Out.vDiffuse.a < g_fAlphaTestValue)discard;


	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);



	Out.vEmissive = g_fEmissive;

	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

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


	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);

	Out.vLimLight = g_vLimLight;


	Out.vDiffuse.a = 1.f;

	return Out;
}



technique11      DefaultTechnique
{

	pass Shadow      //0
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Shadow();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow();
	}
	pass Shadow_AttachedBone      //1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Shadow_AttachedBone();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow();
	}
	pass Default      //2
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AllMost_Discard      //3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ZTESTALLMOST();
	}
	pass CullModeNone      //4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AllMost_Discard_CullModeNone      //5
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ZTESTALLMOST();
	}


	pass SkyBox      //6
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SKYBOX();
	}

	pass AttachBone      //7
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHBONE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass AttachBone_ccw      //8
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);
		//SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_ATTACHBONE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	pass SkyBox_ForFog      //9
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
	pass Flower_Discard      //14
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FlowerDiscard();
	}
	
	pass Default_AffectedColor      //15
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_AffectedColor();
	}
	pass Distortion //16
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Distortion();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_DiffuseMix();
	}
	pass Distortion_AppearNDisApper //17
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Distortion();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_DiffuseMix_AppearNDisApper();
	}
	pass Noise//18
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Distortion();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Noise();
	}
	pass Noise_AppearNDisApper //19
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Distortion();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Noise_AppearNDisApper();
	}
	pass Noise_AppearNDisApper_CW //20
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_Distortion();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Noise_AppearNDisApper();
	}


	pass RED //21
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RED();
	}

}