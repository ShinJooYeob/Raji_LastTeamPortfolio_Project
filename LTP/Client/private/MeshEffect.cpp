#include "stdafx.h"
#include "..\public\MeshEffect.h"


CMeshEffect::CMeshEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMeshEffect::CMeshEffect(const CMeshEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMeshEffect::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	return S_OK;
}

HRESULT CMeshEffect::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));


	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&m_tInstanceDesc, pArg, sizeof(INSTMESHDESC));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(Ready_InstanceDesc());
	FAILED_CHECK(Initialize_Child_Clone());


	Safe_AddRef(m_tInstanceDesc.FollowingTarget);

	Set_IsOcllusion(m_tInstanceDesc.bIsOclusion);

	return S_OK;
}

_int CMeshEffect::Update(_double TimeDelta)
{
	if (0 > __super::Update(TimeDelta))
		return -1;
	Update_ParticleAttribute(TimeDelta);


	return _int();
}

_int CMeshEffect::LateUpdate(_double TimeDelta)
{
	if (0 > __super::LateUpdate(TimeDelta))
		return -1;
	

	m_vecWorld.clear();
	m_vecWorld.swap(vector<_float4x4>());
	m_vecLimLight.clear();
	m_vecLimLight.swap(vector<_float4>());
	m_vecEmisive.clear();
	m_vecEmisive.swap(vector<_float4>());
	m_vecTimer.clear();
	m_vecTimer.swap(vector<_float4>());

	m_vecWorld.reserve(m_iNumInstance);
	m_vecLimLight.reserve(m_iNumInstance);
	m_vecEmisive.reserve(m_iNumInstance);
	m_vecTimer.reserve(m_iNumInstance);

	_Matrix mat;

	for (_uint i = 0; i < m_iNumInstance; i++)
	{
		if (m_vecParticleAttribute[i]._age <= 0 || !m_vecParticleAttribute[i]._isAlive) 
			continue;

		mat = m_vecParticleAttribute[i]._LocalMatirx.XMatrix();
		mat.r[3] = mat.r[3] + m_vecParticleAttribute[i]._TargetParentPosition.XMVector();


		if (m_tInstanceDesc.bEmissive)
			m_vecEmisive.push_back(_float4(m_tInstanceDesc.vEmissive_SBB, 1));

		if (m_tInstanceDesc.ePassID >= MeshPass_MaskingNoising && m_tInstanceDesc.ePassID <= MeshPass_MaskingNoising_Appear_Bright
			|| m_tInstanceDesc.ePassID >= MeshPass_AllDistortion && m_tInstanceDesc.ePassID <= MeshPass_Distortion_ColorMix_Bright)
			m_vecTimer.push_back(_float4(m_vecParticleAttribute[i]._age, m_vecParticleAttribute[i]._lifeTime, (_float)TimeDelta, 1));


		m_vecLimLight.push_back(m_vecParticleAttribute[i]._color);
		m_vecWorld.push_back(mat);
	}

	if (m_pModel->Get_IsAnimModel())
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, nullptr, m_pModelInstance, m_pShaderCom, m_pModel, &m_vecWorld ));

	}
	else
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE, this, nullptr, m_pModelInstance, m_pShaderCom, m_pModel, &m_vecWorld));
	}

	if (m_tInstanceDesc.ePassID >= MeshPass_MaskingNoising && m_tInstanceDesc.ePassID <= MeshPass_MaskingNoising_Appear_Bright)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_AFTERALLOBJ, this));
	}
	else if (m_tInstanceDesc.ePassID >= MeshPass_AllDistortion && m_tInstanceDesc.ePassID <= MeshPass_Distortion_ColorMix_Bright)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this));
	}
	else
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	}
	   
	return _int();
}

_int CMeshEffect::Render()
{
	FAILED_CHECK(SetUp_ConstantTable());

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	vector<_float4>* vecEmissive = nullptr;
	vector<_float4>* vecTimer = nullptr;

	if (m_tInstanceDesc.bEmissive)
		vecEmissive = &m_vecEmisive;

	if (m_tInstanceDesc.ePassID >= MeshPass_MaskingNoising && m_tInstanceDesc.ePassID <= MeshPass_MaskingNoising_Appear_Bright
		|| m_tInstanceDesc.ePassID >= MeshPass_AllDistortion && m_tInstanceDesc.ePassID <= MeshPass_Distortion_ColorMix_Bright)
		vecTimer = &m_vecTimer;


	

	FAILED_CHECK(m_pModelInstance->Render_By_float4x4(m_pShaderCom, m_tInstanceDesc.ePassID + 1, &m_vecWorld, 0, &m_vecLimLight, vecEmissive, vecTimer));


	




	return _int();
}

