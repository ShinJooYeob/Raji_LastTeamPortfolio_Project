


#define FarDist			300.f
#define FloatCorrectionValue 0.00000125f
#define XTexelSize		0.00078125f
#define YTexelSize		0.00138889f
#define	DiagTexelSize	0.00159353f


texture2D			g_DiffuseTexture;
texture2D			g_SpecularTexture;
texture2D			g_AmbientTexture;
texture2D			g_EmissiveTexture;
texture2D			g_HeightTexture;
texture2D			g_NormalTexture;
texture2D			g_ShininessTexture;
texture2D			g_OpacityTexture;
texture2D			g_DisplaceTexture;
texture2D			g_LightMapTexture;
texture2D			g_ReflectTexture;
texture2D			g_BaseColorTexture;
texture2D			g_NormalCamTexture;
texture2D			g_EmissionColorTexture;
texture2D			g_MetalTexture;
texture2D			g_DiffuseRoughTexture;
texture2D			g_AmbientOcculusionTexture;


texture2D			g_BurnRampTexture;
texture2D			g_DissolveNoiseTexture;

cbuffer	DissolveBuffer
{
	float	g_fDissolveValue = 0;
};


struct BoneMatrixArray
{
	matrix				BoneMatrices[150];
};

cbuffer Matrices
{
	BoneMatrixArray		g_BoneMatrices;
};


cbuffer	RenderingPipeLine
{
	matrix			g_WorldMatrix;
	matrix			g_ViewMatrix;
	matrix			g_ProjMatrix;
};

cbuffer	ShadowPipeLine
{
	matrix			g_LightViewMatrix;
	matrix			g_LightProjMatrix;

	float			g_fOclussionObject = 0.9f;
};

cbuffer ScreenSizeBuffer
{
	float				fScreemWidth = 1280;
float				fScreemHeight = 720;
float2				iResolution = float2(1280, 720);
};

cbuffer EmissiveNLimLight
{
	float4			g_vLimLight = 0;
	float4			g_fEmissive = 0;
};


sampler DefaultSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};
sampler PointSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_point;
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
sampler PointClampSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_point;
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

BlendState	AlphaBlendingJustDiffuse
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
SrcBlend = Src_Alpha;
DestBlend = Inv_Src_Alpha;
};


BlendState	NonBlending
{
	BlendEnable[0] = false;
BlendEnable[1] = false;
BlendEnable[2] = false;
BlendEnable[3] = false;
BlendEnable[4] = false;
BlendEnable[5] = false;
BlendEnable[6] = false;
BlendEnable[7] = false;

};
BlendState	ParticleBlending
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


BlendState	TestBlending
{
	BlendEnable[0] = true;
BlendEnable[1] = true;
BlendEnable[2] = true;
BlendEnable[3] = false;
BlendEnable[4] = true;
BlendEnable[5] = true;
BlendEnable[6] = false;
BlendEnable[7] = false;

BlendOp = Add;
SrcBlend = Src_Alpha;
DestBlend = Inv_Src_Alpha;
};


/*
BOOL DepthEnable;
D3D11_DEPTH_WRITE_MASK DepthWriteMask;
D3D11_COMPARISON_FUNC DepthFunc;
BOOL StencilEnable;
UINT8 StencilReadMask;
UINT8 StencilWriteMask;
D3D11_DEPTH_STENCILOP_DESC FrontFace;
D3D11_DEPTH_STENCILOP_DESC BackFace;
*/

DepthStencilState  ZTestAndWriteState
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;
};

DepthStencilState  NonZTestAndWriteState
{
	DepthEnable = false;
	DepthWriteMask = zero;	
};


/*
D3D11_FILL_MODE FillMode;
D3D11_CULL_MODE CullMode;
BOOL FrontCounterClockwise;
INT DepthBias;
FLOAT DepthBiasClamp;
FLOAT SlopeScaledDepthBias;
BOOL DepthClipEnable;
BOOL ScissorEnable;
BOOL MultisampleEnable;
BOOL AntialiasedLineEnable;
*/

RasterizerState FillMode_Wireframe
{
	FillMode = wireframe;
	CullMode = none;
	FrontCounterClockwise = false;
};
RasterizerState FillMode_Solid
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState CullMode_None
{
	FillMode = Solid;
CullMode = none;
FrontCounterClockwise = false;

};

RasterizerState CullMode_cw
{
	FillMode = Solid;
	CullMode = front;
	FrontCounterClockwise = false;

};

RasterizerState CullMode_ccw
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;

};








