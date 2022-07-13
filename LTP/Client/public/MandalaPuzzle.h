#pragma once
#include "MapObject.h"


BEGIN(Client)

class CMandalaPuzzle :public CMapObject
{
private:
	CMandalaPuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMandalaPuzzle(const CMandalaPuzzle& rhs);
	virtual ~CMandalaPuzzle() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CTransform*						m_pTransformCom = nullptr;

	_int							m_iMandalaMeshCount = 0;
	_bool							m_bIsAngleClear = false;

	vector<class CMandalaMesh*>		m_MandalaMeshs;

	_bool							m_bIsClear = false;
	_bool							m_bTest = false;

private:
	HRESULT SetUp_Components();

public:
	static CMandalaPuzzle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END