
#define FloatCorrectionValue 0.00000125f
//#define FloatCorrectionValue 0.000125f
#define XTexelSize		0.00078125f
#define YTexelSize		0.00138889f
#define	DiagTexelSize	0.00159353f
#define FarDist			300.f
#define vec2			float2
#define vec3			float3
#define vec4			float4
#define mix				lerp

texture2D			g_TargetTexture;
texture2D			g_SourTexture;
texture2D			g_DiffuseTexture;
texture2D			g_NormalTexture;
texture2D			g_SpecularTexture;
texture2D			g_EmissiveTexture;
texture2D			g_DepthTexture;
texture2D			g_WorldPosTexture;
texture2D			g_LimLightTexture;
texture2D			g_ShadeTexture;
texture2D			g_MaskTexture;

texture2D			g_NoiseTexture;

texture2D			g_ShadowMapTexture;

texture2D			g_BluredTexture;
texture2D			g_UpScaledTexture1;
texture2D			g_UpScaledTexture2;
texture2D			g_UpScaledTexture3;
texture2D			g_UpScaledTexture4;
texture2D			g_UpScaledTexture5;

texture2D			g_OldWorldPosTexture;
texture2D			g_VelocityMapTexture;


cbuffer	RenderingPipeLine
{
	matrix			g_WorldMatrix;
	matrix			g_ViewMatrix;
	matrix			g_ProjMatrix;
};
cbuffer InverseMatrices
{
	matrix			g_ProjMatrixInv;
	matrix			g_ViewMatrixInv;
};
cbuffer	ShadowPipeLine
{
	matrix			g_LightViewMatrix;
	matrix			g_LightProjMatrix;
};

cbuffer LightDesc
{
	vector			g_vLightDir;
	vector			g_vLightPos;
	float			g_fLightRange;
	vector			g_vLightDiffuse;
	vector			g_vLightAmbient;
	vector			g_vLightSpecular;
	vector			g_vCamPosition;
};

cbuffer MtrlDesc
{
	vector			g_vMtrlDiffuse = (vector)1.f;
	vector			g_vMtrlAmbient = (vector)1.f;
	vector			g_vMtrlSpecular = (vector)1.f;
	vector			g_vOcdMaskColor = (vector)1.f;
};

cbuffer ScreenSizeBuffer
{
	float				fScreemWidth = 1280;
	float				fScreemHeight = 720;
	float2				iResolution = float2(1280, 720);
	float2				iMouse = float2(100, 10);
};

cbuffer ForPostProcessing
{
	float				g_fTexelSize = 2.f;
	
	float				g_fShadowIntensive = 0.65f;

	float3				g_vDofTargetPosition = 0;
	float				g_fDofTargetLength = 10;
	
	float				g_fOverLuminence = 1.8f;
	float				g_fBloomMul = 1.f;

	float				g_fGodRayLength = 64;
	float				g_fGodRayNumDelta = 1.f / 63.f;
	float				g_fGodRayIntensity = 0.5f;

	float				g_fToonShadingValue = 1.f;
	float				g_fToonMaxIntensive = 5.f;
	float				g_MixRate = 0.f;
};
cbuffer cbFog					
{
	float3 FogColor = float3(0.5f, 0.5f, 0.5f);
	float FogStartDist = 37.0f;
	float3 FogHighlightColor = float3(0.8f, 0.7f, 0.4f);
	float FogGlobalDensity = 1.5f;
	float3 FogSunDir;
	float FogHeightFalloff = 0.2f;
}

cbuffer RayTraceConstants
{
	float2 SunPos;
	float InitDecay;
	float DistDecay;
	float3 RayColor;
	float MaxDeltaLen;

	float	g_SunSize =  10.f;
	float	g_LensfaleSupportSunSize = 128.f;
}
cbuffer VelocityMatrix
{
	matrix		g_matNowView;
	matrix		g_matNowProj;

	matrix		g_matOldView;
	matrix		g_matOldProj;
}



