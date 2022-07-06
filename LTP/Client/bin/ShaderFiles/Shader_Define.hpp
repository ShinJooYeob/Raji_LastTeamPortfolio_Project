
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
};

cbuffer EmissiveNLimLight
{
	float4			g_vLimLight = 0;
	float			g_fEmissive = 0;
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

BlendState	AlphaBlending
{
	BlendEnable[0] = true;
	BlendOp = Add;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
};

BlendState	NonBlending
{
	BlendEnable[0] = false;
};
BlendState	ParticleBlending
{
	BlendEnable[0] = true;
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








