
#include "Shader_Define.hpp"

cbuffer InstanceDesc
{
	vector			g_vCamPosition;
	float2			g_vTextureFigureUVSize;
	float			g_fAlphaTestValue;
	float			g_fWorldMixing = 0.f;
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


	float	g_fAppearTimer = 1.f;
	float g_fDistortionNoisingPushPower = 0.5f;
}


texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;
texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;


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
	float4		vTimer : TEXCOORD4;

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

		Out[0].vTimer = Out[1].vTimer = Out[2].vTimer = Out[3].vTimer = In[0].fTimer;

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


		Out[0].vTimer = Out[1].vTimer = Out[2].vTimer = Out[3].vTimer = In[0].fTimer;


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
	float4		vTimer : TEXCOORD4;
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
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vWorldPosition : SV_TARGET4;
	vector		vLimLight : SV_TARGET5;
};
struct PS_OUT_NODEFERRED
{
	vector		vDiffuse : SV_TARGET0;
};
struct PS_OUT_WorldBlend
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vDepth : SV_TARGET4;
	vector		vWorldPosition : SV_TARGET5;
	vector		vLimLight : SV_TARGET6;
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

	
	Out.vWorldPosition = vector(In.vWorldPos.xyz, g_fWorldMixing);
	Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x , g_fEmissive.y, g_fEmissive.z,1);
	Out.vLimLight = 0.f;

	Out.vDiffuse = saturate(Out.vDiffuse);
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

	Out.vWorldPosition = vector(In.vWorldPos.xyz, g_fWorldMixing);
	Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x, g_fEmissive.y, g_fEmissive.z, 1);
	Out.vLimLight = 0.f;
	Out.vDiffuse = saturate(Out.vDiffuse);

	return Out;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	Out.vDiffuse = saturate(Out.vDiffuse);
	Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x, g_fEmissive.y, g_fEmissive.z, 1);
	return Out;

}

PS_OUT PS_MAIN_NoiseFireEffect_Bright(PS_Noise_IN In)
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

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse = vector(pow(fireColor,1.f/2.2f).xyz , fireColor.a);

	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;

	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;
	Out.vDiffuse = saturate(Out.vDiffuse);
	Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x, g_fEmissive.y, g_fEmissive.z, 1);

	return Out;

}

PS_OUT PS_MAIN_NoiseAppear(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

	PS_OUT		Out = (PS_OUT)0;

	if (In.vTimer.x < g_fAppearTimer)
	{
		In.vTexUV = saturate((In.vTexUV - noisingdir * (g_fAppearTimer - In.vTimer.x)));
		In.texCoords1 = saturate((In.texCoords1 - noisingdir * (g_fAppearTimer - In.vTimer.x)));
		In.texCoords2 = saturate((In.texCoords2 - noisingdir * (g_fAppearTimer - In.vTimer.x)));
		In.texCoords3 = saturate((In.texCoords3 - noisingdir * (g_fAppearTimer - In.vTimer.x)));
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

	Out.vDiffuse = saturate(Out.vDiffuse);
	Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x, g_fEmissive.y, g_fEmissive.z, 1);
	return Out;

}

PS_OUT PS_MAIN_NoiseAppear_Bright(PS_Noise_IN In)
{
	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

	PS_OUT		Out = (PS_OUT)0;

	if (In.vTimer.x < g_fAppearTimer)
	{
		In.vTexUV = saturate((In.vTexUV - noisingdir * (g_fAppearTimer - In.vTimer.x)));
		In.texCoords1 = saturate((In.texCoords1 - noisingdir * (g_fAppearTimer - In.vTimer.x)));
		In.texCoords2 = saturate((In.texCoords2 - noisingdir * (g_fAppearTimer - In.vTimer.x)));
		In.texCoords3 = saturate((In.texCoords3 - noisingdir * (g_fAppearTimer - In.vTimer.x)));
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

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse = vector(pow(fireColor, 1.f / 2.2f).xyz, fireColor.a);

	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;

	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;
	Out.vDiffuse = saturate(Out.vDiffuse);
	Out.vEmissive = vector(Out.vDiffuse.a * g_fEmissive.x, g_fEmissive.y, g_fEmissive.z, 1);

	return Out;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PS_OUT_NODEFERRED PS_Distortion_All(PS_Noise_IN In)
{
	PS_OUT_NODEFERRED		Out = (PS_OUT_NODEFERRED)0;

	//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture


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


	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));
	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);
	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));
	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);
	Out.vDiffuse = BackBuffer;
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	//Out.vDepth.a = Out.vWorldPosition.a = 0;
	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}


PS_OUT_NODEFERRED PS_Distortion_All_Bright(PS_Noise_IN In)
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



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	Out.vDiffuse = pow(BackBuffer,1.f/2.2f);
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	//Out.vDepth.a = Out.vWorldPosition.a = 0;

	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}

PS_OUT_NODEFERRED PS_Distortion_DiffuseMix(PS_Noise_IN In)
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



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	float MixRate = abs(0.5f - Out.vDiffuse.a) * 2.f;
	Out.vDiffuse = BackBuffer * (1 - MixRate) + Out.vDiffuse * (MixRate);
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}

