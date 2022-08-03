
#include "Shader_Define.hpp" 

texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;
texture2D			g_BlurTargetTexture;
texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;

texture2D			g_CurlTexture;

float				g_Alpha;


texture2D		g_SourDiffuseTexture;
texture2D		g_DestDiffuseTexture1;
texture2D		g_DestDiffuseTexture2;
texture2D		g_DestDiffuseTexture3;
texture2D		g_DestDiffuseTexture4;

texture2D		g_FilterTexture;
texture2D		g_BrushTexture;

texture2D		g_HeightMapTexture;

static const float PixelSize = 2.0 / 720.0f;
static const int nNumSteps = 1280;

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
	float3 g_vFogColor = float3(0, 0, 0);
};


cbuffer SSReflectionVSConstants
{
	float ViewAngleThreshold = 0.5f;
	//float ViewAngleThreshold = 0.2f;
	float EdgeDistThreshold = 1.f;
	//float EdgeDistThreshold	= 0.45f;
	float DepthBias = 1.f;
	//float DepthBias = 0.5f;
	float ReflectionScale = 1.0f;
	float4 PerspectiveValues;



	float g_WaveHeight = 3.f;; //< 파도의 높낮이
	float g_Speed = 10.f; //< 시간흘러가는 속도 조절 변수 : 울렁거리는 속도
	float g_WaveFrequency = 10.f; //< 파도가 치는 느낌을 더 빠르게 조절하는 변수
}


cbuffer BrushDesc
{
	float4		g_vBrushPos = float4(10.0f, 0.0f, 10.f, 1.f);
	float		g_fRadius = 3.f;
};
cbuffer MtrlDesc
{
	float		g_fMimMapSize = 20;
};
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

////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
		
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


	float2		texCoords1 : COLOR0;
	float2		texCoords2 : COLOR1;
	float2		texCoords3 : COLOR2;

};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vWorldNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float		bIsNotDraw : TEXCOORD3;
};

struct VS_OUT_SHADOW
{
	float4		vPosition : SV_POSITION;
	float4		vClipPosition : TEXCOORD1;
};

VS_OUT_SHADOW VS_Shadow(VS_IN In)
{

	VS_OUT_SHADOW			Out = (VS_OUT_SHADOW)0;


	Out.vPosition = mul(vector(In.vPosition, 1), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_LightProjMatrix);

	Out.vClipPosition = Out.vPosition;
	return Out;
};




VS_OUT VS_MAIN_TERRAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	if (In.vPosition.y <= -99999.f)
	{
		Out.bIsNotDraw = 1.f;
	}
	else
	{
		matrix			matWV, matWVP;

		matWV = mul(g_WorldMatrix, g_ViewMatrix);
		matWVP = mul(matWV, g_ProjMatrix);

		Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
		Out.vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
		Out.vTexUV = In.vTexUV;
		Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
		Out.vProjPos = Out.vPosition;
	}
	return Out;
}

VS_OUT_EnvNoise VS_EnvMappedWater_Noise(VS_IN In)
{
	VS_OUT_EnvNoise			Out = (VS_OUT_EnvNoise)0;

	float cosTime = cos(g_fTimer * g_Speed + In.vTexUV.y * g_WaveFrequency) * g_WaveHeight; //< 시간에 따른 코사인값
	In.vPosition.y += cosTime;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);


	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vNormal = vector((mul(vector(In.vNormal.xyz, 0), g_WorldMatrix)));

	Out.vViewPos = mul(vector(In.vPosition, 1.f), matWV);
	Out.vViewNormal = normalize(vector((mul(vector(In.vNormal.xyz, 0), matWV))));
	Out.vCsProjPos = Out.vPosition.xyz / Out.vPosition.w;

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
	float4		vWorldNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float		bIsNotDraw : TEXCOORD3;
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
struct PS_OUT_EnvMapped
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vWorldPosition : SV_TARGET4;
	vector		vLimLight : SV_TARGET5;
};


