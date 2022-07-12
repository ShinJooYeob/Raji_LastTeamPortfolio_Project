
#include "Shader_Define.hpp"

cbuffer InstanceDesc
{
	vector			g_vCamPosition;
	float2			g_vTextureFigureUVSize;
	float			g_fAlphaTestValue;
};

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
}


texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;
texture2D			g_SourTexture;


struct VS_IN
{
	float3		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

	float4		vUV_WHSize: TEXCOORD5;
	float4		vColor : TEXCOORD6;
	float4		fTimer : TEXCOORD7;

};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight		: TEXCOORD0;
	float4		vUp			: TEXCOORD1;
	float4		vLook		: TEXCOORD2;
	float4		vUV_WHSize	: TEXCOORD3;
	float4		vColor		: TEXCOORD4;
	float4		fTimer		: TEXCOORD5;
};



VS_OUT VS_MAIN_INST(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);




	Out.vPosition = vPosition;
	Out.fPSize = In.fPSize;

	Out.vRight = In.vRight;
	Out.vUp = In.vUp;
	Out.vLook = In.vLook;

	Out.vUV_WHSize = In.vUV_WHSize;
	Out.vColor = In.vColor;
	Out.fTimer = In.fTimer; 

	return Out;
}


struct GS_IN
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight		: TEXCOORD0;
	float4		vUp			: TEXCOORD1;
	float4		vLook		: TEXCOORD2;
	float4		vUV_WHSize	: TEXCOORD3;
	float4		vColor		: TEXCOORD4;
	float4		fTimer		: TEXCOORD5;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vWorldPos : TEXCOORD3;
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
};


