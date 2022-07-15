#pragma once
#include "Base.h"

BEGIN(Engine)

class CSoundMgr :public CBase
{
	DECLARE_SINGLETON(CSoundMgr);

#define MaxChannelCount 10
#define BGMChannelCount	2

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	enum E_SOUNDMODE
	{
		SOUNDMODE_NOMAL,
		SOUNDMODE_3D,
		SOUNDMODE_END,
	};
public:
	HRESULT Initialize_FMOD();
	HRESULT Initialize_FMOD3D();
	HRESULT Update_FMOD(_float fTimeDelta);

public:
	_int  Set_ChannelVolume(CHANNELID eID, _float _vol);
	_int  Pause_ChannelSound(CHANNELID eID);

	HRESULT PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float fLouderMultiple = 1.f);
	HRESULT PlayBGM(TCHAR* pSoundKey,_uint iBGMIndex = 0 ,_float fLouderMultiple = 1.f);

	// 3D SoundFunc
	HRESULT Play3D_Sound(TCHAR* pSoundKey, _float3 Pos, CHANNELID eID, _float fLouderMultiple = 1.f);
	HRESULT Set_3DSound_Distance_World(_float rolloffscale);

	void	Stop_ChannelSound(CHANNELID eID);
	void	Stop_AllChannel();

	_float Get_Channel_Volume(CHANNELID eID);
	_bool  Get_Channel_IsPaused(CHANNELID eID);

	FMOD_SYSTEM*	Get_SOUNDSYSTEM()
	{
		return m_pSystem;
	}

private:
	HRESULT Setup_Listender_Camera(_fMatrix CameraMat, int listenerIndex = 0);


private:
	_float				m_fPassedTimeArr[MaxChannelCount];
	_float				m_VolumeArr[CHANNEL_MAXCHANNEL];
	_bool				m_PauseArr[CHANNEL_MAXCHANNEL];

	_uint				m_Verson;
	E_SOUNDMODE			me_SoundMode = SOUNDMODE_END;

	_float3				mCamPostiotn = _float3(0,0,0);


private:
	HRESULT LoadSoundFile();

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MaxChannelCount];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM*	m_pSystem;
	const _uint		m_iNumOfEachChannel;
public:
	virtual void Free() override;
};


END