PS_OUT PS_MAIN_TERRAIN_Default(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (In.bIsNotDraw)
	{
		discard;
	}
	else {
		vector	vSourMtrlDiffuse = g_SourDiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse1 = g_DestDiffuseTexture1.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse2 = g_DestDiffuseTexture2.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse3 = g_DestDiffuseTexture3.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse4 = g_DestDiffuseTexture4.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);


		vector	vFilterColor = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);


		vector	vMtrlDiffuse = vSourMtrlDiffuse * (1.f - vFilterColor.a) + vDestMtrlDiffuse1 * (vFilterColor.a);
		vMtrlDiffuse = vMtrlDiffuse * (1.f - vFilterColor.r) + vDestMtrlDiffuse2 * (vFilterColor.r);
		vMtrlDiffuse = vMtrlDiffuse * (1.f - vFilterColor.g) + vDestMtrlDiffuse3 * (vFilterColor.g);
		vMtrlDiffuse = vMtrlDiffuse * (1.f - vFilterColor.b) + vDestMtrlDiffuse4 * (vFilterColor.b) ;



		float  FogShaderRate = 1 - min(  max((2.8f - In.vWorldPos.y), 0) / 2.8f,1);

		Out.vDiffuse = vMtrlDiffuse;

		Out.vDiffuse.a *= FogShaderRate;

		if (Out.vDiffuse.a == 0.0f)
			discard;


		Out.vSpecular = vector((Out.vDiffuse.r + Out.vDiffuse.g + Out.vDiffuse.b) * 0.34f, 0.167f, 0.f,1);
		
		Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, 0.f);
		Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
		Out.vEmissive = vector(g_fEmissive.xyz, 1);;
		Out.vLimLight = g_vLimLight;
	}

	return Out;
}

PS_OUT PS_MAIN_TERRAIN_WIRE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	if (In.bIsNotDraw)
	{
		discard;
	}
	else {
		vector	vBrushColor = (vector)0.f;

		if (g_vBrushPos.x - g_fRadius < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fRadius &&
			g_vBrushPos.z - g_fRadius < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fRadius)
		{
			float2		vBrushUV;

			vBrushUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fRadius)) / (2.f * g_fRadius);
			vBrushUV.y = ((g_vBrushPos.z + g_fRadius) - In.vWorldPos.z) / (2.f * g_fRadius);


			vBrushColor = g_BrushTexture.Sample(DefaultSampler, vBrushUV);
		}

		vector	vTileKinds = g_HeightMapTexture.Sample(PointSampler, In.vTexUV);

		//Out.vColor = vector(0, 0.5f, 0, 1) + vBrushColor;

		//vector	vTileKinds = g_HeightMapTexture.Sample(DefaultSampler, HeightUV);

		Out.vDiffuse = vTileKinds;

		if (vTileKinds.r <= 0.f)// 1 못가는곳
		{
			Out.vDiffuse = vector(0, 0.5f, 0, 1) + vBrushColor;
		}
		else if (vTileKinds.r <= 0.00785f) // 2 이하 특수타일
		{
			Out.vDiffuse = vector(0, 0, 1, 1) + vBrushColor;
		}
		else if(vTileKinds.r <= 0.01177f) // 움직일수 있는곳
		{
			Out.vDiffuse = vector(1, 0, 0, 1) + vBrushColor;
		}



		/* -1 -> 0*/
		/* 1 - > 1*/
		Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, 0.f);
		Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		Out.vSpecular = 1;
		Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
		Out.vEmissive = vector(g_fEmissive.xyz, 1);;
		Out.vLimLight = g_vLimLight;
	}

	return Out;
}

