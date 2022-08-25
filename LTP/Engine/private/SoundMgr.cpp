
#include "..\Public\SoundMgr.h"
#include "..\Public\PipeLineMgr.h"


IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
	:m_iNumOfEachChannel(_uint((MaxChannelCount - BGMChannelCount) / (CHANNEL_MAXCHANNEL - 1)))
{
}
HRESULT CSoundMgr::Initialize_FMOD()
{
	FMOD_System_Create(&m_pSystem);

	m_Verson = FMOD_VERSION;
	FMOD_System_GetVersion(m_pSystem, &m_Verson);


	me_SoundMode = CSoundMgr::SOUNDMODE_3D;
	
	if (me_SoundMode == CSoundMgr::SOUNDMODE_NOMAL)
	{
		// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
		FMOD_System_Init(m_pSystem, MaxChannelCount, FMOD_INIT_NORMAL, NULL);
	}
	else if (me_SoundMode == CSoundMgr::SOUNDMODE_3D)
	{
		// 3D 초기화
		FMOD_System_Init(m_pSystem, MaxChannelCount, FMOD_INIT_NORMAL| FMOD_INIT_CHANNEL_DISTANCEFILTER, NULL);
		FAILED_CHECK(Initialize_FMOD3D());
	}


	for (_uint i = 0; i < CHANNEL_MAXCHANNEL;i++)
	{
		m_VolumeArr[i] = SOUND_DEFAULT;
	}

	ZeroMemory(m_PauseArr, sizeof(_bool)*CHANNEL_MAXCHANNEL);
	ZeroMemory(m_fPassedTimeArr, sizeof(_float) * MaxChannelCount);

	// FMOD_System_SetUserData(m_pSystem, );
	FAILED_CHECK(LoadSoundFile_3D());
	FAILED_CHECK(LoadSoundFile_2D());
	return S_OK;
}

HRESULT CSoundMgr::Update_FMOD(_float fTimeDelta)
{
	FMOD_BOOL bPlay = FALSE;

	CPipeLineMgr*		pPipeLineMgr = GetSingle(CPipeLineMgr);

	_Matrix		ViewMatrixInv;
	
	ViewMatrixInv = XMMatrixInverse(nullptr, pPipeLineMgr->Get_Transform_Matrix(PLM_VIEW));
	Setup_Listender_Camera(ViewMatrixInv);


	for (_uint i = 0; i< MaxChannelCount - BGMChannelCount; i++)
	{
		if (m_fPassedTimeArr[i] != 0)
		{
			if (FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay))
				m_fPassedTimeArr[i] = 0;
			else
				m_fPassedTimeArr[i] += fTimeDelta;
		}
	}

	if (m_bChangingBGM)
	{
		m_ChangingBGMTime += fTimeDelta;

		if (m_ChangingBGMTime < 1.f)
		{
			for (_uint i = 0 ; i < BGMChannelCount; i++)
			{
				if (i !=  m_iBGMIndex)
				{
					
						
					FMOD_Channel_SetVolume(m_pChannelArr[MaxChannelCount - BGMChannelCount + i], (1 - (_float)m_ChangingBGMTime)* m_tSoundDesc[i].fTargetSound * m_VolumeArr[CHANNEL_BGM]);

				}
			}
		}
		else if (m_ChangingBGMTime < 2.f)
		{
			for (_uint i = 0; i < BGMChannelCount; i++)
			{
				if (i == m_iBGMIndex)
				{
					FMOD_Channel_SetVolume(m_pChannelArr[MaxChannelCount - BGMChannelCount + i], ((_float)m_ChangingBGMTime - 1.f) * m_tSoundDesc[i].fTargetSound * m_VolumeArr[CHANNEL_BGM]);

				}
				else
				{
					FMOD_Channel_Stop(m_pChannelArr[MaxChannelCount - BGMChannelCount + i]);
				}
			}

		}
		else
		{
			m_bChangingBGM = false;
		}

	}
	// Update One
	FMOD_System_Update(m_pSystem);
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
	return S_OK;

}

