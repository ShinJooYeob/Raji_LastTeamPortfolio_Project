#pragma once

#include "GameObject.h"



BEGIN(Client)

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.
typedef struct tagAttachedDesc
{

	HRESULT Initialize_AttachedDesc(CGameObject*	pAttachTarget, const char*	pAttachBoneName, _float3 vScale, _float3 vRot, _float3 vPosition)
	{
		pAttachingObjectTransform = (CTransform*)pAttachTarget->Get_Component(TAG_COM(Com_Transform));
		NULL_CHECK_BREAK(pAttachingObjectTransform);
		pModel = (CModel*)pAttachTarget->Get_Component(TAG_COM(Com_Model));
		NULL_CHECK_BREAK(pModel);
		m_pAttachedNode = pModel->Find_HierarchyNode(pAttachBoneName);
		NULL_CHECK_BREAK(m_pAttachedNode);

		m_DefaultBonePivot = XMMatrixScaling(vScale.x, vScale.y, vScale.z)
			* XMMatrixRotationX(XMConvertToRadians(vRot.x))* XMMatrixRotationY(XMConvertToRadians(vRot.y))* XMMatrixRotationZ(XMConvertToRadians(vRot.z))
			*XMMatrixTranslation(vPosition.x / vScale.x, vPosition.y/ vScale.y, vPosition.z /vScale.z);
		m_vSize = vScale;
		return S_OK;
	}

	void Set_DefaultBonePivot(_float3 vScale, _float3 vRot, _float3 vPosition)
	{
		m_DefaultBonePivot = XMMatrixScaling(vScale.x, vScale.y, vScale.z)
			* XMMatrixRotationX(XMConvertToRadians(vRot.x))* XMMatrixRotationY(XMConvertToRadians(vRot.y))* XMMatrixRotationZ(XMConvertToRadians(vRot.z))
			*XMMatrixTranslation(vPosition.x / vScale.x, vPosition.y / vScale.y, vPosition.z / vScale.z);
	}

	_Matrix Caculate_AttachedBoneMatrix()
	{
		NULL_CHECK_BREAK(pModel);
		NULL_CHECK_BREAK(m_pAttachedNode);
		NULL_CHECK_BREAK(pAttachingObjectTransform);

		_Matrix BoneMatrix = pModel->Caculate_AttachedBone(m_pAttachedNode) * pAttachingObjectTransform->Get_WorldMatrix();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		return m_DefaultBonePivot.XMatrix() * BoneMatrix;
	}




	_Vector Get_AttachedBoneWorldPosition()
	{
		NULL_CHECK_BREAK(pModel);
		NULL_CHECK_BREAK(m_pAttachedNode);
		NULL_CHECK_BREAK(pAttachingObjectTransform);

		return (m_DefaultBonePivot.XMatrix() * pModel->Caculate_AttachedBone(m_pAttachedNode) * pAttachingObjectTransform->Get_WorldMatrix()).r[3];
	}

	_Matrix Caculate_AttachedBoneMatrix_BlenderFixed()
	{
		NULL_CHECK_BREAK(pModel);
		NULL_CHECK_BREAK(m_pAttachedNode);
		NULL_CHECK_BREAK(pAttachingObjectTransform);

		_Matrix BoneMatrix = m_DefaultBonePivot.XMatrix() * pModel->Caculate_AttachedBone(m_pAttachedNode) * pAttachingObjectTransform->Get_WorldMatrix();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		return BoneMatrix;
	}
	_Vector Get_AttachedBoneWorldPosition_BlenderFixed()
	{
		NULL_CHECK_BREAK(pModel);
		NULL_CHECK_BREAK(m_pAttachedNode);
		NULL_CHECK_BREAK(pAttachingObjectTransform);

		return (m_DefaultBonePivot.XMatrix() * pModel->Caculate_AttachedBone(m_pAttachedNode) * pAttachingObjectTransform->Get_WorldMatrix()).r[3];
	}


public:

	void Set_ReAttachBoneName(const char*	pAttachBoneName)
	{
		m_pAttachedNode = pModel->Find_HierarchyNode(pAttachBoneName);
	}

	CTransform* Get_AttachObjectTransform()
	{
		return pAttachingObjectTransform;
	}

public:
	CTransform*			pAttachingObjectTransform = nullptr;
	CModel*				pModel = nullptr;
	CHierarchyNode*		m_pAttachedNode = nullptr;
	_float4x4			m_DefaultBonePivot;
	_float3				m_vSize = _float3(0);
}ATTACHEDESC;