sampler DefaultSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};
sampler WrapSampler = sampler_state
{
	// D3D11_SAMPLER_DESC
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float2 UV2FragCoord(float2 UV)
{
	return float2(1.f - UV.x, UV.y);
}

float3 ApplyFog(float3 originalColor, float eyePosY, float3 eyeToPixel)
{
	float pixelDist = length(eyeToPixel);
	float3 eyeToPixelNorm = eyeToPixel / pixelDist;

	// Find the fog staring distance to pixel distance
	float fogDist = max(pixelDist - FogStartDist, 0.0);

	// Distance based fog intensity
	float fogHeightDensityAtViewer = exp(-FogHeightFalloff * eyePosY);
	float fogDistInt = fogDist * fogHeightDensityAtViewer;

	// Height based fog intensity
	float eyeToPixelY = eyeToPixel.y * (fogDist / pixelDist);
	float t = FogHeightFalloff * eyeToPixelY;
	const float thresholdT = 0.01;
	float fogHeightInt = abs(t) > thresholdT ?
		(1.0 - exp(-t)) / t : 1.0;

	// Combine both factors to get the final factor
	float fogFinalFactor = exp(-FogGlobalDensity * fogDistInt * fogHeightInt);

	// Find the sun highlight and use it to blend the fog color
	float sunHighlightFactor = saturate(dot(eyeToPixelNorm, FogSunDir));
	sunHighlightFactor = pow(sunHighlightFactor, 8.0);
	float3 fogFinalColor = lerp(FogColor, FogHighlightColor, sunHighlightFactor);

	return lerp(fogFinalColor, originalColor, fogFinalFactor);
}

float3 Calculate_ClipUV_N_CurrentDepth(texture2D WorldTexture, float2 UVPos, matrix mViewProj)
{
	vector		vWorldPosition = WorldTexture.Sample(DefaultSampler, UVPos);
	//float		fViewZ = vDepthDesc.x * FarDist;
	vector		vWorldPos = vector(vWorldPosition.xyz, 1);

	float4 ClipPos = mul(vWorldPos, g_LightViewMatrix);
	ClipPos = mul(ClipPos, g_LightProjMatrix);

	float CurrentDepth = ClipPos.z / ClipPos.w;
	float2 UV = ClipPos.xy / ClipPos.w;

	UV.y = UV.y * -0.5f + 0.5f;
	UV.x = UV.x * 0.5f + 0.5f;

	float3 Out = float3(UV.x, UV.y, CurrentDepth);

	return Out;
}


float2 iChannelResolution = float2(256, 256);

float noise(float t)
{
	return g_NoiseTexture.Sample(DefaultSampler, UV2FragCoord( float2(t, .0) )/ iChannelResolution.xy).x;
}
float noise(float2 t)
{
	return g_NoiseTexture.Sample(DefaultSampler, UV2FragCoord(t) / iChannelResolution.xy).x;
}

vec3 lensflare(vec2 uv, vec2 pos)
{
	float2 main = uv - pos;
	float2 uvd = uv*(length(uv));

	float ang = atan2(main.x, main.y);
	float dist = length(main); dist = pow(dist, .1);
	float n = noise(vec2(ang*16.0, dist*32.0));

	float f0 = 1.0 / (length(uv - pos)* g_LensfaleSupportSunSize + 1.0);
	//float f0 = 1.0 / (length(uv - pos)*16.0 + 1.0);

	//제일 큰 해모양
	f0 = f0 + f0*(sin(noise(sin(ang*2. + pos.x)*4.0 - cos(ang*3. + pos.y))*16.)*.1 + dist*.1 + .8);
	//f0 = 0;


	//제일 멀리있는 찌부된 원
	float f1 = max(0.01 - pow(length(uv + 1.2*pos), 1.9), .0)*7.0;

	float f2 = max(1.0 / (1.0 + 32.0*pow(length(uvd + 0.8*pos), 2.0)), .0)*00.25;
	float f22 = max(1.0 / (1.0 + 32.0*pow(length(uvd + 0.85*pos), 2.0)), .0)*00.23;
	float f23 = max(1.0 / (1.0 + 32.0*pow(length(uvd + 0.9*pos), 2.0)), .0)*00.21;


	vec2 uvx = mix(uv, uvd, -0.5);

	float f4 = max(0.01 - pow(length(uvx + 0.4*pos), 2.4), .0)*6.0;
	float f42 = max(0.01 - pow(length(uvx + 0.45*pos), 2.4), .0)*5.0;
	float f43 = max(0.01 - pow(length(uvx + 0.5*pos), 2.4), .0)*3.0;

	uvx = mix(uv, uvd, -.4);

	float f5 = max(0.01 - pow(length(uvx + 0.2*pos), 5.5), .0)*2.0;
	float f52 = max(0.01 - pow(length(uvx + 0.4*pos), 5.5), .0)*2.0;
	float f53 = max(0.01 - pow(length(uvx + 0.6*pos), 5.5), .0)*2.0;

	uvx = mix(uv, uvd, -0.5);

	//제일 가까이에 있는 찌부된 원
	float f6 = max(0.01 - pow(length(uvx - 0.3*pos), 1.6), .0)*6.0;
	float f62 = max(0.01 - pow(length(uvx - 0.325*pos), 1.6), .0)*3.0;
	float f63 = max(0.01 - pow(length(uvx - 0.35*pos), 1.6), .0)*5.0;

	//f6 = f62 = f63 = 0;

	vec3 c = vec3(0.0,0.0, 0.0);

	c.r += f2 + f4 + f5 + f6; c.g += f22 + f42 + f52 + f62; c.b += f23 + f43 + f53 + f63;
	float temp = length(uvd)*.05;
	c = c*1.3 - vec3(temp, temp, temp);
	c += vec3(f0, f0, f0);

	return c;
}

vec3 cc(vec3 color, float factor, float factor2) // color modifier
{
	float w = color.x + color.y + color.z;
	return mix(color, vec3(w, w, w)*factor, w*factor2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};
struct VS_OUT_ForAvgLumi
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float		fNeedToDraw : TEXCOORD1;
};
struct VS_OUTVelocityMap
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vNormal : TEXCOORD1;
	float4		vDir : TEXCOORD2;
	
};


struct VS_OUT_BLUR
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT_ForAvgLumi VS_ForLumi(VS_IN In)
{
	VS_OUT_ForAvgLumi			Out = (VS_OUT_ForAvgLumi)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	if (In.vPosition.x < 0 && In.vPosition.y > 0)
	{
		Out.fNeedToDraw = 0;
	}
	else
	{
		Out.fNeedToDraw = 1;
	}

	return Out;
}


VS_OUT_BLUR VS_VerticleBlur(VS_IN In)
{
	VS_OUT_BLUR			Out = (VS_OUT_BLUR)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	float texelSize = g_fTexelSize / fScreemWidth;


	Out.texCoord1 = In.vTexUV + float2(texelSize * -4.0f, 0.0f);
	Out.texCoord2 = In.vTexUV + float2(texelSize * -3.0f, 0.0f);
	Out.texCoord3 = In.vTexUV + float2(texelSize * -2.0f, 0.0f);
	Out.texCoord4 = In.vTexUV + float2(texelSize * -1.0f, 0.0f);
	Out.texCoord5 = In.vTexUV + float2(texelSize *  0.0f, 0.0f);
	Out.texCoord6 = In.vTexUV + float2(texelSize *  1.0f, 0.0f);
	Out.texCoord7 = In.vTexUV + float2(texelSize *  2.0f, 0.0f);
	Out.texCoord8 = In.vTexUV + float2(texelSize *  3.0f, 0.0f);
	Out.texCoord9 = In.vTexUV + float2(texelSize *  4.0f, 0.0f);



	return Out;
}

