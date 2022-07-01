#pragma once

#include "Monster_Bullet.h"

class Monster_Bullet_Universal final : public CMonster_Bullet
{
public:
	typedef struct tagMonster_Bullet_Universal_MeshDesc {
		_uint	iBulletMeshNumber;

		_float	fSpeedPerSec;
		_float3 vScale;
		_float3 vLook;
		_float4	vPosition;


	}UNIVERSAL_BULLET_MESHDESC;
private:
	explicit Monster_Bullet_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit Monster_Bullet_Universal(const Monster_Bullet_Universal& rhs);
	virtual ~Monster_Bullet_Universal() = default;

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
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;



public:
	static Monster_Bullet_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