HRESULT CSoundMgr::PlayBGM(TCHAR * pSoundKey, _uint iBGMIndex ,_float fLouderMultiple)
{
	//if (BGMChannelCount <= iBGMIndex)
	//{
	//	DEBUGBREAK;
	//	return E_FAIL;
	//}

	//map<TCHAR*, FMOD_SOUND*>::iterator iter;
	//FMOD_Channel_Stop(m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)]);

	//iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	//{
	//	return !lstrcmp(pSoundKey, iter.first);
	//});

	//if (iter == m_mapSound.end())
	//{
	//	DEBUGBREAK;
	//	return E_FAIL;
	//}

	//FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)]);


	//FMOD_Channel_SetVolume(m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)], m_VolumeArr[CHANNEL_BGM] * fLouderMultiple);

	//FMOD_Channel_SetMode(m_pChannelArr[(MaxChannelCount - BGMChannelCount + iBGMIndex)], FMOD_LOOP_NORMAL);



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

	m_iBGMIndex = (m_iBGMIndex) ? 0 : 1;


	m_tSoundDesc[m_iBGMIndex].fTargetSound = fLouderMultiple;
	m_bChangingBGM = true;
	m_ChangingBGMTime = 0;

	_uint iChannelIndex = (MaxChannelCount - BGMChannelCount + m_iBGMIndex);

	FMOD_Channel_Stop(m_pChannelArr[iChannelIndex]);
	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[iChannelIndex]);

	FMOD_Channel_SetVolume(m_pChannelArr[iChannelIndex], 0);

	FMOD_Channel_SetMode(m_pChannelArr[iChannelIndex], FMOD_LOOP_NORMAL);

	FMOD_System_Update(m_pSystem);


	return S_OK;
}