_int CMeshEffect::LateRender()
{
	return _int();
}

HRESULT CMeshEffect::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));


	FAILED_CHECK(Add_Component(SCENE_STATIC, m_tInstanceDesc.szModelMeshProtoTypeTag, TAG_COM(Com_Model), (CComponent**)&m_pModel));

	if (m_pModel->Get_IsAnimModel())
	{
		MSGBOX("Do Not Input Anim Model to Particle Effect");
		__debugbreak();


		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
		m_bIsAnimModel = true;
		m_pModel->Change_AnimIndex(m_tInstanceDesc.iModelAnimIndex);
		FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime));
	}
	else
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
		m_bIsAnimModel = false;
	}

	if (m_tInstanceDesc.eInstanceCount < Prototype_ModelInstance_1 || m_tInstanceDesc.eInstanceCount > Prototype_ModelInstance_512)
	{
		__debugbreak();
		return E_FAIL;
	}

	CModelInstance::MODELINSTDESC tModelDesc;
	tModelDesc.m_pTargetModel = m_pModel;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(m_tInstanceDesc.eInstanceCount), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance,&tModelDesc));
	m_iNumInstance = _uint(pow(2,m_tInstanceDesc.eInstanceCount - Prototype_ModelInstance_2 + 1));



	return S_OK;
}


HRESULT CMeshEffect::Ready_InstanceDesc()
{
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.FollowingTarget)
	{
		switch (m_tInstanceDesc.iFollowingDir)
		{
		case FollowingDir_Right:
			m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_RIGHT).Get_Nomalize();
			break;
		case FollowingDir_Up:
			m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_UP).Get_Nomalize();
			break;
		case FollowingDir_Look:
			m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_LOOK).Get_Nomalize();
			break;
		default:
			__debugbreak();
			return E_FAIL;
			break;
		}

	}

	else
		m_vUp = m_tInstanceDesc.vPowerDirection.Get_Nomalize();


	if (m_vUp.y == 1 || m_vUp.y == -1) m_vUp = _float3(0.000001f, 1, 0).Get_Nomalize();


	m_vRight = XMVector3Normalize(_float3(0, 1, 0).Get_Cross(m_vUp.XMVector()));
	m_vLook = XMVector3Normalize(m_vRight.Get_Cross(m_vUp.XMVector()));
	

	return S_OK;
}



