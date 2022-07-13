#pragma once

#include "MapObject.h"

BEGIN(Client)

class CFemaleStatue :public CMapObject
{
private:
	CFemaleStatue(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CFemaleStatue(const CFemaleStatue& rhs);
	virtual ~CFemaleStatue() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	_float				m_fAnimmultiple = 0.1f;

	class CDemon_Tree*	m_pDemonTree = nullptr;

	_float				m_fRate = 0.f;
	_float				m_DemomTreeClear = 1 * 0.4f;

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CFemaleStatue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END