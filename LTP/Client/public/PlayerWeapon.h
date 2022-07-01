#pragma once

#include "GameObject.h"

BEGIN(Client)

class CPlayerWeapon abstract : public CGameObject
{
public:
	enum EWeaponState { STATE_STRUCTURE, STATE_EQUIP, STATE_NON_EQUIP, STATE_END };

	typedef struct tagPlayerWeaponDesc
	{
		EWeaponState	eWeaponState;
		ATTACHEDESC		eAttachedDesc;
	}PlayerWeaponDesc;

protected:
	CPlayerWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayerWeapon(const CPlayerWeapon& rhs);
	virtual ~CPlayerWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	virtual _int Update(_double fDeltaTime) override;
	virtual _int LateUpdate(_double fDeltaTimer) override;
	virtual _int Render() override;
	virtual _int LateRender() override;

private:
	virtual _fVector Get_BonePos(const char* pBoneName) override;
	virtual void Update_AttachCamPos() override;

protected:
	PlayerWeaponDesc			m_tPlayerWeaponDesc;

public:
	virtual CGameObject*		Clone(void* pArg)PURE;
	virtual void				Free() override;
};
END
