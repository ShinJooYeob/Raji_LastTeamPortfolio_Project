
#include "Shader_Define.hpp"


texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;
texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;


cbuffer ForNoise
{
	float3				g_vScale = float3(1, 2, 3);
	float3				g_vScrollSpeeds = float3(2.f, 3.f, 4.f);

	float2 noisingdir = float2(1, 0);
	float2 distortion1 = float2(0.1f, 0.2f);
	float2 distortion2 = float2(0.1f, 0.3f);
	float2 distortion3 = float2(0.1f, 0.1f);
	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	float4	g_vColor = float4(0,0,0,0);
	float	g_fAppearTimer = 1.f;
	float g_fDistortionNoisingPushPower = 0.5f;
	float g_fAlphaTestValue = 0;

}




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
	float4		vLimLightColor : TEXCOORD5;
	float4		vEmissive : TEXCOORD6;
	float4		vTimer : TEXCOORD7;

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


	float4		vLimLightColor : TEXCOORD3;
	float4		vEmissive : TEXCOORD4;
	float4		vTimer : TEXCOORD5;
};
struct VS_OUT_Noise
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;

	float4		vLimLightColor : TEXCOORD3;
	float4		vEmissive : TEXCOORD4;
	float4		vTimer : TEXCOORD5;

	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;
};
struct VS_OUT_SHADOW
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vClipPosition : TEXCOORD1;
	float4		vTimer : TEXCOORD2;
};



VS_OUT_SHADOW VS_Shadow(VS_IN In)
{

	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;


	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);


	Out.vPosition = mul(vector(In.vPosition, 1), TransformMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;


	Out.vTexUV = In.vTexUV;
	Out.vTimer = In.vTimer;
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
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), TransformMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), TransformMatrix);
	Out.vProjPos = Out.vPosition;


	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));

	Out.vLimLightColor = In.vLimLightColor;
	Out.vEmissive = In.vEmissive;
	Out.vTimer = In.vTimer;


	return Out;
}

VS_OUT_Noise VS_MAIN_Noise(VS_IN In)
{
	VS_OUT_Noise			Out = (VS_OUT_Noise)0;

	matrix			matWV, matWVP;
	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matWV = mul(TransformMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), TransformMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), TransformMatrix);
	Out.vProjPos = Out.vPosition;


	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), TransformMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));

	Out.vLimLightColor = In.vLimLightColor;
	Out.vEmissive = In.vEmissive;
	Out.vTimer = In.vTimer;



	Out.texCoords1 = (Out.vTexUV * g_vScale.x);
	Out.texCoords1 += noisingdir*(In.vTimer.x * g_vScrollSpeeds.x);

	Out.texCoords2 = (Out.vTexUV * g_vScale.y);
	Out.texCoords2 += noisingdir*(In.vTimer.x * g_vScrollSpeeds.y);

	Out.texCoords3 = (Out.vTexUV * g_vScale.z);
	Out.texCoords3 += noisingdir*(In.vTimer.x * g_vScrollSpeeds.z);

	return Out;
}


struct GS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;


	float4		vLimLightColor : TEXCOORD3;
	float4		vEmissive : TEXCOORD4;
	float4		vTimer : TEXCOORD5;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;


	float4		vLimLightColor : TEXCOORD3;
	float4		vEmissive : TEXCOORD4;
	float4		vTimer : TEXCOORD5;

};

[maxvertexcount(1)]
void GS_MAIN_INST(in point GS_IN In[1], inout TriangleStream<GS_OUT> Point)
{

	if (In[0].vProjPos.x >= -1 && In[0].vProjPos.x <= 1 &&
		In[0].vProjPos.y >= -1 && In[0].vProjPos.y <= 1 &&
		In[0].vProjPos.z >= 0 && In[0].vProjPos.z <= 1)
	{

		GS_OUT			Out = (GS_OUT)0;

		Out = In;

		Point.Append(Out);

	}

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

	float4		vLimLightColor : TEXCOORD3;
	float4		vEmissive : TEXCOORD4;
	float4		vTimer : TEXCOORD5;
};
struct PS_IN_SHADOW
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vClipPosition : TEXCOORD1;
	float4		vTimer : TEXCOORD2;
};
struct PS_Noise_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;

	float4		vLimLightColor : TEXCOORD3;
	float4		vEmissive : TEXCOORD4;
	float4		vTimer : TEXCOORD5;

	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;
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


	if (In.vTimer.w > 0)
	{
		float DissolveValue = 0;
		if (In.vTimer.w > 1)
			DissolveValue = In.vTimer.x / In.vTimer.y;
		else
			DissolveValue = 1.f - (In.vTimer.x / In.vTimer.y);

		vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - DissolveValue;
		if (NoiseDesc.r < 0)	discard;

	}

	float Depth = In.vClipPosition.z / In.vClipPosition.w;
	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);

	return Out;
}