HRESULT CSoundMgr::Initialize_FMOD3D()
{

	/*
	FMOD 중요 함수 정리 FMOD_System_ + @


	// 생성과 삭제
	Create_FMOD 시스템의 인스턴스를 생성합니다.
	init 시스템 개체를 초기화하고 재생을 위해 FMOD를 준비합니다.
	close 출력에 대한 연결을 닫고 개체를 해제하지 않고 초기화되지 않은 상태로 돌아갑니다.
	release 이 개체와 해당 리소스를 닫고 해제합니다.
	update FMOD 시스템을 업데이트합니다.
	mixerSuspend 믹서 스레드를 일시 중단하고 내부 상태를 유지하면서 오디오 하드웨어 사용을 포기합니다.
	mixerResume 믹서 스레드를 재개하고 오디오 하드웨어에 대한 액세스 권한을 다시 획득합니다.

	FMOD_INITFLAGS 시스템 개체를 초기화할 때 사용되는 구성 플래그입니다.

	Setting:
	setSoftwareChannels 가능한 소프트웨어 혼합 채널의 최대 수를 설정합니다.
	getSoftwareChannels 가능한 소프트웨어 혼합 채널의 최대 수를 검색합니다.
	setDSPBufferSize FMOD 소프트웨어 믹싱 엔진의 버퍼 크기를 설정합니다.
	getDSPBufferSize FMOD 소프트웨어 믹싱 엔진에 대한 버퍼 크기 설정을 검색합니다.
	setStreamBufferSize 새로 열린 스트림의 기본 파일 버퍼 크기를 설정합니다.
	getStreamBufferSize 새로 열린 스트림에 대한 기본 파일 버퍼 크기를 검색합니다.
	setAdvancedSettings 일반적으로 리소스 사용 또는 오디오 품질과 관련된 설정을 조정할 수 있도록 시스템 개체에 대한 고급 설정을 지정합니다.
	getAdvancedSettings 시스템 개체에 대한 고급 설정을 검색합니다.

	setSpeakerPosition 현재 스피커 모드에 대해 지정된 스피커의 위치를 ​​설정합니다.
	getSpeakerPosition 현재 스피커 모드에 대해 지정된 스피커의 위치를 ​​검색합니다.

	set3DSettings FMOD의 모든 3D 사운드에 대한 전역 도플러 스케일, 거리 계수 및 로그 롤오프 스케일을 설정합니다.
	get3DSettings 모든 3D 사운드에 대한 전역 도플러 스케일, 거리 계수 및 롤오프 스케일을 검색합니다.
	set3DNumListeners 3D 사운드 장면에서 3D '청취자'의 수를 설정합니다.
	get3DNumListeners 3D 수신기의 수를 검색합니다.
	set3DRolloffCallback 거리 감쇠의 사용자 정의 계산을 허용하도록 콜백을 설정합니다.

	FMOD_ADVANCEDSETTINGS 고급 구성 설정.
	FMOD_3D_ROLLOFF_CALLBACK 거리 감쇠의 사용자 정의 계산을 허용하는 콜백.
	FMOD_DSP_RESAMPLER 리샘플링에 사용되는 보간 유형 목록입니다.

	3D 기능.
	FMOD_RESULT F_API FMOD_Channel_Set3DAttributes(FMOD_CHANNEL *channel, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel);
	FMOD_RESULT F_API FMOD_Channel_Get3DAttributes(FMOD_CHANNEL *channel, FMOD_VECTOR *pos, FMOD_VECTOR *vel);
	FMOD_RESULT F_API FMOD_Channel_Set3DMinMaxDistance(FMOD_CHANNEL *channel, float mindistance, float maxdistance);
	FMOD_RESULT F_API FMOD_Channel_Get3DMinMaxDistance(FMOD_CHANNEL *channel, float *mindistance, float *maxdistance);
	FMOD_RESULT F_API FMOD_Channel_Set3DConeSettings(FMOD_CHANNEL *channel, float insideconeangle, float outsideconeangle, float outsidevolume);
	FMOD_RESULT F_API FMOD_Channel_Get3DConeSettings(FMOD_CHANNEL *channel, float *insideconeangle, float *outsideconeangle, float *outsidevolume);
	FMOD_RESULT F_API FMOD_Channel_Set3DConeOrientation(FMOD_CHANNEL *channel, FMOD_VECTOR *orientation);
	FMOD_RESULT F_API FMOD_Channel_Get3DConeOrientation(FMOD_CHANNEL *channel, FMOD_VECTOR *orientation);
	FMOD_RESULT F_API FMOD_Channel_Set3DCustomRolloff(FMOD_CHANNEL *channel, FMOD_VECTOR *points, int numpoints);
	FMOD_RESULT F_API FMOD_Channel_Get3DCustomRolloff(FMOD_CHANNEL *channel, FMOD_VECTOR **points, int *numpoints);
	FMOD_RESULT F_API FMOD_Channel_Set3DOcclusion(FMOD_CHANNEL *channel, float directocclusion, float reverbocclusion);
	FMOD_RESULT F_API FMOD_Channel_Get3DOcclusion(FMOD_CHANNEL *channel, float *directocclusion, float *reverbocclusion);
	FMOD_RESULT F_API FMOD_Channel_Set3DSpread(FMOD_CHANNEL *channel, float angle);
	FMOD_RESULT F_API FMOD_Channel_Get3DSpread(FMOD_CHANNEL *channel, float *angle);
	FMOD_RESULT F_API FMOD_Channel_Set3DLevel(FMOD_CHANNEL *channel, float level);
	FMOD_RESULT F_API FMOD_Channel_Get3DLevel(FMOD_CHANNEL *channel, float *level);
	FMOD_RESULT F_API FMOD_Channel_Set3DDopplerLevel(FMOD_CHANNEL *channel, float level);
	FMOD_RESULT F_API FMOD_Channel_Get3DDopplerLevel(FMOD_CHANNEL *channel, float *level);
	FMOD_RESULT F_API FMOD_Channel_Set3DDistanceFilter(FMOD_CHANNEL *channel, FMOD_BOOL custom, float customLevel, float centerFreq);
	FMOD_RESULT F_API FMOD_Channel_Get3DDistanceFilter(FMOD_CHANNEL *channel, FMOD_BOOL *custom, float *customLevel, float *centerFreq);

	SoundCreate:
	createSound 사운드를 메모리에 로드하고 스트리밍을 위해 열거나 콜백 기반 사운드용으로 설정합니다.
	createStream 스트리밍할 사운드를 엽니다.
	createDSP 플러그인 설명 구조가 주어지면 DSP 객체를 생성합니다.
	createDSPByType 내장형 인덱스가 주어지면 DSP 객체를 생성합니다.
	createChannelGroup ChannelGroup 객체를 생성합니다.
	createSoundGroup SoundGroup 개체를 만듭니다.
	createReverb3D '가상 반향' 개체를 만듭니다. 이 개체는 3D 위치에 반응하고 잔향 개체의 중심에 얼마나 가까운지에 따라 잔향 환경을 변형합니다.
	playSound 채널에서 사운드를 재생합니다.
	playDSP 채널의 입력과 함께 DSP를 재생합니다.
	getChannel ID로 채널에 대한 핸들을 검색합니다.
	getMasterChannelGroup 모든 사운드가 궁극적으로 라우팅되는 마스터 ChannelGroup을 검색합니다.
	getMasterSoundGroup 모든 사운드가 생성될 때 배치되는 기본 SoundGroup을 검색합니다.

	정보보기:
	getVersion FMOD 버전 번호를 검색합니다.
	getOutputHandle 출력 유형별 내부 기본 인터페이스를 검색합니다.
	getChannelsPlaying 현재 재생 중인 채널의 수를 검색합니다.
	getCPUUsage CPU 시간의 백분율로 검색합니다. FMOD가 스트리밍/믹싱 및 업데이트를 합친 데 사용하는 CPU 사용량입니다.
	getFileUsage 파일 읽기에 대한 정보를 검색합니다.
	getDefaultMixMatrix 한 스피커 모드에서 다른 스피커 모드로 변환하는 데 사용되는 기본 매트릭스를 검색합니다.
	getSpeakerModeChannels 주어진 스피커 모드에 대한 채널 수를 검색합니다.



	Runtime:
	set3DListenerAttributes 지정된 3D 사운드 리스너의 위치, 속도 및 방향을 설정합니다.
	get3DListenerAttributes 지정된 3D 사운드 리스너의 위치, 속도 및 방향을 검색합니다.
	setReverbProperties 전역 리버브 환경에 대한 매개변수를 설정합니다.
	getReverbProperties 지정된 잔향 인스턴스에 대한 현재 잔향 환경을 검색합니다.
	attachChannelGroupToPort 지정된 ChannelGroup의 출력을 출력 드라이버의 오디오 포트에 연결합니다.
	detachChannelGroupFromPort 출력 드라이버의 오디오 포트에서 지정된 ChannelGroup의 출력을 분리합니다.

	FMOD_REVERB_PROPERTIES 리버브 환경을 정의하는 구조.
	FMOD_PORT_INDEX 포트 유형의 인스턴스가 여러 개인 경우에 대한 출력 유형별 인덱스입니다.
	FMOD_PORT_TYPE 플랫폼별 포트 열거형의 기본 유형입니다.
	FMOD_REVERB_MAXINSTANCES 글로벌/물리적 리버브 인스턴스의 최대 수.
	FMOD_REVERB_PRESETS 사전 정의된 리버브 구성. 사용을 단순화하고 리버브 매개변수를 수동으로 선택하지 않도록 하기 위해 공통 사전 설정 테이블이 제공되어 사용이 간편합니다.


	Gerneal:
	lockDSP FMOD DSP 엔진(다른 스레드에서 비동기식으로 실행)을 잠그어 실행되지 않도록 하는 상호 배제 기능.
	unlockDSP FMOD DSP 엔진(다른 스레드에서 비동기식으로 실행)을 잠금 해제하고 계속 실행하도록 하는 상호 배제 기능.
	setCallback 시스템 수준 알림에 대한 콜백을 설정합니다.
	setUserData System 개체와 연결된 사용자 값을 설정합니다.
	getUserData System 개체와 연결된 사용자 값을 검색합니다.

	FMOD_ERRORCALLBACK_INFO 발생한 오류를 설명하는 정보입니다.
	FMOD_SYSTEM_CALLBACK 시스템 알림에 대한 콜백입니다.
	FMOD_ERRORCALLBACK_INSTANCETYPE 오류 콜백에서 다양한 유형의 인스턴스를 나타내는 데 사용되는 식별자입니다.
	FMOD_SYSTEM_CALLBACK_TYPE 시스템에서 호출하는 콜백 유형입니다.


	장치 선택: // 사운드 출력시 이용(자동선택됨)
	setOutput 믹서를 실행하는 데 사용되는 출력 인터페이스 유형을 설정합니다.
	getOutput 믹서를 실행하는 데 사용되는 출력 인터페이스의 유형을 검색합니다.
	getNumDrivers 선택한 출력 유형에 사용할 수 있는 출력 드라이버의 수를 검색합니다.
	getDriverInfo 인덱스로 지정된 사운드 장치에 대한 식별 정보를 검색하고 선택한 출력 모드에 고유합니다.
	setDriver 선택한 출력 유형에 대한 출력 드라이버를 설정합니다.
	getDriver 선택한 출력 유형에 대한 출력 드라이버를 검색합니다.

	FMOD_OUTPUTTYPE 믹서를 실행하는 데 사용할 수 있는 내장 출력 유형입니다.

	*/



	// m_pSystem
	int devbug = 5;
	// 3D 스케일 리스너 설정
	FMOD_System_Set3DSettings(m_pSystem, 1, 1, 1);
	FMOD_System_Set3DNumListeners(m_pSystem, 1);



//	FMOD_Sound_Get3DMinMaxDistance(FMOD_SOUND *sound, float *min, float *max);


//	FMOD_RESULT R = FMOD_System_SetSoftwareFormat(m_pSystem, 48000, FMOD_SPEAKERMODE_STEREO, 0);

	
	// FMOD_System_SetSpeakerPosition

	//	set3DListenerAttributes
	//	get3DListenerAttributes
	//	setReverbProperties
	//	createDSP
	//	playSound 
	//	playDSP 

	
//	createReverb3D

	// 일정 거리로 떨어지면 콜백 발생
//	FMOD_System_SetCallback(m_pSystem, Callback_3DTest, FMOD_SYSTEM_CALLBACK_POSTUPDATE);
	


	return S_OK;
}




