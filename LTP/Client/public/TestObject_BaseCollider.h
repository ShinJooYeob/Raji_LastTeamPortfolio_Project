#pragma once
#include "GameObject.h"

BEGIN(Client)

// Collision Test Class
class CTestObject_BaseCollider final : public CGameObject
{
public:
	enum E_TESTMODEL
	{
		TESTMODEL_PLAYER,
		TESTMODEL_MONSTER,
		TESTMODEL_MONSTER_INSTANCE,
		TESTMODEL_ANY,
		TESTMODEL_END,

	};

private:
	CTestObject_BaseCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestObject_BaseCollider(const CTestObject_BaseCollider& rhs);
	virtual ~CTestObject_BaseCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT Set_ColSetID(E_TESTMODEL id);
	HRESULT Set_ModelSetting(E_TESTMODEL id);

private:
	CShader*							m_pShaderCom = nullptr;
	CRenderer*							m_pRendererCom = nullptr;
	CNavigation*						m_pNavigationCom = nullptr;
	CModel*								m_pModel = nullptr;

	CCollider*							m_pCollider = nullptr;

	CModelInstance*		m_pModelInstance = nullptr;
	vector<CTransform*> m_vecInstancedTransform;


	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;




private:
	HRESULT Update_Player(_double time);
	HRESULT Update_Static(_double time);
	HRESULT Update_Dynamic(_double time);


private:
	HRESULT SetUp_Components();




public:
	static CTestObject_BaseCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CTestObject_BaseCollider* Clone(void* pArg);
	virtual void Free() override;
};
END
