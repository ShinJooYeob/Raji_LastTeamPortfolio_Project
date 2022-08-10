#pragma once

#include "MapObject.h"

BEGIN(Client)

class CMiniGameBuilding :public CMapObject
{
public:

	typedef struct tagMiniGameBulidingDesc
	{
		_float3 vPosition = _float3(0);
		_float3 vScale = _float3(1);
		_float3 vLookDir = _float3(1,0,0);

	}MGBDESC;


private:
	CMiniGameBuilding(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMiniGameBuilding(const CMiniGameBuilding& rhs);
	virtual ~CMiniGameBuilding() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CRenderer*			m_pRendererCom = nullptr;

	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;

	CShader*			m_pSubShaderCom = nullptr;
	CTransform*			m_pSubTransformCom = nullptr;
	CTexture*			m_pTexture = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	MGBDESC				m_tMGBDesc;
	_float2				m_ArrTextureDir[4];
private:
	HRESULT SetUp_Components();
	HRESULT Render_Texture(_uint iIndex);
	HRESULT Ready_MiniGameScreenTarget_For_Prototype();

public:
	static HRESULT Copy_NowScreenToBuliding(_uint iIndex, ID3D11ShaderResourceView* pTargetSRV = nullptr);
private:
	static vector<MGBTARGET>	m_vecMGBTargets;
	static ID3D11Device* m_pDevice_ForStatic;
	static ID3D11DeviceContext* m_pDeviceContext_ForStatic;
	static MATRIXWVP			m_pFullScreenWVP;
	static _bool				m_bIsFirst;

public:
	static CMiniGameBuilding* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END