typedef struct tagFonts
{
	wstring szString;
	_float2 vPosition;
	_float4 vColor = _float4(1);
	_float2 vFontScale = _float2(1);
	_float fAngle = 0;
}FONTSDESC;

typedef struct tagNonInstanceMeshEffect
{
	tagNonInstanceMeshEffect() {}
	tagNonInstanceMeshEffect(COMPONENTPROTOTYPEID type, _float3 pos, _float timeMax, _int defaultTex, _int maskTex, _int noiseTex, _uint Passindex = 16)
	{
		eMeshType = type;
		fMaxTime_Duration = timeMax;
		NoiseTextureIndex = defaultTex;
		MaskTextureIndex = maskTex;
		iDiffuseTextureIndex = noiseTex;
		m_iPassIndex = Passindex;
		vPosition = pos;

		vColor = _float4(1);
		vLookDir = _float3(0, 1, 0);
		vLimLight = _float4(0);
		vEmissive = _float4(0);
		noisingdir = _float2(1, 1).Get_Nomalize();
		fDistortionNoisingPushPower = 0.5f;
		fAppearTime = 2.f;

	}

	COMPONENTPROTOTYPEID eMeshType = Prototype_Mesh_ConeMesh;
	_float4				vColor = _float4(1);
	_float				fMaxTime_Duration = 5.f;

	_uint				NoiseTextureIndex = 0;
	_uint				MaskTextureIndex = 0;
	_uint				iDiffuseTextureIndex = 299;

	_float3				vPosition = _float3(0);
	_float3				vLookDir = _float3(0, 1, 0);


	_float4				vLimLight = _float4(0);
	_float4				vEmissive = _float4(0);

	_float2				noisingdir = _float2(1, 1).Get_Nomalize();
	_float				fDistortionNoisingPushPower = 0.5f;
	_float				fAppearTime = 2.f;

	_float3				vSize = _float3(1);
	eFollowingDirID		RotAxis = FollowingDir_Up;
	_float				OnceStartRot = 0.f;
	_float				StartRot = 0.f;
	_float				RotationSpeedPerSec = 0.f;

	eFollowingDirID		MoveDir = FollowingDir_Look;
	_float				MoveSpeed = 0.f;

	_uint				m_iPassIndex = 16;


	_uint				m_CheckerBuffer = 0;
}NONINSTNESHEFTDESC;


typedef struct tagInstanceParticleDesc
{
	tagInstanceParticleDesc()
	{
		lstrcpy(szTextureProtoTypeTag, TAG_CP(Prototype_Texture_TestEffect));
		lstrcpy(szTextureLayerTag, L"dds");
	}

	eInstanceEffectID		eParticleTypeID = InstanceEffect_Ball;
	COMPONENTPROTOTYPEID	eInstanceCount = Prototype_VIBuffer_Point_Instance_1;
	eInstancePassID			ePassID = InstancePass_BrightColor;


	_bool			bBillboard = false;

	_float3		vFixedPosition = _float3(0);
	_float3		vPowerDirection = _float3(0, 1, 0);
	CTransform* FollowingTarget = nullptr;
	eFollowingDirID		iFollowingDir = FollowingDir_Look;

	_tchar		szTextureProtoTypeTag[128];
	_tchar		szTextureLayerTag[128];
	_uint		iTextureLayerIndex = 0;

	_int		iNoiseTextureIndex = 0;
	_int		iMaskingTextureIndex = 0;
	_float		fAppearTimer = 1.f;
	_float2		vNoisePushingDir = _float2(0, 1);
	_float		fDistortionNoisingPushPower = 0.5f;

	_uint		TextureChageFrequency = 0;
	_float2		vTextureXYNum = _float2(1, 1);
	//텍스쳐 안에 형태가 몇개 있는지 -1이면 간격 기준으로 꽉차있는거
	_int		iFigureCount_In_Texture = -1;



	_float		TotalParticleTime = 2.f;
	_float		EachParticleLifeTime = 0.35f;

	_uint		SizeChageFrequency = 0;
	_float3		ParticleSize = _float3(1, 1, 1);
	_float3		ParticleSize2 = _float3(0, 0, 0);

	_uint		ColorChageFrequency = 0;
	_float4		TargetColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float4		TargetColor2 = _float4(1.f, 1.f, 1.f, 1.f);

	_float		fMaxBoundaryRadius = 15.f;

	_float Particle_Power = 5.f;
	_float2 PowerRandomRange = _float2(0.5f, 1.5f);
	_float3 SubPowerRandomRange_RUL = _float3(1.f, 1.f, 1.f);

	_float3 ParticleStartRandomPosMin = _float3(-1.0f, -1.0f, -1.0f);
	_float3 ParticleStartRandomPosMax = _float3(1.f, 1.f, 1.f);


	_bool	bEmissive = false;
	_bool	AlphaBlendON = true;
	_float3 vEmissive_SBB = _float3(0);

	_float	m_fAlphaTestValue = 0.1f;


	_float4 TempBuffer_0 = _float4(0);
	_float4 TempBuffer_1 = _float4(0);
	_float4 TempBuffer_2 = _float4(0);
	_float4 TempBuffer_3 = _float4(0);

}INSTPARTICLEDESC;

