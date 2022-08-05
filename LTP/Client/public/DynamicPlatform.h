#pragma once

#include "GameObject.h"

BEGIN(Client)
class CRepelWall;
class CDynamicPlatform final : public CGameObject
{
public:
	/** Platform Pattern
	**	One Wall Patterns
	*	Pattern 1 : Front
	*	Pattern 2 : Back
	*	Pattern 3 : Left
	*	Pattern 4 : Right

	**	Two Wall Patterns
	*	Pattern 5 : Cross hair
	*	Pattern 6 : Custom
	*	Pattern 7 : X
	*/
	enum EPATTERN { PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4, 
					PATTERN_5, PATTERN_6, PATTERN_7,
					PATTERN_END };

	enum EONTARGET { TARGET_RAJI, TARGET_GOLU, TARGET_BOTH, TARGET_END };

	typedef struct tagDynamicPlatformDesc
	{
		_float3			fSpawnPos;
		EONTARGET		eOnTarget;
		EPATTERN		ePattern;
		_uint			iIndex;

		_bool			bEndPlatform = false;
	}DYNAMICPLATFORMDESC;

private:
	explicit CDynamicPlatform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDynamicPlatform(const CDynamicPlatform& rhs);
	virtual ~CDynamicPlatform() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	virtual void		CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
						_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	CTransform*			Get_TransformCom() {
		return m_pTransformCom;
	}

public:
	void				Set_Appear(_bool bAppear);

private:
	_int				Update_OnTarget_Raji(_double fDeltaTime);
	_int				Update_OnTarget_Golu(_double fDeltaTime);

	void				Update_Colliders();

	void				Update_Pattern_1(_double fDeltaTime);
	void				Update_Pattern_2(_double fDeltaTime);
	void				Update_Pattern_3(_double fDeltaTime);
	void				Update_Pattern_4(_double fDeltaTime);

	void				Update_Pattern_5(_double fDeltaTime);
	void				Update_Pattern_6(_double fDeltaTime);
	void				Update_Pattern_7(_double fDeltaTime);

private:
	void				CollisionTriger_OnTarget_Raji(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
						_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType);

	void				CollisionTriger_OnTarget_Golu(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
						_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Collider();
	HRESULT				SetUp_Etc();

	HRESULT				SetUp_Patterns();
	HRESULT				SetUp_Pattern_1();
	HRESULT				SetUp_Pattern_2();
	HRESULT				SetUp_Pattern_3();
	HRESULT				SetUp_Pattern_4();

	HRESULT				SetUp_Pattern_5();
	HRESULT				SetUp_Pattern_6();
	HRESULT				SetUp_Pattern_7();

private:
	EONTARGET				m_eOnTarget = TARGET_END;
	EPATTERN				m_ePattern = PATTERN_END;
	_float					m_fDestroyTimer = 3.f;
	_bool					m_bOnPlayer = false;
	_bool					m_bOnGolu = false;
	_bool					m_bOnceSwitch = false;
	_bool					m_bOnceSwitch2 = false;
	_bool					m_bOnceSwitch_Golu = false;
	_bool					m_bOnceSwitch2_Golu = false;
	_float					m_fFallingAcc = 0.f;
	vector<CRepelWall*>		m_pRepelWalls;
	_bool					m_bDisappear = true;
	_uint					m_iIndex = 0;
	_bool					m_bBlockRender = true;
	_bool					m_bEndPlatform = false;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CDynamicPlatform*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
