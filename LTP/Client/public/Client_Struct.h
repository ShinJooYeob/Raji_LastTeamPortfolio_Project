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

typedef struct tagInstanceParticleDesc
{
	eInstanceEffectID		eParticleTypeID = InstanceEffect_End;
	COMPONENTPROTOTYPEID	eInstanceCount = Prototype_VIBuffer_Point_Instance_1;
	eInstancePassID			ePassID = InstancePass_BrightColor;


	_bool			bBillboard = false;

	_float3		vFixedPosition = _float3(0);
	_float3		vPowerDirection = _float3(0, 1, 0);
	CTransform* FollowingTarget = nullptr;
	_uint		iFollowingDir = 0;

	const _tchar* szTextureProtoTypeTag = nullptr;
	const _tchar* szTextureLayerTag = nullptr;
	_uint		iTextureLayerIndex = 0;

	const _tchar* szNoiseTextureLayerTag = nullptr;
	_uint		iNoiseTextureIndex = 0;

	_uint		TextureChageFrequency = 1;
	_float2		vTextureXYNum = _float2(1, 1);
	
	//텍스쳐 안에 형태가 몇개 있는지 -1이면 간격 기준으로 꽉차있는거
	_int		iFigureCount_In_Texture = -1;



	_float		TotalParticleTime = 0;
	_float		EachParticleLifeTime = 0;

	_uint		SizeChageFrequency = 0;
	_float3		ParticleSize = _float3(1, 1, 1);
	_float3		ParticleSize2 = _float3(0, 0, 0);

	_uint		ColorChageFrequency = 0;
	_float4		TargetColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float4		TargetColor2 = _float4(1.f, 1.f, 1.f, 1.f);

	_float		fMaxBoundaryRadius = 15.f;

	_float Particle_Power = 1.f;
	_float2 PowerRandomRange = _float2(0.5f, 1.5f);
	_float3 SubPowerRandomRange = _float3(1.f, 1.f, 1.f);

	_float3 ParticleStartRandomPosMin = _float3(-1.0f, -1.0f, -1.0f);
	_float3 ParticleStartRandomPosMax = _float3(1.f, 1.f, 1.f);


	_bool	bEmissive = false;
	_bool	AlphaBlendON = true;

	_float	m_fAlphaTestValue = 0.1f;

}INSTPARTICLEDESC;

typedef struct tagInstanceMeshDesc
{
	eInstanceEffectID		eParticleTypeID = InstanceEffect_End;
	COMPONENTPROTOTYPEID	eInstanceCount = Prototype_ModelInstance_32;
	eMeshInstancePassID			ePassID = MeshPass_BrightColor;


	_float3		vFixedPosition = _float3(0);
	_float3		vPowerDirection = _float3(0, 1, 0);
	CTransform* FollowingTarget = nullptr;
	_uint		iFollowingDir = 0;

	const _tchar* szModelMeshProtoTypeTag = nullptr;

	const _tchar* szNoiseTextureLayerTag = nullptr;
	_uint		iNoiseTextureIndex = 0;


	_float		TotalParticleTime = 0;
	_float		EachParticleLifeTime = 0;

	_uint		SizeChageFrequency = 0;
	_float3		ParticleSize = _float3(1, 1, 1);
	_float3		ParticleSize2 = _float3(0, 0, 0);

	_uint		ColorChageFrequency = 0;
	_float4		TargetColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float4		TargetColor2 = _float4(1.f, 1.f, 1.f, 1.f);

	_float		fMaxBoundaryRadius = 15.f;

	_float Particle_Power = 1.f;
	_float2 PowerRandomRange = _float2(0.5f, 1.5f);
	_float3 SubPowerRandomRange = _float3(1.f, 1.f, 1.f);

	_float3 ParticleStartRandomPosMin = _float3(-1.0f, -1.0f, -1.0f);
	_float3 ParticleStartRandomPosMax = _float3(1.f, 1.f, 1.f);

	_bool	bAutoTurn = false;
	_bool	bIsOclusion = true;
	_bool	bEmissive = false;
	_float3 vEmissive_SBB = _float3(0);
	_float	fRotSpeed_Radian = XMConvertToRadians(1080);


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