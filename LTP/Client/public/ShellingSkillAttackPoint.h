#pragma once

#include "GameObject.h"

BEGIN(Client)

class CShellingSkillAttackPoint final : public CGameObject
{
private:
	CShellingSkillAttackPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CShellingSkillAttackPoint(const CShellingSkillAttackPoint& rhs);
	virtual ~CShellingSkillAttackPoint() = default;

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
	void Set_OwnerTransform(CTransform* pOwnerTransform);
	void Set_MaxRange(_float fMaxRange);

	_fVector Get_AttackPointPos();

private:
	void	Update_Transform();
	_float3 Check_MousePicking();
	_bool	Check_MaxRange();

private:
	_bool				m_bActive = false;
	CTransform*			m_pOwnerTransform = nullptr;
	_float				m_fMaxRange = 0.f;

private:
	CRenderer*			m_pRendererCom = nullptr;

	CShader*			m_pShaderCom_Parabola = nullptr;
	CModel*				m_pModel_Parabola = nullptr;
	CTransform*			m_pTransformCom_Parabola = nullptr;

	CShader*			m_pShaderCom_Point = nullptr;
	CTexture*			m_pTextureCom_Point = nullptr;
	CTransform*			m_pTransformCom_Point = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom_Point = nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CShellingSkillAttackPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