HRESULT CMeshEffect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	if (m_tInstanceDesc.ePassID >= MeshPass_MaskingNoising && m_tInstanceDesc.ePassID <= MeshPass_MaskingNoising_Appear_Bright)
	{
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		//g_NoiseTexture / g_DiffuseTexture / g_SourTexture /  g_DepthTexture

		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_tInstanceDesc.iNoiseTextureIndex));
		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_tInstanceDesc.iMaskingTextureIndex));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tInstanceDesc.vNoisePushingDir, sizeof(_float2)));


		if (m_tInstanceDesc.ePassID == MeshPass_MaskingNoising_Appear_Bright || m_tInstanceDesc.ePassID == MeshPass_MaskingNoising_Appear)
		{
			FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &m_tInstanceDesc.fAppearTimer, sizeof(_float)));
		}

	}
	else if (m_tInstanceDesc.ePassID >= MeshPass_AllDistortion && m_tInstanceDesc.ePassID <= MeshPass_Distortion_ColorMix_Bright)
	{
		//g_NoiseTexture g_DiffuseTexture g_BackBufferTexture
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_tInstanceDesc.iNoiseTextureIndex));

		FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceDefferred")));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tInstanceDesc.vNoisePushingDir, sizeof(_float2)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &m_tInstanceDesc.fDistortionNoisingPushPower, sizeof(_float)));
		
	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CMeshEffect::ResetParticle(INSTMESHATT * attribute)
{
	_float4 RandomPos = _float4(0);

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	//랜덤 위치에 재생성
	RandomPos = _float4(pUtil->RandomFloat3(m_tInstanceDesc.ParticleStartRandomPosMin, m_tInstanceDesc.ParticleStartRandomPosMax),1);

	memcpy(&(attribute->_LocalMatirx._41), &RandomPos,sizeof(_float4));
	 

	//랜덤한 라이프타임
	attribute->_lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
	attribute->_age = 0;

	attribute->_Targetforce = attribute->_force = m_tInstanceDesc.Particle_Power * pUtil->RandomFloat(m_tInstanceDesc.PowerRandomRange.x, m_tInstanceDesc.PowerRandomRange.y);
	attribute->_size = m_tInstanceDesc.ParticleSize;
	attribute->_color = m_tInstanceDesc.TargetColor;


	if (m_tInstanceDesc.FollowingTarget)
		attribute->_TargetParentPosition = m_tInstanceDesc.FollowingTarget->Get_MatrixState(CTransform::STATE_POS);
	else
		attribute->_TargetParentPosition = m_tInstanceDesc.vFixedPosition.XMVector();

	


	//자식들의 Velocity를 초기화하는 과정
	Reset_Velocity(attribute->_velocity);

	if (m_tInstanceDesc.bAutoTurn)
	{
		attribute->_randRotAxis = pUtil->RandomFloat3(-1, 1).Get_Nomalize();
		attribute->_randRotSpeed = pUtil->RandomFloat(0.75f, 1.25f) * m_tInstanceDesc.fRotSpeed_Radian;
	}

	_Vector vRight, vUp, vLook;

	vUp = attribute->_velocity.Get_Nomalize();
	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

	XMStoreFloat4(((_float4*)(&attribute->_LocalMatirx.m[0])), vRight * attribute->_size.x);
	XMStoreFloat4(((_float4*)(&attribute->_LocalMatirx.m[1])), vUp* attribute->_size.y);
	XMStoreFloat4(((_float4*)(&attribute->_LocalMatirx.m[2])), vLook* attribute->_size.z);

}

void CMeshEffect::Update_ParticleAttribute(_double fDeltaTime)
{

	m_PassedTime += (_float)fDeltaTime;

	CGameInstance* pInstance = g_pGameInstance;


	if (m_bIsAnimModel)
	{
		if ((m_pModel->Update_AnimationClip(fDeltaTime)) < 0)
		{
			__debugbreak();
			return;
		}
	}

	if (m_tInstanceDesc.FollowingTarget)
	{
		if (m_tInstanceDesc.FollowingTarget->Get_IsOwnerDead())
		{

			this->Set_IsDead();
			return;

		}
		else
		{
			if (m_tInstanceDesc.FollowingTarget)
			{
				switch (m_tInstanceDesc.iFollowingDir)
				{
				case FollowingDir_Right:
					m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_RIGHT).Get_Nomalize();
					break;
				case FollowingDir_Up:
					m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_UP).Get_Nomalize();
					break;
				case FollowingDir_Look:
					m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_LOOK).Get_Nomalize();
					break;
				default:
					__debugbreak();
					break;
				}


			}


			if (m_vUp.y == 1 || m_vUp.y == -1) m_vUp = _float3(0.000001f, 1, 0).Get_Nomalize();


			m_vRight = XMVector3Normalize(_float3(0, 1, 0).Get_Cross(m_vUp.XMVector()));
			m_vLook = XMVector3Normalize(m_vRight.Get_Cross(m_vUp.XMVector()));
			

		}
	}


	auto& iter = m_vecParticleAttribute.begin();

	if (m_PassedTime < m_tInstanceDesc.TotalParticleTime)
	{
		for (; iter != m_vecParticleAttribute.end();iter++)
		{
			iter->_age += (_float)fDeltaTime;
			if (iter->_age < 0) continue;

			if (iter->_age - (_float)fDeltaTime < 0)
			{
				ResetParticle(&(*iter));
			}

			Update_Position_by_Velocity(&(*iter), fDeltaTime);

			if ((*(_float3*)&iter->_LocalMatirx._41).Get_Lenth() > m_tInstanceDesc.fMaxBoundaryRadius	|| (iter->_age > iter->_lifeTime))
				ResetParticle(&(*iter));

			if (m_tInstanceDesc.ColorChageFrequency)
				Update_ColorChange(&(*iter), fDeltaTime, pInstance);

			if (m_tInstanceDesc.SizeChageFrequency) 
				Update_SizeChange(&(*iter), fDeltaTime, pInstance);


		}


	}
	else
	{
		_uint iDeadCount = 0;

		for (; iter != m_vecParticleAttribute.end(); iter++)
		{
			iter->_age += (_float)fDeltaTime;

			if (iter->_age < 0) continue;

			if (iter->_age < iter->_lifeTime)
			{
				Update_Position_by_Velocity(&(*iter), fDeltaTime);

				if (m_tInstanceDesc.ColorChageFrequency)
					Update_ColorChange(&(*iter), fDeltaTime, pInstance);

				if (m_tInstanceDesc.SizeChageFrequency)
					Update_SizeChange(&(*iter), fDeltaTime, pInstance);

			}
			else
			{
				iDeadCount++;
				iter->_isAlive = false;
				iter->_size = _float3(0);
			}

			if (iDeadCount >= m_iNumInstance)
			{
				Set_IsDead();
				return;
			}
			
		}


	}



	return;



}

