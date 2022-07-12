#pragma once
#include "MapObject.h"

BEGIN(Client)

class CTreeMesh :public CMapObject
{
public:
	enum DemonTreeNumber 
	{
		DEMONTREE_01, DEMONTREE_02, DEMONTREE_03, DEMONTREE_04 , DEMONTREE_05 , DEMONTREE_END
	};

	typedef struct DemonTreeDesc
	{
		DemonTreeNumber	m_eDemonTreeNum;
		_float3		m_Pos;

	}DEMONTREEDESC;
private:
	CTreeMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTreeMesh(const CTreeMesh& rhs);
	virtual ~CTreeMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_float		Get_RotationAngle() { return m_fRotationAngle; }
	_bool		Get_AngleZero() { return m_bIsAngleZero; }

public:
	void		Set_AngleZero(_bool State) { m_bIsAngleZero = State; }

public:
	void		Turn_CCW();
	void		Turn_CW();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;

	DEMONTREEDESC		m_DemonTreeDesc;

	_bool				m_bIsAngleZero = false;
	_float				m_fRotationAngle = 0.f;


private:
	HRESULT SetUp_Components();

public:
	static CTreeMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END