HRESULT CSoundMgr::Setup_Listender_Camera(_fMatrix CameraMat, int listenerIndex)
{
	// 귀 업데이트
	FMOD_System_Set3DNumListeners(m_pSystem, listenerIndex);

	FMOD_VECTOR up = *(FMOD_VECTOR*)&CameraMat.r[1];
	FMOD_VECTOR foward = *(FMOD_VECTOR*)&CameraMat.r[2];
	FMOD_VECTOR pos = *(FMOD_VECTOR*)&CameraMat.r[3];
	FMOD_VECTOR vel = { 0,0,0 };
	
	mCamPostiotn = _float3(pos.x, pos.y, pos.z);

	FMOD_System_Set3DListenerAttributes(m_pSystem, 0, &pos, &vel, &foward, &up);

	return S_OK;
}



HRESULT CSoundMgr::Play3D_Sound(TCHAR * pSoundKey, _float3 Pos, CHANNELID eID, _float fLouderMultiple, _float3 dir, _float speed)
{

	// 방향과 속도를 주면 해당 방향의 속도 구현 가능

	FMOD_RESULT r;

	FMOD_VECTOR		Dir = { dir.x*speed ,dir.y*speed ,dir.z*speed };
	FMOD_VECTOR		Vec = { Pos.x,Pos.y,Pos.z };
	FMOD_CHANNEL*	Chanel = nullptr;

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


			Chanel = m_pChannelArr[i];
			r = FMOD_Channel_Set3DAttributes(Chanel, &Vec, &Dir);
			if (r != FMOD_OK)
				return E_FAIL;

			FMOD_Channel_SetVolume(m_pChannelArr[i], m_VolumeArr[eID] * fLouderMultiple);
			m_fPassedTimeArr[i] = 0.1f;

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

	Chanel = m_pChannelArr[fOldSoundIndx];
	r = FMOD_Channel_Set3DAttributes(Chanel, &Vec, &Dir);
	if (r != FMOD_OK)
		return E_FAIL;

	FMOD_Channel_SetVolume(m_pChannelArr[fOldSoundIndx], m_VolumeArr[eID] * fLouderMultiple);

	m_fPassedTimeArr[fOldSoundIndx] = 0.1f;

	return S_OK;
}

