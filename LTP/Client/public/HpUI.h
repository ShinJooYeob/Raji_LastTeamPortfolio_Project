#pragma once
#include "GameObject.h"

BEGIN(Client)

class CHpUI final : public CGameObject
{
public:
	enum HpType {HP_RAJI, HP_MONSTER, HP_END};

	typedef struct HPDesc
	{
		HpType			m_HPType = HP_END;
		CGameObject*	m_pObjcect = nullptr;
		_float3			m_vPos;
		_float			m_Dimensions = 1.f;
	}HPDESC;

private:
	CHpUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CHpUI(const CHpUI& rhs);
	virtual ~CHpUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_int	Get_HitCount() { return m_iHitCount; }

public:
	void	Set_ObjectPos();

	void	Set_ADD_HitCount(_int iAdd = 1) { m_iHitCount += iAdd; }

private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CVIBuffer_Point_Instance*		m_pVIBufferCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;

	vector<CTransform*>				m_vPosTransforms;
	vector<CTransform*>				m_vecInstancedHPTransform;
	vector<CTransform*>				m_vecInstancedEmptyHPTransform;

	HPDESC				m_HpDesc;
	_int				m_iHitCount = 0;

	CGameObject*		m_pObject = nullptr;
	_float3				m_pObjectPos;


private:
	HRESULT SetUp_Components();

public:
	static CHpUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END