#pragma once

#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{


private:
	explicit CLoader();
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENEID eSceneID);

public:
	_bool IsLoadFinished() { return m_bIsLoadingFinished; };
	_int Get_ProgressCount() { return m_iLoadingProgressCount; };
	_int Get_MaxCount() { return m_iLoadingMaxCount; };
	SCENEID Get_SceneID() {	return m_eSceneID;}


public:
	HRESULT Load_Scene_Loby(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_StageSelect(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage1(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage2(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage3(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage4(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage5(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage6(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage7(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Boss(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Ending(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);


	HRESULT Load_Scene_Edit(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);

private:
	//�ε��� �������� �˷��ִ� ����
	_bool m_bIsLoadingFinished = false;
	
	//�ε��� �������� ���� ����
	_int m_iLoadingMaxCount = 1;
	_int m_iLoadingProgressCount = 0;

	SCENEID  m_eSceneID = SCENEID::SCENE_END;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	static _bool	bIsAlreadyloadedArr[SCENE_END];

public:
	static CLoader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENEID eSceneID);
	virtual void Free() override;
};

END