PS_OUT PS_MAIN_OriginColor(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vEmissive = In.vEmissive;


	if (In.vTimer.w > 0)
	{

		float DissolveValue = 0;
		if (In.vTimer.w > 1)
			DissolveValue = In.vTimer.x / In.vTimer.y;
		else
			DissolveValue = 1.f - (In.vTimer.x / In.vTimer.y);

		vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - DissolveValue;

		if (NoiseDesc.r < 0)
			discard;


		if (NoiseDesc.r < 0.15 && DissolveValue > 0 && DissolveValue < 1)
		{
			vector		BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);
			vDiffuse = BurnRampDesc;
			Out.vEmissive = vector(1.f, 0.5f, 1.f, 1.f);
		}

	}
	//else
	//{
	//	if (vDiffuse.a < 0.1f)
	//		discard;
	//}





	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = In.vLimLightColor;

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;

	Out.vDiffuse = saturate(Out.vDiffuse);




	return Out;
}

PS_OUT PS_MAIN_BrightColor(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector		vDiffuse = pow(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV), 1.f / 2.2f);
	Out.vEmissive = In.vEmissive;


	if (In.vTimer.w > 0)
	{

		float DissolveValue = 0;
		if (In.vTimer.w > 1)
			DissolveValue = In.vTimer.x / In.vTimer.y;
		else
			DissolveValue = 1.f - (In.vTimer.x / In.vTimer.y);

		vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - DissolveValue;

		if (NoiseDesc.r < 0)
			discard;


		if (NoiseDesc.r < 0.15 && DissolveValue > 0 && DissolveValue < 1)
		{
			vector		BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);
			vDiffuse = BurnRampDesc;
			Out.vEmissive = vector(1.f, 0.5f, 1.f, 1.f);
		}

	}
	//else
	//{
	//	if (vDiffuse.a < 0.1f)
	//		discard;
	//}



	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = In.vLimLightColor;

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;
	Out.vDiffuse = saturate(Out.vDiffuse);


	return Out;
}

PS_OUT PS_MAIN_NoiseFireEffect(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

	PS_OUT		Out = (PS_OUT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = In.vEmissive;
	Out.vLimLight = In.vLimLightColor;



	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// 노이즈의 x와 y값을 세 개의 다른 왜곡 x및 y좌표로 흩뜨립니다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// 왜곡된 세 노이즈 값들을 하나의 노이즈로 함성합니다.
	vector finalNoise = noise1 + noise2 + noise3;

	// 입력으로 들어온 텍스쳐의 Y좌표를 왜곡 크기와 바이어스 값으로 교란시킵니다.
	// 이 교란은 텍스쳐의 위쪽으로 갈수록 강해져서 맨 위쪽에는 깜박이는 효과를 만들어냅니다.
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);

	// 불꽃 색상 텍스쳐를 샘플링하는데 사용될 왜곡 및 교란된 텍스쳐 좌표를 만듭니다.
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);

	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 불꽃 텍스쳐에서 색상을 샘플링합니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	//ClampSampler
	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 알파 텍스쳐에서 알파값을 샘플링합니다.
	// 불꽃의 투명도를 지정하는 데 사용될 것입니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz);
	Out.vDiffuse = fireColor;
	Out.vLimLight.xyz = (Out.vLimLight.xyz * Out.vDiffuse.a).xyz;
	//Out.vDiffuse = pow(fireColor,1.f/2.2f);
	//Out.vDiffuse.a = length(Out.vDiffuse.xyz);

	//float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	//vUV.x = vUV.x * 0.5f + 0.5f;
	//vUV.y = vUV.y * -0.5f + 0.5f;

	//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	//float		fViewZ = vDepthDesc.x * FarDist;

	//Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);


	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;


	Out.vDiffuse = saturate(Out.vDiffuse);

	return Out;

}

