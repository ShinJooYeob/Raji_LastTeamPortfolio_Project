#pragma once
#include "MapObject.h"
#include "Gear.h"


BEGIN(Client)

class CGear_Puzzle :public CMapObject
{
	enum Gear_TriggerType{GEAR_IDLE,GEAR_START,GEAR_SUCCESS,GEAR_END};
private:
	CGear_Puzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGear_Puzzle(const CGear_Puzzle& rhs);
	virtual ~CGear_Puzzle() = default;

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
	HRESULT	SetUp_Collider();
	HRESULT	SetUp_ColliderPos();
	
	HRESULT	SetUp_Gear();

private:
	HRESULT Update_Collider(_double dDeltaTime);
	HRESULT	Mini_Collision(_double dDeltaTime);

private:
	HRESULT KeyboardInput(_double dDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;

	CCollider*			m_pColliderCom = nullptr;
	vector<_float3>		m_vecColliderPos;
	CGear*				m_pGear[CGear::GEAR_END];


	_uint				m_iTriggerOn = CGear_Puzzle::GEAR_IDLE;
	_uint				m_GearNumber = 1;

	_bool				m_bDifferentDirectiOn[CGear::GEAR_END] = {false};


	_float3				m_fPushDir;
	_uint				m_iCurrentGearNumber = 1;


public:
	static CGear_Puzzle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END