VS_OUT_BLUR VS_HorizBlur(VS_IN In)
{
	VS_OUT_BLUR			Out = (VS_OUT_BLUR)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	float texelSize = g_fTexelSize / fScreemHeight;


	Out.texCoord1 = In.vTexUV + float2(0.0f, texelSize * -4.0f);
	Out.texCoord2 = In.vTexUV + float2(0.0f, texelSize * -3.0f);
	Out.texCoord3 = In.vTexUV + float2(0.0f, texelSize * -2.0f);
	Out.texCoord4 = In.vTexUV + float2(0.0f, texelSize * -1.0f);
	Out.texCoord5 = In.vTexUV + float2(0.0f, texelSize *  0.0f);
	Out.texCoord6 = In.vTexUV + float2(0.0f, texelSize *  1.0f);
	Out.texCoord7 = In.vTexUV + float2(0.0f, texelSize *  2.0f);
	Out.texCoord8 = In.vTexUV + float2(0.0f, texelSize *  3.0f);
	Out.texCoord9 = In.vTexUV + float2(0.0f, texelSize *  4.0f);



	return Out;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_IN_ForAvgLumi
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float		fNeedToDraw : TEXCOORD1;
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};
struct PS_OUT_AfterDeferred
{
	vector		vColor : SV_TARGET0;
	vector		vColor2 : SV_TARGET1;
};

struct PS_OUT_TargetThree
{
	vector		vColor : SV_TARGET0;
	vector		vColor2 : SV_TARGET1;
	vector		vColor3 : SV_TARGET2;
};
struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

struct PS_OUT_Mtrl
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vDepth : SV_TARGET4;
	vector		vWorldPosition : SV_TARGET5;
	vector		vLimLight : SV_TARGET6;
};

struct PS_IN_BLUR
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;

	float2 texCrd1 : TEXCOORD1;
	float2 texCrd2 : TEXCOORD2;
	float2 texCrd3 : TEXCOORD3;
	float2 texCrd4 : TEXCOORD4;
	float2 texCrd5 : TEXCOORD5;
	float2 texCrd6 : TEXCOORD6;
	float2 texCrd7 : TEXCOORD7;
	float2 texCrd8 : TEXCOORD8;
	float2 texCrd9 : TEXCOORD9;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PS_OUT PS_MAIN_RECT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT	Out = (PS_OUT_LIGHT)0;

	vector		vEmissiveDesc	= g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc		= g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDepthDesc		= g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vMtrlSpecularMap = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	
	float		fViewZ = vDepthDesc.x * FarDist;

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = g_vLightDiffuse * saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * 0.2f);
	//Out.vShade = 0.5f;

	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector		vWorldPos;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * w나누기. */
	//vWorldPos.x = (In.vTexUV.x * 2.f - 1.f);
	//vWorldPos.y = (In.vTexUV.y * -2.f + 1.f);
	//vWorldPos.z = vDepthDesc.y; /* 0 ~ 1 */
	//vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */  
	vWorldPos.w = 1.f * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	/* 로컬위치 * 월드행렬 */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);


	vector		vLook = vWorldPos - g_vCamPosition;
	

	Out.vSpecular = (g_vLightSpecular * 1.f) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30);
	Out.vSpecular.a = 0.f;

	if (vEmissiveDesc.x > 0)
	{
		Out.vShade = Out.vShade + vEmissiveDesc.x;
		Out.vSpecular = max(Out.vSpecular - vEmissiveDesc.x,0);
		Out.vSpecular.a = 0.f;
	}


	return Out;	
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT	Out = (PS_OUT_LIGHT)0;

	vector		vEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vMtrlSpecularMap = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);

	float		fViewZ = vDepthDesc.x * FarDist;

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vWorldPos;

	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = 1.f * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬 */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	//Out.vEmissive = max((g_fLightRange * 0.25f) - fDistance, 0);
		

	float		fAtt = max(g_fLightRange - fDistance, 0.f) / g_fLightRange;

		
	Out.vShade = (g_vLightDiffuse * saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient *  0.2f)) * fAtt;
	 
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vLightDir), vNormal);

	vector		vLook = vWorldPos - g_vCamPosition;

	//Out.vSpecular = (g_vLightSpecular * 1) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), pow(1 / vMtrlSpecularMap.g, 2)) * fAtt;
	Out.vSpecular = (g_vLightSpecular * 1.f) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30) * fAtt;
	Out.vSpecular.a = 0.f;

	if (vEmissiveDesc.x > 0)
	{
		Out.vShade = Out.vShade + vEmissiveDesc.x;
		Out.vSpecular = Out.vSpecular + vEmissiveDesc.x;
		Out.vSpecular.a = 0.f;
	}

	return Out;
}
PS_OUT_AfterDeferred PS_MAIN_EndDeferred(PS_IN In)
{
	PS_OUT_AfterDeferred		Out = (PS_OUT_AfterDeferred)0;

	//vector		vDiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vDiffuseDesc = pow(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
	vector		vShadeDesc = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vSpecularDesc = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vWorldPositionDesc = g_WorldPosTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vShadowMapDesc = g_ShadowMapTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vMtrlNormalMapDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	
	
	Out.vColor = vDiffuseDesc * vShadeDesc + vSpecularDesc;


	vector		vLimLightDesc = g_LimLightTexture.Sample(DefaultSampler, In.vTexUV);


	if (vLimLightDesc.a > 0)
	{
		vector		vNormal = vector(vMtrlNormalMapDesc.xyz * 2.f - 1.f, 0.f);
		float3 vCameraPos = normalize(g_vCamPosition.xyz - vWorldPositionDesc.xyz);
		float RimLightColor = smoothstep(1.0f - vLimLightDesc.a, 1.0f, 1 - max(0, dot(vNormal.xyz, vCameraPos)));
		Out.vColor += RimLightColor * vector(vLimLightDesc.rgb, 0);
	}

	//vector Hdr = max((g_TargetTexture.Sample(DefaultSampler, In.vTexUV) + LinerTex) - 0.004, 0);
	//vector Hdr =max(Out.vColor - 0.004f,0);
	//Out.vColor = (Hdr * (6.2f * Hdr + 0.5f)) / (Hdr * (6.2f * Hdr + 1.7f) + 0.06f);

	Out.vColor = pow(Out.vColor, 1.f / 2.2f);

	if (Out.vColor.a == 0.0f)
		discard;

	

	if (vWorldPositionDesc.g > 0)
	{
		Out.vColor *= (1 - vShadowMapDesc);
	}

	//if (dot(vDiffuseDesc.xyz, float3(0.2125f, 0.7154f, 0.0721f)) > vAvgLumineceMapDesc.r)
	//{
	//	Out.vColor3 = 1.f;
	//}

	Out.vColor2 = Out.vColor;
	return Out;
}
PS_OUT PS_MAIN_GausianBlur(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = pow(g_UpScaledTexture1.Sample(DefaultSampler, In.vTexUV), 2.2f)+
		pow(g_UpScaledTexture2.Sample(DefaultSampler, In.vTexUV), 2.2f) +
		pow(g_UpScaledTexture3.Sample(DefaultSampler, In.vTexUV), 2.2f) +
		pow(g_UpScaledTexture4.Sample(DefaultSampler, In.vTexUV), 2.2f) +
		pow(g_UpScaledTexture5.Sample(DefaultSampler, In.vTexUV), 2.2f);


	Out.vColor /= 5.f;

	Out.vColor = pow(Out.vColor ,1/2.2f);

	return Out;
}