PS_OUT PS_MAIN_NoiseFireEffect_Bright(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

	PS_OUT		Out = (PS_OUT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = In.vEmissive;
	Out.vLimLight = In.vLimLightColor;



	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// 노이즈의 x와 y값을 세 개의 다른 왜곡 x및 y좌표로 흩뜨립니다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// 왜곡된 세 노이즈 값들을 하나의 노이즈로 함성합니다.
	vector finalNoise = noise1 + noise2 + noise3;

	// 입력으로 들어온 텍스쳐의 Y좌표를 왜곡 크기와 바이어스 값으로 교란시킵니다.
	// 이 교란은 텍스쳐의 위쪽으로 갈수록 강해져서 맨 위쪽에는 깜박이는 효과를 만들어냅니다.
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);

	// 불꽃 색상 텍스쳐를 샘플링하는데 사용될 왜곡 및 교란된 텍스쳐 좌표를 만듭니다.
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);

	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 불꽃 텍스쳐에서 색상을 샘플링합니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	//ClampSampler
	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 알파 텍스쳐에서 알파값을 샘플링합니다.
	// 불꽃의 투명도를 지정하는 데 사용될 것입니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz);
	Out.vDiffuse = pow((fireColor), 1.f / 2.2f);
	Out.vDiffuse.a = length(alphaColor.xyz);
	Out.vLimLight.xyz = (Out.vLimLight.xyz * Out.vDiffuse.a).xyz;
	//Out.vDiffuse = pow(fireColor,1.f/2.2f);
	//Out.vDiffuse.a = length(Out.vDiffuse.xyz);

	//float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	//vUV.x = vUV.x * 0.5f + 0.5f;
	//vUV.y = vUV.y * -0.5f + 0.5f;

	//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	//float		fViewZ = vDepthDesc.x * FarDist;

	//Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);


	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;

	Out.vDiffuse = saturate(Out.vDiffuse);


	return Out;

}

PS_OUT PS_MAIN_NoiseFireEffect_Appear(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture
	PS_OUT		Out = (PS_OUT)0;

	if (In.vTimer.x  < g_fAppearTimer)
	{

		float2 OldTexUV = In.vTexUV;
		In.vTexUV = (In.vTexUV - normalize(noisingdir)  * (g_fAppearTimer - In.vTimer.x) *(1 / g_fAppearTimer));
		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;

		In.vTexUV = OldTexUV;
	}
	else if (In.vTimer.x  > In.vTimer.y - g_fAppearTimer)
	{
		float2 OldTexUV = In.vTexUV;
		In.vTexUV = (In.vTexUV + normalize(noisingdir) * (In.vTimer.x - (In.vTimer.y - g_fAppearTimer))* (1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;

		In.vTexUV = OldTexUV;
	}


	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = In.vEmissive;
	Out.vLimLight = In.vLimLightColor;



	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// 노이즈의 x와 y값을 세 개의 다른 왜곡 x및 y좌표로 흩뜨립니다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// 왜곡된 세 노이즈 값들을 하나의 노이즈로 함성합니다.
	vector finalNoise = noise1 + noise2 + noise3;

	// 입력으로 들어온 텍스쳐의 Y좌표를 왜곡 크기와 바이어스 값으로 교란시킵니다.
	// 이 교란은 텍스쳐의 위쪽으로 갈수록 강해져서 맨 위쪽에는 깜박이는 효과를 만들어냅니다.
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);

	// 불꽃 색상 텍스쳐를 샘플링하는데 사용될 왜곡 및 교란된 텍스쳐 좌표를 만듭니다.
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);

	//if (In.vTimer.x < 1)
	//	noiseCoords = saturate((noiseCoords + float2(-1, 0) *  (1.f - In.vTimer.x)));

	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 불꽃 텍스쳐에서 색상을 샘플링합니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector fireColor = pow(g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy), 1.f / 2.2f);
	//ClampSampler
	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 알파 텍스쳐에서 알파값을 샘플링합니다.
	// 불꽃의 투명도를 지정하는 데 사용될 것입니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz);
	Out.vDiffuse = fireColor;
	Out.vLimLight.xyz = (Out.vLimLight.xyz * Out.vDiffuse.a).xyz;
	//Out.vDiffuse = pow(fireColor,1.f/2.2f);
	//Out.vDiffuse.a = length(Out.vDiffuse.xyz);

	//float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	//vUV.x = vUV.x * 0.5f + 0.5f;
	//vUV.y = vUV.y * -0.5f + 0.5f;

	//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	//float		fViewZ = vDepthDesc.x * FarDist;

	//Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;

	Out.vDiffuse = saturate(Out.vDiffuse);

	return Out;

}

