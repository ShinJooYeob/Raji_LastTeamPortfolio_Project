#pragma once
#include "GameObject.h"

BEGIN(Client)
class CSnake_Poison_Raser final : public CGameObject
{
public:
	typedef struct SnakeRaserDesc
	{
		CGameObject* SnakeObj = nullptr;
		ATTACHEDESC m_eAttachedDesc;
	}SNAKERASERDESC;

private:
	CSnake_Poison_Raser(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSnake_Poison_Raser(const CSnake_Poison_Raser& rhs);
	virtual ~CSnake_Poison_Raser() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	void	Set_WeaponDesc(SNAKERASERDESC WeaponDesc) {
		m_RaserDesc = WeaponDesc;
	}

	void	Set_WaistgirdScal() { m_pTransformCom->Scaled_All(_float3(0.6f)); }
	void	Set_SpinScal() { m_pTransformCom->Scaled_All(_float3(0.8f)); }

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	SNAKERASERDESC		m_RaserDesc;

	_float4x4			m_fAttachedMatrix;

	ATTACHEDESC			m_AttachedDesc;

private:
	void				Update_AttachMatrix();
	HRESULT				SetUp_Components();

public:
	static CSnake_Poison_Raser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END