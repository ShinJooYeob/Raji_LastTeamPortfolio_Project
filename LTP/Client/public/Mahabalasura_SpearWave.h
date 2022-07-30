#pragma once

#include "GameObject.h"

BEGIN(Client)

class CMahabalasura_SpearWave final : public CGameObject
{
public:
	typedef struct tagSpearWaveDesc
	{
		_float3				fStartPos;
		_float3				fLookDir;
		_uint				iDir = 0;
	}SPEARWAVEDESC;

private:
	CMahabalasura_SpearWave(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura_SpearWave(const CMahabalasura_SpearWave& rhs);
	virtual ~CMahabalasura_SpearWave() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	HRESULT ReInitialize(_float3 vPosition, _float3 vLookDir);
private:
	void				Update_Colliders();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Collider();

private:
	_float				m_fMaxTime_Duration = 5.f;
	_float				m_fCurTime_Duration = 0.f;
	_float				m_fDirTurningTime_Duration = 0.f;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	CCollider*			m_pCollider = nullptr;


	_float3				m_vRight;


	SPEARWAVEDESC					m_tDesc;
	INSTPARTICLEDESC				m_tTextureParticleDesc;

public:
	static CMahabalasura_SpearWave*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END