PS_OUT PS_MAIN_NoiseFireEffect_Appear_Bright(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture
	PS_OUT		Out = (PS_OUT)0;

	if (In.vTimer.x  < g_fAppearTimer)
	{

		float2 OldTexUV = In.vTexUV;
		In.vTexUV = (In.vTexUV - normalize(noisingdir)  * (g_fAppearTimer - In.vTimer.x) *(1 / g_fAppearTimer));
		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;

		In.vTexUV = OldTexUV;
	}
	else if (In.vTimer.x  > In.vTimer.y - g_fAppearTimer)
	{
		float2 OldTexUV = In.vTexUV;
		In.vTexUV = (In.vTexUV + normalize(noisingdir) * (In.vTimer.x - (In.vTimer.y - g_fAppearTimer))* (1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;

		In.vTexUV = OldTexUV;
	}

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vEmissive = In.vEmissive;
	Out.vLimLight = In.vLimLightColor;



	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	// 노이즈의 x와 y값을 세 개의 다른 왜곡 x및 y좌표로 흩뜨립니다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	// 왜곡된 세 노이즈 값들을 하나의 노이즈로 함성합니다.
	vector finalNoise = noise1 + noise2 + noise3;

	// 입력으로 들어온 텍스쳐의 Y좌표를 왜곡 크기와 바이어스 값으로 교란시킵니다.
	// 이 교란은 텍스쳐의 위쪽으로 갈수록 강해져서 맨 위쪽에는 깜박이는 효과를 만들어냅니다.
	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);

	// 불꽃 색상 텍스쳐를 샘플링하는데 사용될 왜곡 및 교란된 텍스쳐 좌표를 만듭니다.
	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);

	//if (In.vTimer.x < 1)
	//	noiseCoords = saturate((noiseCoords + float2(-1, 0) *  (1.f - In.vTimer.x)));

	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 불꽃 텍스쳐에서 색상을 샘플링합니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector fireColor =g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
	//ClampSampler
	// 왜곡되고 교란된 텍스쳐 좌표를 이용하여 알파 텍스쳐에서 알파값을 샘플링합니다.
	// 불꽃의 투명도를 지정하는 데 사용될 것입니다.
	// wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz);
	Out.vDiffuse = pow(fireColor, 1.f / 2.2f);
	Out.vDiffuse.a = length(alphaColor.xyz);
	Out.vLimLight.xyz = (Out.vLimLight.xyz * Out.vDiffuse.a).xyz;
	//Out.vDiffuse = pow(fireColor,1.f/2.2f);
	//Out.vDiffuse.a = length(Out.vDiffuse.xyz);

	//float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	//vUV.x = vUV.x * 0.5f + 0.5f;
	//vUV.y = vUV.y * -0.5f + 0.5f;

	//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	//float		fViewZ = vDepthDesc.x * FarDist;

	//Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;
	Out.vDiffuse = saturate(Out.vDiffuse);

	return Out;

}




PS_OUT_NODEFERRED PS_Distortion_All(PS_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	//Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	

	vector BackBuffer = g_BackBufferTexture.Sample(DefaultSampler, TargetUV);

	Out.vDiffuse = BackBuffer;
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	//Out.vDepth.a = Out.vWorldPosition.a = 0;
	return Out;
}


