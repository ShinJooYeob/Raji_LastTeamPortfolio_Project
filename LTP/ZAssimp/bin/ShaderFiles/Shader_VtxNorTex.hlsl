
#include "Shader_Define.hpp" 


texture2D		g_DiffuseTexture;


texture2D		g_SourDiffuseTexture;
texture2D		g_DestDiffuseTexture1;
texture2D		g_DestDiffuseTexture2;
texture2D		g_DestDiffuseTexture3;
texture2D		g_DestDiffuseTexture4;

texture2D		g_FilterTexture;
texture2D		g_BrushTexture;

texture2D		g_HeightMapTexture;

cbuffer LightPosition
{
	float4 g_vLightPosition;
};

cbuffer BrushDesc
{
	float4		g_vBrushPos = float4(10.0f, 0.0f, 10.f, 1.f);
	float		g_fRadius = 3.f;
};
cbuffer MtrlDesc
{
	float		g_fMimMapSize = 20;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
		
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


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vWorldNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float		bIsNotDraw : TEXCOORD3;
};


struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vSpecular : SV_TARGET2;
	vector		vEmissive : SV_TARGET3;
	vector		vDepth : SV_TARGET4;
	vector		vWorldPosition : SV_TARGET5;
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
		Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
		
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
		Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		Out.vSpecular = 1;
		Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
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
		Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
		Out.vSpecular = 1;
		Out.vWorldPosition = vector(In.vWorldPos.xyz, 0);
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

	Out.vDiffuse = float4(Depth.x, In.vClipPosition.z, In.vClipPosition.w, 1);

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

}