void CMeshEffect::Update_SizeChange(INSTMESHATT * tParticleAtt, _double fTimeDelta, CGameInstance * pInstance)
{
	if (m_tInstanceDesc.SizeChageFrequency)
	{
		_double TimeInterver = tParticleAtt->_lifeTime / m_tInstanceDesc.SizeChageFrequency; //=> 15.0초 파티클의 2번 진동수면 7.5가 나오는 것
		_uint iFrequencyIndex = _uint(tParticleAtt->_age / TimeInterver); //=> 7.5초 주기에 3.4초를 지나고있으면 0번째 10초를 지나고있으면 1번째를 의미함
		_double FrequencyAge = tParticleAtt->_age - (TimeInterver * iFrequencyIndex); // =>7.5초 주기에 3.4초를 지나고있으면 3.4 10초를 지나고있으면 2.5

		if (iFrequencyIndex % 2)
			tParticleAtt->_size = pInstance->Easing_Vector(TYPE_Linear, m_tInstanceDesc.ParticleSize2, m_tInstanceDesc.ParticleSize, _float(FrequencyAge), _float(TimeInterver));

		else
			tParticleAtt->_size = pInstance->Easing_Vector(TYPE_Linear, m_tInstanceDesc.ParticleSize, m_tInstanceDesc.ParticleSize2, _float(FrequencyAge), _float(TimeInterver));


		if (tParticleAtt->_size.x <= 0) tParticleAtt->_size.x = 0.000001f;
		if (tParticleAtt->_size.y <= 0) tParticleAtt->_size.y = 0.000001f;
		if (tParticleAtt->_size.z <= 0) tParticleAtt->_size.z = 0.000001f;


	

		memcpy(&tParticleAtt->_LocalMatirx._11, &(_float3(((_float3*)(&tParticleAtt->_LocalMatirx._11))->Get_Nomalize() * tParticleAtt->_size.x)), sizeof(_float3));
		memcpy(&tParticleAtt->_LocalMatirx._21, &(_float3(((_float3*)(&tParticleAtt->_LocalMatirx._21))->Get_Nomalize() * tParticleAtt->_size.y)), sizeof(_float3));
		memcpy(&tParticleAtt->_LocalMatirx._31, &(_float3(((_float3*)(&tParticleAtt->_LocalMatirx._31))->Get_Nomalize() * tParticleAtt->_size.z)), sizeof(_float3));


	}


}



void CMeshEffect::Update_ColorChange(INSTMESHATT * tParticleAtt, _double fTimeDelta, CGameInstance * pInstance)
{
	if (m_tInstanceDesc.ColorChageFrequency)
	{
		_double TimeInterver = tParticleAtt->_lifeTime / m_tInstanceDesc.ColorChageFrequency; //=> 15.초 파티클의 2번 진동수면 7.5가 나오는 것
		_uint iFrequencyIndex = _uint(tParticleAtt->_age / TimeInterver); //=> 7.5초 주기에 3.4초를 지나고있으면 0번째 10초를 지나고있으면 1번째를 의미함
		_double FrequencyAge = tParticleAtt->_age - (TimeInterver * iFrequencyIndex); // =>7.5초 주기에 3.4초를 지나고있으면 3.4 10초를 지나고있으면 2.5

		if (iFrequencyIndex % 2)
		{
			tParticleAtt->_color = pInstance->Easing_Vector(TYPE_Linear, m_tInstanceDesc.TargetColor2, m_tInstanceDesc.TargetColor, _float(FrequencyAge), _float(TimeInterver));
			tParticleAtt->_color.w = pInstance->Easing(TYPE_Linear, m_tInstanceDesc.TargetColor2.w, m_tInstanceDesc.TargetColor.w, _float(FrequencyAge), _float(TimeInterver));
		}
		else
		{
			tParticleAtt->_color = pInstance->Easing_Vector(TYPE_Linear, m_tInstanceDesc.TargetColor, m_tInstanceDesc.TargetColor2, _float(FrequencyAge), _float(TimeInterver));
			tParticleAtt->_color.w = pInstance->Easing(TYPE_Linear, m_tInstanceDesc.TargetColor.w, m_tInstanceDesc.TargetColor2.w, _float(FrequencyAge), _float(TimeInterver));

		}

		tParticleAtt->_color.x = min(max(tParticleAtt->_color.x, 0), 1);
		tParticleAtt->_color.y = min(max(tParticleAtt->_color.y, 0), 1);
		tParticleAtt->_color.z = min(max(tParticleAtt->_color.z, 0), 1);
		tParticleAtt->_color.w = min(max(tParticleAtt->_color.w, 0), 1);
	}
}