PS_OUT_NODEFERRED PS_Distortion_All_Bright(PS_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	//Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));
	//vector DiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	//
	//float Alpha = max(BlurDesc.r - 0.5f, 0) * 2.f;

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	Out.vDiffuse = saturate(pow(BackBuffer,1.f/2.2f));

	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	return Out;
}
PS_OUT_NODEFERRED PS_Distortion_All_DiffuseMix(PS_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	//Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));
	vector DiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	float Alpha = BlurDesc.r * In.vLimLightColor.a;

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	//Out.vDiffuse = pow(BackBuffer, 1.f / 2.2f);

	Out.vDiffuse = saturate(BackBuffer * (1 - Alpha) + (Alpha * DiffuseDesc));

	Out.vDiffuse.a = 1.f;
	return Out;
}
PS_OUT_NODEFERRED PS_Distortion_All_DiffuseMix_Bright(PS_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	//Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));
	vector DiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float Alpha = BlurDesc.r* In.vLimLightColor.a;

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	//Out.vDiffuse = pow(BackBuffer, 1.f / 2.2f);

	Out.vDiffuse = saturate(pow(BackBuffer * (1 - Alpha) + (Alpha * DiffuseDesc),1.f/2.2f));
	Out.vDiffuse.a = 1.f;

	return Out;
}
PS_OUT_NODEFERRED PS_Distortion_All_ColorMix(PS_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	//Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));
	//vector DiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float Alpha = BlurDesc.r* In.vLimLightColor.a;

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	//Out.vDiffuse = pow(BackBuffer, 1.f / 2.2f);

	Out.vDiffuse = saturate(BackBuffer * (1 - Alpha) + (Alpha * In.vLimLightColor));
	Out.vDiffuse.a = 1.f;

	return Out;
}

PS_OUT_NODEFERRED PS_Distortion_All_ColorMix_Bright(PS_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;


	//Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));
	//vector DiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float Alpha = BlurDesc.r* In.vLimLightColor.a;

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);

	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	//Out.vDiffuse = pow(BackBuffer, 1.f / 2.2f);

	Out.vDiffuse = saturate(pow(BackBuffer * (1 - Alpha) + (Alpha * In.vLimLightColor), 1.f / 2.2f));
	Out.vDiffuse.a = 1.f;

	return Out;
}


PS_OUT PS_MAIN_MapObject(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);



	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = In.vLimLightColor;
	Out.vEmissive = 0;

	Out.vDiffuse = saturate(Out.vDiffuse);

	return Out;
}
PS_OUT PS_MAIN_MapObject_Discard(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (vDiffuse.a < 0.1f) discard;

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);



	vNormal = mul(vNormal, WorldMatrix);



	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = In.vLimLightColor;
	Out.vEmissive = 0;

	Out.vDiffuse = saturate(Out.vDiffuse);

	return Out;
}


PS_OUT PS_MAIN_LillyDiscard(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	vector      vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector      vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vDiffuse;


	Out.vEmissive = vector(1.f, 0.5f, 1.f, 1);

	//if (vEmissiveDesc.a > 0)
	//{
	//	Out.vEmissive.xyz = min(g_fEmissive.xyz * length(vEmissiveDesc.xyz), 1.f);
	//	if (length(Out.vEmissive.xyz) > 0)
	//		Out.vDiffuse += length(Out.vEmissive.xyz) * pow(g_vLimLight, 1.f / 2.2f);
	//}
	//else
	//{
	//	Out.vEmissive = vector(1.f,0.5f,1.f, 1);
	//}

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
	Out.vEmissive = float4(1.f, 0.5f, 1.f, 0.f);
	Out.vLimLight = float4(vDiffuse.xyz, 0.5f);

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
	pass OriginColor		//1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OriginColor();
	}
	pass BrightColor		//2
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BrightColor();
	}
	pass Noise		//3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect();
	}
	pass Noise_Bright		//4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect_Bright();
	}
	pass Noise_Appear		//5
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect_Appear();
	}
	pass Noise_Appear_Bright		//6
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect_Appear_Bright();
	}

	pass Distortion_Default		//7
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All();
	}
	pass Distortion_Bright		//8
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_Bright();
	}
	pass Distortion_DiffuseAlphaMix		//9
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_DiffuseMix();
	}
	pass Distortion_DiffuseAlphaMix_Bright		//10
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_DiffuseMix_Bright();
	}
	pass Distortion_ColorMix		//11
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_ColorMix();
	}
	pass Distortion_ColorMix_Bright		//12
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Distortion_All_ColorMix_Bright();
	}

	pass MapObject_Default		//13
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MapObject();
	}
	pass MapObject_CullNone		//14
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MapObject();
	}
	pass MapObject_Discard		//15
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MapObject_Discard();
	}

	pass LillyDiscard //16
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LillyDiscard();
	}

}

