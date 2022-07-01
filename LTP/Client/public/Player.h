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

	enum EUTILITYSKILL_STATE {
		UTILITY_START, UTILITY_LOOP, UTILITY_ACTIVE, UTILITY_END
	};

	enum EBOWMAINATK_STATE {
		BOWMAINATK_START, BOWMAINATK_LOOP, BOWMAINATK_SHOT, BOWMAINATK_END
	};

	enum EWEAPON_TYPE {
		WEAPON_NONE, WEAPON_SPEAR, WEAPON_BOW, WEAPON_SWORD, WEAPON_CHAKRA, WEAPON_END
	};
	
	enum EPLAYER_STATE {
		STATE_IDLE, STATE_MOV, STATE_ATTACK, STATE_JUMPATTACK, STATE_UTILITYSKILL, STATE_ULTIMATESKILL,        STATE_EVASION, STATE_TAKE_DAMAGE, STATE_EXECUTION, STATE_DEAD, STATE_END
	};


	//**	   Enum_Anim		**//
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
		SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK, SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK, SPEAR_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK,
		SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK, SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK, SPEAR_ANIM_POWER_ATK_COMBO_2_JUMPATTACK,
		SPEAR_ANIM_END
	};

	enum EPLAYERANIM_BOW {
		BOW_ANIM_MAIN_ATK_START = SPEAR_ANIM_END, BOW_ANIM_MAIN_ATK_LOOP, BOW_ANIM_MAIN_ATK_SHOT,
		BOW_ANIM_MAIN_ATK_LOOP_RUN_F, BOW_ANIM_MAIN_ATK_LOOP_RUN_B, BOW_ANIM_MAIN_ATK_LOOP_RUN_L, BOW_ANIM_MAIN_ATK_LOOP_RUN_R,
		BOW_ANIM_POWER_COMBO_0, BOW_ANIM_POWER_COMBO_1, BOW_ANIM_POWER_COMBO_2,
		BOW_ANIM_ULTIMATE, BOW_ANIM_UTILITY_START, BOW_ANIM_UTILITY_LOOP, BOW_ANIM_UTILITY_SHOT,
		BOW_ANIM_EXECUTION_GADASURA, BOW_ANIM_EXECUTION_JALSURA, BOW_ANIM_EXECUTION_MAHINASURA, BOW_ANIM_EXECUTION_TEZABSURA, BOW_ANIM_EXECUTION_NINJA, BOW_ANIM_EXECUTION_MAHINASURA_2,
		BOW_ANIM_POWER_COMBO_1_JUMP, BOW_ANIM_POWER_COMBO_2_JUMP,
		BOW_ANIM_END
	};

	enum EPLAYERANIM_SWORD {
		SWORD_ANIM_MAIN_ATK_COMBO_0 = BOW_ANIM_END, SWORD_ANIM_MAIN_ATK_COMBO_1, SWORD_ANIM_MAIN_ATK_COMBO_2, SWORD_ANIM_MAIN_ATK_COMBO_2To0,
		SWORD_ANIM_POWER_ATK_COMBO_0, SWORD_ANIM_POWER_ATK_COMBO_1, SWORD_ANIM_POWER_ATK_COMBO_2,
		SWORD_ANIM_ULTIMATE, SWORD_ANIM_SHIELD_IDLE,
		SWORD_ANIM_EXECUTION_GADASURA, SWORD_ANIM_EXECUTION_JAISURA, SWORD_ANIM_EXECUTION_MAHINASURA, SWORD_ANIM_EXECUTION_NINJA, SWORD_ANIM_EXECUTION_MAHINASURA_0, SWORD_ANIM_EXECUTION_TEZABSURA,
		SWORD_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK, SWORD_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK, SWORD_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK,
		SWORD_ANIM_POWER_ATK_COMBO_0_JUMPATTACK, SWORD_ANIM_POWER_ATK_COMBO_1_JUMPATTACK, SWORD_ANIM_POWER_ATK_COMBO_2_JUMPATTACK,
		SWORD_ANIM_SHIELD_WALK_F, SWORD_ANIM_SHIELD_WALK_B, SWORD_ANIM_SHIELD_WALK_L, SWORD_ANIM_SHIELD_WALK_R,
		SWORD_ANIM_END
	};
	//



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
	void	Set_State_IdleStart(_double fDeltaTime);								// Idle
	void	Set_State_MoveStart(_double fDeltaTime);								// Move

	void	Set_State_DodgeStart(_double fDeltaTime);								// Dodge
	void	Set_State_MainAttackStart(_double fDeltaTime);							// MainAttack
	void	Set_State_UtilitySkillStart(_double fDeltaTime);						// Utility
	void	Set_State_UltimateSkillStart(_double fDeltaTime);						// Ultimate
	void	Set_State_TurnBackStart(_double fDeltaTime);							// TurnBack


