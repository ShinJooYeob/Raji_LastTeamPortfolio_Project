
#include "Shader_Define.hpp"

texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;
texture2D			g_BlurTargetTexture;
texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;

texture2D			g_CurlTexture;

float				g_Alpha = 0.1f;
float				g_UV_Y = 1.f;
float				g_fAlphaTestValue = 0.1f;

cbuffer RadialBlur
{
	float2				g_vCenter = float2(0.5f, 0.5f);
	float				g_fZoomSize = 0.25f;
	float				g_fZoomPower = 0.5f;
}
cbuffer Distortion
{
	float3				g_vScale = float3(1, 2, 3);
	float3				g_vScrollSpeeds = float3(2.f, 3.f, 4.f);

	float g_fAppearTimer = 1.f;
	float g_fMaxTime = 2.f;
	float g_fTimer = 0;
	float4 g_vColor = float4(1, 1, 1, 1);
	float2 noisingdir = float2(1, 0);


	float2 distortion1 = float2(0.1f, 0.2f);
	float2 distortion2 = float2(0.1f, 0.3f);
	float2 distortion3 = float2(0.1f, 0.1f);
	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	float g_fDistortionNoisingPushPower = 0.5f;

	float g_fToonMaxIntensive = 5.f;
	float g_fPaperCurlIntensive = 0.f;

	float4 g_vLightPosition = float4(0, 0, 0, 1);
	float4 g_vCamPosition = float4(0, 0, 0, 1);
};
/*
float g_fViewAngleThresholdMin = -0.25f;
float g_fViewAngleThresholdMax = 1.0f;
float g_fViewAngleThreshold = 0.2f;
float g_fEdgeDistThresholdMax = 0.999f;
float g_fEdgeDistThreshold = 0.45f;
float g_fDepthBiasMax = 1.5f;//0.005f;
float g_fDepthBias = 0.5;//0.0005f;
float g_fReflectionScale = 1.0f;
*/
cbuffer SSReflectionVSConstants 
{
	float ViewAngleThreshold = 0.f;
	//float ViewAngleThreshold = 0.2f;
	float EdgeDistThreshold	= 1.f;
	//float EdgeDistThreshold	= 0.45f;
	float DepthBias = 1.f;
	//float DepthBias = 0.5f;
	float ReflectionScale = 1.0f;
	float4 PerspectiveValues;
}
////////////////////////////////////////////////////////////////////////////////////////////////
float ConvertZToLinearDepth(float depth)
{
	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
	return linearDepth;
}

float3 CalcViewPos(float2 csPos, float depth)
{
	float3 position;

	position.xy = csPos.xy * PerspectiveValues.xy * depth;
	position.z = depth;

	return position;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_Noise
{
	float4      vPosition : SV_POSITION;
	float2      vTexUV : TEXCOORD0;


	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};

struct VS_OUT_Noise_2
{
	float4      vPosition : SV_POSITION;
	float2      vTexUV : TEXCOORD0;

	float2		texCoords1 : COLOR0;
};

struct VS_OUT_EnvNoise
{
	float4      vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float4		vViewPos : TEXCOORD1;
	float4		vViewNormal : TEXCOORD2;
	float3		vCsProjPos : TEXCOORD3;
	float4      vWorldPos : TEXCOORD4;
	float4      vProjPos : TEXCOORD5;


	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};

VS_OUT VS_MAIN_RECT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);


	Out.vTexUV = In.vTexUV;

	return Out;
}


VS_OUT_Noise VS_Rect_Noise(VS_IN In)
{
	VS_OUT_Noise			Out = (VS_OUT_Noise)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);


	Out.vTexUV = In.vTexUV;


	Out.texCoords1 = (Out.vTexUV * g_vScale.x);
	Out.texCoords1 += noisingdir*(g_fTimer * g_vScrollSpeeds.x);

	Out.texCoords2 = (Out.vTexUV * g_vScale.y);
	Out.texCoords2 += noisingdir*(g_fTimer * g_vScrollSpeeds.y);

	Out.texCoords3 = (Out.vTexUV * g_vScale.z);
	Out.texCoords3 += noisingdir*(g_fTimer * g_vScrollSpeeds.z);




	return Out;
}