HRESULT CSoundMgr::Play3D_Sound_IsPlay(TCHAR * pSoundKey, _float3 Pos, CHANNELID eID, _float fLouderMultiple, _float3 dir, _float speed, _bool* playing)
{

	// 방향과 속도를 주면 해당 방향의 속도 구현 가능

	//FMOD_RESULT r;

	//FMOD_VECTOR		Dir = { dir.x*speed ,dir.y*speed ,dir.z*speed };
	//FMOD_VECTOR		Vec = { Pos.x,Pos.y,Pos.z };
	//FMOD_CHANNEL*	Chanel = nullptr;

	//if (eID == CHANNEL_BGM)
	//{
	//	__debugbreak();
	//	return E_FAIL;
	//}


	//map<TCHAR*, FMOD_SOUND*>::iterator iter;

	//iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	//{
	//	return !lstrcmp(pSoundKey, iter.first);
	//});

	//if (iter == m_mapSound.end())
	//{
	//	__debugbreak();
	//	return E_FAIL;
	//}

	//FMOD_BOOL bPlay = FALSE;
	//_uint fOldSoundIndx = -1;
	//_float fOldestTime = -1.f;

	//for (_uint i = eID * m_iNumOfEachChannel; i < (eID + 1) * m_iNumOfEachChannel; i++)
	//{
	//	if (FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay))
	//	if (m_fPassedTimeArr[i] == 0)
	//	{
	//		FMOD_Channel_Stop(m_pChannelArr[i]);
	//		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[i]);


	//		Chanel = m_pChannelArr[i];
	//		r = FMOD_Channel_Set3DAttributes(Chanel, &Vec, &Dir);
	//		if (r != FMOD_OK)
	//			return E_FAIL;

	//		FMOD_Channel_SetVolume(m_pChannelArr[i], m_VolumeArr[eID] * fLouderMultiple);
	//		m_fPassedTimeArr[i] = 0.1f;

	//		return S_OK;
	//	}

	//	if (m_fPassedTimeArr[i] > fOldestTime)
	//	{
	//		fOldestTime = m_fPassedTimeArr[i];
	//		fOldSoundIndx = i;
	//	}

	//}


	//FMOD_Channel_Stop(m_pChannelArr[fOldSoundIndx]);

	//FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[fOldSoundIndx]);

	//Chanel = m_pChannelArr[fOldSoundIndx];
	//r = FMOD_Channel_Set3DAttributes(Chanel, &Vec, &Dir);
	//if (r != FMOD_OK)
	//	return E_FAIL;

	//FMOD_Channel_SetVolume(m_pChannelArr[fOldSoundIndx], m_VolumeArr[eID] * fLouderMultiple);

	//m_fPassedTimeArr[fOldSoundIndx] = 0.1f;

	return S_OK;
}