void CMeshEffect::Free()
{
	__super::Free();



	Safe_Release(m_pModel);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelInstance);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_tInstanceDesc.FollowingTarget);

	m_vecParticleAttribute.clear();
	
}

///////////구 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region BallParticle
CMeshEffect_Ball::CMeshEffect_Ball(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMeshEffect(pDevice, pDeviceContext)
{
}

CMeshEffect_Ball::CMeshEffect_Ball(const CMeshEffect_Ball & rhs)
	: CMeshEffect(rhs)
{
}

void CMeshEffect_Ball::Reset_Velocity(_float3 & fAttVlocity)
{
	fAttVlocity = GetSingle(CUtilityMgr)->RandomFloat3(_float3(-5, -5, -5), _float3(5, 5, 5)).Get_Nomalize();
}

void CMeshEffect_Ball::Update_Position_by_Velocity(INSTMESHATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	if (m_tInstanceDesc.bAutoTurn)
	{
		_Matrix TempMat = tParticleAtt->_LocalMatirx.XMatrix();
		TempMat.r[3] = XMVectorSet(0, 0, 0, 1);
		TempMat = TempMat * XMMatrixRotationAxis(tParticleAtt->_randRotAxis.XMVector(), tParticleAtt->_randRotSpeed * (_float)fTimeDelta);
		tParticleAtt->_LocalMatirx = TempMat;
	}


	position = position.XMVector() + (tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta));
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));


}

HRESULT CMeshEffect_Ball::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTMESHATT part;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	for (_uint i = 0; i < m_iNumInstance; i++)
	{
		part._LocalMatirx = XMMatrixIdentity();

		//랜덤한 라이프타임
		part._lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
		part._age = -m_tInstanceDesc.EachParticleLifeTime + (_float(i + 1) / _float(m_iNumInstance)) * m_tInstanceDesc.EachParticleLifeTime;

		part._color = m_tInstanceDesc.TargetColor;

		m_vecParticleAttribute.push_back(part);
	}




	return S_OK;
}

_int CMeshEffect_Ball::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CMeshEffect_Ball::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CMeshEffect_Ball * CMeshEffect_Ball::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMeshEffect_Ball* pInstance = new CMeshEffect_Ball(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CMeshEffect_Ball");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CMeshEffect_Ball::Clone(void * pArg)
{
	CMeshEffect_Ball* pInstance = new CMeshEffect_Ball(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CMeshEffect_Ball");
		Safe_Release(pInstance);
	}

	return pInstance;
}

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////직선 형태 파티클//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region StraightParticle

CMeshEffect_Straight::CMeshEffect_Straight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMeshEffect(pDevice, pDeviceContext)
{
}

CMeshEffect_Straight::CMeshEffect_Straight(const CMeshEffect_Straight & rhs)
	: CMeshEffect(rhs)
{
}

void CMeshEffect_Straight::Reset_Velocity(_float3 & fAttVlocity)
{
	fAttVlocity = m_vUp.Get_Nomalize();


}

void CMeshEffect_Straight::Update_Position_by_Velocity(INSTMESHATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	if (m_tInstanceDesc.bAutoTurn)
	{
		_Matrix TempMat = tParticleAtt->_LocalMatirx.XMatrix();
		TempMat.r[3] = XMVectorSet(0, 0, 0, 1);
		TempMat = TempMat * XMMatrixRotationAxis(tParticleAtt->_randRotAxis.XMVector(), tParticleAtt->_randRotSpeed *(_float) fTimeDelta);
		tParticleAtt->_LocalMatirx = TempMat;
	}


	position = position.XMVector() + tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));

}