[maxvertexcount(6)]
void GS_MAIN_INST(in point GS_IN In[1], inout TriangleStream<GS_OUT> Trianglestream)
{
	GS_OUT			Out[4];

	//float3		vLook = (g_vCamPosition - In[0].vPosition).xyz;
	//float3		vAxisY = float3(0.f, 1.f, 0.f);
	//float3		vRight = cross(vAxisY, vLook);
	//float3		vUp = cross(vLook, vRight);

	if (In[0].vUV_WHSize.w > 0)
	{


		float3 vLook = normalize(In[0].vLook).xyz;
		float3 vRight = normalize(In[0].vRight).xyz;
		float3 vUp = normalize(In[0].vUp).xyz;


		float RightLength = length(In[0].vRight);
		float UpLength = length(In[0].vUp);

		matrix		matVP;
		matVP = mul(g_ViewMatrix, g_ProjMatrix);

		float3		vPosition;

		vPosition = In[0].vPosition.xyz + vRight *RightLength + vUp *UpLength;
		Out[0].vWorldPos = vector(vPosition.xyz,1.f);
		Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[0].vTexUV = float2(In[0].vUV_WHSize.x , In[0].vUV_WHSize.y );
		//Out[0].vTexUV = float2(In[0].vUV_WHSize.x + g_vTextureFigureUVSize.x, In[0].vUV_WHSize.y + g_vTextureFigureUVSize.y);
		Out[0].vColor = In[0].vColor;
		Out[0].vProjPos = Out[0].vPosition;

		vPosition = In[0].vPosition.xyz - vRight *RightLength + vUp * UpLength;
		Out[1].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[1].vTexUV = float2(In[0].vUV_WHSize.x + g_vTextureFigureUVSize.x, In[0].vUV_WHSize.y);
		Out[1].vColor = In[0].vColor;
		Out[1].vProjPos = Out[1].vPosition;

		vPosition = In[0].vPosition.xyz - vRight *RightLength - vUp *UpLength;
		Out[2].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[2].vTexUV = float2(In[0].vUV_WHSize.x + g_vTextureFigureUVSize.x, In[0].vUV_WHSize.y + g_vTextureFigureUVSize.y);
		Out[2].vColor = In[0].vColor;
		Out[2].vProjPos = Out[2].vPosition;

		vPosition = In[0].vPosition.xyz + vRight * RightLength - vUp * UpLength;
		Out[3].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[3].vTexUV = float2(In[0].vUV_WHSize.x, In[0].vUV_WHSize.y + g_vTextureFigureUVSize.y);
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

}



[maxvertexcount(6)]
void GS_MAIN_Noise(in point GS_IN In[1], inout TriangleStream<GS_Noise_OUT> Trianglestream)
{
	GS_Noise_OUT			Out[4];

	if (In[0].vUV_WHSize.w > 0)
	{


		float3 vLook = normalize(In[0].vLook).xyz;
		float3 vRight = normalize(In[0].vRight).xyz;
		float3 vUp = normalize(In[0].vUp).xyz;


		float RightLength = length(In[0].vRight);
		float UpLength = length(In[0].vUp);

		matrix		matVP;
		matVP = mul(g_ViewMatrix, g_ProjMatrix);

		float3		vPosition;

		vPosition = In[0].vPosition.xyz + vRight *RightLength + vUp *UpLength;

		Out[0].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[0].vTexUV = float2(In[0].vUV_WHSize.x, In[0].vUV_WHSize.y);
		//Out[0].vTexUV = float2(In[0].vUV_WHSize.x + g_vTextureFigureUVSize.x, In[0].vUV_WHSize.y + g_vTextureFigureUVSize.y);
		Out[0].vColor = In[0].vColor;
		Out[0].vProjPos = Out[0].vPosition;


		// 첫번째 노이즈 텍스쳐의 좌표를 첫번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[0].texCoords1 = (Out[0].vTexUV * g_vScale.x);
		Out[0].texCoords1 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.x);
		//Out[0].texCoords1.y = Out[0].texCoords1.y + (In[0].fTimer.x * g_vScrollSpeeds.x);

		// 두번째 노이즈 텍스쳐의 좌표를 두번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[0].texCoords2 = (Out[0].vTexUV * g_vScale.y);
		Out[0].texCoords2 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.y);
		//Out[0].texCoords2.y = Out[0].texCoords2.y + (In[0].fTimer.x * g_vScrollSpeeds.y);

		// 세번째 노이즈 텍스쳐의 좌표를 세번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[0].texCoords3 = (Out[0].vTexUV * g_vScale.z);
		Out[0].texCoords3 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.z);
		//Out[0].texCoords3.y = Out[0].texCoords3.y + (In[0].fTimer.x * g_vScrollSpeeds.z);




		vPosition = In[0].vPosition.xyz - vRight *RightLength + vUp * UpLength;
		Out[1].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[1].vTexUV = float2(In[0].vUV_WHSize.x + g_vTextureFigureUVSize.x, In[0].vUV_WHSize.y);
		Out[1].vColor = In[0].vColor;
		Out[1].vProjPos = Out[1].vPosition;


		// 첫번째 노이즈 텍스쳐의 좌표를 첫번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[1].texCoords1 = (Out[1].vTexUV * g_vScale.x);
		Out[1].texCoords1 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.x);
		//Out[1].texCoords1.y = Out[1].texCoords1.y + (In[0].fTimer.x * g_vScrollSpeeds.x);

		// 두번째 노이즈 텍스쳐의 좌표를 두번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[1].texCoords2 = (Out[1].vTexUV * g_vScale.y);
		Out[1].texCoords2 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.y);
		//Out[1].texCoords2.y = Out[1].texCoords2.y + (In[0].fTimer.x * g_vScrollSpeeds.y);

		// 세번째 노이즈 텍스쳐의 좌표를 세번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[1].texCoords3 = (Out[1].vTexUV * g_vScale.z);
		Out[1].texCoords3 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.z);
		//Out[1].texCoords3.y = Out[1].texCoords3.y + (In[0].fTimer.x * g_vScrollSpeeds.z);




		vPosition = In[0].vPosition.xyz - vRight *RightLength - vUp *UpLength;
		Out[2].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[2].vTexUV = float2(In[0].vUV_WHSize.x + g_vTextureFigureUVSize.x, In[0].vUV_WHSize.y + g_vTextureFigureUVSize.y);
		Out[2].vColor = In[0].vColor;
		Out[2].vProjPos = Out[2].vPosition;

		// 첫번째 노이즈 텍스쳐의 좌표를 첫번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[2].texCoords1 = (Out[2].vTexUV * g_vScale.x);
		//Out[2].texCoords1.y = Out[2].texCoords1.y + (In[0].fTimer.x * g_vScrollSpeeds.x);
		Out[2].texCoords1 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.x);
		// 두번째 노이즈 텍스쳐의 좌표를 두번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[2].texCoords2 = (Out[2].vTexUV * g_vScale.y);
		Out[2].texCoords2 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.y);
		//Out[2].texCoords2.y = Out[2].texCoords2.y + (In[0].fTimer.x * g_vScrollSpeeds.y);

		// 세번째 노이즈 텍스쳐의 좌표를 세번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[2].texCoords3 = (Out[2].vTexUV * g_vScale.z);
		Out[2].texCoords3 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.z);
		//Out[2].texCoords3.y = Out[2].texCoords3.y + (In[0].fTimer.x * g_vScrollSpeeds.z);


		vPosition = In[0].vPosition.xyz + vRight * RightLength - vUp * UpLength;
		Out[3].vWorldPos = vector(vPosition.xyz, 1.f);
		Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
		Out[3].vTexUV = float2(In[0].vUV_WHSize.x, In[0].vUV_WHSize.y + g_vTextureFigureUVSize.y);
		Out[3].vColor = In[0].vColor;
		Out[3].vProjPos = Out[3].vPosition;

		Out[3].texCoords1 = (Out[3].vTexUV * g_vScale.x);
		Out[3].texCoords1 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.x);
		//Out[3].texCoords1.y = Out[3].texCoords1.y + (In[0].fTimer.x * g_vScrollSpeeds.x);

		// 두번째 노이즈 텍스쳐의 좌표를 두번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[3].texCoords2 = (Out[3].vTexUV * g_vScale.y);
		Out[3].texCoords2 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.y);
		//Out[3].texCoords2.y = Out[3].texCoords2.y + (In[0].fTimer.x * g_vScrollSpeeds.y);

		// 세번째 노이즈 텍스쳐의 좌표를 세번째 크기 및 윗방향 스크롤 속도 값을 이용하여 계산합니다.
		Out[3].texCoords3 = (Out[3].vTexUV * g_vScale.z);
		Out[3].texCoords3 += noisingdir*(In[0].fTimer.x * g_vScrollSpeeds.z);
		//Out[3].texCoords3.y = Out[3].texCoords3.y + (In[0].fTimer.x * g_vScrollSpeeds.z);




		Trianglestream.Append(Out[0]);
		Trianglestream.Append(Out[1]);
		Trianglestream.Append(Out[2]);
		Trianglestream.RestartStrip();

		Trianglestream.Append(Out[0]);
		Trianglestream.Append(Out[2]);
		Trianglestream.Append(Out[3]);
		Trianglestream.RestartStrip();

	}

}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vWorldPos : TEXCOORD3;
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
};


struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vWorldPosition : SV_TARGET4;
	vector		vLimLight : SV_TARGET5;
};

PS_OUT PS_MAIN_INST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = pow(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV),2.2f);
	Out.vDiffuse *= In.vColor;


	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;
	

	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);


	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;


	Out.vWorldPosition = In.vWorldPos;
	Out.vEmissive = vector(Out.vDiffuse.a,0,0,1);
	Out.vLimLight = 0.f;

	return Out;
}

PS_OUT PS_BrightColor(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = pow(Out.vDiffuse.a, 2.2f);
	Out.vDiffuse *= In.vColor;


	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;


	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);


	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;

	Out.vWorldPosition = In.vWorldPos;
	Out.vEmissive = vector(Out.vDiffuse.a,0,0,1);
	Out.vLimLight = 0.f;

	return Out;
}

PS_OUT PS_MAIN_NoiseFireEffect(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

	PS_OUT		Out = (PS_OUT)0;

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
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse = fireColor;

	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;

	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;

	return Out;

}





technique11		DefaultTechnique
{
	pass Point_OrigingColor_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST();
	}
	pass Point_OrigingColor_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST();
	}
	pass Point_BrightColor_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_BrightColor();
	}
	pass Point_BrightColor_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_BrightColor();
	}
	pass Point_Noise_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect();
	}
	pass Point_Noise_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect();
	}

}