HRESULT CSoundMgr::Set_3DSound_Distance_World(_float rolloffscale)
{
	// roll 값만 적용
	// rolloffscale 거리에 따른 감쇠 값 
	// 모든 사운드는 MIN = 0 / MAX = 100으로 고정하고 감쇠값만 사용해서 조절

	_float3 Value(0, 0, rolloffscale);

	FMOD_System_Get3DSettings(m_pSystem, &Value.x, &Value.y,nullptr);

	FMOD_System_Set3DSettings(m_pSystem, Value.x, Value.y, Value.z);
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
	for (_uint i = 0; i < MaxChannelCount - BGMChannelCount; ++i) 
	{

		FMOD_Channel_Stop(m_pChannelArr[i]);
		m_fPassedTimeArr[i] = 0;
	}

	m_bChangingBGM = true;
	m_ChangingBGMTime = 0;
	m_iBGMIndex = (m_iBGMIndex) ? 0 : 1;
}

_float CSoundMgr::Get_Channel_Volume(CHANNELID eID)
{
	return m_VolumeArr[eID];
}

_bool CSoundMgr::Get_Channel_IsPaused(CHANNELID eID)
{
	return m_PauseArr[eID];
}

HRESULT CSoundMgr::LoadSoundFile_3D()
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

	//	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT | FMOD_3D_LINEARROLLOFF, 0, &pSound);
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_3D | FMOD_3D_LINEARROLLOFF, 0, &pSound);
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
//	FMOD_System_Update(m_pSystem);
	_findclose(handle);

	for (auto sound: m_mapSound)
	{
		FMOD_Sound_Set3DMinMaxDistance(sound.second, 0, 100);
	}
	

	return S_OK;
}

HRESULT CSoundMgr::LoadSoundFile_2D()
{
	_tfinddata64_t fd;
	__int64 handle = _tfindfirst64(L"../Bin/Resources/SoundFiles_2D/*.*", &fd);

	if (handle == -1 || handle == 0)
	{
		__debugbreak();
		return E_FAIL;
	}

	_int iResult = 0;

	char szCurPath[128] = "../Bin/Resources/SoundFiles_2D/";
	char szFullPath[128] = "";
	char szFilename[MAX_PATH];
	while (iResult != -1)
	{
		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, szFilename);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		// FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_3D | FMOD_3D_LINEARROLLOFF, 0, &pSound);
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
	//	FMOD_System_Update(m_pSystem);
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
