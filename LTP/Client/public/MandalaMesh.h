#pragma once
#include "MapObject.h"

BEGIN(Client)

class CMandalaMesh :public CMapObject
{
public:
	enum MandalaNumber
	{
		MANDALA_00, MANDALA_01, MANDALA_02, MANDALA_03, MANDALA_04, MANDALA_END
	};

	typedef struct MandalaDesc
	{
		MandalaNumber	m_eMandalaNum;
		_float3			m_Pos;
		_float			m_fAngle = 0.f;

	}MANDALADESC;

private:
	CMandalaMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMandalaMesh(const CMandalaMesh& rhs);
	virtual ~CMandalaMesh() = default;

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
	_float3		Get_ObjPos() { return m_pTransformCom->Get_MatrixState(CTransform::STATE_POS); }
	_float		Get_BeginningAngle() { return m_BeginningAngle; }

public:
	void		Set_AngleZero(_bool State) { m_bIsAngleZero = State; }
	void		Set_ObjPos(_float3 Pos) { m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos); }

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;

	MANDALADESC			m_MandalaDesc;

	_bool				m_bIsAngleZero = false;
	_float				m_fRotationAngle = 0.f;

	_float				m_BeginningAngle = 0.f;


public:
	void		Turn_CCW();
	void		Turn_CW();

	_bool		CheckAngle();

private:
	HRESULT SetUp_Components();

public:
	static CMandalaMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END