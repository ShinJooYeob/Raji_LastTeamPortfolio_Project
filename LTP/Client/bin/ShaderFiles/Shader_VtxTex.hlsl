
#include "Shader_Define.hpp"

texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;
texture2D			g_BlurTargetTexture;
texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;

float				g_Alpha;


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
};



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



PS_OUT_NOLIGHT PS_MAIN_RECT(PS_IN In)
{
	PS_OUT_NOLIGHT		Out = (PS_OUT_NOLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);//vector(1.f, 0.f, 0.f, 1.f);rgba

	if (Out.vDiffuse.a < 0.1f)
		discard; 

	//Out.vDiffuse *= g_vColor;

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



}