typedef struct tagInstanceMeshDesc
{
	tagInstanceMeshDesc()
	{

		lstrcpy(szModelMeshProtoTypeTag, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge));
	}

	eInstanceEffectID		eParticleTypeID = InstanceEffect_Ball;
	COMPONENTPROTOTYPEID	eInstanceCount = Prototype_ModelInstance_1;
	eMeshInstancePassID			ePassID = MeshPass_BrightColor;


	_float3		vFixedPosition = _float3(0);
	_float3		vPowerDirection = _float3(0, 1, 0);
	CTransform* FollowingTarget = nullptr;
	eFollowingDirID		iFollowingDir = FollowingDir_Look;

	_tchar		szModelMeshProtoTypeTag[128];
	_uint		iModelAnimIndex = 0;


	_int		iNoiseTextureIndex = 0;
	_int		iMaskingTextureIndex = 0;
	_float2		vNoisePushingDir = _float2(0, 1);
	_float		fAppearTimer = 1.f;
	_float		fDistortionNoisingPushPower = 0.5f;


	_float		TotalParticleTime = 2.f;
	_float		EachParticleLifeTime = 0.35f;

	_uint		SizeChageFrequency = 0;
	_float3		ParticleSize = _float3(1, 1, 1);
	_float3		ParticleSize2 = _float3(0, 0, 0);

	_uint		ColorChageFrequency = 0;
	_float4		TargetColor = _float4(1.f, 1.f, 1.f, 0.f);
	_float4		TargetColor2 = _float4(1.f, 1.f, 1.f, 0.f);

	_float		fMaxBoundaryRadius = 15.f;

	_float Particle_Power = 5.f;
	_float2 PowerRandomRange = _float2(0.5f, 1.5f);
	_float3 SubPowerRandomRange_RUL = _float3(1.f, 1.f, 1.f);

	_float3 ParticleStartRandomPosMin = _float3(-1.0f, -1.0f, -1.0f);
	_float3 ParticleStartRandomPosMax = _float3(1.f, 1.f, 1.f);

	_bool	bAutoTurn = false;
	_bool	bIsOclusion = true;
	_bool	bEmissive = false;
	_float3 vEmissive_SBB = _float3(0);
	_float	fRotSpeed_Radian = XMConvertToRadians(1080);


	_float4 TempBuffer_0 = _float4(0);
	_float4 TempBuffer_1 = _float4(0);
	_float4 TempBuffer_2 = _float4(0);
	_float4 TempBuffer_3 = _float4(0);
}INSTMESHDESC;



typedef struct tagObjectElement
{
	_tchar			ObjectID[128] = L"";
	_tchar			MeshID[128] = L"";
	_uint			PassIndex = 0;
	_float			FrustumRange = 5;
	_bool			bIsOcllsuion = false;
	_float4x4		matSRT;
	_float4x4		matTransform;
	CGameObject*	pObject = nullptr;

}OBJELEMENT;


typedef struct tagCamActionDesc
{
	_float  fDuration;
	_float3 vPosition;
}CAMACTDESC;

typedef struct tagCameraAction
{
	_double					 TotalTime = 0;
	vector<CAMACTDESC>		 vecCamPos;
	vector<CAMACTDESC>		 vecLookAt;

}CAMERAACTION;




typedef struct tagUIDesc
{
	_float fX = 0, fY = 0, fCX = 0, fCY = 0;

}UIDESC;



typedef struct tagFloatRect
{
	_float left = 0;
	_float top = 0;
	_float right = 0;
	_float bottom = 0;

}FLOATRECT;

typedef struct tagEditSceneUI {

	UIDESC			UIDesc ;
	FLOATRECT		UIRect ;
	_bool			bIsRect = false;
	_float			fAngle = 0;
	_float			fDepth = 0;
	ID3D11ShaderResourceView* SRV = nullptr;
}EDITUI;


// 전역화함

static bool GetRandomBool()
{
	int random = (rand() % 10000) + 1;
	return 10000 * 0.5 < random;
}
END