HRESULT CMeshEffect_Straight::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTMESHATT part;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);



	for (_uint i = 0; i < m_iNumInstance; i++)
	{

		part._lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
		part._age = -m_tInstanceDesc.EachParticleLifeTime + (_float(i + 1) / _float(m_iNumInstance)) * m_tInstanceDesc.EachParticleLifeTime;

		m_vecParticleAttribute.push_back(part);
	}




	return S_OK;
}

_int CMeshEffect_Straight::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CMeshEffect_Straight::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CMeshEffect_Straight * CMeshEffect_Straight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMeshEffect_Straight* pInstance = new CMeshEffect_Straight(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CMeshEffect_Straight");
		Safe_Release(pInstance);

	}
	return pInstance;
}


CGameObject * CMeshEffect_Straight::Clone(void * pArg)
{
	CMeshEffect_Straight* pInstance = new CMeshEffect_Straight(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CMeshEffect_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////원뿔 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region ConeParticle

CMeshEffect_Cone::CMeshEffect_Cone(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMeshEffect(pDevice, pDeviceContext)
{
}

CMeshEffect_Cone::CMeshEffect_Cone(const CMeshEffect_Cone & rhs)
	: CMeshEffect(rhs)
{
}

void CMeshEffect_Cone::Reset_Velocity(_float3 & fAttVlocity)
{

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	fAttVlocity = XMVector3Normalize(m_vRight.XMVector() * pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange_RUL.x, m_tInstanceDesc.SubPowerRandomRange_RUL.x)
		+ m_vLook.XMVector() *  pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange_RUL.y , m_tInstanceDesc.SubPowerRandomRange_RUL.y)
		+ m_vUp.Get_Nomalize()	* pUtil->RandomFloat(0, m_tInstanceDesc.SubPowerRandomRange_RUL.z));

}

void CMeshEffect_Cone::Update_Position_by_Velocity(INSTMESHATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	if (m_tInstanceDesc.bAutoTurn)
	{
		_Matrix TempMat = tParticleAtt->_LocalMatirx.XMatrix();
		TempMat.r[3] = XMVectorSet(0, 0, 0, 1);
		TempMat = TempMat * XMMatrixRotationAxis(tParticleAtt->_randRotAxis.XMVector(), tParticleAtt->_randRotSpeed * (_float)fTimeDelta);
		tParticleAtt->_LocalMatirx = TempMat;
	}


	position = position.XMVector() + tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
}

HRESULT CMeshEffect_Cone::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTMESHATT part;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	for (_uint i = 0; i < m_iNumInstance; i++)
	{


		part._lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
		part._age = -m_tInstanceDesc.EachParticleLifeTime + (_float(i + 1) / _float(m_iNumInstance)) * m_tInstanceDesc.EachParticleLifeTime;

		part._color = m_tInstanceDesc.TargetColor;
		m_vecParticleAttribute.push_back(part);
	}




	return S_OK;
}

