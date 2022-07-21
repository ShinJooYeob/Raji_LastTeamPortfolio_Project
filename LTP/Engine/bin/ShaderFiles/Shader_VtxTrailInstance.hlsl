
#define FarDist			300.f

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


	float	g_fAppearTimer = 1.f;
	float g_fDistortionNoisingPushPower = 0.5f;
}


texture2D			g_DiffuseTexture;
texture2D			g_BackBufferTexture;
texture2D			g_NoiseTexture;
float				g_fTime;

texture2D			g_DepthTexture;
texture2D			g_SourTexture;

sampler DefaultSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};
sampler ClampSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};
BlendState	AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	BlendEnable[2] = true;
	BlendEnable[3] = true;
	BlendEnable[4] = true;
	BlendEnable[5] = true;
	BlendEnable[6] = true;
	BlendEnable[7] = true;

	BlendOp = Add;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
};
BlendState	AlphaBlendingExpceptWorldPosition
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	BlendEnable[2] = true;
	BlendEnable[3] = true;
	BlendEnable[4] = true;
	BlendEnable[5] = false;
	BlendEnable[6] = true;
	BlendEnable[7] = true;

	BlendOp = Add;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
};

BlendState	OneBlendingJustDiffuse
{
	BlendEnable[0] = true;
	BlendEnable[1] = false;
	BlendEnable[2] = false;
	BlendEnable[3] = false;
	BlendEnable[4] = false;
	BlendEnable[5] = false;
	BlendEnable[6] = false;
	BlendEnable[7] = false;

	BlendOp = Add;
	SrcBlend = one;
	DestBlend = one;
};

/*

vector		vDiffuse : SV_TARGET0;
vector		vNormal : SV_TARGET1;
vector		vSpecular : SV_TARGET2;
vector		vEmissive : SV_TARGET3;
vector		vDepth : SV_TARGET4;
vector		vWorldPosition : SV_TARGET5;
vector		vLimLight : SV_TARGET6;

*/

BlendState	OneBlendingExceptWorld
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	BlendEnable[2] = true;
	BlendEnable[3] = true;
	BlendEnable[4] = true;
	BlendEnable[5] = false;
	BlendEnable[6] = true;
	BlendEnable[7] = true;

	BlendOp = Add;
	SrcBlend = one;
	DestBlend = one;
};


BlendState	Blending_One
{
	BlendEnable[0] = true;
	BlendEnable[1] = false;
	BlendEnable[2] = false;
	BlendEnable[3] = false;
	BlendEnable[4] = false;
	BlendEnable[5] = false;
	BlendEnable[6] = false;
	BlendEnable[7] = false;

	BlendOp = Add;
	SrcBlend = one;
	DestBlend = one;
};
cbuffer	RenderingPipeLine
{
	matrix			g_ViewMatrix;
	matrix			g_ProjMatrix;
};
DepthStencilState  ZTestAndWriteState
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;
};

RasterizerState CullMode_None
{
	FillMode = Solid;
	CullMode = none;
	FrontCounterClockwise = false;

};


struct VS_IN
{
	float3		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vNowSour : TEXCOORD1;
	float4		vNowDest : TEXCOORD2;
	float4		vNextSour	: TEXCOORD3;
	float4		vNextDest : TEXCOORD4;

	float4		vUV_N_Time: TEXCOORD5;
	float4		vColor : TEXCOORD6;
	float4		fTimer : TEXCOORD7;

};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vNowSour	 : TEXCOORD1;
	float4		vNowDest	: TEXCOORD2;
	float4		vNextSour	: TEXCOORD3;
	float4		vNextDest	: TEXCOORD4;

	float4		vUV_N_Time	: TEXCOORD5;
	float4		vColor		: TEXCOORD6;
	float4		fTimer : TEXCOORD7;
};	

