#pragma once

#include "GameObject.h"

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	enum EINPUT_MOVDIR {
		MOVDIR_F, MOVDIR_B, MOVDIR_L, MOVDIR_R, MOVDIR_FL, MOVDIR_FR, MOVDIR_BL, MOVDIR_BR, MOVDIR_END
	};

	enum EINPUT_COMBO {
		COMBO_DODGE, COMBO_MAINATTACK, COMBO_POWERATTACK, COMBO_END
	};

	enum EWEAPON_TYPE {
		WEAPON_SPEAR, WEAPON_END
	};
	
	enum EPLAYER_STATE {
		STATE_IDLE, STATE_MOV, STATE_ATTACK, STATE_EVASION, STATE_TAKE_DAMAGE, STATE_EXECUTION, STATE_DEAD, STATE_END
	};

	enum EPLAYERANIM_BASE {
		BASE_ANIM_IDLE, 
		BASE_ANIM_WALK_F, BASE_ANIM_WALK_B, 
		BASE_ANIM_RUN_F, BASE_ANIM_RUN_B, BASE_ANIM_RUN_L, BASE_ANIM_RUN_R, 
		BASE_ANIM_TURN_BACK,
		BASE_ANIM_DODGE_ROLL, BASE_ANIM_DODGE_CARTWHEEL, BASE_ANIM_DODGE_FLIP,
		BASE_ANIM_END
	};

	enum EPLAYERANIM_SPEAR {
		SPEAR_ANIM_MAIN_ATK_COMBO_0 = BASE_ANIM_END, SPEAR_ANIM_MAIN_ATK_COMBO_0_RECORVERY, SPEAR_ANIM_MAIN_ATK_COMBO_1, SPEAR_ANIM_MAIN_ATK_COMBO_1_RECORVERY, SPEAR_ANIM_MAIN_ATK_COMBO_2, SPEAR_ANIM_MAIN_ATK_COMBO_2_RECORVERY,
		SPEAR_ANIM_POWER_ATK_COMBO_0, SPEAR_ANIM_POWER_ATK_COMBO_1, SPEAR_ANIM_POWER_ATK_COMBO_2, SPEAR_ANIM_ULTIMATE,
		SPEAR_ANIM_EXECUTION_GADASURA, SPEAR_ANIM_EXECUTION_JALSURA, SPEAR_ANIM_EXECUTION_MAHINASURA, SPEAR_ANIM_EXECUTION_TEZABSURA, SPEAR_ANIM_EXECUTION_NINJA,
		SPEAR_ANIM_THROW_START, SPEAR_ANIM_THROW_LOOP, SPEAR_ANIM_THROW_END, SPEAR_ANIM_THROW_LOOP_MOV_F, SPEAR_ANIM_THROW_LOOP_MOV_B, SPEAR_ANIM_THROW_LOOP_MOV_L, SPEAR_ANIM_THROW_LOOP_MOV_R,
		SPEAR_ANIM_END
	};


private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	virtual _int Update(_double fDeltaTime) override;
	virtual _int LateUpdate(_double fDeltaTimer) override;
	virtual _int Render() override;
	virtual _int LateRender() override;
	
public:
	_fVector Get_BonePos(const char* pBoneName);

private: /* Change Start State */
	void	Set_State_IdleStart(_double fDeltaTime);			// Idle
	void	Set_State_MoveStart(_double fDeltaTime);			// Move

	void	Set_State_DodgeStart(_double fDeltaTime);			// Dodge
	void	Set_State_MainAttackStart(_double fDeltaTime);		// MainAttack

	void	Set_State_TurnBackStart(_double fDeltaTime);		// TurnBack

private:
	virtual void Update_AttachCamPos() override;

private:
	HRESULT	Update_CamLookPoint(_double fDeltaTime);

private:
	HRESULT Update_State_Idle(_double fDeltaTime);
	HRESULT Update_State_Move(_double fDeltaTime);
	
	HRESULT Update_State_ComboAction(_double fDeltaTime);
	HRESULT Update_State_Attack(_double fDeltaTime);
	HRESULT Update_State_Evasion(_double fDeltaTime);


	HRESULT Update_State_Damage(_double fDeltaTime);
	HRESULT Update_State_Execution(_double fDeltaTime);
	HRESULT Update_State_Dead(_double fDeltaTime);

private: /* Key Input */
	/////////////////// Parent /////////////////////////////////
	void				Check_PlayerKeyInput(_double fDeltaTime);
	/////////////////////////////////////////////////////////////

	/////////////////// Child /////////////////////////////////
	_bool				Check_Mov_KeyInput(_double fDeltaTime);
	_bool				Check_Action_KeyInput(_double fDeltaTime);
	///////////////////////////////////////////////////////////

private: /* Actions */
	void				Move(EINPUT_MOVDIR eMoveDir, _double fDeltaTime);
	void				Turn_Back(_double fDeltaTime);
	void				Dodge(_double fDeltaTime);
	void				Attack(_double fDeltaTime);
	void				LookAtInputDir(_double fDeltaTime);

private: /* Select Anim */
	void				Play_DodgeAnim();

private: /* Setter */
	void				Set_InputDir(_int iAxis_F, _int iAxis_R, _double fDeltaTime);
	void				Set_PlayerState(EPLAYER_STATE eState);
	void				Set_TurnInputDir();
	void				Set_TurnInputDir_CalDir();
	void				Set_MainAttackAnim();

private: /* Getter */
	_fVector			LookAt_MousePos();

private:
	_float4				m_fCamLookPoint;

private: /* Key Input State */
	EINPUT_MOVDIR		m_eInputDir = MOVDIR_END;
	EINPUT_COMBO		m_eInputCombo = COMBO_END;
	_bool				m_bPressedComboKey = false;
	_bool				m_bPressedDodgeKey = false;
	_bool				m_bPressedMainAttackKey = false;
	_bool				m_bPressedPowerAttackKey = false;

private: /* Enum Stats */
	_uint				m_eCurAnim = BASE_ANIM_IDLE;
	EWEAPON_TYPE 		m_eCurWeapon = WEAPON_END;
	EPLAYER_STATE		m_eCurState = STATE_END;

private: /* Animation Control */
	_bool				m_bPlayTurnBackAnim = false;
	const _int			m_iMaxCombo = 3;
	_int				m_iCurCombo = 1;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

	_float				m_fAnimSpeed = 1.f;
	_float				m_fAnimSpeed_DecAcc = 0.f;
	_float				m_fMovSpeed_DecAcc = 0.f;

	_float3				m_fMovDir;

	_bool				m_bPlayNextCombo = false;

	_bool				m_bAttackEnd = false;

	_bool				m_bMainAttacking = false;
	_bool				m_bDodging = false;

private: /* Activate */
	_bool				m_bActivateLookDir = false;
	

private: /* Player Stats (이 중 몇몇은PlayerStatComp로 처리 예정) */
	_float				m_fPlayerCurSpeed = 0.f;
	_float				m_fPlayerMaxSpeed = 0.f;
	_float				m_fCurMovSpeedIncRate = 0.f;
	_float				m_fInerpSpeedMaxTime = 5.f;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_EtcInfo();


	HRESULT Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CPlayer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};
END