_int CMeshEffect_Cone::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CMeshEffect_Cone::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CMeshEffect_Cone * CMeshEffect_Cone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMeshEffect_Cone* pInstance = new CMeshEffect_Cone(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CMeshEffect_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeshEffect_Cone::Clone(void * pArg)
{
	CMeshEffect_Cone* pInstance = new CMeshEffect_Cone(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CMeshEffect_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}
#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////퍼지는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SpreadParticle

CMeshEffect_Spread::CMeshEffect_Spread(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMeshEffect(pDevice, pDeviceContext)
{
}

CMeshEffect_Spread::CMeshEffect_Spread(const CMeshEffect_Spread & rhs)
	: CMeshEffect(rhs)
{
}

void CMeshEffect_Spread::Reset_Velocity(_float3 & fAttVlocity)
{

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
	_float Rand1 = pUtil->RandomFloat(-1.f, 1.f);
	_float Rand2 = pUtil->RandomFloat(-1.f, 1.f);


	fAttVlocity = XMVector3Normalize(m_vUp.XMVector() * Rand1 + m_vRight.XMVector() * Rand2);


}

void CMeshEffect_Spread::Update_Position_by_Velocity(INSTMESHATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	if (m_tInstanceDesc.bAutoTurn)
	{
		_Matrix TempMat = tParticleAtt->_LocalMatirx.XMatrix();
		TempMat.r[3] = XMVectorSet(0, 0, 0, 1);
		TempMat = TempMat * XMMatrixRotationAxis(tParticleAtt->_randRotAxis.XMVector(), tParticleAtt->_randRotSpeed *(_float)fTimeDelta);
		tParticleAtt->_LocalMatirx = TempMat;
	}


	position = position.XMVector() + tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
}

HRESULT CMeshEffect_Spread::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTMESHATT part;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	for (_uint i = 0; i < m_iNumInstance; i++)
	{


		part._lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
		part._age = -m_tInstanceDesc.EachParticleLifeTime + (_float(i + 1) / _float(m_iNumInstance)) * m_tInstanceDesc.EachParticleLifeTime;

		part._color = m_tInstanceDesc.TargetColor;
		m_vecParticleAttribute.push_back(part);
	}



	return S_OK;
}

_int CMeshEffect_Spread::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CMeshEffect_Spread::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}


CMeshEffect_Spread * CMeshEffect_Spread::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMeshEffect_Spread* pInstance = new CMeshEffect_Spread(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CMeshEffect_Spread");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeshEffect_Spread::Clone(void * pArg)
{
	CMeshEffect_Spread* pInstance = new CMeshEffect_Spread(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CMeshEffect_Spread");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////분수 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region FountainParticle

CMeshEffect_Fountain::CMeshEffect_Fountain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMeshEffect(pDevice, pDeviceContext)
{
}

CMeshEffect_Fountain::CMeshEffect_Fountain(const CMeshEffect_Fountain & rhs)
	: CMeshEffect(rhs)
{
}

void CMeshEffect_Fountain::Reset_Velocity(_float3 & fAttVlocity)
{
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	fAttVlocity = XMVector3Normalize(m_vRight.XMVector() * pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange_RUL.x, m_tInstanceDesc.SubPowerRandomRange_RUL.x)
		+ m_vLook.XMVector() *  pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange_RUL.y, m_tInstanceDesc.SubPowerRandomRange_RUL.y)
		+ m_vUp.Get_Nomalize()	* pUtil->RandomFloat(m_tInstanceDesc.SubPowerRandomRange_RUL.z, m_tInstanceDesc.SubPowerRandomRange_RUL.z * 2.f));
	

	if (fAttVlocity.y < 0) fAttVlocity.y *= -1.f;

}

void CMeshEffect_Fountain::Update_Position_by_Velocity(INSTMESHATT * tParticleAtt, _double fTimeDelta)
{

	if (m_tInstanceDesc.bAutoTurn)
	{
		_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

		_Matrix TempMat = tParticleAtt->_LocalMatirx.XMatrix();
		TempMat.r[3] = XMVectorSet(0, 0, 0, 1);
		TempMat = TempMat * XMMatrixRotationAxis(tParticleAtt->_randRotAxis.XMVector(), tParticleAtt->_randRotSpeed * (_float)fTimeDelta);
		tParticleAtt->_LocalMatirx = TempMat;


		_float Rate = tParticleAtt->_age / tParticleAtt->_lifeTime;
		_float3 Dir = tParticleAtt->_velocity;


		if (Rate < 0.7f)
		{
			Dir.y = g_pGameInstance->Easing(TYPE_SinOut, Dir.y, 0, tParticleAtt->_age, tParticleAtt->_lifeTime* 0.7f);
			position = position.XMVector() + Dir.Get_Nomalize() * tParticleAtt->_force * _float(fTimeDelta);
		}
		else
		{
			Dir.y = g_pGameInstance->Easing(TYPE_SinIn, 0, -Dir.y * 0.3f, tParticleAtt->_age - tParticleAtt->_lifeTime* 0.7f, tParticleAtt->_lifeTime * 0.3f);
			position = position.XMVector() + Dir.Get_Nomalize()* tParticleAtt->_force * _float(fTimeDelta);
		}

		memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
	}
	else
	{
		_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

		_float Rate = tParticleAtt->_age / tParticleAtt->_lifeTime;
		_float3 Dir = tParticleAtt->_velocity;


		if (Rate < 0.7f)
		{
			Dir.y = g_pGameInstance->Easing(TYPE_SinOut, Dir.y, 0, tParticleAtt->_age, tParticleAtt->_lifeTime* 0.7f);
			position = position.XMVector() + Dir.Get_Nomalize() * tParticleAtt->_force * _float(fTimeDelta);
		}
		else
		{
			Dir.y = g_pGameInstance->Easing(TYPE_SinIn, 0, -Dir.y * 0.3f, tParticleAtt->_age - tParticleAtt->_lifeTime* 0.7f, tParticleAtt->_lifeTime * 0.3f);
			position = position.XMVector() + Dir.Get_Nomalize()* tParticleAtt->_force * _float(fTimeDelta);
		}

		memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));


		_Vector vRight, vUp, vLook;


		vUp = Dir.Get_Nomalize();
		//vUp = XMVector3Normalize(((_float3*)(&tParticleAtt->_LocalMatirx.m[1]))->Get_Nomalize() * 0.7f + Dir.Get_Nomalize() * 0.3f);
		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
		vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

		XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[0])), vRight);
		XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[1])), vUp);
		XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[2])), vLook);

	}


	


}