VS_OUT VS_MAIN_INST(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	//matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	//vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);


	Out.vPosition = vector(In.vPosition,1.f);
	Out.fPSize = In.fPSize;


	Out.vNowSour = In.vNowSour;
	Out.vNowDest = In.vNowDest;
	Out.vNextSour = In.vNextSour;
	Out.vNextDest = In.vNextDest;

	Out.vUV_N_Time = In.vUV_N_Time;
	Out.vColor = In.vColor;


	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vNowSour	 : TEXCOORD1;
	float4		vNowDest	: TEXCOORD2;
	float4		vNextSour	: TEXCOORD3;
	float4		vNextDest	: TEXCOORD4;

	float4		vUV_N_Time	: TEXCOORD5;
	float4		vColor		: TEXCOORD6;
	float4		fTimer : TEXCOORD7;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;

	float4		vProjPos : TEXCOORD2;
	float4		vWorldPosition : TEXCOORD3;
};

struct GS_Noise_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vWorldPos : TEXCOORD3;

	float2		texCoords1 : TEXCOORD4;
	float2		texCoords2 : TEXCOORD5;
	float2		texCoords3 : TEXCOORD6;


	float4		vTimer : TEXCOORD7;
};

[maxvertexcount(6)]
void GS_MAIN_INST(in point GS_IN In[1], inout TriangleStream<GS_OUT> Trianglestream)
{
	GS_OUT			Out[4];


	matrix		matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	 

	vector		vPosition;

	vPosition = vector(In[0].vNowDest.xyz,1);
	Out[0].vWorldPosition = vPosition;
	Out[0].vPosition = mul(vPosition, matVP);
	Out[0].vTexUV = float2(In[0].vUV_N_Time.x, In[0].vUV_N_Time.y);
	Out[0].vColor = In[0].vColor;
	Out[0].vProjPos = Out[0].vPosition;

	vPosition = vector(In[0].vNextDest.xyz, 1);
	Out[1].vWorldPosition = vPosition;
	Out[1].vPosition = mul(vPosition, matVP);
	Out[1].vTexUV = float2(In[0].vUV_N_Time.z, In[0].vUV_N_Time.y);
	Out[1].vColor = In[0].vColor;
	Out[1].vProjPos = Out[1].vPosition;

	vPosition = vector(In[0].vNextSour.xyz, 1);
	Out[2].vWorldPosition = vPosition;
	Out[2].vPosition = mul(vPosition, matVP);
	Out[2].vTexUV = float2(In[0].vUV_N_Time.z, In[0].vUV_N_Time.w);
	Out[2].vColor = In[0].vColor;
	Out[2].vProjPos = Out[2].vPosition;

	vPosition = vector(In[0].vNowSour.xyz, 1);
	Out[3].vWorldPosition = vPosition;
	Out[3].vPosition = mul(vPosition, matVP);
	Out[3].vTexUV = float2(In[0].vUV_N_Time.x, In[0].vUV_N_Time.w);
	Out[3].vColor = In[0].vColor;
	Out[3].vProjPos = Out[3].vPosition;


	Trianglestream.Append(Out[0]);
	Trianglestream.Append(Out[1]);
	Trianglestream.Append(Out[2]);
	Trianglestream.RestartStrip();

	Trianglestream.Append(Out[0]);
	Trianglestream.Append(Out[2]);
	Trianglestream.Append(Out[3]);
	Trianglestream.RestartStrip();


}