PS_OUT_NODEFERRED PS_Distortion_DiffuseMix_Bright(PS_Noise_IN In)
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

	fireColor *= alphaColor;
	Out.vDiffuse = pow(fireColor, 1.f/2.2f);
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse.a = fireColor.a;

	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;
	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);
	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = pow(g_BackBufferTexture.Sample(ClampSampler, TargetUV),1.f/2.2f);

	float MixRate = abs(0.5f - Out.vDiffuse.a) * 2.f;

	Out.vDiffuse = BackBuffer * (1 - MixRate) + Out.vDiffuse * (MixRate);
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}


PS_OUT_NODEFERRED PS_Distortion_ClolorMix(PS_Noise_IN In)
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

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse.a = fireColor.a;

	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;

	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;


	float ColorRate = max((fireColor.a - 0.5f) * 2.f, 0);
	Out.vDiffuse = vector(pow(In.vColor, (2.2f - ColorRate * 1.2f)).xyz, Out.vDiffuse.a);



	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	float MixRate = abs(0.5f - Out.vDiffuse.a) * 2.f;
	Out.vDiffuse = BackBuffer * (1 - MixRate) + Out.vDiffuse * (MixRate);
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}

PS_OUT_NODEFERRED PS_Distortion_ClolorMix_Bright(PS_Noise_IN In)
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

	fireColor *= alphaColor;
	fireColor.a = length(alphaColor.xyz) * In.vColor.a;
	Out.vDiffuse.a = fireColor.a;

	float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	vUV.x = vUV.x * 0.5f + 0.5f;
	vUV.y = vUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fViewZ = vDepthDesc.x * 300.f;

	Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);

	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;


	float ColorRate = max((fireColor.a - 0.5f) * 2.f, 0);
	Out.vDiffuse = vector(pow(In.vColor, (0.5f + ColorRate * 1.2f)).xyz, Out.vDiffuse.a);


	vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + In.vTimer.x * g_fDistortionNoisingPushPower * noisingdir));

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y))*0.25f));


	vector BackBuffer = g_BackBufferTexture.Sample(ClampSampler, TargetUV);

	float MixRate = abs(0.5f - Out.vDiffuse.a) * 2.f;
	Out.vDiffuse = BackBuffer * (1 - MixRate) + Out.vDiffuse * (MixRate);
	//Out.vDiffuse =  BackBuffer * (1 - Alpha) + (Alpha * g_vMixColor);

	Out.vDiffuse.a = 1.f;
	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}

PS_OUT_WorldBlend PS_MAIN_NoSoft(PS_IN In)
{
	PS_OUT_WorldBlend		Out = (PS_OUT_WorldBlend)0;

	Out.vDiffuse = pow(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
	Out.vDiffuse *= In.vColor;


	//float2		vUV = In.vProjPos.xy / In.vProjPos.w;
	//vUV.x = vUV.x * 0.5f + 0.5f;
	//vUV.y = vUV.y * -0.5f + 0.5f;

	//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	//float		fViewZ = vDepthDesc.x * 300.f;


	//Out.vDiffuse.a = Out.vDiffuse.a * pow(saturate((fViewZ - In.vProjPos.w)), 1.5f);


	if (Out.vDiffuse.a < g_fAlphaTestValue)
		discard;


	Out.vWorldPosition = In.vWorldPos;
	Out.vEmissive = vector(1, 0.5f, 1.f, 1);
	Out.vLimLight = 0.f;
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Out.vDiffuse = saturate(Out.vDiffuse);
	return Out;
}




technique11		DefaultTechnique
{
	//0
	pass Point_OrigingColor_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_INST();
	}	  //0
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//4
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
	pass Point_NoiseBright_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect_Bright();
	}
	pass Point_NoiseBright_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseFireEffect_Bright();
	}
	pass Point_NoiseAppear_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseAppear();
	}
	pass Point_NoiseAppear_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseAppear();
	}
	pass Point_NoiseAppearBright_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseAppear_Bright();
	}
	pass Point_NoiseAppearBright_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_MAIN_NoiseAppear_Bright();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//12
	pass Point_Distortion_All_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_All();
	}
	pass Point_Distortion_All_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_All();
	}

	pass Point_Distortion_All_Bright_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_All_Bright();
	}
	pass Point_Distortion_All_Bright_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_All_Bright();
	}
	
	pass Point_Distortion_DiffuseMix_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_DiffuseMix();
	}
	pass Point_Distortion_DiffuseMix_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_DiffuseMix();
	}

	pass Point_Distortion_DiffuseMixBright_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_DiffuseMix_Bright();
	}
	pass Point_Distortion_DiffuseMixBright_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_DiffuseMix_Bright();
	}


	pass Point_Distortion_ColoreMix_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_ClolorMix();
	}
	pass Point_Distortion_ColoreMix_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_ClolorMix();
	}


	pass Point_Distortion_ColoreMix_Bright_CullCW
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_cw);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_ClolorMix_Bright();
	}
	pass Point_Distortion_ColoreMix_Bright_CullNone
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_Noise();
		PixelShader = compile ps_5_0 PS_Distortion_ClolorMix_Bright();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//24
	pass PS_MAIN_NoSoft
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN_INST();
		GeometryShader = compile gs_5_0 GS_MAIN_INST();
		PixelShader = compile ps_5_0 PS_MAIN_NoSoft();
	}


	
}