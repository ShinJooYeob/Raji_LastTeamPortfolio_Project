#pragma once

BEGIN(Client)
class CTaiko_Monster final : public CGameObject
{
public:
	enum NoteSizeType {NOTE_SMALL, NOTE_BIG, NOTE_END};
	enum NotePositionType {NOTEPOS_IN, NOTEPOS_OUT, NOTEPOS_END};

	typedef struct  NoteDesc
	{
		NoteSizeType		NoteType = NOTE_END;
		NotePositionType	NotePosType = NOTEPOS_END;
		_float3				vNotePosition;
		_float3				vScaled;
		_bool				bIsUse = false;
	}NOTEDESC;

private:
	explicit CTaiko_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTaiko_Monster(const CTaiko_Monster& rhs);
	virtual ~CTaiko_Monster() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	void	InitializeMonster();

public:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

	NOTEDESC			NoteDesc;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

	_float				m_PosX = 0;
	_float3				m_vPos;

public:
	static CTaiko_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END