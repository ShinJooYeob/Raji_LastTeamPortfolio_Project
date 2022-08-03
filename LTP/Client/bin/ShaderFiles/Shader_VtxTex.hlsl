
#include "Shader_Define.hpp"

texture2D			g_SourTexture;
texture2D			g_BackBufferTexture;
texture2D			g_BlurTargetTexture;
texture2D			g_DepthTexture;
texture2D			g_NoiseTexture;

texture2D			g_CurlTexture;

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

//static const float PixelSize = 2.0 / 720.0f;
//static const int nNumSteps = 1280;
//
///**/
//PS_OUT_EnvMapped PS_EnvMappedWater_Noise_N_Distort(PS_IN_EMW_Noise In)
//{
//	PS_OUT_EnvMapped		Out = (PS_OUT_EnvMapped)0;
//
//	vector noise1 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords1);
//	vector noise2 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords2);
//	vector noise3 = g_NoiseTexture.Sample(DefaultSampler, In.texCoords3);
//
//	noise1 = (noise1 - 0.5f) * 2.0f;
//	noise2 = (noise2 - 0.5f) * 2.0f;
//	noise3 = (noise3 - 0.5f) * 2.0f;
//
//	noise1.xy = noise1.xy * distortion1.xy;
//	noise2.xy = noise2.xy * distortion2.xy;
//	noise3.xy = noise3.xy * distortion3.xy;
//
//	vector finalNoise = noise1 + noise2 + noise3;
//	float perturb = saturate(((1.0f - length(In.vTexUV.xy)) * distortionScale) + distortionBias);
//	float2 noiseCoords = saturate((finalNoise.xy * perturb) + In.vTexUV.xy);
//
//
//
//	vector fireColor = g_DiffuseTexture.Sample(ClampSampler, noiseCoords.xy);
//	vector alphaColor = g_SourTexture.Sample(ClampSampler, noiseCoords.xy);
//
//	fireColor *= g_vColor;
//	fireColor = pow(fireColor, 1. / 1.86181f);
//	fireColor.a = length(alphaColor.xyz) * g_vColor.a;
//
//
//	Out.vDiffuse = saturate(fireColor);
//
//
//	////vector BlurDesc = g_NoiseTexture.Sample(DefaultSampler, (In.vTexUV + g_fTimer * g_fDistortionNoisingPushPower * noisingdir));
//	////float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);
//
//	////vector		vNormal = vector(0,1,0, 0.f);
//
//
//
//	////vector ReflectDir = mul(mul(reflect(normalize(vector((g_vLightPosition - In.vWorldPos).xyz,0)), vNormal) , g_ViewMatrix), g_ProjMatrix);
//	////float2 ReflectUV = normalize(ReflectDir.xy);
//	////	
//	////ReflectUV.x = (ReflectUV.x) * XTexelSize * 32.36f;
//	////ReflectUV.y = (ReflectUV.y ) *  YTexelSize * 32.36f;
//
//	//////////////float2 TargetUV = saturate(float2(PosToUv.x , PosToUv.y));
//	////float2 TargetUV = float2(PosToUv.x + (ReflectUV.x) , PosToUv.y + (ReflectUV.y) );
//	////TargetUV.x = min(max(TargetUV.x, 0), 1);
//	////TargetUV.y = min(max(TargetUV.y, 0), 1);
//
//
//	//////float2 TargetUV = saturate(float2(PosToUv.x + (0.5f - (BlurDesc.x)) * 0.15625f, PosToUv.y + (0.5f - (BlurDesc.y)) * 0.25f));
//
//	//vector BackBuffer = pow(g_BackBufferTexture.Sample(ClampSampler, TargetUV), 1.f / 1.0751f);
//	//Out.vDiffuse = BackBuffer * (1 - MixRate) + Out.vDiffuse * (MixRate);
//	//Out.vDiffuse.a = 1.f;
//	//Out.vDiffuse = saturate(Out.vDiffuse);
//
//
//	/*
//	//float4      vPosition : SV_POSITION;
//	//float4		vNormal : NORMAL;
//	//float2      vTexUV : TEXCOORD0;
//	//float4		vViewPos : TEXCOORD1;
//	//float4		vViewNormal : TEXCOORD2;
//	//float4		vCsProjPos : TEXCOORD3;
//	*/
//
//	// Pixel position and normal in view space
//	float3 vsPos = In.vViewPos.xyz;
//	float3 vsNorm = normalize(In.vViewNormal);
//
//	// Calculate the camera to pixel direction
//	float3 eyeToPixel = normalize(vsPos);
//
//	// Calculate the reflected view direction
//	float3 vsReflect = reflect(eyeToPixel, vsNorm);
//
//	// The initial reflection color for the pixel
//
//	float4 reflectColor = float4(0,0,0,0);
//
//	if (vsReflect.z >= ViewAngleThreshold)
//	{
//		// Fade the reflection as the view angles gets close to the threshold
//		float viewAngleThresholdInv = 1.0 - ViewAngleThreshold;
//		float viewAngleFade = saturate(3.0 * (vsReflect.z - ViewAngleThreshold) / viewAngleThresholdInv);
//
//		// Transform the View Space Reflection to clip-space
//		float3 vsPosReflect = vsPos + vsReflect;
//		float3 csPosReflect = mul(float4(vsPosReflect, 1.0), g_ProjMatrix).xyz / vsPosReflect.z;
//		float3 csReflect = csPosReflect - In.vCsProjPos;
//
//		// Resize Screen Space Reflection to an appropriate length.
//		float reflectScale = PixelSize / length(csReflect.xy);
//		csReflect *= reflectScale;
//
//		// Calculate the first sampling position in screen-space
//		float2 ssSampPos = (In.vCsProjPos + csReflect).xy;
//		ssSampPos = ssSampPos * float2(0.5, -0.5) + 0.5;
//
//		// Find each iteration step in screen-space
//		float2 ssStep = csReflect.xy * float2(0.5 , -0.5) * 10.f;
//
//		// Build a plane laying on the reflection vector
//		// Use the eye to pixel direction to build the tangent vector
//		float4 rayPlane;
//		float3 vRight = cross(normalize(eyeToPixel), normalize(vsReflect));
//		rayPlane.xyz = normalize(cross(normalize(vsReflect), normalize(vRight)));
//		rayPlane.w = dot(rayPlane.xyz, vsPos);
//
//		// Iterate over the HDR texture searching for intersection
//		[unroll(128)]
//		for (int nCurStep = 0; nCurStep < nNumSteps; nCurStep+=10)
//		{
//			// Sample from depth buffer
//
//		
//			float curDepth = g_DepthTexture.Sample(PointClampSampler, ssSampPos).x;
//			//float curDepth = DepthTex.SampleLevel(PointSampler, ssSampPos, 0.0).x;
//			
//			float curDepthLin = ConvertZToLinearDepth(curDepth);
//			float3 curPos = CalcViewPos(In.vCsProjPos.xy + csReflect.xy * ((float)nCurStep + 10.0), curDepthLin);
//
//			// Find the intersection between the ray and the scene
//			// The intersection happens between two positions on the oposite sides of the plane
//			if (rayPlane.w >= dot(rayPlane.xyz, curPos) + DepthBias)
//			{
//				// Calculate the actual position on the ray for the given depth value
//				float3 vsFinalPos = vsPos + (vsReflect / abs(vsReflect.z)) * abs(curDepthLin - vsPos.z + DepthBias);
//				float2 csFinalPos = vsFinalPos.xy / PerspectiveValues.xy / vsFinalPos.z;
//				ssSampPos = csFinalPos.xy * float2(0.5, -0.5) + 0.5;
//
//				// Get the HDR value at the current screen space location
//				reflectColor.xyz = g_BackBufferTexture.Sample(PointClampSampler, ssSampPos).xyz;
//				//reflectColor.xyz = HDRTex.SampleLevel(PointSampler, ssSampPos, 0.0).xyz;
//
//				// Fade out samples as they get close to the texture edges
//				float edgeFade = saturate(distance(ssSampPos, float2(0.5, 0.5)) * 2.0 - EdgeDistThreshold);
//
//				// Calculate the fade value
//				reflectColor.w = min(viewAngleFade, 1.0 - edgeFade * edgeFade);
//
//				// Apply the reflection sacle
//				reflectColor.w *= ReflectionScale;
//
//				// Advance past the final iteration to break the loop
//				nCurStep = nNumSteps;
//			}
//
//			// Advance to the next sample
//			ssSampPos += ssStep;
//		}
//	}
//
//	//return reflectColor;
//
//	float2 PosToUv = float2(In.vPosition.x / 1280, In.vPosition.y / 720);
//
//
//
//	float MixRate = Out.vDiffuse.a;
//
//	Out.vDiffuse = reflectColor;
//
//	vector BackBuffer = pow(g_BackBufferTexture.Sample(ClampSampler, PosToUv), 1.f / 1.0751f);
//	Out.vDiffuse = BackBuffer * (1 - Out.vDiffuse.a) + Out.vDiffuse * (Out.vDiffuse.a);
//
//
//	if (reflectColor.a == 0)
//	{
//		MixRate = 1.f;
//		reflectColor = pow(g_BackBufferTexture.Sample(ClampSampler, PosToUv), 1.f / 1.0751f);
//	}
//
//
//	Out.vDiffuse = reflectColor * (1 - MixRate) + Out.vDiffuse * (MixRate);
//
//
//
//	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
//	Out.vEmissive = g_fEmissive;
//	//Out.vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
//	Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
//	//Out.vLimLight = g_vLimLight;
//
//	return Out;
//}
//
///**/
//PS_OUT_EnvMapped PS_EnvMappedWater_Noise_N_Distort2(PS_IN_EMW_Noise In)
//{
//	PS_OUT_EnvMapped		Out = (PS_OUT_EnvMapped)0;
//
//	discard;
//	
//	return Out;
//}



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
	//pass EnvMappedWater_Noise_N_Distort//9
	//{
	//	SetBlendState(AlphaBlendingJustDiffuse, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	//	SetDepthStencilState(ZTestAndWriteState, 0);
	//	SetRasterizerState(CullMode_None);

	//	VertexShader = compile vs_5_0 VS_EnvMappedWater_Noise();
	//	GeometryShader = NULL;
	//	PixelShader = compile ps_5_0 PS_EnvMappedWater_Noise_N_Distort2();
	//}
	//
	
}