VS_OUT_EnvNoise VS_EnvMappedWater_Noise(VS_IN In)
{
	VS_OUT_EnvNoise			Out = (VS_OUT_EnvNoise)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vNormal = vector((mul(vector(0, 0, -1.f, 0), g_WorldMatrix)));

	Out.vViewPos = mul(vector(In.vPosition, 1.f), matWV);
	Out.vViewNormal = normalize(vector((mul(vector(0, 0, -1.f, 0), matWV))));
	Out.vCsProjPos = Out.vPosition.xyz / Out.vPosition.w;
	Out.vProjPos = Out.vPosition;

	//float4      vPosition : SV_POSITION;
	//float4		vViewNormal : NORMAL;
	//float2      vTexUV : TEXCOORD0;
	//float4		vViewPos : TEXCOORD1;
	//float4		vCsProjPos : TEXCOORD2;

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
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};
struct PS_IN_Noise
{

	float4      vPosition : SV_POSITION;
	float2      vTexUV : TEXCOORD0;


	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};

struct PS_IN_EMW_Noise
{
	float4      vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2      vTexUV : TEXCOORD0;
	float4		vViewPos : TEXCOORD1;
	float4		vViewNormal : TEXCOORD2;
	float3		vCsProjPos : TEXCOORD3;
	float4      vWorldPos : TEXCOORD4;
	float4      vProjPos : TEXCOORD5;



	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};

struct PS_OUT_MTRL
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

struct PS_OUT_EnvMapped
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vWorldPosition : SV_TARGET4;
	vector		vLimLight : SV_TARGET5;
};

PS_OUT_NOLIGHT PS_MAIN_RECT(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba

	if (Out.vDiffuse.a < 0.1f)
		discard; 

	//Out.vDiffuse *= g_vColor;

	return Out;
}

PS_OUT_NOLIGHT PS_GAUGE_RECT(PS_IN_Noise In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	if (length(In.vTexUV - float2(0.5f, 0.5f)) > 0.4f)
		discard;

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

	if (g_UV_Y > noiseCoords.y)
		alphaColor = 0;


	fireColor *= g_vColor;
	fireColor = pow(fireColor, 1. / 1.86181f);
	fireColor.a = length(alphaColor.xyz) * g_vColor.a;
	Out.vDiffuse = saturate(fireColor);

	if (Out.vDiffuse.a < 0.1f)discard;


	return Out;
}

PS_OUT_NOLIGHT PS_MAIN_Default(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba

	if (Out.vDiffuse.a == 0.f)
		discard;


	return Out;
}

PS_OUT_NOLIGHT PS_MAIN_ALLMOSTDISCARD(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba

	if (Out.vDiffuse.a < 1.f)
		discard;



	return Out;
}

PS_OUT_NOLIGHT PS_Effect_Alpha_UP(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	Out.vDiffuse.a = Out.vDiffuse.a * g_Alpha;



	return Out;
}