private:
	virtual void Update_AttachCamPos() override;


private:
	HRESULT	Update_CamLookPoint(_double fDeltaTime);


private:
	HRESULT Update_State_Idle(_double fDeltaTime);
	HRESULT Update_State_Move(_double fDeltaTime);
	

	HRESULT Update_State_Attack(_double fDeltaTime);
	HRESULT Update_State_UtilitySkill(_double fDeltaTime);
	HRESULT Update_State_UltimateSkill(_double fDeltaTime);
	HRESULT Update_State_Evasion(_double fDeltaTime);


	HRESULT Update_State_Damage(_double fDeltaTime);
	HRESULT Update_State_Execution(_double fDeltaTime);
	HRESULT Update_State_Dead(_double fDeltaTime);


private: /* Key Input */
	void				Check_PlayerKeyInput(_double fDeltaTime);

	_bool				Check_Mov_KeyInput(_double fDeltaTime);
	_bool				Check_Action_KeyInput(_double fDeltaTime);
	_bool				Check_SwapWeapon_KeyInput(_double fDeltaTime);


private: /* Actions */
	void				Move(EINPUT_MOVDIR eMoveDir, _double fDeltaTime);
	void				Move_NotTurn(EINPUT_MOVDIR eMoveDir, _double fDeltaTime);
	void				Turn_Back(_double fDeltaTime);
	void				Dodge(_double fDeltaTime);
	void				LookAtInputDir(_double fDeltaTime);
	void				Attack_Spear(_double fDeltaTime);
	void				Attack_Bow(_double fDeltaTime);
	void				Attack_Sword(_double fDeltaTime);


	/** For Equip Weapon_Spear */
	void				Javelin(_double fDeltaTime);
	void				Throw_Spear(_double fDeltaTime);
	void				Spear_Ultimate(_double fDeltaTime);
	
	/** For Equip Weapon Bow */
	void				Shelling(_double fDeltaTime);
	void				Shot_Shelling(_double fDeltaTime);
	void				Bow_Ultimate(_double fDeltaTime);

	/** For Equip Weapon Sword */
	void				Shield_Mode(_double fDeltaTime);
	void				Sword_Ultimate(_double fDeltaTime);

private: /* Select Anim */
	void				Play_DodgeAnim();


private: /* Setter */
	void				Set_InputDir(_int iAxis_F, _int iAxis_R, _double fDeltaTime);
	void				Set_PlayerState(EPLAYER_STATE eState);
	void				Set_TurnInputDir();
	void				Set_TurnInputDir_CalDir();

	void				Set_MainAttackAnim(_bool bJumpAttack);


private:
	void				Check_NextComboCommand();
	void				Change_NextCombo();


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
	_bool				m_bPressedUtilityKey = false;
	_bool				m_bPressedUltimateKey = false;

private: /* Enum Stats */
	_uint					m_eCurAnim = BASE_ANIM_IDLE;
	EWEAPON_TYPE 			m_eCurWeapon = WEAPON_END;
	EPLAYER_STATE			m_eCurState = STATE_END;
	EUTILITYSKILL_STATE		m_eCurUtilityState = UTILITY_START;
	EBOWMAINATK_STATE		m_eCurBowMainAtkState = BOWMAINATK_START;

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

	_bool				m_bPlayMainAttackCombo = false;
	_bool				m_bReadyMainAttackCombo = false;
	_bool				m_bPlayDodgeCombo = false;
	_bool				m_bReadyDodgeCombo = false;
	_bool				m_bPlayJumpAttackCombo = false;
	_bool				m_bReadyJumpAttackCombo = false;


	_bool				m_bAttackEnd = true;
	_bool				m_bPlayJumpAttack = false;
	_bool				m_bMainAttacking = false;
	_bool				m_bPlayPowerAttack = false;
	_bool				m_bDodging = false;
	

private: /* Activate */
	_bool				m_bActivateLookDir = false;


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