PS_OUT PS_Blur(PS_IN_BLUR In)
{
	PS_OUT		Out = (PS_OUT)0;

	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;

	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	//weight0 = 	weight1 = 	weight2 =	weight3 = 	weight4 = 0.1f;

	normalization = (weight0 + 2.f * (weight1 + weight2 + weight3 + weight4));

	//// 가중치들을 정규화합니다.
	//weight0 = weight0 / normalization;
	//weight1 = weight1 / normalization;
	//weight2 = weight2 / normalization;
	//weight3 = weight3 / normalization;
	//weight4 = weight4 / normalization;

	// 색깔을 검정색으로 초기화합니다.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 수평선상의 아홉 픽셀값들을 가중치를 곱해 더합니다.
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd1), 2.2f) * weight4;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd2), 2.2f) * weight3;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd3), 2.2f) * weight2;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd4), 2.2f) * weight1;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd5), 2.2f) * weight0;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd6), 2.2f) * weight1;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd7), 2.2f) * weight2;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd8), 2.2f) * weight3;
	color +=  pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd9), 2.2f) * weight4;
	color /= normalization;
	// 알파 채널을 1로 설정합니다.

	color = pow(color, 1.f / 2.2f);
	color.a = 1.0f;

	Out.vColor = color;
	return Out;
}

PS_OUT PS_Masking_Blur(PS_IN_BLUR In)
{
	PS_OUT		Out = (PS_OUT)0;

	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;

	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	//weight0 = 	weight1 = 	weight2 =	weight3 = 	weight4 = 0.1f;

	normalization = (weight0 + 2.f * (weight1 + weight2 + weight3 + weight4));

	//// 가중치들을 정규화합니다.
	//weight0 = weight0 / normalization;
	//weight1 = weight1 / normalization;
	//weight2 = weight2 / normalization;
	//weight3 = weight3 / normalization;
	//weight4 = weight4 / normalization;

	// 색깔을 검정색으로 초기화합니다.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	
	// 수평선상의 아홉 픽셀값들을 가중치를 곱해 더합니다.
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd1).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd1), 2.2f) * weight4;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd2).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd2), 2.2f) * weight3;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd3).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd3), 2.2f) * weight2;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd4).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd4), 2.2f) * weight1;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd5).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd5), 2.2f) * weight0;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd6).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd6), 2.2f) * weight1;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd7).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd7), 2.2f) * weight2;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd8).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd8), 2.2f) * weight3;
	color += g_MaskTexture.Sample(DefaultSampler, In.texCrd9).b * pow(g_TargetTexture.Sample(DefaultSampler, In.texCrd9), 2.2f) * weight4;
	color /= normalization;
	// 알파 채널을 1로 설정합니다.

	color = pow(color, 1.f / 2.2f);
	color.a = 1.0f;

	Out.vColor = color;
	return Out;
}