HRESULT CMeshEffect_Fountain::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTMESHATT part;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	for (_uint i = 0; i < m_iNumInstance; i++)
	{


		part._lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
		part._age = -m_tInstanceDesc.EachParticleLifeTime + (_float(i + 1) / _float(m_iNumInstance)) * m_tInstanceDesc.EachParticleLifeTime;

		m_vecParticleAttribute.push_back(part);
	}



	return S_OK;
}

_int CMeshEffect_Fountain::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CMeshEffect_Fountain::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CMeshEffect_Fountain * CMeshEffect_Fountain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMeshEffect_Fountain* pInstance = new CMeshEffect_Fountain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CMeshEffect_Fountain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeshEffect_Fountain::Clone(void * pArg)
{
	CMeshEffect_Fountain* pInstance = new CMeshEffect_Fountain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CMeshEffect_Fountain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////빨아들이는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SuckParticle

CMeshEffect_Suck::CMeshEffect_Suck(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMeshEffect(pDevice, pDeviceContext)
{
}

CMeshEffect_Suck::CMeshEffect_Suck(const CMeshEffect_Suck & rhs)
	: CMeshEffect(rhs)
{
}

void CMeshEffect_Suck::Reset_Velocity(_float3 & fAttVlocity)
{

}

void CMeshEffect_Suck::Update_Position_by_Velocity(INSTMESHATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	if (m_tInstanceDesc.bAutoTurn)
	{
		_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

		_Matrix TempMat = tParticleAtt->_LocalMatirx.XMatrix();
		TempMat.r[3] = XMVectorSet(0, 0, 0, 1);
		TempMat = TempMat * XMMatrixRotationAxis(tParticleAtt->_randRotAxis.XMVector(), tParticleAtt->_randRotSpeed * (_float)fTimeDelta);
		tParticleAtt->_LocalMatirx = TempMat;
	}

	_float3 Oldposition = position;
	position = position.XMVector() + position.Get_Nomalize() * -tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
	

	if (Oldposition.x * position.x <= 0)
	{
		if (m_PassedTime < m_tInstanceDesc.TotalParticleTime)
		{
			ResetParticle(tParticleAtt);

			_Vector vRight, vUp, vLook;

			vUp = (*(_float3*)&tParticleAtt->_LocalMatirx._41).Get_Nomalize() * -1.f;
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[2])), vLook);


		}
		else
			tParticleAtt->_isAlive = false;
	}

}

HRESULT CMeshEffect_Suck::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	m_tInstanceDesc.fMaxBoundaryRadius = 99999.f;

	INSTMESHATT part;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	for (_uint i = 0; i < m_iNumInstance; i++)
	{


		part._lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
		part._age = -m_tInstanceDesc.EachParticleLifeTime + (_float(i + 1) / _float(m_iNumInstance)) * m_tInstanceDesc.EachParticleLifeTime;

		part._color = m_tInstanceDesc.TargetColor;
		m_vecParticleAttribute.push_back(part);
	}



	return S_OK;
}

_int CMeshEffect_Suck::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CMeshEffect_Suck::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CMeshEffect_Suck * CMeshEffect_Suck::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMeshEffect_Suck* pInstance = new CMeshEffect_Suck(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CMeshEffect_Suck");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeshEffect_Suck::Clone(void * pArg)
{
	CMeshEffect_Suck* pInstance = new CMeshEffect_Suck(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CMeshEffect_Suck");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion