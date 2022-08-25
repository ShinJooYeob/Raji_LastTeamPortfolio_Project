#pragma once

#include "GameObject.h"

BEGIN(Client)


class CPlayerIndicator final : public CGameObject
{
private:
	CPlayerIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerIndicator(const CPlayerIndicator& rhs);
	virtual ~CPlayerIndicator() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT Ready_Components();
	HRESULT Ready_EtcInfo();

private:
	class CPlayer*					m_pPlayer = nullptr;
	ATTACHEDESC						m_tPlayerHipBoneDesc;
	_float							m_fRotAngle = 0;
	_float							m_fForfloatYPassedTime = 0;
	_float3							m_vScale; 
	_float4							m_vColor = _float4(1);
	_bool							m_bIsOnIndicated = false;
	_bool							m_bTurningOff = false;
	_float							m_fTurnOffTime = 0;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;
	CDissolve*			m_pDissolveCom = nullptr;

private:
	HRESULT Check_Monster(_double fDeltaTime);
	HRESULT Update_Moving(_double fDeltaTime);

public:
	static CPlayerIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
