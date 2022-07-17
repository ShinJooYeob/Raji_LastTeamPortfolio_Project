#pragma once

#include "GameObject.h"

BEGIN(Client)

class CNonInstanceMeshEffect final : public CGameObject
{
public:
	typedef struct tagNonInstanceMeshEffect
	{
		COMPONENTPROTOTYPEID eMeshType = Prototype_Mesh_ConeMesh;
		_float4				vColor = _float4(1);
		_float				fMaxTime_Duration = 5.f;

		_uint				NoiseTextureIndex = 0;
		_uint				MaskTextureIndex = 0;
		_uint				iDiffuseTextureIndex = 299;

		_float3				vPosition = _float3(0);
		_float3				vLookDir = _float3(0,1,0);


		_float4				vLimLight = _float4(0);
		_float4				vEmissive = _float4(0);

		_float2				noisingdir = _float2(1, 1).Get_Nomalize();
		_float				fDistortionNoisingPushPower = 0.5f;
		_float				fAppearTime = 2.f;


		_uint				m_iPassIndex = 16;

	}NONINSTNESHEFTDESC;

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

private:
	HRESULT				SetUp_Components();

private:
	NONINSTNESHEFTDESC		m_tMeshDesc;
	_float				m_fCurTime_Duration = 0.f;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;



public:
	static CNonInstanceMeshEffect*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
