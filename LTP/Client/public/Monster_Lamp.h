#pragma once
#include "Monster.h"

BEGIN(Client)

class CMonster_Lamp final : public CMonster
{
	enum AnimationType{ANIM_IDLE,ANIM_RUN,ANIM_ATTACK,ANIM_HIT,ANIM_DIE, ANIM_END};

	typedef struct tagState
	{
		CTransform*	pTransform = nullptr;
		AnimationType eType = ANIM_END;
	}TRANSFORM_STATE;

private:
	CMonster_Lamp(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_Lamp(const CMonster_Lamp& rhs);
	virtual ~CMonster_Lamp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT	SetUp_Info();

private:
	HRESULT FollowMe(_double dDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	CModel*				m_pModel[ANIM_END];
	CModelInstance*		m_pModelInstance[ANIM_END];

	vector<TRANSFORM_STATE> m_vecInstancedTransform;

	vector<CTransform*> m_ModelTransGroup[ANIM_END];


	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;


	CTransform*			m_pPlayerTransformCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

public:
	static CMonster_Lamp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