PS_OUT PS_MAIN_TERRAIN_EDIT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	if (In.bIsNotDraw)
	{
		discard;
	}
	else {

		vector	vSourMtrlDiffuse = g_SourDiffuseTexture.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse1 = g_DestDiffuseTexture1.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse2 = g_DestDiffuseTexture2.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse3 = g_DestDiffuseTexture3.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);
		vector	vDestMtrlDiffuse4 = g_DestDiffuseTexture4.Sample(DefaultSampler, In.vTexUV * g_fMimMapSize);


		//vector	vBrushColor = g_BrushTexture.Sample(DefaultSampler, In.vTexUV);	
		vector	vBrushColor = (vector)0.f;
		vector	vFilterColor = g_FilterTexture.Sample(DefaultSampler, In.vTexUV);

		if (g_vBrushPos.x - g_fRadius < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fRadius &&
			g_vBrushPos.z - g_fRadius < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fRadius)
		{
			float2		vBrushUV;

			vBrushUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fRadius)) / (2.f * g_fRadius);
			vBrushUV.y = ((g_vBrushPos.z + g_fRadius) - In.vWorldPos.z) / (2.f * g_fRadius);


			vBrushColor = g_BrushTexture.Sample(DefaultSampler, vBrushUV);
		}
		
	
		vector	vMtrlDiffuse = vSourMtrlDiffuse * (1.f - vFilterColor.a) + vDestMtrlDiffuse1 * (vFilterColor.a);
		vMtrlDiffuse = vMtrlDiffuse * (1.f - vFilterColor.r) + vDestMtrlDiffuse2 * (vFilterColor.r);
		vMtrlDiffuse = vMtrlDiffuse * (1.f - vFilterColor.g) + vDestMtrlDiffuse3 * (vFilterColor.g);
		vMtrlDiffuse = vMtrlDiffuse * (1.f - vFilterColor.b) + vDestMtrlDiffuse4 * (vFilterColor.b) + vBrushColor;



	/*	float	fShade = max(dot(normalize(g_vLightVector) * -1.f, In.vWorldNormal), 0.f);

		vector	vReflect = reflect(normalize(g_vLightVector), In.vWorldNormal);
		vector	vLook = normalize(In.vWorldPos - g_CamPosition);

		float	fSpecular = pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 30.f);

		Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
			+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;*/

		Out.vDiffuse = vMtrlDiffuse;

		Out.vNormal = vector(In.vWorldNormal.xyz * 0.5f + 0.5f, 0.f);
		Out.vDepth = vector(In.vProjPos.w / FarDist, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		Out.vSpecular = 1;
		Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
		Out.vEmissive = vector(g_fEmissive.xyz, 1);;
		Out.vLimLight = g_vLimLight;
	}
	return Out;
}

struct PS_IN_SHADOW
{
	float4		vPosition : SV_POSITION;
	float4		vClipPosition : TEXCOORD1;
};

struct PS_OUT_SHADOW
{
	vector		vDiffuse : SV_TARGET0;
};


PS_OUT_SHADOW PS_Shadow(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	float Depth = In.vClipPosition.z / In.vClipPosition.w;

	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, g_fOclussionObject);

	return Out;
}

