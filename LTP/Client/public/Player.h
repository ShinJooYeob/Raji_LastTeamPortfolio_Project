#pragma once

#include "GameObject.h"
#include "PlayerWeapon.h"
#include "TriggerObject.h"

BEGIN(Engine)
class CTimer;
class CNavigation;
END

BEGIN(Client)
class CCamera_Main;
class CShellingSkillRange;
class CTriggerObject;
class CPlayer final : public CGameObject
{
public:
	enum EPLAYER_STATE {
		STATE_IDLE, STATE_MOV, STATE_ATTACK, STATE_JUMPATTACK, STATE_UTILITYSKILL, STATE_ULTIMATESKILL, STATE_PARKOUR, STATE_JUMP, STATE_FALL, STATE_CURTAIN, STATE_WALLRUN, STATE_PILLAR, STATE_PETAL, STATE_STOPACTION, STATE_ELEVATOR, STATE_EVASION, STATE_TAKE_DAMAGE, STATE_EXECUTION, STATE_SLEEP, STATE_DEAD, 
		STATE_DYNAMICPLATFORM_IDLE, STATE_DYNAMICPLATFORM_MOVE, STATE_DYNAMICPLATFORM_DEAD, STATE_FIRSTPERSONVIEW, STATE_END
	};

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
		WEAPON_NONE, WEAPON_SPEAR, WEAPON_BOW, WEAPON_SWORD, WEAPON_CHAKRA, WEAPON_SHIELD, WEAPON_END
	};
	


	//**	   Enum_Anim		**//
	enum EPLAYERANIM_BASE {
		BASE_ANIM_IDLE, 
		BASE_ANIM_WALK_F, BASE_ANIM_WALK_B, 
		BASE_ANIM_RUN_F, BASE_ANIM_RUN_B, BASE_ANIM_RUN_L, BASE_ANIM_RUN_R, 
		BASE_ANIM_TURN_BACK,
		BASE_ANIM_DODGE_ROLL, BASE_ANIM_DODGE_CARTWHEEL, BASE_ANIM_DODGE_FLIP,
		BASE_ANIM_RUN_BOW, 
		BASE_ANIM_IDLE_SWORD, BASE_ANIM_RUN_F_SWORD,
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

	enum EPLAYERANIM_LEDGE {
		LEDGE_ANIM_GRAB = SWORD_ANIM_END, LEDGE_ANIM_IDLE, LEDGE_ANIM_CLAMB_UP, 
		LEDGE_ANIM_JUMP_BACK, LEDGE_ANIM_JUMP_LEFT, LEDGE_ANIM_JUMP_RIGHT, LEDGE_ANIM_JUMP_UP, 
		LEDGE_ANIM_MOVE_LEFT, LEDGE_ANIM_MOVE_RIGHT,
		LEDGE_ANIM_REACHOUT_BACK_IDLE, LEDGE_ANIM_REACHOUT_LEFT, LEDGE_ANIM_REACHOUT_RIGHT, LEDGE_ANIM_REACHOUT_UP,
		LEDGE_ANIM_TURN_BACK, LEDGE_ANIM_TURN_LEFT, LEDGE_ANIM_TURN_RIGHT,
		LEDGE_ANIM_HANGING_IDLE,
		LEDGE_ANIM_HANGING_MOVE_LEFT, LEDGE_ANIM_HANGING_MOVE_RIGHT, LEDGE_ANIM_HANGING_REACHOUT_BACK_IDLE, LEDGE_ANIM_HANGING_REACHOUT_UP,
		LEDGE_ANIM_JUMP, LEDGE_ANIM_DOUBLEJUMP, LEDGE_ANIM_FALLING, LEDGE_ANIM_HANGING_CLIMBDOWN, BASE_ANIM_JUMP, LEDGE_ANIM_END
	};

	enum EPLAYERANIM_CURTAIN {
		CURTAIN_ANIM_TRANSITION = LEDGE_ANIM_END, CURTAIN_ANIM_FALLING, CURTAIN_ANIM_END
	};

	enum EPLAYERANIM_WALLRUN {
		WALLRUN_ANIM_LEFT = CURTAIN_ANIM_END, WALLRUN_ANIM_RIGHT, WALLRUN_ANIM_END
	};

	enum EPLAYERANIM_PILLAR {
		PILLAR_ANIM_GRAB = WALLRUN_ANIM_END, PILLAR_ANIM_IDLE, PILLAR_ANIM_CLIMB_UP, PILLAR_ANIM_CLIMB_DOWN, PILLAR_ANIM_MOVE_DOWN, PILLAR_ANIM_ROT_CLOCK, PILLAR_ANIM_ROT_ANTICLOCK,
		PILLAR_ANIM_TOP_CLIMB, PILLAR_ANIM_TOP_CLIMB_IDLE, PILLAR_ANIM_TOP_CLIMB_ROT_CLOCK, PILLAR_ANIM_TOP_CLIMB_ROT_ANTICLOCK,
		PILLAR_ANIM_BACKJUMP, PILLAR_ANIM_LOOKBACK, PILLAR_ANIM_DOWN_CLIMB, PILLAR_ANIM_JUMP, PILLAR_ANIM_TOP_JUMP, PILLAR_ANIM_END
	};

	enum EPLAYERANIM_PETAL {
		PETAL_ANIM_PLUCK = PILLAR_ANIM_END, PETAL_ANIM_THROW_INIT, PETAL_ANIM_THROW_LOOP, PETAL_ANIM_THROW_THROW, PETAL_ANIM_END
	};

	enum EPLAYERANIM_ETC {
		DAMAGE_ANIM_DAMAGE = PETAL_ANIM_END, DAMAGE_ANIM_DEATH, BASE_ANIM_JUMP_READY, BASE_ANIM_JUMP_CHARGING, BASE_ANIM_JUMP_JUMPING, BASE_ANIM_SLEEP, BASE_ANIM_WAKEUP, 
		BASE_ANIM_DYNAMICPLATFORM_IDLE, BASE_ANIM_DYNAMICPLATFORM_WALK, 
		ETC_ANIM_END
	};
	//

	enum ETARGETING_STATE {
		TARGETING_SEARCH, TARGETING_LOOP, TARGETING_BOSS, TARGETING_END
	};

	enum EPARKOUR_LEDGESTATE {
		LEDGE_JUMP, LEDGE_DOUBLEJUMP, LEDGE_HANGING_IDLE, LEDGE_HANGING_MOVE, LEDGE_HANGING_TURN, LEDGE_HANGING_JUMPUP, LEDGE_HANGING_FALLING, LEDGE_HANGING_FALLINGDOWN, LEDGE_LOOP,
		LEDGE_HANGING_CLIMBUP, LEDGE_HANGING_CLIMBDOWN
	};

	enum EPERAL_STATE {
		PETAL_PLUCK, PETAL_IDLE, PETAL_WALK, PETAL_THROW_INIT, PETAL_THROW_LOOP, PETAL_THROW_THROW, PETAL_END
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


	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	_bool	m_IsConfilicted = false;


public:
	virtual _fVector	Get_BonePos(const char* pBoneName) override;
	virtual _fMatrix	Get_BoneMatrix(const char* pBoneName) override;
	CTransform*			Get_Transform() const { return m_pTransformCom; }
	_bool				Get_IsLedgeReachBackState();
	_int				Get_CurPlayAnimation();
	_bool				Is_Hiding();
	_bool				Get_IsBlockCamTriggerState();

public: /* For CheckPoint/Resurrection */
	void	SetUp_CheckPoint_CurPos();
	void	SetUp_CheckPoint(_fVector vCurPos, _uint iNavIndex);
	void	Resurrection();

public: /* public Setter */
	void	Set_JumpPower(_float fJumpPower);
	void	Set_CurParkourTrigger(CTriggerObject* pParkourTrigger);
	void	Set_PlayerNavIndex(_uint iNavIndex);
	void	Set_PillarBlockClimbUp(_bool bBlock, _float vBlockLimitHeight);
	void	Set_FallingDead(_bool bFallingDead);
	void	Set_PosY(_float fPos_y);
	void	Set_OnLilyPad(_bool bOnLilyPad);
	void	Set_PlayerState(EPLAYER_STATE eState);
	void	Set_TargetingLookDir(_float3 fTargetingLookDir);
	void	Set_MaxBossTargetingDist(_float fDist);
	void	Set_MinBossTargetingDist(_float fDist);
	void	Set_BlockUpdate(_bool bBlockUpdate);

public: /* Damage Logic*/
	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;
	virtual _float	Take_Damage_Instance(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f);
	_float	Apply_Damage(CGameObject* pTargetObject, _float fDamageAmount, _bool bKnockback);

public:
	void	Move_OppositeDir();

public:
	// First Start
	void	Set_State_FirstStart();

	// Ledge
	void	Set_State_ParkourStart(_double fDeltaTime);																
	void	Set_State_LedgeClimbDownStart(_float3 fLookDir, _double fDeltaTime);
	void	Set_State_LedgeClimbUpStart(_double fDeltaTime);
	void	Set_State_WallJump();


	// Pillar
	void	Set_State_PillarStart(_double fDeltaTime);																
	void	Set_State_PillarGrabStart(_bool bTurnReflect, _double fDeltaTime);


	// WallRun
	void	Set_State_WallRunStart(_bool bAnimDir, _float3 fStartPos, _float3 fEndPos, _double fDeltaTime);	


	// Elevator
	void	Set_State_ElevatorStart();																			
	void	Set_State_ElevatorEnd();																				

	// Stop Action
	void	Set_State_StopActionStart();
	void	Set_State_StopActionEnd();

	// Curtain
	void	Set_State_CurtainStart(_double fDeltaTime);

	// Petal
	void	Set_State_PetalStart(_float3 vPetalPos, _double fDeltaTime);

	// Dynamic Platform
	void	Set_State_OnDynamicPlatformStart();
	void	Set_State_OnDynamicPlatformDead();

	// Falling
	void	Set_State_FallingStart(_double fDeltaTime);

	
public:
	EPLAYER_STATE Get_PlayerState();
	EPARKOUR_LEDGESTATE Get_LedgeState();

public:
	void				Set_CurParkurLedge(class CTestLedgeTrigger* pTargetLedge);
	CTriggerObject*		Get_CurParkurTriger();

public:
	CGameObject*		Get_CurTargetingMonster();

private: /* Change Start State */
	void	Set_State_IdleStart(_double fDeltaTime);								// Idle
	void	Set_State_MoveStart(_double fDeltaTime);								// Move
	
	void	Set_State_DodgeStart(_double fDeltaTime);								// Dodge
	void	Set_State_MainAttackStart(_double fDeltaTime);							// MainAttack
	void	Set_State_UtilitySkillStart(_double fDeltaTime);						// Utility
	void	Set_State_UltimateSkillStart(_double fDeltaTime);						// Ultimate
	void	Set_State_TurnBackStart(_double fDeltaTime);							// TurnBack

	void	Set_State_JumpStart(_double fDeltaTime);								// Jump

	void	Set_State_DamageStart(_float fKnockbackPower, _fVector vDamageDir);		// TakeDamage
	void	Set_State_DeathStart();													// Death


public:
	virtual void Update_AttachCamPos() override;

public:
	void	Set_Targeting(CGameObject* pTarget);
	void	Check_TargetingTarget(CGameObject* pTarget);

private:
	HRESULT	Update_CamLookPoint(_double fDeltaTime);

	HRESULT Update_State_Sleep(_double fDeltaTime);
	HRESULT Update_State_Idle(_double fDeltaTime);
	HRESULT Update_State_Move(_double fDeltaTime);
	HRESULT Update_State_Jump(_double fDeltaTime);
	HRESULT Update_State_Fall(_double fDeltaTime);

	HRESULT Update_State_DynamicPlatform_Idle(_double fDeltaTime);
	HRESULT Update_State_DynamicPlatform_Move(_double fDeltaTime);
	HRESULT Update_State_DynamicPlatform_Dead(_double fDeltaTime);

	HRESULT Update_State_Attack(_double fDeltaTime);
	HRESULT Update_State_UtilitySkill(_double fDeltaTime);
	HRESULT Update_State_UltimateSkill(_double fDeltaTime);
	HRESULT Update_State_Evasion(_double fDeltaTime);

	HRESULT Update_State_Parkour(_double fDeltaTime);
	HRESULT Update_State_Curtain(_double fDeltaTime);
	HRESULT Update_State_WallRun(_double fDeltaTime);
	HRESULT Update_State_Pillar(_double fDeltaTime);

	HRESULT Update_State_Petal(_double fDeltaTime);

	HRESULT Update_State_Elevator(_double fDeltaTime);

	HRESULT Update_State_StopAction(_double fDeltaTime);

	HRESULT Update_State_FirstPersonView(_double fDeltaTime);

	HRESULT Update_State_Damage(_double fDeltaTime);
	HRESULT Update_State_Execution(_double fDeltaTime);
	HRESULT Update_State_Dead(_double fDeltaTime);


	HRESULT	Update_Collider(_double fDeltaTime);
	HRESULT Update_HPUI(_double fDeltaTime);
	HRESULT LateUpdate_UI(_double fDeltaTime);

	HRESULT Update_PhysX();

private: /* Check */
	_bool				Check_InputDirIsForward();
	void				Check_CurNaviCellOption();
	void				Check_PlayerOnNavi();
	void				Check_Execution();

private: /* Key Input */
	_bool				Check_PlayerKeyInput(_double fDeltaTime);

	_bool				Check_Mov_KeyInput(_double fDeltaTime);
	_bool				Check_ChangeCameraView_KeyInput_ForDebug(_double fDeltaTime);
	_bool				Check_Action_KeyInput(_double fDeltaTime);
	_bool				Check_SwapWeapon_KeyInput(_double fDeltaTime);

private: /* Actions */
	void				Move(EINPUT_MOVDIR eMoveDir, _double fDeltaTime);
	void				Move_NotTurn(EINPUT_MOVDIR eMoveDir, _double fDeltaTime, _bool bOnNavigation = false);
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

	/** For Parkours */
	void				Ledging(_double fDeltaTime);

private: /* Select Anim */
	void				Play_DodgeAnim();


private: /* private Setter */
	void				Set_InputDir(_int iAxis_F, _int iAxis_R, _double fDeltaTime);
	void				Set_TurnInputDir();
	void				Set_TurnInputDir_CalDir();
	void				Set_MainAttackAnim(_bool bJumpAttack);
	void				Set_PhysX_Head();

private:
	void				Update_Targeting(_double fDeltaTime);
	void				Targeting_Search();
	void				Targeting_Loop();
	void				Targeting_Boss();

private: /* Motion Trail*/
	void				CheckOn_MotionTrail();
	void				Active_MotionTrail();

private: /* Check */
	void				Check_NextComboCommand();
	_bool				Change_NextCombo();
	_float3				Check_MousePicking();


private: /* Getter */
	void				LookAt_MousePos(_float fWeight = 0.85f);
	_fVector			Get_MousePos();

private:
	void				DebugingCode();

private:
	_float4				m_fCamLookPoint;

private: /* Relate Parkour */
	CTriggerObject::EParkourTriggerType 		m_eCurParkourState;
	CTriggerObject*								m_pCurParkourTrigger = nullptr;
	EPARKOUR_LEDGESTATE							m_eCurLedgeState = EPARKOUR_LEDGESTATE::LEDGE_JUMP;

	_float3										m_fPillarParkourInitPos = _float3(0.f);
	_bool										m_bBlockClimbUp = false;
	_float										m_fPillarClimbUpBlockHeight = 0.f;

	_float3										m_fWallRunStartPos = _float3(0.f);
	_float3										m_fWallRunEndPos = _float3(0.f);
	
	_float3										m_fPetalPos = _float3(0.f);


private: /* Key Input State */
	EINPUT_MOVDIR		m_eInputDir = MOVDIR_END;
	EINPUT_COMBO		m_eInputCombo = COMBO_END;
	_bool				m_bPressedComboKey = false;
	_bool				m_bPressedDodgeKey = false;
	_bool				m_bPressedMainAttackKey = false;
	_bool				m_bPressedPowerAttackKey = false;
	_bool				m_bPressedUtilityKey = false;
	_bool				m_bPressedUltimateKey = false;
	_bool				m_bPressedInteractKey = false;


private: /* For TurnBack Timing*/
	_float				m_fCurTime_PressedMoveKeyDuration = 0.f;
	_float				m_fMaxTime_PressedMoveKeyDuration = 3.f;


private: /* Enum Stats */
	_uint					m_eCurAnim = BASE_ANIM_IDLE;
	EWEAPON_TYPE 			m_eCurWeapon = WEAPON_END;
	EPLAYER_STATE			m_eCurState = STATE_END;
	EUTILITYSKILL_STATE		m_eCurUtilityState = UTILITY_START; 
	EBOWMAINATK_STATE		m_eCurBowMainAtkState = BOWMAINATK_START;
	EPERAL_STATE			m_eCurPetalState = PETAL_END;


private: /* Animation Control */
	_bool					m_bPlayTurnBackAnim = false;
	const _int				m_iMaxCombo = 3;
	_int					m_iCurCombo = 1;
	

	_uint					m_iOldAnimIndex = INT_MAX;
	_uint					m_iAdjMovedIndex = 0;

	_float					m_fAnimSpeed = 1.f;
	_float					m_fAnimSpeed_DecAcc = 0.f;
	_float					m_fMovSpeed_DecAcc = 0.f;

	_float3					m_fMovDir;

	_bool					m_bPlayNextCombo = false;

	_bool					m_bPlayMainAttackCombo = false;
	_bool					m_bReadyMainAttackCombo = false;
	_bool					m_bPlayDodgeCombo = false;
	_bool					m_bReadyDodgeCombo = false;
	_bool					m_bPlayJumpAttackCombo = false;
	_bool					m_bReadyJumpAttackCombo = false;


	_bool					m_bAttackEnd = true;
	_bool					m_bPlayJumpAttack = false;
	_bool					m_bMainAttacking = false;
	_bool					m_bPlayPowerAttack = false;
	_bool					m_bDodging = false;
	
	_bool					m_bThrowSpear = false;

	_bool					m_bAnimChangeSwitch = false;
	_bool					m_bActionSwitch = false;
	_bool					m_bTrailSwitch = false;
	_float					m_fChargingTime = 0.f;
	_float					m_fArrowRange = 0.f;

	_float					m_fJumpStart_Y = 0.f;
	_float					m_fFallingStart_Y = 0.f;
	_float					m_fFallingAcc = 0.f;
	_float					m_fJumpPower = 0.f;

	EINPUT_MOVDIR			m_eLedgingMoveDir = MOVDIR_END;
	_bool					m_bLedgingMove = false;

	EINPUT_MOVDIR			m_ePreInputMovDir = MOVDIR_END;

	_bool					m_bUpdateAnimation = true;

	_bool					m_bOnNavigation = false;
	_bool					m_bOnLilyPad = false;

	_bool					m_bShieldMode;

	_bool					m_bActiveCollider = true;
	_bool					m_bActiveRepel = true;

	_float3					m_fLookDir = _float3(0.f, 0.f, 0.f);

	_bool					m_bLedge_ReachBackState = false;

	_bool					m_bFallingDead = false;

	_bool					m_bBlockUpdate = false;

private: /* Control */
	_float					m_fDelayTime = 0.f;
	_bool					m_bBlockCamTrigger = false;

private: /* For Cheat Mode*/
	_bool					m_bPowerOverwhelming = false;


private: /* Resurrect Info */
	_float3					m_fResurrectPos = _float3(0.f, 0.f, 0.f);
	_uint					m_iResurrectNavIndex = 0;
	_float					m_fResurrectionDelay = 0.f;


private: /* For Navi */
	CCell::CELL_OPTION		m_eCurPosNavCellOption = CCell::CELL_OPTION::CELL_END;


private: /* For Sound */
	_bool					m_bOncePlaySound = false;
	_bool					m_bOncePlaySwingSound = false;


private: /* Camera Shake */
	_bool					m_bActive_ActionCameraShake = true;


private: /* Motion Trail */
	_float					m_fInterval_MotionTrail = 0.f;
	_bool					m_bOn_MotionTrail = false;


private: /* Activate */
	_bool					m_bActivateLookDir = false;


private: /* Cam View */
	_int					m_iMaxCamViewIndex = 0;
	_int					m_iCurCamViewIndex = 0;


private: /* Timer */
	_float					m_fMaxTime_ShellingDelay = 0.f;
	_float					m_fCurTime_ShellingDelay = 0.f;


private: /* Damage */
	_float					m_fKnockbackPower = 0.f;
	_float3					m_fKnockbackDir = _float3(0.f, 0.f, 0.f);


private:
	CShellingSkillRange*	m_pShellingSkillRange = nullptr;


private: /* Targeting */
	ETARGETING_STATE		m_eCur_TargetingState = ETARGETING_STATE::TARGETING_SEARCH;
	CGameObject*			m_pTargetingMonster = nullptr;
	CTransform*				m_pTargetingMonster_Transform = nullptr;


private: /* ETC */
	_bool					m_bPlayerHide = false;
	_bool					m_bBlockRender = false;
	_float3					m_fTargetingLookDir = _float3(0.f, -0.5f, 0.3f);
	_float					m_fDist_MaxBossTargeting = 90.f;
	_float					m_fDist_MinBossTargeting = 20.f;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CMotionTrail*			m_pMotionTrail = nullptr;
	CCamera_Main*			m_pMainCamera = nullptr;
	CTransform*				m_pMainCameraTransform = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;

	CCollider*				m_pCollider = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc;

	CCollider*				m_pCollider_Parkur = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc_Parkur;

	class CCollider_PhysX_Joint*	m_pHeadJoint = nullptr;

	class CHpUI*			m_pHPUI = nullptr;

	_bool					m_bMehsArrow= false;

private:
	CPlayerWeapon*			m_pPlayerWeapons[WEAPON_END - 1];

	//JJB
	_bool					m_bIsSkillUI = false;
	class CSkillUI*			m_pSkillUI = nullptr;
	class CIngameUI*		m_pIngameUI = nullptr;

private:
	CTransform*												m_pTextureParticleTransform = nullptr;
	CTransform*												m_pMeshParticleTransform = nullptr;
	CTransform*												m_pJYTextureParticleTransform = nullptr;
	vector<INSTPARTICLEDESC>								m_vecTextureParticleDesc;
	vector<INSTMESHDESC>									m_vecMeshParticleDesc;
	vector<NONINSTNESHEFTDESC>								m_vecNonInstMeshDesc;
	class CInstanceEffect*									m_ArrToEraseEffect[2] = { nullptr ,nullptr};

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_EtcInfo();
	HRESULT SetUp_PlayerWeapons();
	HRESULT SetUp_PlayerEffects();

	HRESULT Adjust_AnimMovedTransform(_double fDeltatime);
	

private: /* For Particle */
	HRESULT Ready_ParticleDesc();
	HRESULT Update_Partilce_Position();


public:
	static CPlayer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END
