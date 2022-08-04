#include "stdafx.h"
#include "..\public\Gear_Puzzle.h"


CGear_Puzzle::CGear_Puzzle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CGear_Puzzle::CGear_Puzzle(const CGear_Puzzle & rhs)
	: CMapObject(rhs)
{
}

HRESULT CGear_Puzzle::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	ZeroMemory(&m_pGear, sizeof(CGear*) * CGear::GEAR_END);

	return S_OK;
}

HRESULT CGear_Puzzle::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_pGear, sizeof(CGear*) * CGear::GEAR_END);

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));



	SetUp_ColliderPos();

	SetUp_Gear();

	//m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DEBUGCOLLIDER, true);

	m_pGear[0]->Set_TurnOn(true);
	return S_OK;
}

_int CGear_Puzzle::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)
		return -1;

	if (m_bTriggerOn == true)
	{
		KeyboardInput(dDeltaTime);
	}

	Update_Collider(dDeltaTime);

	for (_uint i = 0; i < CGear::GEAR_END; i++)
	{
		m_pGear[i]->Update(dDeltaTime);
	}

	return _int();
}

_int CGear_Puzzle::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)
		return -1;



	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}


	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));

	for (_uint i = 0; i < CGear::GEAR_END; i++)
	{
		m_pGear[i]->LateUpdate(dDeltaTime);
	}

	return _int();
}

_int CGear_Puzzle::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));



	FAILED_CHECK(__super::SetUp_ConstTable(m_pShaderCom));


	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i));
	}

	for (_uint i = 0; i < CGear::GEAR_END; i++)
	{
		m_pGear[i]->Render();
	}

	return 0;
}

_int CGear_Puzzle::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	for (_uint i = 0; i < CGear::GEAR_END; i++)
	{
		m_pGear[i]->LateRender();
	}

	return _int();
}
HRESULT CGear_Puzzle::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_PuzzleAssembly_Right_01), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	SetUp_Collider();

	m_fFrustumRadius = 27.f;

	return S_OK;
}

HRESULT CGear_Puzzle::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	//기어 조작대에 있는 콜라이더는 X가 뒤 -X가 앞 Z는 왼쪽, -Z는 오른쪽
	COLLIDERDESC			ColliderDesc;
	//Index == 0;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(30.f, 30.f, 30.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));


	//스타트 기어
	//============================================
	//Index == 1;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-5.5f, -11.5f, -28.251f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	//============================================

	//첫번째 기어
	//===================================================================================
	//Index == 2;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -17.8f, 6.422f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 3;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -17.8f, -18.536f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	//Index == 4;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -9.5f, -18.536f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	//====================================================================================

	//두번째 기어
	//====================================================================================
	//Index == 5;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -14.f, -13.2f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	//Index == 6;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -3.7f, -13.2f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	//Index == 7;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, 1.f, -21.303f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	//====================================================================================

	//세번째 기어
	//====================================================================================
	//Index == 8;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, 11.2f, -25.171f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 9;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, 2.8f, -7.839f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 10;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -11.f, -7.839f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	//====================================================================================

	//네번째 기어
	//====================================================================================
	//Index == 11;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-8.f, 24.3f, -4.397f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 12;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, 19.3f, 2.083f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//Index == 13;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -11.4f, 2.083f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();

	//====================================================================================

	//====================================================================================
	//다섯번째 기어
	//Index == 14;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-8.f, 24.2f, 26.815f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	//Index == 15;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, 16.f, 14.446f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();


	//Index == 16;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-6.5f, -10.9f, 14.446f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	//====================================================================================


	//엔드 기어
	//============================================
	//Index == 17;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(-7.f, -11.5f, 30.175f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pColliderCom->Set_ParantBuffer();
	//============================================

	return S_OK;
}

HRESULT CGear_Puzzle::SetUp_ColliderPos()
{
	_uint iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0 ; i < iNumCollider; i++)
	{
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
		_float3 fColliderPos = m_pColliderCom->Get_ColliderPosition(i);
		m_vecColliderPos.push_back(fColliderPos);
	}
	//벡터를 배열처럼 사용하여 구현하자
	return S_OK;
}