PS_OUT_AfterDeferred PS_ShadowDrawLightWorldToWorld(PS_IN In)
{
	PS_OUT_AfterDeferred		Out = (PS_OUT_AfterDeferred)0;




	vector		vWorldPosition = g_WorldPosTexture.Sample(DefaultSampler, In.vTexUV);
	//float		fViewZ = vDepthDesc.x * FarDist;

	matrix LightViewProj = mul(g_LightViewMatrix, g_LightProjMatrix);
	float3 ClipUV_N_CurrentDepth = Calculate_ClipUV_N_CurrentDepth(g_WorldPosTexture, In.vTexUV, LightViewProj);

	vector		vWorldPos = vector(vWorldPosition.xyz, 1);
	vector		vShadowDesc = g_TargetTexture.Sample(DefaultSampler, ClipUV_N_CurrentDepth.xy);

	//if ()
	//	discard;


	float ShadowDepth = vShadowDesc.r;


	if (length(float2(SunPos.x *iResolution.x / iResolution.y, SunPos.y) -
		float2(In.vTexUV.x  *iResolution.x / iResolution.y, In.vTexUV.y)) < DiagTexelSize * g_SunSize)
	{
		//(vWorldPosition.x == 1 && )
		Out.vColor2 = 1.f;

	}


	if (ClipUV_N_CurrentDepth.z > ShadowDepth + FloatCorrectionValue)
	{
		if (vShadowDesc.a != 0.0f && vWorldPosition.x != 1)
		{
			Out.vColor = vector(g_fShadowIntensive, g_fShadowIntensive, g_fShadowIntensive, 1);
			Out.vColor2 = 0.f;
		}
	}
	else if (vShadowDesc.a < 1.f)
	{

		float OriginDepth = ClipUV_N_CurrentDepth.z;
		bool IsGodRay = false;

		[unroll(10)]
		for (int i = -5; i <= 5; i++)
		{
			[unroll(10)]
			for (int j = -5; j <= 5; j++)
			{
				float2 NewUV = In.vTexUV + float2(float(i) * XTexelSize, float(j) * YTexelSize);
				ClipUV_N_CurrentDepth = Calculate_ClipUV_N_CurrentDepth(g_WorldPosTexture, NewUV, LightViewProj);

				vWorldPosition = g_WorldPosTexture.Sample(DefaultSampler, NewUV);
				vWorldPos = vector(vWorldPosition.xyz, 1);
				vShadowDesc = g_TargetTexture.Sample(DefaultSampler, ClipUV_N_CurrentDepth.xy);

				if (ClipUV_N_CurrentDepth.z > OriginDepth + FloatCorrectionValue)
					//||(vWorldPosition.x == 1 && length(SunPos - NewUV)  < 0.00159353f * 100 ))
				{
					IsGodRay = true;
					break;
				}

			}
			if (IsGodRay) break;
		}

		if (IsGodRay)
		{
			Out.vColor2 = 1.f;
		}
	}








	return Out;
}

PS_OUT PS_MAIN_DepthOfFiled(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.x * FarDist;

	vector		vWorldPos;

	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vDepthDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = 1.f * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);


	float fLengthRate =  min(length((vWorldPos.xyz - g_vDofTargetPosition).xyz), g_fDofTargetLength) / g_fDofTargetLength;

	Out.vColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV) * (1 - fLengthRate) + g_UpScaledTexture1.Sample(DefaultSampler, In.vTexUV) * fLengthRate;


	return Out;
}

PS_OUT PS_CaculateAvgLumi(PS_IN_ForAvgLumi In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (In.fNeedToDraw != 0.5f) discard;
	 
	double vAvgColor = 0;
	 
	 
	int count = 0;
	[unroll(225)]
	for (int i = 0; i < 1280; i += 64)
	{
		for (int j = 0; j < 720; j += 64)
		{
			vAvgColor += dot(pow(g_TargetTexture.Sample(DefaultSampler, float2(i / 1280.f, j / 780.f)), 2.2f).xyz, float3(0.2125f, 0.7154f, 0.0721f));
			count++;
		}
	}
	//[unroll(921600)]
	//for (int i = 0; i < 1280; i ++)
	//{
	//	for (int j = 0; j < 720; j ++)
	//	{
	//		vAvgColor += dot(pow(g_TargetTexture.Sample(DefaultSampler, float2(i / 1280.f, j / 780.f)), 2.2f).xyz, float3(0.2125f, 0.7154f, 0.0721f));
	//		count++;
	//	}
	//}
	vAvgColor = vAvgColor / float(count);
	Out.vColor = float(vAvgColor);



	return Out;
}

PS_OUT PS_LumineceMask(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector		vTargetDesc = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
	//vector		vAvgLumineceMapDesc = g_MaskTexture.Sample(DefaultSampler, float2(0.5f, 0.5f));
	vector		vAvgLumineceMapDesc = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	if (dot(vTargetDesc.xyz, float3(0.2125f, 0.7154f, 0.0721f)) > vAvgLumineceMapDesc.r * g_fOverLuminence)
	{
		Out.vColor = 1.f;
	}

	return Out;
}

PS_OUT PS_Bloom(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector BluredMaskDesc = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vTargetDesc = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
	vector		vAvgLumineceMapDesc = g_WorldPosTexture.Sample(DefaultSampler, In.vTexUV);

	

	Out.vColor = pow(


		(pow(g_BluredTexture.Sample(DefaultSampler, In.vTexUV), 2.2f) * BluredMaskDesc + vTargetDesc * (1 - BluredMaskDesc))
		* g_fBloomMul * (dot(vTargetDesc.xyz, float3(0.2125f, 0.7154f, 0.0721f)) / vAvgLumineceMapDesc.r) * 0.5f

		//(g_BluredTexture.Sample(DefaultSampler, In.vTexUV) * BluredMaskDesc + vTargetDesc * (1 - BluredMaskDesc))
		//* g_fBloomMul * (dot(vTargetDesc.xyz, float3(0.2125f, 0.7154f, 0.0721f)) / vAvgLumineceMapDesc.r) * 0.5f


		, 1.f / 2.2f);
		
	




	return Out;
}

PS_OUT PS_ToneMapping(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	Out.vColor = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);

	//vector Hdr = max((g_TargetTexture.Sample(DefaultSampler, In.vTexUV) + LinerTex) - 0.004, 0);
	vector Hdr =max(Out.vColor - 0.004f,0);
	Out.vColor = (Hdr * (6.2f * Hdr + 0.5f)) / (Hdr * (6.2f * Hdr + 1.7f) + 0.06f);


	return Out;
}

