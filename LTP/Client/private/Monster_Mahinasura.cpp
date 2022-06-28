#include "stdafx.h"
#include "..\public\Monster_Mahinasura.h"


CMonster_Mahinasura::CMonster_Mahinasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMonster_Mahinasura::CMonster_Mahinasura(const CMonster_Mahinasura & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Mahinasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Mahinasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	return S_OK;
}

_int CMonster_Mahinasura::Update(_double fDeltaTime)
{

	if (__super::Update(fDeltaTime) < 0)return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_Z) & DIS_Down)
	{
		m_iTest -= 1;

		if (m_iTest < 0)
			m_iTest = 22;
	}
	if (pGameInstance->Get_DIKeyState(DIK_X) & DIS_Down)
	{
		m_iTest += 1;

		if (m_iTest > 22)
			m_iTest = 0;
	}
	RELEASE_INSTANCE(CGameInstance);

	m_pModel->Change_AnimIndex(m_iTest);

	//������ ������ bBlockAnimUntilReturnChange���� true�� �����ؼ� �����۵��� �ȴٸ� false�� �ȴ�.
	//m_pModel->Change_AnimIndex();
	//m_pModel->Change_AnimIndex_ReturnTo(); //� �ִϸ��̼��� ������ Ư�� �ִϸ��̼����� ����
	//m_pModel->Change_AnimIndex_ReturnTo_Must(); //�߰��� �ִϸ��̼��� ĵ���ϰ� �ٸ� �ִϸ��̼��� ���۽�Ŵ
	//m_pModel->Change_AnimIndex_UntilTo(); //1~5���� �����ٰ� ���ɾ �Է��ϸ� 1~5���� ���ư�
	//m_pModel->Change_AnimIndex_UntilNReturn();//1~5���� ������ �� ���� Ư�� �ִϸ��̼����� Return ��ų ���
	//m_pModel->Change_AnimIndex_UntilNReturn_Must();


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));


	return _int();
}

_int CMonster_Mahinasura::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	//////////
	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////

	//FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	return _int();
}

_int CMonster_Mahinasura::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 1, i, "g_BoneMatrices"));
	}



	return _int();
}

_int CMonster_Mahinasura::LateRender()
{
	return _int();
}



HRESULT CMonster_Mahinasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Mahinasura_Minion), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CMonster_Mahinasura::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 1://�ִϸ��̼� �ε������� ����ָ� ��
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // �̷��� �Ǹ� ���� �ִϸ��̼ǿ��� �����Ǵ� �ð� ������ ���� �ٷ� ����
			{

				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //Ư�� ������ �÷��� ����Ʈ���Ŀ� ���������
			{


				m_iAdjMovedIndex++;
			}

			break;
		case 2:

			break;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Mahinasura * CMonster_Mahinasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Mahinasura*	pInstance = new CMonster_Mahinasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Mahinasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Mahinasura::Clone(void * pArg)
{
	CMonster_Mahinasura*	pInstance = new CMonster_Mahinasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Mahinasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Mahinasura::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}