PS_OUT_EnvMapped PS_EnvMappedWater_Noise_N_Distort(PS_IN_EMW_Noise In)
{
	PS_OUT_EnvMapped		Out = (PS_OUT_EnvMapped)0;

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

	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);


	// Pixel position and normal in view space
	float3 vsPos = In.vViewPos.xyz;
	float3 vsNorm = normalize(In.vViewNormal);

	// Calculate the camera to pixel direction
	float3 eyeToPixel = normalize(vsPos);

	// Calculate the reflected view direction
	float3 vsReflect = reflect(eyeToPixel, vsNorm);

	// The initial reflection color for the pixel

	float4	reflectColor = float4(0,0,0,0);

	if (vsReflect.z >= ViewAngleThreshold)
	{
		// Fade the reflection as the view angles gets close to the threshold
		float viewAngleThresholdInv = 1.0 - ViewAngleThreshold;
		float viewAngleFade = saturate(3.0 * (vsReflect.z - ViewAngleThreshold) / viewAngleThresholdInv);

		// Transform the View Space Reflection to clip-space
		float3 vsPosReflect = vsPos + vsReflect;
		float3 csPosReflect = mul(float4(vsPosReflect, 1.0), g_ProjMatrix).xyz / vsPosReflect.z;
		float3 csReflect = csPosReflect - In.vCsProjPos;

		// Resize Screen Space Reflection to an appropriate length.
		float reflectScale = PixelSize / length(csReflect.xy);
		csReflect *= reflectScale;

		// Calculate the first sampling position in screen-space
		float2 ssSampPos = (In.vCsProjPos + csReflect).xy;
		ssSampPos = ssSampPos * float2(0.5, -0.5) + 0.5;

		// Find each iteration step in screen-space
		float2 ssStep = csReflect.xy * float2(0.5, -0.5) * 10.f;

		// Build a plane laying on the reflection vector
		// Use the eye to pixel direction to build the tangent vector
		float4 rayPlane;
		float3 vRight = cross(normalize(eyeToPixel), normalize(vsReflect));
		rayPlane.xyz = normalize(cross(normalize(vsReflect), normalize(vRight)));
		rayPlane.w = dot(rayPlane.xyz, vsPos);

		// Iterate over the HDR texture searching for intersection
		[unroll(128)]
		for (int nCurStep = 0; nCurStep < nNumSteps; nCurStep += 10)
		{
			// Sample from depth buffer


			float curDepth = g_DepthTexture.Sample(PointClampSampler, ssSampPos).x;
			//float curDepth = DepthTex.SampleLevel(PointSampler, ssSampPos, 0.0).x;

			float curDepthLin = ConvertZToLinearDepth(curDepth);
			float3 curPos = CalcViewPos(In.vCsProjPos.xy + csReflect.xy * ((float)nCurStep + 10.0), curDepthLin);

			// Find the intersection between the ray and the scene
			// The intersection happens between two positions on the oposite sides of the plane
			if (rayPlane.w >= dot(rayPlane.xyz, curPos) + DepthBias)
			{
				// Calculate the actual position on the ray for the given depth value
				float3 vsFinalPos = vsPos + (vsReflect / abs(vsReflect.z)) * abs(curDepthLin - vsPos.z + DepthBias);
				float2 csFinalPos = vsFinalPos.xy / PerspectiveValues.xy / vsFinalPos.z;
				ssSampPos = csFinalPos.xy * float2(0.5, -0.5) + 0.5;

				// Get the HDR value at the current screen space location
					
				if (g_DepthTexture.Sample(PointClampSampler, ssSampPos).x == 1.f)
				{
					reflectColor = pow(g_BackBufferTexture.Sample(ClampSampler, PosToUv) + pow(vector(g_vFogColor, 0), 2.2f), 1.f / 1.0751f);
				}
				else
				{
					reflectColor.xyz = g_BackBufferTexture.Sample(PointClampSampler, ssSampPos).xyz + pow(vector(g_vFogColor,0),2.2f).xyz;
				}
			
				//reflectColor.xyz = HDRTex.SampleLevel(PointSampler, ssSampPos, 0.0).xyz;

				// Fade out samples as they get close to the texture edges
				float edgeFade = saturate(distance(ssSampPos, float2(0.5, 0.5)) * 2.0 - EdgeDistThreshold);

				// Calculate the fade value
				reflectColor.w = min(viewAngleFade, 1.0 - edgeFade * edgeFade);

				// Apply the reflection sacle
				reflectColor.w *= ReflectionScale;

				// Advance past the final iteration to break the loop
				nCurStep = nNumSteps;
			}

			// Advance to the next sample
			ssSampPos += ssStep;
		}
	}

	//return reflectColor;

	float MixRate = Out.vDiffuse.a;

	//Out.vDiffuse = reflectColor;

	//vector BackBuffer = pow(g_BackBufferTexture.Sample(ClampSampler, PosToUv), 1.f / 1.0751f);
	//Out.vDiffuse = BackBuffer * (1 - Out.vDiffuse.a) + Out.vDiffuse * (Out.vDiffuse.a);
	reflectColor.xyz = reflectColor.a * reflectColor.xyz + (1.f - reflectColor.a) *pow(g_BackBufferTexture.Sample(ClampSampler, PosToUv) +  pow(vector(g_vFogColor, 0), 2.2f), 1.f / 1.0751f);

	reflectColor.w = 1.f;



	Out.vDiffuse = reflectColor * (1 - MixRate) + Out.vDiffuse * (MixRate);



	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vEmissive = g_fEmissive;
	//Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
	//Out.vLimLight = g_vLimLight;

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
	pass Terrain_Default // 1
	{
		SetBlendState(AlphaBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_Default();
	}	
	pass Terrain_Wire // 2
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(FillMode_Wireframe);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_WIRE();
	}
	pass EnvMappedWater_Noise_N_Distort//3
	{
		SetBlendState(AlphaBlendingJustDiffuse, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_EnvMappedWater_Noise();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EnvMappedWater_Noise_N_Distort();
	}

}