PS_OUT PS_AddMaskToTarget(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	Out.vColor =  pow(pow(g_MaskTexture.Sample(DefaultSampler, In.vTexUV), 2.2f) * g_fGodRayIntensity + pow( g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f) ,1.f/2.2f);


	return Out;
}


PS_OUT_AfterDeferred PS_LensFlare(PS_IN In)
{
	PS_OUT_AfterDeferred Out = (PS_OUT_AfterDeferred)0;


	float2 fragCoord = UV2FragCoord(In.vTexUV);
	vec2 uv = fragCoord.xy - 0.5;
	uv.x *= iResolution.x / iResolution.y; //fix aspect ratio


	vec2 mouse = (UV2FragCoord(SunPos.xy)) - 0.5;
	mouse.x *= iResolution.x / iResolution.y; //fix aspect ratio


	vec3 color = vec3(1.4, 1.2, 1.0)*lensflare(uv, mouse.xy);
	color -= noise(fragCoord.xy)*.015;
	color = cc(color, .5, .1);
	color = max(color, 0);

	Out.vColor = pow(pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f) + pow(vec4(color, 1.0), 2.2f), 1.f / 2.2f);
	Out.vColor2 = vec4(color, 1.0);

	return Out;
	//}
}


PS_OUT PS_VolumeMatricFog(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	float3 WorldPosition = g_WorldPosTexture.Sample(DefaultSampler, In.vTexUV).xyz;
	
	if (WorldPosition.x == 1)
	{
		vector GodRayDesc = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
		vector LensflareDesc = g_UpScaledTexture1.Sample(DefaultSampler, In.vTexUV);

		Out.vColor = vector(FogColor.xyz, 1.f);

		if (length(LensflareDesc.xyz) > 0)
		{
			Out.vColor += pow(LensflareDesc, 2.2f);
		}
		if (GodRayDesc.r != 0)
		{
			Out.vColor += pow(GodRayDesc, 2.2f) * g_fGodRayIntensity;
		}


		Out.vColor = pow(Out.vColor, 1.f / 2.2f);
		Out.vColor.a = 1.f;
		
	}
	else
	{
		Out.vColor = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
		float3 eyeToPixel = WorldPosition - g_vCamPosition.xyz;
		Out.vColor = pow(vector(ApplyFog(Out.vColor.xyz, g_vCamPosition.y, eyeToPixel).xyz, 1.f), 1.f / 2.2f);
		Out.vColor.a = 1.f;
	}
	
	return Out;
}

PS_OUT PS_GodRay(PS_IN In)
{


	PS_OUT		Out = (PS_OUT)0;
	// Find the direction and distance to the sun
	float2 dirToSun = (SunPos - In.vTexUV);
	float lengthToSun = length(dirToSun);
	dirToSun /= lengthToSun;

	// Find the ray delta
	float deltaLen = min(MaxDeltaLen, lengthToSun * g_fGodRayNumDelta);
	float2 rayDelta = dirToSun * deltaLen;

	// Each step decay	
	float stepDecay = DistDecay * deltaLen;

	// Initial values
	float2 rayOffset = float2(0.0, 0.0);
	float decay = InitDecay;
	float rayIntensity = 0.0f;

	// Ray march towards the sun
	for (int i = 0; i < g_fGodRayLength; i++)
	{
		// Sample at the current location
		float2 sampPos = In.vTexUV + rayOffset;
		float fCurIntensity = g_TargetTexture.Sample(DefaultSampler, sampPos).r;

		// Sum the intensity taking decay into account
		rayIntensity += fCurIntensity * decay;

		// Advance to the next position
		rayOffset += rayDelta;

		// Update the decay
		decay = saturate(decay - stepDecay);
	}

	Out.vColor = float4(rayIntensity*RayColor.x, rayIntensity*RayColor.y, rayIntensity*RayColor.z, 1.f);


	return Out;
}

PS_OUT PS_Emissive(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	 
	vector		BluredEmissiveMaskDesc = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	vector		NonBlurEmissiveDesc = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);
	vector		BluredDeferredDesc = pow(g_BluredTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
	vector		NonBluredDeferredDesc = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV), 2.2f);
	vector MaskedNBluredDeferedDesc = pow(g_UpScaledTexture1.Sample(DefaultSampler, In.vTexUV), 2.2f);


	if (MaskedNBluredDeferedDesc.a && NonBlurEmissiveDesc.z == 0)
	{

		Out.vColor =
			pow(MaskedNBluredDeferedDesc + NonBluredDeferredDesc, 1.f / 2.2f);
				


	}
	else
	{

		Out.vColor = pow(


			(BluredDeferredDesc * BluredEmissiveMaskDesc.y + NonBluredDeferredDesc * (1 - BluredEmissiveMaskDesc.y))
			, 1.f / 2.2f);



	}


	//Out.vColor = pow(
	//
	//
	//	(BluredDeferredDesc * BluredEmissiveMaskDesc.y + NonBluredDeferredDesc * (1 - BluredEmissiveMaskDesc.y))
	//	*  (BluredEmissiveMaskDesc.z  + 1)
	//
	//
	//
	//	, 1.f / 2.2f);



	return Out;
}

PS_OUT PS_MakeVelocityMap(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector		WorldPosDesc = g_WorldPosTexture.Sample(DefaultSampler, In.vTexUV);
	vector		OldWorldPosDesc = g_OldWorldPosTexture.Sample(DefaultSampler, In.vTexUV);
	vector		NormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	
	float4 vTargetPosition;
	float4 vTargetDir;

	if (WorldPosDesc.x == 1)
	{
		Out.vColor = vector(0, 0, 0, 0);
	}
	else
	{

		float4 vNewPos = mul(vector(WorldPosDesc.xyz, 1), g_matNowView);
		vNewPos = mul(vNewPos, g_matNowProj);


		float4 vOldPos = mul(vector(WorldPosDesc.xyz, 1), g_matOldView);
		vOldPos = mul(vOldPos, g_matOldProj);


		float3 vDir = vNewPos.xyz - vOldPos.xyz;
		float a = dot(normalize(vDir), normalize(NormalDesc.xyz));

		if (a < 0.f)
			vTargetPosition = vOldPos;
		else
			vTargetPosition = vNewPos;

		float2 velocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);

		vTargetDir.xy = velocity * 0.5f;
		vTargetDir.y *= -1.f;
		vTargetDir.z = vTargetPosition.z;
		vTargetDir.w = vTargetPosition.w;


		Out.vColor.xy = vTargetDir.xy * 2.5f;
		Out.vColor.z = 1.0f;
		Out.vColor.w = vTargetDir.z / vTargetDir.w;
	}

	return Out;
}

PS_OUT PS_MotionBlur(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

#define NumBlurSample  25.f
	// 샘플링을 위한 개수입니다.
	// 샘플링을 늘린다면 조금 더 부드러워 지겠지만, 연산량 증가
	
	vector		Velocity = g_VelocityMapTexture.Sample(DefaultSampler, In.vTexUV);
	// Velocity Map에서 방향벡터 및 깊이를 가져옵니다.

	float2 PushDir = Velocity.xy;

	//if (length(PushDir) > 0.05f)
	//{
	//	PushDir = normalize(PushDir) * 0.05f;
	//}

	PushDir /= NumBlurSample;
	// 속도를 샘플링을 수행 할 개수 만큼 나눠줍니다.

	int iCount = 0;

	float4 BColor = float4(0,0,0,0);
	
	for (int i = iCount; i < NumBlurSample; ++i)
	{
		
		BColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV + (PushDir.xy) * (float)i);
		// 현재 UV( 검사하는 픽셀 ) 위치에서 Velocity , ( 벡터 ) * SampleNum 만큼.이니
		// 현재픽셀에서 이동하는 방향쪽으로 샘플링해주는 것.
		//if (Velocity.a < BColor.a + 0.04f)
		//	// 깊이를 비교하여 그려줄 지 판단합니다. ( 뒤쪽에 묻어나는걸 방지 )
		//{
			iCount++;
			Out.vColor += BColor;

		//}
	}
	Out.vColor /= (float)iCount;
	Out.vColor.a = 1.f;
	return Out;
}


PS_OUT PS_MAIN_ToonShading(PS_IN In)
{

	//float ToonValue = (ToonMaxIntensive - 100.f) * g_fToonShadingValue + 100.f;

	vector DiffuseDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector DefferedDesc = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);

	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = DefferedDesc * (1.f - g_fToonShadingValue) + DiffuseDesc * (g_fToonShadingValue);
	Out.vColor.w = 1.f;

	//DiffuseDesc = (ceil(DiffuseDesc * ToonValue) / ToonValue);
	//Out.vColor =(DefferedDesc * DiffuseDesc);

	//vector Hdr = max(Out.vColor - 0.004f, 0);
	//Out.vColor = (Hdr * (6.2f * Hdr + 0.5f)) / (Hdr * (6.2f * Hdr + 1.7f) + 0.06f);

	////Out.vColor = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV),1.f / 2.2f);

	//Out.vColor = pow(g_TargetTexture.Sample(DefaultSampler, In.vTexUV),2.2f);

	//Out.vColor = vector((ceil(Out.vColor * ToonValue) / ToonValue).xyz ,1.f); //반올림
	//Out.vColor = pow(Out.vColor * , 1.f / 2.2f);
	//Out.vColor.w = 1.f;

	return Out;
}

PS_OUT_AfterDeferred PS_CopyLastDeferred(PS_IN In)
{


	PS_OUT_AfterDeferred		Out = (PS_OUT_AfterDeferred)0;

	Out.vColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);

	float Value= length(vector(g_TargetTexture.Sample(DefaultSampler, In.vTexUV).xyz, 0.f));
	Value = (ceil(Value * g_fToonMaxIntensive) / g_fToonMaxIntensive) ;


	//float CenterRate = (1.f - abs(In.vTexUV.y - 0.5f) * 2.f )*(1.f - abs(In.vTexUV.x - 0.5f) * 2.f);
	//float SideRate = min( abs(In.vTexUV.y - 0.5f) * 2.f, abs(In.vTexUV.x - 0.5f) * 2.f);
	float CenterRate = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).r;
	
	vector vecSideColor= vector(0.90625f, 0.796875f, 0.609375f, 1.f) * (CenterRate) +
		vector(0.5859375f, 0.25390625f, 0.0390625f, 1.f) * (1.f - CenterRate);

	Out.vColor2 = vector(Value, Value, Value,1.f) * vecSideColor;

	return Out;

}

PS_OUT Copy_OrignNBlured_To_Defferd(PS_IN In)
{

	PS_OUT		Out = (PS_OUT)0;

	if (g_MaskTexture.Sample(DefaultSampler, In.vTexUV).a <= 0) discard;

	Out.vColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);

	return Out;

}

