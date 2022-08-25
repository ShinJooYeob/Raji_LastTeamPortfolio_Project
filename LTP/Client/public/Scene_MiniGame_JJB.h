#pragma once
#include "Scene.h"
#include "Taiko_Monster.h"

BEGIN(Client)

class CScene_MiniGame_JJB final : public CScene
{
public:
	enum TimingNoteType
	{
		TYPE_SMALLRED, TYPE_BIGRED, TYPE_SMALLBLUE, TYPE_BIGBLUE, TYPE_END
	};


private:
	explicit CScene_MiniGame_JJB(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CScene_MiniGame_JJB() = default;


public:
	virtual HRESULT		Initialize();
	virtual _int		Update(_double fDeltaTime)override;
	virtual _int		LateUpdate(_double fDeltaTime)override;
	virtual _int		Render()override;
	virtual _int		LateRender()override;

public:
	void				Push_NoteSmallRedMonsterLsit(CTaiko_Monster* pMonster);
	void				Push_NoteBigRedMonsterLsit(CTaiko_Monster* pMonster);
	void				Push_NoteSmallBlueMonsterLsit(CTaiko_Monster* pMonster);
	void				Push_NoteBigBlueMonsterLsit(CTaiko_Monster* pMonster);

	void				Pop_NoteSmallRedMonsterLsit();
	void				Pop_NoteBigRedMonsterLsit();
	void				Pop_NoteSmallBlueMonsterLsit();
	void				Pop_NoteBigBlueMonsterLsit();

	void				TimingNoteSetting();
	void				ProduceTimingNote();
	void				KeyClick();

public:
	_int				Get_ComboCount() { return m_iComboCount; }
	_int				Get_MaxCombo() { return m_iMaxCombo; }

public:
	void				Set_ComboZero() { m_iComboCount = 0; }
	void				Set_ComboPlus() { ++m_iComboCount; }

	void				ShowComboUI();
	void				ShowGoodEffectUI();

private:
	HRESULT				Ready_Light();
	HRESULT				Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Objects(const _tchar * pLayerTag);
	HRESULT				Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GoalCollider(const _tchar* pLayerTag);

	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT				Ready_PostPorcessing();

private:
	list<CTaiko_Monster*>		m_NoteSmallRedMonsterList;
	list<CTaiko_Monster*>		m_NoteBigRedMonsterList;
	list<CTaiko_Monster*>		m_NoteSmallBlueMonsterList;
	list<CTaiko_Monster*>		m_NoteBigBlueMonsterList;

	_bool						m_bIsStartBGM = false;
	_bool						m_bIsGameEnd = false;
	_bool						m_bIsResult = false;
	_float						m_AccumulateTime = 0.f;
	_int						m_iComboCount = 0;
	_int						m_iMaxCombo = 0;

	vector<NOTETIMINGDESC>		m_vecNoteTimings;
	_int						m_iVecCount = 0;

	class CGaolCollider*		m_pGoodCollider = nullptr;
	class CTaikoUI*				m_pTaikoUI = nullptr;

	//준비 타임
	_float						m_fReadyTime = 3.5f;
	_float						m_fEndTime = 3.f;

private:
	class CCamera_Main*				m_pMainCam = nullptr;
	CUtilityMgr*					m_pUtilMgr = nullptr;

public:
	static CScene_MiniGame_JJB*	Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual void				Free() override;

};
END
