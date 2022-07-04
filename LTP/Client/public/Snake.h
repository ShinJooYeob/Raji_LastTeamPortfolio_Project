#pragma once
#include "Boss.h"

BEGIN(Client)
class CSnake final : public CBoss
{
private:
	CSnake(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSnake(const CSnake& rhs);
	virtual ~CSnake() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	_bool				m_bIsHalf = false;
	_bool				m_bIsLookAt = true;

	//Attacking
	_bool				m_bIsAttack = false;

	//rotation
	_int				m_iRotationRandom = 0;
	_float				m_fRotTime = 0.f;

	_bool				m_bTestHodeing = false;


	CGameObject*		m_pPlayerObj;

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CSnake* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END