#pragma once
#include "MapObject.h"


BEGIN(Client)

class CGear :public CMapObject
{
public:
	enum GearType { GEARTYPE_1, GEARTYPE_2, GEARTYPE_3, GEARTYPE_4, GEARTYPE_5, GEARTYPE_6, GEARTYPE_7, GEAR_END };

	typedef struct tagGear_StateDesc
	{
		_uint	iGearTypeNumber;
		_float3	fPos;
	}GEAR_STATEDESC;
private:
	CGear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGear(const CGear& rhs);
	virtual ~CGear() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	HRESULT SetUp_Components();

	GEAR_STATEDESC		m_GearStateDesc;

	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;
	CShader*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

public:
	static CGear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END