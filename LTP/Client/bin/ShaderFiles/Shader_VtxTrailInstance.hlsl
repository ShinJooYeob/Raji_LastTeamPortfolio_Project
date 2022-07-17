
#define FarDist			300.f

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


texture2D			g_DiffuseTexture;
texture2D			g_BackBufferTexture;
texture2D			g_NoiseTexture;
float				g_fTime;

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

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;

	float4		vProjPos : TEXCOORD2;
	float4		vWorldPosition : TEXCOORD3;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vWorldPosition : SV_TARGET2;
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
PS_OUT PS_MAIN_INSTINVERSE(PS_IN In)
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
PS_OUT PS_MAIN_INST_DISORT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	Out.vDiffuse = g_DiffuseTexture.Sample(ClampSampler, In.vTexUV);


	float Alpha = max(max(Out.vDiffuse.r, Out.vDiffuse.g), Out.vDiffuse.b);

	if (Alpha < 0.3)
		discard;


	float2 PosToUv = float2(In.vPosition.x/1280, In.vPosition.y / 720);

	vector Noise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV + g_fTime);

	float2 TargetUV = float2(PosToUv.x + (0.5f - (Noise.x))*0.15625f, PosToUv.y + (0.5f - (Noise.y))*0.25f);
	//float2 TargetUV = In.vTexUV + float2(PosToUv.x + (0.5f - (Noise.x))*0.0015625f, PosToUv.y + (0.5f - (Noise.y))*0.0025f);

	vector BackBuffer = g_BackBufferTexture.Sample(DefaultSampler, TargetUV);

	Out.vDiffuse = BackBuffer;

	Out.vDiffuse.a = 1;
	Out.vWorldPosition = In.vWorldPosition;
	Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);


	return Out;
}

technique11		DefaultTechnique
{
	pass ToBackBuffer
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST();
	}
	pass ToBackBufferInverse
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INSTINVERSE();
	}
	pass Distortion
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST_DISORT();
	}

}