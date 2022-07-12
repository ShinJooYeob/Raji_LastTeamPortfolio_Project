#pragma once

#include "GameObject.h"


BEGIN(Client)
class CShellingSkillAttackPoint;
class CShellingSkillRange :public CGameObject
{
public:
	typedef struct tagShellingSkillRangeDesc
	{
		_float			fMaxDist;
		_float			fStartDist;
		_float			fSpeed;
		_float3			fInitPos;
		CGameObject*	pOwner;
	}SHELLINGSKILLRANGEDESC;

private:
	CShellingSkillRange(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CShellingSkillRange(const CShellingSkillRange& rhs);
	virtual ~CShellingSkillRange() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	void Set_Active(_bool bActive);
	_bool Get_IsActive();

	_float3 Get_AttackPoint();

private:
	SHELLINGSKILLRANGEDESC			m_ShellingSkillRangeDesc;
	_bool							m_bActive = false;
	
private:
	CTransform*						m_pOwnerTransform = nullptr;
	CShellingSkillAttackPoint*		m_pShellingSkillAttackPoint = nullptr;

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Rect*					m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_Etc();

public:
	static CShellingSkillRange* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};


END