#pragma once

#include "GameObject.h"

BEGIN(Client)

class CEditorTerrain final : public CGameObject
{
private:
	CEditorTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CEditorTerrain(const CEditorTerrain& rhs);
	virtual ~CEditorTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_float3 Pick_OnTerrain(_bool* pbIsObTerrain, _fVector ObjectWorldPos, _fVector ObjectOldWorldPos,_float3* vOutPlaneNormalVec = nullptr);

	void Set_ShaderPass(_uint iPassIndex) { m_iPassIndex = iPassIndex; };

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CVIBuffer_EditorTerrain*		m_pVIBufferCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;


	D3D11_TEXTURE2D_DESC		m_tTextureDesc;
private:
	_float				m_fRadius = 3.f;
	_uint				m_iPassIndex = 2;
	_float3				m_vPickedPos = _float3(1,1,1);


private:
	HRESULT SetUp_Components();


private:
	_float4x4			m_InverseWorldMat;
	

public:
	static CEditorTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END