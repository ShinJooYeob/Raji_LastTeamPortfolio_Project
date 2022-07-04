#pragma once

#include "PlayerWeapon.h"

BEGIN(Client)

class CPlayerWeapon_Shield final : public CPlayerWeapon
{
public:
	enum EShieldPivot { SHIELD_PIVOT_NORMAL, SHIELD_PIVOT_THROW, SHIELD_PIVOT_TAKEDOWN, SHIELD_PIVOT_END };
	enum EShieldUltimateState { SHIELDULTIMATE_MOV, SHIELDULTIMATE_ATTACK, SHIELDULTIMATE_END };

private:
	CPlayerWeapon_Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon_Shield(const CPlayerWeapon_Shield& rhs);
	virtual ~CPlayerWeapon_Shield() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

public:
	void				Start_UltimateMode(_fVector fStartPos, _float fUltimateTargetHeight);
	void				End_UltimateMode();

	void				Start_SmashMode(_fVector vPutOnPos);
	void				End_SmashMode();

	void				Start_ThrowMode(_fVector vStartPos, _float fThrowDist);
	void				End_ThrowMode();

public:
	void				Activate_Ultimate(_double fDeltaTime);
	void				Update_ThrowPos(_fVector vStartPos, _fVector vThrowDir, _float fAnimRate);

private:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual void		Update_AttachCamPos() override;

	_int				Update_Structure(_double fDeltaTime);
	_int				Update_Equip(_double fDeltaTime);
	_int				Update_NoEquip(_double fDeltaTime);

	_int				LateUpdate_Structure(_double fDeltaTime);
	_int				LateUpdate_Equip(_double fDeltaTime);
	_int				LateUpdate_NoEquip(_double fDeltaTime);

	void				Update_AttachMatrix();

public:
	void				Change_Pivot(EShieldPivot ePitvot);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();

private:
	_float4x4				m_fAttachedMatrix;
	_uint					m_iPassNum;
	_uint					m_iCurAnim;
	_bool					m_bSmashMode = false;
	_bool					m_bUltimateMode = false;
	_bool					m_bThrowMode = false;
	_float					m_fUltimateTargetHeight;
	_float					m_fStartDist;
	_float					m_fTargetDist;
	EShieldUltimateState	m_eUltimateState = SHIELDULTIMATE_MOV;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	static CPlayerWeapon_Shield*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
