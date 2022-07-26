#pragma once
#include "MapObject.h"


BEGIN(Client)

class CDemon_Tree :public CMapObject
{
private:
	CDemon_Tree(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CDemon_Tree(const CDemon_Tree& rhs);
	virtual ~CDemon_Tree() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_int		Get_iCompleteCount() {return m_iCompleteCount; }
	_float		Get_NowTreeMesh_Angle();

	_float		Get_NowTreeMesh_BeginningAngle();

	vector<class CTreeMesh*>* Get_VecMeshs();

public:
	//JH
	void		Active_Puzzle(_bool bActive);
	_bool		IsClear();

private:
//	CShader*						m_pShaderCom = nullptr;
//	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
//	CModel*							m_pModel = nullptr;

	_int							m_iCompleteCount = 0;//다 각도가 맞는지를 체크
	_bool							m_bIsInterpolation = false; //보간켜기/끄기
	_float							m_fInterpolation = 0.f;

	vector<class CTreeMesh*>		m_pTreeMeshs;

	_bool							m_bIsClear = false;
	_bool							m_bTest = false;
private:
	HRESULT SetUp_Components();

public:
	static CDemon_Tree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END