PS_OUT_NOLIGHT PS_Effect_Noise_AppearNDisAppear(PS_IN_Noise In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	if (g_fTimer < g_fAppearTimer)
	{


		In.vTexUV = (In.vTexUV - noisingdir * (g_fAppearTimer - g_fTimer) *(1 / g_fAppearTimer));

		if (In.vTexUV.x < 0 || In.vTexUV.x >1 || In.vTexUV.y < 0 || In.vTexUV.y >1)
			discard;
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

	fireColor *= g_vColor;
	fireColor = pow(fireColor, 1. / 1.86181f);
	fireColor.a = length(alphaColor.xyz) * g_vColor.a;
	Out.vDiffuse = saturate(fireColor);

	if (Out.vDiffuse.a < 0.1f)discard;



	//Out.vDiffuse.a = 1.f;



	return Out;
}

PS_OUT_NOLIGHT PS_Effect_Noise(PS_IN_Noise In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

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

	fireColor *= g_vColor;
	fireColor = pow(fireColor, 1. / 1.86181f);
	fireColor.a = length(alphaColor.xyz) * g_vColor.a;
	Out.vDiffuse = saturate(fireColor);

	if (Out.vDiffuse.a < 0.1f)discard;



	//Out.vDiffuse.a = 1.f;



	return Out;
}

PS_OUT_NOLIGHT PS_PaperCurl(PS_IN In)
{

	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	
	vector PaperCurl = g_CurlTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = PaperCurl;

	
	if (PaperCurl.b > PaperCurl.r + PaperCurl.g)
	{
		Out.vDiffuse = g_SourTexture.Sample(DefaultSampler, In.vTexUV);
	}
	else if (PaperCurl.r > PaperCurl.b + PaperCurl.g)
	{
		float CenterRate = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).r;

		vector vecSideColor = vector(0.90625f, 0.796875f, 0.609375f, 1.f) * (CenterRate)+
			vector(0.5859375f, 0.25390625f, 0.0390625f, 1.f) * (1.f - CenterRate);

		Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * vecSideColor;
	}

	Out.vDiffuse.a = 1;


	return Out;
}
PS_OUT_NOLIGHT PS_PaperCurlOut(PS_IN In)
{

	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	vector BackbufferDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector ToonDesc = g_SourTexture.Sample(DefaultSampler, In.vTexUV);

	//float Value = length(vector(BackbufferDesc.xyz, 0.f));
	//Value = (ceil(Value * g_fToonMaxIntensive) / g_fToonMaxIntensive);


	//float CenterRate = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).r;

	//vector vecSideColor = vector(0.90625f, 0.796875f, 0.609375f, 1.f) * (CenterRate)+
	//	vector(0.5859375f, 0.25390625f, 0.0390625f, 1.f) * (1.f - CenterRate);

	Out.vDiffuse = ToonDesc* (1.f - g_fPaperCurlIntensive)
		+ BackbufferDesc * g_fPaperCurlIntensive;


	Out.vDiffuse.a = 1;


	return Out;
}

PS_OUT_MTRL PS_ChiedFlame(PS_IN_EMW_Noise In)
{
	PS_OUT_MTRL		Out = (PS_OUT_MTRL)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba

	if (Out.vDiffuse.a < g_Alpha)
		discard;

	Out.vDiffuse.a = pow(Out.vDiffuse.a, 2.2f);

	Out.vNormal = vector(vector(0, 1, 0, 0).xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	Out.vLimLight = g_vLimLight;
	Out.vEmissive = g_fEmissive;

	vector BrightNoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV + float2(g_fTimer, g_fTimer*0.25f));

	Out.vDiffuse.xyz = pow(Out.vDiffuse, 1.f / (1.f + BrightNoiseDesc.r));

	if (g_fTimer > g_fMaxTime - g_fAppearTimer)
	{

		float DissolveValue = (g_fTimer -(g_fMaxTime - g_fAppearTimer))  / g_fAppearTimer;

		vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - DissolveValue;

		if (NoiseDesc.r < 0)
			discard;


		if (NoiseDesc.r < 0.15 && DissolveValue > 0 && DissolveValue < 1)
		{
			vector		BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);
			Out.vDiffuse = vector(BurnRampDesc.xyz, Out.vDiffuse.a);
			Out.vEmissive = vector(1.f, 0.5f, 1.f, 1.f);
		}

	}
	else if(g_fTimer < g_fPaperCurlIntensive)
	{
		float DissolveValue = (g_fPaperCurlIntensive - g_fTimer) / g_fPaperCurlIntensive;

		vector		NoiseDesc = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV) - DissolveValue;

		if (NoiseDesc.r < 0)
			discard;


		if (NoiseDesc.r < 0.15 && DissolveValue > 0 && DissolveValue < 1)
		{
			vector		BurnRampDesc = pow(g_BurnRampTexture.Sample(DefaultSampler, float2(NoiseDesc.r *(1 / 0.15), 0)), 1.5f);
			Out.vDiffuse = vector(BurnRampDesc.xyz, Out.vDiffuse.a);
			Out.vEmissive = vector(1.f, 0.5f, 1.f, 1.f);
		}

	}

	return Out;
}

