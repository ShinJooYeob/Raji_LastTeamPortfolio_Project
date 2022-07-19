#pragma once

#include "GameObject.h"

BEGIN(Client)

class CNonInstanceMeshEffect final : public CGameObject
{
public:
	enum E_NonInstanceMeshEffect
	{
		E_NonInstanceMeshEffect_NONE,
		E_NonInstanceMeshEffect_BASE,
		E_NonInstanceMeshEffect_END,
	};

public:
	//typedef struct tagNonInstanceMeshEffect
	//{
	//	tagNonInstanceMeshEffect() {}
	//	tagNonInstanceMeshEffect(COMPONENTPROTOTYPEID type, _float3 pos, _float timeMax, _int defaultTex, _int maskTex, _int noiseTex, _uint Passindex = 16)
	//	{
	//		eMeshType = type;
	//		fMaxTime_Duration = timeMax;
	//		NoiseTextureIndex = defaultTex;
	//		MaskTextureIndex = maskTex;
	//		iDiffuseTextureIndex = noiseTex;
	//		m_iPassIndex = Passindex;
	//		vPosition = pos;

	//		vColor = _float4(1);
	//		vLookDir = _float3(0, 1, 0);
	//		vLimLight = _float4(0);
	//		vEmissive = _float4(0);
	//		noisingdir = _float2(1, 1).Get_Nomalize();
	//		fDistortionNoisingPushPower = 0.5f;
	//		fAppearTime = 2.f;

	//	}

	//	COMPONENTPROTOTYPEID eMeshType = Prototype_Mesh_ConeMesh;
	//	_float4				vColor = _float4(1);
	//	_float				fMaxTime_Duration = 5.f;

	//	_uint				NoiseTextureIndex = 0;
	//	_uint				MaskTextureIndex = 0;
	//	_uint				iDiffuseTextureIndex = 299;

	//	_float3				vPosition = _float3(0);
	//	_float3				vLookDir = _float3(0, 1, 0);

	//	_float3						vSize = _float3(1);
	//	CTransform::TransformState	RotAxis = CTransform::STATE_END;
	//	_float						RotationSpeedPerSec = 1.0f;


	//	_float4				vLimLight = _float4(0);
	//	_float4				vEmissive = _float4(0);

	//	_float2				noisingdir = _float2(1, 1).Get_Nomalize();
	//	_float				fDistortionNoisingPushPower = 0.5f;
	//	_float				fAppearTime = 2.f;


	//	_uint				m_iPassIndex = 16;

	//}NONINSTNESHEFTDESC;

	



private:
	CNonInstanceMeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNonInstanceMeshEffect(const CNonInstanceMeshEffect& rhs);
	virtual ~CNonInstanceMeshEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	void				Set_Base() { m_eUpdateType = E_NonInstanceMeshEffect_BASE; }

	void				Set_ParentTransform(CTransform* parentTrans) {
		Safe_Release(m_pParentTranscom);
		m_pParentTranscom = parentTrans;
		Safe_AddRef(m_pParentTranscom);
	}

private:
	HRESULT				SetUp_Components();

private:
	NONINSTNESHEFTDESC		m_tMeshDesc;
	_float					m_fCurTime_Duration = 0.f;

	_float3				m_vRotAxis = _float3(0, 1, 0);
	_float3				m_vMoveDir = _float3(0, 1, 0);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pParentTranscom = nullptr;

	E_NonInstanceMeshEffect m_eUpdateType = E_NonInstanceMeshEffect_NONE;


public:
	static CNonInstanceMeshEffect*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