[maxvertexcount(6)]
void GS_MAIN_Noise(in point GS_IN In[1], inout TriangleStream<GS_Noise_OUT> Trianglestream)
{
	GS_Noise_OUT			Out[4];



	matrix		matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);



	vector		vPosition;

	vPosition = vector(In[0].vNowDest.xyz, 1);
	Out[0].vWorldPos = vPosition;
	Out[0].vPosition = mul(vPosition, matVP);
	Out[0].vTexUV = float2(In[0].vUV_N_Time.x, In[0].vUV_N_Time.y);
	Out[0].vColor = In[0].vColor;
	Out[0].vProjPos = Out[0].vPosition;

	Out[0].texCoords1 = (Out[0].vTexUV * g_vScale.x);
	Out[0].texCoords1 += noisingdir*(g_fTime * g_vScrollSpeeds.x);
	Out[0].texCoords2 = (Out[0].vTexUV * g_vScale.y);
	Out[0].texCoords2 += noisingdir*(g_fTime * g_vScrollSpeeds.y);
	Out[0].texCoords3 = (Out[0].vTexUV * g_vScale.z);
	Out[0].texCoords3 += noisingdir*(g_fTime * g_vScrollSpeeds.z);

	vPosition = vector(In[0].vNextDest.xyz, 1);
	Out[1].vWorldPos = vPosition;
	Out[1].vPosition = mul(vPosition, matVP);
	Out[1].vTexUV = float2(In[0].vUV_N_Time.z, In[0].vUV_N_Time.y);
	Out[1].vColor = In[0].vColor;
	Out[1].vProjPos = Out[1].vPosition;

	Out[1].texCoords1 = (Out[1].vTexUV * g_vScale.x);
	Out[1].texCoords1 += noisingdir*(g_fTime * g_vScrollSpeeds.x);
	Out[1].texCoords2 = (Out[1].vTexUV * g_vScale.y);
	Out[1].texCoords2 += noisingdir*(g_fTime * g_vScrollSpeeds.y);
	Out[1].texCoords3 = (Out[1].vTexUV * g_vScale.z);
	Out[1].texCoords3 += noisingdir*(g_fTime * g_vScrollSpeeds.z);


	vPosition = vector(In[0].vNextSour.xyz, 1);
	Out[2].vWorldPos = vPosition;
	Out[2].vPosition = mul(vPosition, matVP);
	Out[2].vTexUV = float2(In[0].vUV_N_Time.z, In[0].vUV_N_Time.w);
	Out[2].vColor = In[0].vColor;
	Out[2].vProjPos = Out[2].vPosition;

	Out[2].texCoords1 = (Out[2].vTexUV * g_vScale.x);
	Out[2].texCoords1 += noisingdir*(g_fTime * g_vScrollSpeeds.x);
	Out[2].texCoords2 = (Out[2].vTexUV * g_vScale.y);
	Out[2].texCoords2 += noisingdir*(g_fTime * g_vScrollSpeeds.y);
	Out[2].texCoords3 = (Out[2].vTexUV * g_vScale.z);
	Out[2].texCoords3 += noisingdir*(g_fTime * g_vScrollSpeeds.z);



	vPosition = vector(In[0].vNowSour.xyz, 1);
	Out[3].vWorldPos = vPosition;
	Out[3].vPosition = mul(vPosition, matVP);
	Out[3].vTexUV = float2(In[0].vUV_N_Time.x, In[0].vUV_N_Time.w);
	Out[3].vColor = In[0].vColor;
	Out[3].vProjPos = Out[3].vPosition;

	Out[3].texCoords1 = (Out[3].vTexUV * g_vScale.x);
	Out[3].texCoords1 += noisingdir*(g_fTime * g_vScrollSpeeds.x);
	Out[3].texCoords2 = (Out[3].vTexUV * g_vScale.y);
	Out[3].texCoords2 += noisingdir*(g_fTime * g_vScrollSpeeds.y);
	Out[3].texCoords3 = (Out[3].vTexUV * g_vScale.z);
	Out[3].texCoords3 += noisingdir*(g_fTime * g_vScrollSpeeds.z);

	
	Out[0].vTimer = Out[1].vTimer = Out[2].vTimer = Out[3].vTimer = g_fTime;

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

	float4		vProjPos : TEXCOORD2;
	float4		vWorldPosition : TEXCOORD3;
};
struct PS_Noise_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vWorldPos : TEXCOORD3;

	float2		texCoords1 : TEXCOORD4;
	float2		texCoords2 : TEXCOORD5;
	float2		texCoords3 : TEXCOORD6;


	float4		vTimer : TEXCOORD7;
};


struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vWorldPosition : SV_TARGET2;
};

struct PS_OUT_Distortion
{
	vector		vDiffuse : SV_TARGET0;
};


PS_OUT PS_MAIN_INST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(ClampSampler, In.vTexUV);


	Out.vDiffuse = (Out.vDiffuse) * In.vColor;
	float Alpha = length(vector(Out.vDiffuse.xyz,0));
	//float Alpha = max(max(Out.vDiffuse.r, Out.vDiffuse.g), Out.vDiffuse.b);

	if (Alpha < 0.1)discard;

	

	Out.vDiffuse.a = Alpha;
	Out.vWorldPosition = In.vWorldPosition;
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}
PS_OUT PS_MAIN_AlphaCutting(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(ClampSampler, In.vTexUV);


	float Alpha = max(max(Out.vDiffuse.r, Out.vDiffuse.g), Out.vDiffuse.b);

	if (Alpha < 0.1)
		discard;

	Out.vDiffuse = (1 - Out.vDiffuse) * In.vColor;
	Out.vWorldPosition = In.vWorldPosition;
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);


	return Out;
}
PS_OUT_Distortion PS_MAIN_INST_DISORT(PS_IN In)
{
	PS_OUT_Distortion		Out = (PS_OUT_Distortion)0;


	Out.vDiffuse = g_DiffuseTexture.Sample(ClampSampler, In.vTexUV);


	//float Alpha = Out.vDiffuse.a  * In.vColor.a;
	float Alpha = max(max(Out.vDiffuse.r, Out.vDiffuse.g), Out.vDiffuse.b);

	if (Alpha < 0.3)
		discard;

	float2 PosToUv = float2(In.vPosition.x/1280, In.vPosition.y / 720);

	vector Noise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV + g_fTime);

	float2 TargetUV = float2(PosToUv.x + (0.5f - (Noise.x))*0.15625f, PosToUv.y + (0.5f - (Noise.y))*0.25f);
	//float2 TargetUV = In.vTexUV + float2(PosToUv.x + (0.5f - (Noise.x))*0.0015625f, PosToUv.y + (0.5f - (Noise.y))*0.0025f);

	vector BackBuffer = (g_BackBufferTexture.Sample(DefaultSampler, TargetUV));
	//vector BackBuffer = pow(g_BackBufferTexture.Sample(DefaultSampler, TargetUV) * In.vColor, 1.f/1.085);
	//Out.vDiffuse = BackBuffer;

	Out.vDiffuse.x = pow(BackBuffer.x, 1.f / (1.f + In.vColor.x));
	Out.vDiffuse.y = pow(BackBuffer.x, 1.f / (1.f + In.vColor.y));
	
	Out.vDiffuse.z = pow(BackBuffer.x, 1.f / (1.f + In.vColor.z));

	Out.vDiffuse.a = 1;

	return Out;
}

PS_OUT PS_MAIN_AlphaCutting_Noise(PS_Noise_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//Out.vDiffuse = g_DiffuseTexture.Sample(ClampSampler, In.vTexUV);




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

	fireColor *= alphaColor * In.vColor;
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse = vector(pow(fireColor, 1.f / 2.2f).xyz, fireColor.a);



	if (Out.vDiffuse.a < 0.1f)
		discard;

	Out.vDiffuse = saturate(Out.vDiffuse);
	//Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x, g_fEmissive.y, g_fEmissive.z, 1);
	Out.vWorldPosition = In.vWorldPos;
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

technique11		DefaultTechnique
{
	pass ToBackBuffer // 0
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST();
	}
	// 1
	pass ToBackBufferAlphaCutting
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_AlphaCutting();
	}
	// 2
	pass Distortion
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST_DISORT();
	}
	// 3
	pass ToBackBufferAlphaCuttingNNoise
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_AlphaCutting_Noise();
	}

}