HRESULT CGear_Puzzle::SetUp_Gear()
{
	CGear::GEAR_STATEDESC GearDesc;

	//////////////스타트 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_1;
	GearDesc.fScale = _float3(0.5f, 0.5f, 0.5f);
	GearDesc.fPos = m_vecColliderPos[1];
	GearDesc.iTurnDirection = 0;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_1]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear), &GearDesc);
	/////////////////////

	//////////////첫번째 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_2;
	GearDesc.fScale = _float3(1.f, 1.f, 1.f);
	GearDesc.fPos = m_vecColliderPos[2];
	GearDesc.iTurnDirection = 1;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_2]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear),&GearDesc);
	/////////////////////


	//////////////두번째 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_3;
	GearDesc.fScale = _float3(1.f, 1.f, 1.f);
	GearDesc.fPos = m_vecColliderPos[5];
	GearDesc.iTurnDirection = 0;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_3]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear), &GearDesc);
	/////////////////////


	//////////////세번째 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_4;
	GearDesc.fScale = _float3(1.f, 1.f, 1.f);
	GearDesc.fPos = m_vecColliderPos[8];
	GearDesc.iTurnDirection = 1;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_4]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear), &GearDesc);
	/////////////////////


	//////////////네번째 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_5;
	GearDesc.fScale = _float3(1.f, 1.f, 1.f);
	GearDesc.fPos = m_vecColliderPos[11];
	GearDesc.iTurnDirection = 0;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_5]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear), &GearDesc);
	/////////////////////


	//////////////다섯번째 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_6;
	GearDesc.fScale = _float3(1.f, 1.f, 1.f);
	GearDesc.fPos = m_vecColliderPos[14];
	GearDesc.iTurnDirection = 1;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_6]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear), &GearDesc);
	/////////////////////


	//////////////엔드 기어
	ZeroMemory(&GearDesc, sizeof(CGear::GEAR_STATEDESC));
	GearDesc.iGearTypeNumber = CGear::GEARTYPE_7;
	GearDesc.fScale = _float3(0.5f, 0.5f, 0.5f);
	GearDesc.fPos = m_vecColliderPos[17];
	GearDesc.iTurnDirection = 0;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pGear[CGear::GEARTYPE_7]), m_eNowSceneNum, TAG_OP(Prototype_Object_Map_Gear), &GearDesc);
	/////////////////////

	return S_OK;
}

HRESULT CGear_Puzzle::Update_Collider(_double dDeltaTime)
{
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);

	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	_uint iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
	{
		m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
	}
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pColliderCom));

	return S_OK;
}

HRESULT CGear_Puzzle::KeyboardInput(_double dDeltaTime)
{
	if (g_pGameInstance->Get_DIKeyState(DIK_Y) & DIS_Down)
	{
		if (m_GearNumber > 1)
		{
			m_pGear[m_GearNumber]->Set_LimLight_N_Emissive();
			m_GearNumber -= 1;
			m_pGear[m_GearNumber]->Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1, 0.5f, 1, 1));
		}
		else {
			m_GearNumber = 1;
			m_pGear[m_GearNumber]->Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1, 0.5f, 1, 1));
		}
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_H) & DIS_Down)
	{
		if (m_GearNumber < 5)
		{
			m_pGear[m_GearNumber]->Set_LimLight_N_Emissive();
			m_GearNumber += 1;
			m_pGear[m_GearNumber]->Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1, 0.5f, 1, 1));
		}
		else {
			m_GearNumber = 5;
			m_pGear[m_GearNumber]->Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1, 0.5f, 1, 1));
		}
	}

	switch (m_GearNumber)
	{
	case CGear::GEARTYPE_2:
	{
		_Vector LeftPoint, RightPoint;
		LeftPoint = XMLoadFloat3(&m_vecColliderPos[2]);
		RightPoint = XMLoadFloat3(&m_vecColliderPos[3]);

		_float3 fGearPos;

		fGearPos = m_pGear[m_GearNumber]->Get_Transform()->Get_MatrixState_Float3(CTransform::STATE_POS);

		_Vector vDir;
		vDir = XMVector3Normalize(RightPoint - LeftPoint);

		if (g_pGameInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
		{
			if (m_vecColliderPos[3].z >= fGearPos.z)
			{
				m_pGear[m_GearNumber]->Get_Transform()->MovetoDir(-vDir, dDeltaTime);
			}
		}

		if (g_pGameInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press) //오른쪽으로 가니깐 오른쪽에 있는 인덱스 3기준으로 해야함 병합 후 바꾸자
		{
			if (m_vecColliderPos[2].z <= fGearPos.z)
			{
				m_pGear[m_GearNumber]->Get_Transform()->MovetoDir(vDir, dDeltaTime);
			}
		}
		break;
	}
	default:
		break;
	}
	
	return S_OK;
}

CGear_Puzzle * CGear_Puzzle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGear_Puzzle*	pInstance = NEW CGear_Puzzle(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGear_Puzzle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGear_Puzzle::Clone(void * pArg)
{
	CGear_Puzzle*	pInstance = NEW CGear_Puzzle(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CGear_Puzzle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGear_Puzzle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);

	Safe_Release(m_pColliderCom);
	for (_uint i = 0; i < CGear::GEAR_END; i++)
	{
		Safe_Release(m_pGear[i]);
	}
}