PS_OUT_Mtrl Apply_OcdMsk(PS_IN In)
{

	PS_OUT_Mtrl		Out = (PS_OUT_Mtrl)0;



	//vector		vDiffuse : SV_TARGET0;
	//vector		vNormal : SV_TARGET1;
	//vector		vSpecular : SV_TARGET2;
	//vector		vEmissive : SV_TARGET3;
	//vector		vDepth : SV_TARGET4;
	//vector		vWorldPosition : SV_TARGET5;
	//vector		vLimLight : SV_TARGET6;

	vector vOccludedMaskDesc = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);
	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	float fDepthViewZ = vDepthDesc.x *  FarDist;
	float fViewZ = vOccludedMaskDesc.x *  FarDist;

	
	if (vDepthDesc.x == 1 || fDepthViewZ + 0.002f* FarDist >= fViewZ) discard;


	//vector		vNormal = vector(vOccludedMaskDesc.z * 2.f - 1.f, vOccludedMaskDesc.w * 2.f - 1.f, 0, 0.f);
	//vNormal.z = cross(float3(vNormal.x ), vNormal.y);


	vector		vWorldPos;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vWorldPos.z = vOccludedMaskDesc.y * fViewZ; /* 0 ~ f */
	vWorldPos.w = fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	/* 로컬위치 * 월드행렬 */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);



	Out.vDiffuse = g_vOcdMaskColor;
	Out.vNormal = vector(0, 0, 0, 0);
	Out.vSpecular = vector(0, 0, 0, 0);
	Out.vEmissive = vector(1.f,0.5f,1.f,0);
	Out.vDepth = vector(vOccludedMaskDesc.x, vOccludedMaskDesc.y,0.f,0.f);
	Out.vWorldPosition = vector(vWorldPos.xyz,0);
	Out.vLimLight = vector(0,0,0,0);



	return Out;

}
PS_OUT_AfterDeferred PS_CompletelyCopyLastDeferred(PS_IN In)
{

	PS_OUT_AfterDeferred		Out = (PS_OUT_AfterDeferred)0;

	Out.vColor = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor2 = Out.vColor;

	return Out;
}
PS_OUT PS_ChangeATex2BTex(PS_IN In)
{

	PS_OUT		Out = (PS_OUT)0;

	vector vATexDesc = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);
	vector vBTexDesc = g_SourTexture.Sample(DefaultSampler, In.vTexUV);
	

	Out.vColor = vATexDesc * (1.f - g_MixRate) + vBTexDesc * g_MixRate;

	return Out;
}

PS_OUT PS_MiniGameTex2BTex(PS_IN In)
{

	PS_OUT		Out = (PS_OUT)0;

	vector vATexDesc = g_TargetTexture.Sample(DefaultSampler, In.vTexUV);
	vector SourceDesc = g_SourTexture.Sample(DefaultSampler, In.vTexUV);

	float flen = length(vATexDesc.xyz);

	vector vColor = vector(0.38823529411764f, 0.33333333333333f, 0.290196078431372f, 1.f) * flen +
		vector(0.32941176470588f, 0.27450980392156f, 0.258823529411764f, 1.f) * (1 - flen);

	flen = saturate(flen - 0.2f);

	SourceDesc.xyz = (SourceDesc.xyz * (1 - flen) + vColor.xyz* flen);
	SourceDesc.a = 1.f;


	Out.vColor = vATexDesc * (1.f - g_MixRate) + SourceDesc * g_MixRate;

	return Out;
}








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

BlendState	Blending_One
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	BlendEnable[2] = true;
	BlendEnable[1] = true;
	BlendEnable[2] = true;
	BlendEnable[3] = true;
	BlendEnable[4] = true;
	BlendEnable[5] = true;
	BlendEnable[6] = true;
	BlendEnable[7] = true;

	BlendOp = Add;
	SrcBlend = one;
	DestBlend = one;
};

DepthStencilState  NonZTestAndWriteState
{
	DepthEnable = false;
	DepthWriteMask = zero;
};
DepthStencilState  ZTestAndWriteState
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;
};
RasterizerState CullMode_ccw
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

technique11		DefaultTechnique
{
	pass DebugBuffer // 0
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}	

	pass Light_Directional// 1
	{
		SetBlendState(Blending_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}
	 
	pass Light_Point// 2
	{
		SetBlendState(Blending_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

	pass EndDeferred// 3
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EndDeferred();
	}
	pass  EndDeferredDebugObject// 4
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EndDeferred();
	}
	pass  GausianBlur// 5
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GausianBlur();
	}

	pass  DownScaling_N_VerticleBlur// 6
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_VerticleBlur();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Blur();
	}
	pass  UpScaling_N_HorizonBlur// 7
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_HorizBlur();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Blur();
	}
	pass ShadowDrawLightWorldToWorld// 8
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ShadowDrawLightWorldToWorld();
	}

	pass DepthOfFiled  // 9
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DepthOfFiled();
	}
	pass CalculateAvgLuminece  // 10
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_ForLumi();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CaculateAvgLumi();
	}
	pass MakeLumineceMask // 11
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LumineceMask();
	}

	pass MaskedDownScaling_N_VBlur  // 12
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_VerticleBlur();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Masking_Blur();
	}
	pass RenderBloomToDeffered  // 13
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Bloom();
	}

	pass ToneMapping // 14
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ToneMapping();
	}

	pass VolumatricFog // 15
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_VolumeMatricFog();
	}

	pass GodRay // 16
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GodRay();
	}
	pass AddMaskToTarget // 17
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_AddMaskToTarget();
	}

	pass LensFlare // 18
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LensFlare();
	}

	pass Emissive// 19
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Emissive();
	}

	pass DebugCollider// 20
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}

	pass MakeVelocityMap// 21
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MakeVelocityMap();
	}
	pass MotionBlur// 22
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MotionBlur();
	}
	pass ToonShading// 23
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ToonShading();
	}
	pass CopyLastScene// 24
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CopyLastDeferred();
	}

	pass Copy_OrignNBlured_To_Defferd// 25
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 Copy_OrignNBlured_To_Defferd();
	}

	pass ApplyOccludedMask// 26
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 Apply_OcdMsk();
	}

	pass CompletelyCopyLastScene// 27
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CompletelyCopyLastDeferred();
	}

	pass ChangeATex2BTex// 28
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ChangeATex2BTex();
	}
	pass MiniGameTex2BTex// 29
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(NonZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MiniGameTex2BTex();
	}
	
}