PS_OUT_NOLIGHT PS_MAIN_MinigameBuildingMixColor(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	vector SourceDesc = g_SourTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
	if (Out.vDiffuse.a < 0.1f)
		discard;
	//0 //(0.36328125f, 0.3359375f ,0.30859375f)
	//1 //(0.4375f, 0.35546875f, 0.27734375f)

	float flen = length(Out.vDiffuse.xyz);
	//float flen = max(max(Out.vDiffuse.x, Out.vDiffuse.y), Out.vDiffuse.z);

	vector vColor = vector(0.38823529411764f, 0.33333333333333f, 0.290196078431372f, 1.f) * flen +
		vector(0.32941176470588f, 0.27450980392156f, 0.258823529411764f, 1.f) * (1 - flen);

	flen = saturate(flen - 0.2f);

	Out.vDiffuse.xyz = (SourceDesc.xyz* (1 - flen) + vColor.xyz* flen);
	Out.vDiffuse.a = 1.f;
	return Out;
}



PS_OUT_NOLIGHT PS_CopyScreen(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba

	return Out;
}


PS_OUT_NOLIGHT PS_RadialBlur(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	float weight0;
	float normalization;
	float4 color;

	float2 vDir = In.vTexUV - g_vCenter;
	vDir.x *= iResolution.x / iResolution.y; //fix aspect ratio


	int Count = max((length(vDir) - g_fZoomSize) / DiagTexelSize, 2);

	float fPower = DiagTexelSize * max(g_fZoomPower,0.1f);

	vDir = normalize(vDir);



	// 색깔을 검정색으로 초기화합니다.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll(100)]
	for (int i = 0; i < Count; i++)
	{
		color += g_SourTexture.Sample(ClampSampler, saturate( In.vTexUV + (vDir * fPower * (float)i))    );
	}
	color /= min((Count),100);

	// 알파 채널을 1로 설정합니다.

	color.a = 1.0f;


	Out.vDiffuse = color;
	return Out;
}

PS_OUT_NOLIGHT PS_FADE(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba
	Out.vDiffuse.a = Out.vDiffuse.r;

	//if (Out.vDiffuse.a < g_fAlphaTestValue)
	//	discard;

	Out.vDiffuse *= g_vColor;

	return Out;
}


technique11		DefaultTechnique
{
	pass Rect			//0
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}
	pass UI			//1
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}

	pass Rect_Alpha_Up		//2
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Effect_Alpha_UP();
	}

	pass Rect_NoneCull			//3
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}


	pass Rect_Noise_AppearNDisAppear //4
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Rect_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Effect_Noise_AppearNDisAppear();
	}
	pass Rect_Noise//5
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_Rect_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Effect_Noise();
	}
	pass Rect_NoDicard			//6
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Default();
	}
	pass PaperCurl			//7
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PaperCurl();
	}
	pass PaperCurlOut			//8
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PaperCurlOut();
	}
	pass ChiedFlame			//9
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_EnvMappedWater_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ChiedFlame();
	}
	pass Rect_MiniGameMixColor			//10
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MinigameBuildingMixColor();
	}
	pass Rect_CopyScreen			//11
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CopyScreen();
	}
	pass UI_Gauge		//12
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_Rect_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GAUGE_RECT();
	}

	pass RadialBlur// 13
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RadialBlur();
	}

	pass FadeNHitEffect// 14
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FADE();
	}

	pass UIDissolve		//15
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_EnvMappedWater_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ChiedFlame();
	}
}