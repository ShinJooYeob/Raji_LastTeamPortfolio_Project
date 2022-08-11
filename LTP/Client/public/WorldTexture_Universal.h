#pragma once

#include "GameObject.h"
#include "Camera_Main.h"

BEGIN(Client)

class CWorldTexture_Universal : public CGameObject
{
public:
	enum WorldTexture_UniversalType {
		ALPABET,
		WORLDTEXTURE_UNIVERSAL_END
	};
public:
	typedef struct tagWorldTexture_UniversalDesc {
		_uint		iWorldTextureNumber;

		_float3		fScale;
		_float3		fPositioning;

		_double		dDuration;

		void*		pObject = nullptr;

		_bool		bBillboardOn = false;
		_bool		bMagnetOn = false;
		_bool*		pSwitch = nullptr;
	}WORLDTEXTURE_UNIVERSALDESC;
private:
	explicit CWorldTexture_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWorldTexture_Universal(const CWorldTexture_Universal& rhs);
	virtual ~CWorldTexture_Universal() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_Info();

private:
	HRESULT	Billboard();
	HRESULT Magnet();

private:
	HRESULT PlayOn(_double dDeltaTime);
private:
	HRESULT Alpabet(_double dDeltaTime);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CCamera_Main*		m_pCamera = nullptr;
	CTransform*			m_pCameraTransform = nullptr;
	CTransform*			m_pObjectTransform = nullptr;

private:
	WORLDTEXTURE_UNIVERSALDESC		m_WorldTexture_UniversalDesc;

	_double				m_dDurationTime = 0;

	


public:
	static CWorldTexture_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END