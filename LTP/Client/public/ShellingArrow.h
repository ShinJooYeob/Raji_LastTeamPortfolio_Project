#pragma once

#include "GameObject.h"

BEGIN(Client)

class CShellingArrow final : public CGameObject
{
public:
	typedef struct tagShellingArrowDesc
	{
		_float3		fStartPos;
		_float3		fTargetPos;
	}SHELLINGARROWDESC;

private:
	explicit CShellingArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CShellingArrow(const CShellingArrow& rhs);
	virtual ~CShellingArrow() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	HRESULT SetUp_Components();
	
private:
	SHELLINGARROWDESC	m_tShellingArrowDesc;

	_float				m_fDestroy_Count = 2.f;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;

	CModelInstance*		m_pModelInstance = nullptr;
	vector<CTransform*> m_vecInstancedTransform;

public:
	static CShellingArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END