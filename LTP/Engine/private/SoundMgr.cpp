
#include "..\Public\SoundMgr.h"


IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
	:m_iNumOfEachChannel(_uint((MaxChannelCount - BGMChannelCount) / (CHANNEL_MAXCHANNEL - 1)))
{
}
HRESULT CSoundMgr::Initialize_FMOD()
{
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, MaxChannelCount, FMOD_INIT_NORMAL, NULL);

	for (_uint i = 0; i < CHANNEL_MAXCHANNEL;i++)
	{
		m_VolumeArr[i] = SOUND_DEFAULT;
	}

	ZeroMemory(m_PauseArr, sizeof(_bool)*CHANNEL_MAXCHANNEL);
	ZeroMemory(m_fPassedTimeArr, sizeof(_float) * MaxChannelCount);

	return LoadSoundFile();
}

HRESULT CSoundMgr::Update_FMOD(_float fTimeDelta)
{
	FMOD_System_Update(m_pSystem);

	FMOD_BOOL bPlay = FALSE;

	for (_uint i = 0; i< MaxChannelCount; i++)
	{
		if (m_fPassedTimeArr[i] != 0)
		{
			if (FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay))
				m_fPassedTimeArr[i] = 0;
			else
				m_fPassedTimeArr[i] += fTimeDelta;
		}
	}

	return S_OK;
}

_int CSoundMgr::Set_ChannelVolume(CHANNELID eID, _float _vol)
{
	m_VolumeArr[eID] = _vol;
	m_VolumeArr[eID] = max(min(m_VolumeArr[eID], SOUND_MAX), SOUND_MIN);

	if (eID == CHANNEL_BGM)
	{
		for (_uint i = MaxChannelCount - BGMChannelCount; i < MaxChannelCount; i++)
			FMOD_Channel_SetVolume(m_pChannelArr[i], m_VolumeArr[eID]);

	}
	else
	{
		for (_uint i = eID * m_iNumOfEachChannel; i < (eID + 1)* m_iNumOfEachChannel; i++)
			FMOD_Channel_SetVolume(m_pChannelArr[i], m_VolumeArr[eID]);
	}

	return 0;
}

_int CSoundMgr::Pause_ChannelSound(CHANNELID eID)
{
	m_PauseArr[eID] = !m_PauseArr[eID];

	if (eID == CHANNEL_BGM)
	{
		for (_uint i = MaxChannelCount - BGMChannelCount; i < MaxChannelCount; i++)
			FMOD_Channel_SetPaused(m_pChannelArr[i], m_PauseArr[eID]);

	}
	else
	{
		for (_uint i = eID * m_iNumOfEachChannel; i < (eID + 1)* m_iNumOfEachChannel; i++)
		{
			FMOD_Channel_SetPaused(m_pChannelArr[i], m_PauseArr[eID]);
		}

	}
	return 0;
}


HRESULT CSoundMgr::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float fLouderMultiple)
{
	if (eID == CHANNEL_BGM)
	{
		__debugbreak();
		return E_FAIL;
	}

	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
	{
		__debugbreak();
		return E_FAIL;
	}

	FMOD_BOOL bPlay = FALSE;
	_uint fOldSoundIndx = -1;
	_float fOldestTime = -1.f;

	for (_uint i = eID * m_iNumOfEachChannel; i < (eID + 1) * m_iNumOfEachChannel; i++)
	{
		//if (FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay))
		if (m_fPassedTimeArr[i] == 0)
		{
			FMOD_Channel_Stop(m_pChannelArr[i]);
			FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[i]);


			FMOD_Channel_SetVolume(m_pChannelArr[i], m_VolumeArr[eID]* fLouderMultiple);
			m_fPassedTimeArr[i] = 0.1f;


			FMOD_System_Update(m_pSystem);
			return S_OK;
		}

		if (m_fPassedTimeArr[i] > fOldestTime)
		{
			fOldestTime = m_fPassedTimeArr[i];
			fOldSoundIndx = i;
		}

	}



	FMOD_Channel_Stop(m_pChannelArr[fOldSoundIndx]);
	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[fOldSoundIndx]);


	FMOD_Channel_SetVolume(m_pChannelArr[fOldSoundIndx], m_VolumeArr[eID] * fLouderMultiple);

	m_fPassedTimeArr[fOldSoundIndx]  = 0.1f;
	FMOD_System_Update(m_pSystem);
	return S_OK;

}

HRESULT CSoundMgr::PlayBGM(TCHAR * pSoundKey, _uint iBGMIndex ,_float fLouderMultiple)
{
	if (BGMChannelCount <= iBGMIndex)
	{
		__debugbreak();
		return E_FAIL;
	}

	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	FMOD_Channel_Stop(m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)]);

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
	{
		__debugbreak();
		return E_FAIL;
	}

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)]);


	FMOD_Channel_SetVolume(m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)], m_VolumeArr[CHANNEL_BGM] * fLouderMultiple);

	FMOD_Channel_SetMode(m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
	return S_OK;
}

void CSoundMgr::Stop_ChannelSound(CHANNELID eID)
{
	if (eID == CHANNEL_BGM)
	{

		for (_uint i = MaxChannelCount - BGMChannelCount; i < MaxChannelCount; i++)
			FMOD_Channel_Stop(m_pChannelArr[i]);
	}
	else
	{
		for (_uint i = eID * m_iNumOfEachChannel; i < (eID + 1)* m_iNumOfEachChannel; i++)
		{
			FMOD_Channel_Stop(m_pChannelArr[i]);
			m_fPassedTimeArr[i] = 0;
		}

	}

}

void CSoundMgr::Stop_AllChannel()
{
	for (_uint i = 0; i < MaxChannelCount; ++i) {

		FMOD_Channel_Stop(m_pChannelArr[i]);
		m_fPassedTimeArr[i] = 0;
	}
}

_float CSoundMgr::Get_Channel_Volume(CHANNELID eID)
{
	return m_VolumeArr[eID];
}

_bool CSoundMgr::Get_Channel_IsPaused(CHANNELID eID)
{
	return m_PauseArr[eID];
}

HRESULT CSoundMgr::LoadSoundFile()
{
	_tfinddata64_t fd;
	__int64 handle = _tfindfirst64(L"../Bin/Resources/SoundFiles/*.*", &fd);
	if (handle == -1 || handle == 0)
	{
		__debugbreak();
		return E_FAIL;
	}

	_int iResult = 0;

	char szCurPath[128] = "../Bin/Resources/SoundFiles/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1)
	{
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (eRes == FMOD_OK)
		{
			_uint iLength = _uint(strlen(szFilename) + 1);

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, szFilename, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _tfindnext64(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
	return S_OK;
}


void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
