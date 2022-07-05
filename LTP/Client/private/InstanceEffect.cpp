#include "stdafx.h"
#include "..\public\InstanceEffect.h"


CInstanceEffect::CInstanceEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CInstanceEffect::CInstanceEffect(const CInstanceEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CInstanceEffect::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype(nullptr));

	return S_OK;
}

HRESULT CInstanceEffect::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));


	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&m_tInstanceDesc, pArg, sizeof(INSTPARTICLEDESC));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(Ready_InstanceDesc());
	FAILED_CHECK(Initialize_Child_Clone());


	Safe_AddRef(m_tInstanceDesc.FollowingTarget);

	 

	return S_OK;
}

_int CInstanceEffect::Update(_double TimeDelta)
{
	if (0 > __super::Update(TimeDelta))
		return -1;
	Update_ParticleAttribute(TimeDelta);


	return _int();
}

_int CInstanceEffect::LateUpdate(_double TimeDelta)
{
	if (0 > __super::LateUpdate(TimeDelta))
		return -1;
	 
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
	   
	return _int();
}

_int CInstanceEffect::Render()
{
	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	FAILED_CHECK(SetUp_ConstantTable());

	D3D11_MAPPED_SUBRESOURCE SubResource;
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		_Matrix matInvView = g_pGameInstance->Get_Transform_Float4x4(PLM_VIEW).InverseXMatrix();

		FAILED_CHECK(m_pVIBufferCom->Lock(&SubResource));
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), matInvView.r[0] * m_vecParticleAttribute[i]._size.x);
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), matInvView.r[1]  * m_vecParticleAttribute[i]._size.y);
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), matInvView.r[2] * m_vecParticleAttribute[i]._size.z);

			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), XMVectorSet(1,0,0,0));
			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), XMVectorSet(0, 1, 0, 0));
			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), XMVectorSet(0, 0, 1, 0));
			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation), XMVectorSet(0, 1.5f, 0, 1.f));



			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation),
				((_float3*)(&m_vecParticleAttribute[i]._LocalMatirx.m[3]))->XMVector() + m_vecParticleAttribute[i]._TargetParentPosition.XMVector());
			((VTXINSTMAT*)SubResource.pData)[i].vTranslation.w = 1;


			((VTXINSTMAT*)SubResource.pData)[i].vColor = m_vecParticleAttribute[i]._color;


			if (!m_vecParticleAttribute[i]._isAlive || m_vecParticleAttribute[i]._age <= 0)
			{

				((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize =
					_float4(m_vecParticleAttribute[i]._TextureUV.x, m_vecParticleAttribute[i]._TextureUV.y, 0, 0);
			}
			else
			{
				((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize =
					_float4(m_vecParticleAttribute[i]._TextureUV.x, m_vecParticleAttribute[i]._TextureUV.y, 0, 1);
			}
		}
		m_pVIBufferCom->UnLock();

		FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, m_tInstanceDesc.ePassID * 2 ));
	}
	else
	{
		FAILED_CHECK(m_pVIBufferCom->Lock(&SubResource));
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), ((_float3*)(&m_vecParticleAttribute[i]._LocalMatirx.m[0]))->XMVector() * m_vecParticleAttribute[i]._size.x);
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), ((_float3*)(&m_vecParticleAttribute[i]._LocalMatirx.m[1]))->XMVector()* m_vecParticleAttribute[i]._size.y);
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), ((_float3*)(&m_vecParticleAttribute[i]._LocalMatirx.m[2]))->XMVector()* m_vecParticleAttribute[i]._size.z);

			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), XMVectorSet(1,0,0,0));
			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), XMVectorSet(0, 1, 0, 0));
			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), XMVectorSet(0, 0, 1, 0));
			//XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation), XMVectorSet(0, 1.5f, 0, 1.f));



			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation),
				((_float3*)(&m_vecParticleAttribute[i]._LocalMatirx.m[3]))->XMVector() + m_vecParticleAttribute[i]._TargetParentPosition.XMVector());
			((VTXINSTMAT*)SubResource.pData)[i].vTranslation.w = 1;


			((VTXINSTMAT*)SubResource.pData)[i].vColor = m_vecParticleAttribute[i]._color;

			if (!m_vecParticleAttribute[i]._isAlive || m_vecParticleAttribute[i]._age <= 0)
			{
				((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize =
					_float4(m_vecParticleAttribute[i]._TextureUV.x, m_vecParticleAttribute[i]._TextureUV.y, 0, 0);
			}
			else
			{
				((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize =
					_float4(m_vecParticleAttribute[i]._TextureUV.x, m_vecParticleAttribute[i]._TextureUV.y, 0, 1);
			}
		}
		m_pVIBufferCom->UnLock();
		FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, m_tInstanceDesc.ePassID * 2 + 1));
	}


	return _int();
}

_int CInstanceEffect::LateRender()
{
	return _int();
}

HRESULT CInstanceEffect::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXPOINTINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	if (m_tInstanceDesc.eInstanceCount < Prototype_VIBuffer_Point_Instance_1 || m_tInstanceDesc.eInstanceCount > Prototype_VIBuffer_Point_Instance_64) return E_FAIL;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(m_tInstanceDesc.eInstanceCount), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));
	m_iNumInstance = m_pVIBufferCom->Get_NumInstance();

	FAILED_CHECK(Add_Component(SCENE_STATIC, m_tInstanceDesc.szTextureProtoTypeTag, TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));
	FAILED_CHECK(m_pTextureCom->Change_TextureLayer(m_tInstanceDesc.szTextureLayerTag));
	//m_TextureLayer = m_ParticleDesc.szTextureLayerTag;


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));


	return S_OK;
}


HRESULT CInstanceEffect::Ready_InstanceDesc()
{
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.FollowingTarget)
		m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_LOOK).Get_Nomalize();
	else
		m_vUp = m_tInstanceDesc.vPowerDirection.Get_Nomalize();



	if (m_vUp.y == 1 || m_vUp.y == -1)
	{
		m_vRight = XMVector3Normalize(_float3(0.000001f, 1, 0).Get_Cross(m_vUp.XMVector()));
		m_vLook = XMVector3Normalize(m_vRight.Get_Cross(m_vUp.XMVector()));
	}
	else
	{
		m_vRight = XMVector3Normalize(_float3(0, 1, 0).Get_Cross(m_vUp.XMVector()));
		m_vLook = XMVector3Normalize(m_vRight.Get_Cross(m_vUp.XMVector()));
	}

	return S_OK;
}



HRESULT CInstanceEffect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_tInstanceDesc.iTextureLayerIndex));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_DepthTexture", pGameInstance->Get_SRV(TEXT("Target_Depth"))));

	_float2 UVSize = _float2(1 / m_tInstanceDesc.vTextureXYNum.x, 1 / m_tInstanceDesc.vTextureXYNum.y);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vTextureFigureUVSize", &UVSize, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &m_tInstanceDesc.m_fAlphaTestValue, sizeof(_float)));

	_float4		vCamPosition = pGameInstance->Get_TargetPostion_float4(PLV_CAMERA);
	m_pShaderCom->Set_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4));


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CInstanceEffect::ResetParticle(INSTANCEATT * attribute)
{
	_float4 RandomPos = _float4(0);

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	//랜덤 위치에 재생성
	RandomPos = _float4(pUtil->RandomFloat3(m_tInstanceDesc.ParticleStartRandomPosMin, m_tInstanceDesc.ParticleStartRandomPosMax),1);

	memcpy(&(attribute->_LocalMatirx._41), &RandomPos,sizeof(_float4));


	//랜덤한 라이프타임
	attribute->_lifeTime = m_tInstanceDesc.EachParticleLifeTime * pUtil->RandomFloat(0.7f, 1.3f);
	attribute->_age = 0;

	attribute->_color = m_tInstanceDesc.TargetColor;
	attribute->_Targetforce = attribute->_force = m_tInstanceDesc.Particle_Power * pUtil->RandomFloat(m_tInstanceDesc.PowerRandomRange.x, m_tInstanceDesc.PowerRandomRange.y);
	attribute->_color = m_tInstanceDesc.TargetColor;
	attribute->_size = m_tInstanceDesc.ParticleSize;



	if (m_tInstanceDesc.FollowingTarget)
		attribute->_TargetParentPosition = m_tInstanceDesc.FollowingTarget->Get_MatrixState(CTransform::STATE_POS);
	else
		attribute->_TargetParentPosition = m_tInstanceDesc.vFixedPosition.XMVector();



	//자식들의 Velocity를 초기화하는 과정
	Reset_Velocity(attribute->_velocity);


	if (!m_tInstanceDesc.bBillboard)
	{

		_Vector vRight, vUp, vLook;


		vUp = attribute->_velocity.Get_Nomalize();
		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
		vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

		XMStoreFloat4(((_float4*)(&attribute->_LocalMatirx.m[0])), vRight);
		XMStoreFloat4(((_float4*)(&attribute->_LocalMatirx.m[1])), vUp);
		XMStoreFloat4(((_float4*)(&attribute->_LocalMatirx.m[2])), vLook);


	}






}

void CInstanceEffect::Update_ParticleAttribute(_double fDeltaTime)
{

	m_PassedTime += (_float)fDeltaTime;

	CGameInstance* pInstance = g_pGameInstance;

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
				m_vUp = m_tInstanceDesc.FollowingTarget->Get_MatrixState_Float3(CTransform::STATE_LOOK).Get_Nomalize();


			if (m_vUp.y == 1 || m_vUp.y == -1)
			{
				m_vRight = XMVector3Normalize(_float3(0.000001f, 1, 0).Get_Cross(m_vUp.XMVector()));
				m_vLook = XMVector3Normalize(m_vRight.Get_Cross(m_vUp.XMVector()));
			}
			else
			{
				m_vRight = XMVector3Normalize(_float3(0, 1, 0).Get_Cross(m_vUp.XMVector()));
				m_vLook = XMVector3Normalize(m_vRight.Get_Cross(m_vUp.XMVector()));
			}

		}
	}


	_Vector CamPos = XMVectorSet(0,0,0,0);
	if (m_tInstanceDesc.AlphaBlendON)
		CamPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_CAMERA);


	auto& iter = m_vecParticleAttribute.begin();

	if (m_PassedTime < m_tInstanceDesc.TotalParticleTime)
	{
		for (; iter != m_vecParticleAttribute.end();iter++)
		{
			iter->_age += (_float)fDeltaTime;
			if (iter->_age < 0) continue;

			Update_Position_by_Velocity(&(*iter), fDeltaTime);

			if ((*(_float3*)&iter->_LocalMatirx._41).Get_Lenth() > m_tInstanceDesc.fMaxBoundaryRadius	|| (iter->_age > iter->_lifeTime))
				ResetParticle(&(*iter));

			if (m_tInstanceDesc.AlphaBlendON)
				iter->_CamDist = XMVectorGetX(XMVector3Length(((iter->_LocalMatirx.XMatrix().r[3] + iter->_TargetParentPosition.XMVector()) - CamPos)));

			if (m_tInstanceDesc.ColorChageFrequency)
				Update_ColorChange(&(*iter), fDeltaTime, pInstance);

			if (m_tInstanceDesc.SizeChageFrequency)
				Update_SizeChange(&(*iter), fDeltaTime, pInstance);


			Update_TextureChange(&(*iter), fDeltaTime, pInstance);
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

				if (m_tInstanceDesc.AlphaBlendON)
					iter->_CamDist = XMVectorGetX(XMVector3Length(((iter->_LocalMatirx.XMatrix().r[3] + iter->_TargetParentPosition.XMVector()) - CamPos)));

				if (m_tInstanceDesc.ColorChageFrequency)
					Update_ColorChange(&(*iter), fDeltaTime, pInstance);

				if (m_tInstanceDesc.SizeChageFrequency)
					Update_SizeChange(&(*iter), fDeltaTime, pInstance);

				Update_TextureChange(&(*iter), fDeltaTime, pInstance);
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

	if (m_tInstanceDesc.AlphaBlendON)
	{
		sort(m_vecParticleAttribute.begin(), m_vecParticleAttribute.end(), [](INSTANCEATT tSrc, INSTANCEATT tDest) ->_bool
		{
			return tSrc._CamDist > tDest._CamDist;
		});
	}


	return;



}

void CInstanceEffect::Update_SizeChange(INSTANCEATT * tParticleAtt, _double fTimeDelta, CGameInstance * pInstance)
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


		if (tParticleAtt->_size.x < 0) tParticleAtt->_size.x = 0;
		if (tParticleAtt->_size.y < 0) tParticleAtt->_size.y = 0;
		if (tParticleAtt->_size.z < 0) tParticleAtt->_size.z = 0;
	}


}

void CInstanceEffect::Update_TextureChange(INSTANCEATT * tParticleAtt, _double fTimeDelta, CGameInstance * pInstance)
{
	_uint iTotalTextureNum = m_tInstanceDesc.iFigureCount_In_Texture;

	if (iTotalTextureNum < 0)
		iTotalTextureNum = _uint(m_tInstanceDesc.vTextureXYNum.x * m_tInstanceDesc.vTextureXYNum.y);

	if (m_tInstanceDesc.TextureChageFrequency)
	{
		_double TimeInterver = tParticleAtt->_lifeTime / m_tInstanceDesc.TextureChageFrequency; //=> 15.초 파티클의 2번 진동수면 7.5가 나오는 것
		_uint iFrequencyIndex = _uint(tParticleAtt->_age / TimeInterver); //=> 7.5초 주기에 3.4초를 지나고있으면 0번째 10초를 지나고있으면 1번째를 의미함
		_double FrequencyAge = tParticleAtt->_age - (TimeInterver * iFrequencyIndex); // =>7.5초 주기에 3.4초를 지나고있으면 3.4 10초를 지나고있으면 2.5



		_uint iNowIndex = _uint(FrequencyAge / TimeInterver * iTotalTextureNum);

		_uint fX = (iNowIndex % _uint(m_tInstanceDesc.vTextureXYNum.x));
		_uint fY = (iNowIndex / _uint(m_tInstanceDesc.vTextureXYNum.x));

		tParticleAtt->_TextureUV = _float2(_float(fX) / m_tInstanceDesc.vTextureXYNum.x, _float(fY) / m_tInstanceDesc.vTextureXYNum.y);
	}
	else
		tParticleAtt->_TextureUV = _float2(0, 0);


}

void CInstanceEffect::Update_ColorChange(INSTANCEATT * tParticleAtt, _double fTimeDelta, CGameInstance * pInstance)
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


	//_double TimeInterver = tParticleAtt->_lifeTime / m_tDesc.ColorChangeFrequency; //=> 15.초 파티클의 2번 진동수면 7.5가 나오는 것
	//_uint iFrequencyIndex = _uint(tParticleAtt->_age / TimeInterver); //=> 7.5초 주기에 3.4초를 지나고있으면 0번째 10초를 지나고있으면 1번째를 의미함
	//_double FrequencyAge = tParticleAtt->_age - (TimeInterver * iFrequencyIndex); // =>7.5초 주기에 3.4초를 지나고있으면 3.4 10초를 지나고있으면 2.5

	//if (iFrequencyIndex % 2)
	//{
	//	//_float3(1, 0.64313725f, 0.141176470f);
	//	tParticleAtt->_color = pInstance->Easing_Vector(TYPE_Linear, m_tDesc.vTargetColor, m_tDesc.vStartColor, _float(FrequencyAge), _float(TimeInterver));
	//}
	//else
	//{
	//	tParticleAtt->_color = pInstance->Easing_Vector(TYPE_Linear, m_tDesc.vStartColor, m_tDesc.vTargetColor, _float(FrequencyAge), _float(TimeInterver));

	//}

	//tParticleAtt->_color.x = min(max(tParticleAtt->_color.x, 0), 1);
	//tParticleAtt->_color.y = min(max(tParticleAtt->_color.y, 0), 1);
	//tParticleAtt->_color.z = min(max(tParticleAtt->_color.z, 0), 1);


}






//CInstanceEffect * CInstanceEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
//{
//	CInstanceEffect*	pInstance = new CInstanceEffect(pDevice, pDeviceContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSGBOX("Failed to Created CInstanceEffect");
//		Safe_Release(pInstance);
//	}
//	return pInstance;
//}
//
//CGameObject * CInstanceEffect::Clone(void * pArg)
//{
//	CInstanceEffect*	pInstance = new CInstanceEffect(*this);
//
//	if (FAILED(pInstance->Initialize_Clone(pArg)))
//	{
//		MSGBOX("Failed to Created CInstanceEffect");
//		Safe_Release(pInstance);
//	}
//	return pInstance;
//}



void CInstanceEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_tInstanceDesc.FollowingTarget);

	m_vecParticleAttribute.clear();
	
}

///////////구 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region BallParticle
CInstanceEffect_Ball::CInstanceEffect_Ball(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CInstanceEffect(pDevice, pDeviceContext)
{
}

CInstanceEffect_Ball::CInstanceEffect_Ball(const CInstanceEffect_Ball & rhs)
	: CInstanceEffect(rhs)
{
}

void CInstanceEffect_Ball::Reset_Velocity(_float3 & fAttVlocity)
{
	fAttVlocity = GetSingle(CUtilityMgr)->RandomFloat3(_float3(-5, -5, -5), _float3(5, 5, 5)).Get_Nomalize();
}

void CInstanceEffect_Ball::Update_Position_by_Velocity(INSTANCEATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	position = position.XMVector() + (tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta));

	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
}

HRESULT CInstanceEffect_Ball::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTANCEATT part;
	_Vector vRight, vUp, vLook;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			part._LocalMatirx = XMMatrixIdentity();
			ResetParticle(&part);
			part._age = part._lifeTime* pUtil->RandomFloat(-1.0f, 0.f);

			m_vecParticleAttribute.push_back(part);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{
			part._LocalMatirx = XMMatrixIdentity();

			ResetParticle(&part);


			vUp = part._velocity.Get_Nomalize();
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[2])), vLook);

			part._age = part._lifeTime* pUtil->RandomFloat(-1.0f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}



	return S_OK;
}

_int CInstanceEffect_Ball::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CInstanceEffect_Ball::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CInstanceEffect_Ball * CInstanceEffect_Ball::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceEffect_Ball* pInstance = new CInstanceEffect_Ball(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CInstanceEffect_Ball");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CInstanceEffect_Ball::Clone(void * pArg)
{
	CInstanceEffect_Ball* pInstance = new CInstanceEffect_Ball(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CInstanceEffect_Ball");
		Safe_Release(pInstance);
	}

	return pInstance;
}

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////직선 형태 파티클//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region StraightParticle

CInstanceEffect_Straight::CInstanceEffect_Straight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CInstanceEffect(pDevice, pDeviceContext)
{
}

CInstanceEffect_Straight::CInstanceEffect_Straight(const CInstanceEffect_Straight & rhs)
	: CInstanceEffect(rhs)
{
}

void CInstanceEffect_Straight::Reset_Velocity(_float3 & fAttVlocity)
{
	fAttVlocity = m_vUp.Get_Nomalize();
}

void CInstanceEffect_Straight::Update_Position_by_Velocity(INSTANCEATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);
	position = position.XMVector() + tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));

}

HRESULT CInstanceEffect_Straight::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTANCEATT part;
	_Vector vRight, vUp, vLook;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			part._LocalMatirx = XMMatrixIdentity();
			ResetParticle(&part);
			part._age = part._lifeTime* pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{
			ResetParticle(&part);

			vUp = part._velocity.Get_Nomalize();
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[2])), vLook);

			part._age = part._lifeTime * pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}



	return S_OK;
}

_int CInstanceEffect_Straight::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CInstanceEffect_Straight::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CInstanceEffect_Straight * CInstanceEffect_Straight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceEffect_Straight* pInstance = new CInstanceEffect_Straight(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CInstanceEffect_Straight");
		Safe_Release(pInstance);

	}
	return pInstance;
}


CGameObject * CInstanceEffect_Straight::Clone(void * pArg)
{
	CInstanceEffect_Straight* pInstance = new CInstanceEffect_Straight(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CInstanceEffect_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////원뿔 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region ConeParticle

CInstanceEffect_Cone::CInstanceEffect_Cone(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CInstanceEffect(pDevice, pDeviceContext)
{
}

CInstanceEffect_Cone::CInstanceEffect_Cone(const CInstanceEffect_Cone & rhs)
	: CInstanceEffect(rhs)
{
}

void CInstanceEffect_Cone::Reset_Velocity(_float3 & fAttVlocity)
{

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	fAttVlocity = XMVector3Normalize(m_vRight.XMVector() * pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange.x, m_tInstanceDesc.SubPowerRandomRange.x)
		+ m_vLook.XMVector() *  pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange.y , m_tInstanceDesc.SubPowerRandomRange.y)
		+ m_vUp.Get_Nomalize()	* pUtil->RandomFloat(0, m_tInstanceDesc.SubPowerRandomRange.z));

}

void CInstanceEffect_Cone::Update_Position_by_Velocity(INSTANCEATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);
	position = position.XMVector() + tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
}

HRESULT CInstanceEffect_Cone::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTANCEATT part;
	_Vector vRight, vUp, vLook;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			part._LocalMatirx = XMMatrixIdentity();
			ResetParticle(&part);
			part._age = part._lifeTime* pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			ResetParticle(&part);

			vUp = part._velocity.Get_Nomalize();
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[2])), vLook);

			part._age = part._lifeTime * pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}



	return S_OK;
}

_int CInstanceEffect_Cone::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CInstanceEffect_Cone::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CInstanceEffect_Cone * CInstanceEffect_Cone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceEffect_Cone* pInstance = new CInstanceEffect_Cone(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CInstanceEffect_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInstanceEffect_Cone::Clone(void * pArg)
{
	CInstanceEffect_Cone* pInstance = new CInstanceEffect_Cone(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CInstanceEffect_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}
#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////퍼지는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SpreadParticle

CInstanceEffect_Spread::CInstanceEffect_Spread(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CInstanceEffect(pDevice, pDeviceContext)
{
}

CInstanceEffect_Spread::CInstanceEffect_Spread(const CInstanceEffect_Spread & rhs)
	: CInstanceEffect(rhs)
{
}

void CInstanceEffect_Spread::Reset_Velocity(_float3 & fAttVlocity)
{

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
	_float Rand1 = pUtil->RandomFloat(-1.f, 1.f);
	_float Rand2 = pUtil->RandomFloat(-1.f, 1.f);


	fAttVlocity = XMVector3Normalize(m_vUp.XMVector() * Rand1 + m_vRight.XMVector() * Rand2);


}

void CInstanceEffect_Spread::Update_Position_by_Velocity(INSTANCEATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);
	position = position.XMVector() + tParticleAtt->_velocity.XMVector() * tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
}

HRESULT CInstanceEffect_Spread::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTANCEATT part;
	_Vector vRight, vUp, vLook;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			part._LocalMatirx = XMMatrixIdentity();
			ResetParticle(&part);
			part._age = part._lifeTime* pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			ResetParticle(&part);

			vUp = part._velocity.Get_Nomalize();
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[2])), vLook);

			part._age = part._lifeTime * pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}


	return S_OK;	
}

_int CInstanceEffect_Spread::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CInstanceEffect_Spread::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}


CInstanceEffect_Spread * CInstanceEffect_Spread::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceEffect_Spread* pInstance = new CInstanceEffect_Spread(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CInstanceEffect_Spread");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInstanceEffect_Spread::Clone(void * pArg)
{
	CInstanceEffect_Spread* pInstance = new CInstanceEffect_Spread(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CInstanceEffect_Spread");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////분수 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region FountainParticle

CInstanceEffect_Fountain::CInstanceEffect_Fountain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CInstanceEffect(pDevice, pDeviceContext)
{
}

CInstanceEffect_Fountain::CInstanceEffect_Fountain(const CInstanceEffect_Fountain & rhs)
	: CInstanceEffect(rhs)
{
}

void CInstanceEffect_Fountain::Reset_Velocity(_float3 & fAttVlocity)
{
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	fAttVlocity = XMVector3Normalize(m_vRight.XMVector() * pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange.x, m_tInstanceDesc.SubPowerRandomRange.x)
		+ m_vLook.XMVector() *  pUtil->RandomFloat(-m_tInstanceDesc.SubPowerRandomRange.y, m_tInstanceDesc.SubPowerRandomRange.y)
		+ m_vUp.Get_Nomalize()	* pUtil->RandomFloat(m_tInstanceDesc.SubPowerRandomRange.z, m_tInstanceDesc.SubPowerRandomRange.z * 2.f));
	

	if (fAttVlocity.y < 0) fAttVlocity.y *= -1.f;

}

void CInstanceEffect_Fountain::Update_Position_by_Velocity(INSTANCEATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);

	_float Rate = tParticleAtt->_age / tParticleAtt->_lifeTime;
	_float3 Dir = tParticleAtt->_velocity;


	if (Rate < 0.7f)
	{
		Dir.y = g_pGameInstance->Easing(TYPE_SinOut, Dir.y, 0, Rate, tParticleAtt->_lifeTime* 0.7f);
		position = position.XMVector() + Dir.Get_Nomalize() * tParticleAtt->_force * _float(fTimeDelta);
	}
	else
	{
		Dir.y = g_pGameInstance->Easing(TYPE_SinIn,  0, -Dir.y * 0.3f, Rate - tParticleAtt->_lifeTime* 0.7f, tParticleAtt->_lifeTime * 0.3f);
		position = position.XMVector() + Dir.Get_Nomalize()* tParticleAtt->_force * _float(fTimeDelta);
	}

	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));

	if (!m_tInstanceDesc.bBillboard)
	{

		_Vector vRight, vUp, vLook;


		vUp = Dir.Get_Nomalize();
		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
		vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

		XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[0])), vRight);
		XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[1])), vUp);
		XMStoreFloat4(((_float4*)(&tParticleAtt->_LocalMatirx.m[2])), vLook);


	}
}

HRESULT CInstanceEffect_Fountain::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	INSTANCEATT part;
	_Vector vRight, vUp, vLook;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			part._LocalMatirx = XMMatrixIdentity();
			ResetParticle(&part);
			part._age = part._lifeTime* pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			ResetParticle(&part);

			vUp = part._velocity.Get_Nomalize();
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[2])), vLook);

			part._age = part._lifeTime * pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}


	return S_OK;
}

_int CInstanceEffect_Fountain::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CInstanceEffect_Fountain::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CInstanceEffect_Fountain * CInstanceEffect_Fountain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceEffect_Fountain* pInstance = new CInstanceEffect_Fountain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CInstanceEffect_Fountain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInstanceEffect_Fountain::Clone(void * pArg)
{
	CInstanceEffect_Fountain* pInstance = new CInstanceEffect_Fountain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CInstanceEffect_Fountain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

#pragma endregion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////빨아들이는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region SuckParticle

CInstanceEffect_Suck::CInstanceEffect_Suck(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CInstanceEffect(pDevice, pDeviceContext)
{
}

CInstanceEffect_Suck::CInstanceEffect_Suck(const CInstanceEffect_Suck & rhs)
	: CInstanceEffect(rhs)
{
}

void CInstanceEffect_Suck::Reset_Velocity(_float3 & fAttVlocity)
{

}

void CInstanceEffect_Suck::Update_Position_by_Velocity(INSTANCEATT * tParticleAtt, _double fTimeDelta)
{
	_float3 position = *(_float3*)(&tParticleAtt->_LocalMatirx._41);
	_float3 Oldposition = position;
	position = position.XMVector() + position.Get_Nomalize() * -tParticleAtt->_force * _float(fTimeDelta);
	memcpy(&tParticleAtt->_LocalMatirx._41, &position, sizeof(_float3));
	

	if (Oldposition.x * position.x < 0)
	{
		if (m_PassedTime < m_tInstanceDesc.TotalParticleTime)
			ResetParticle(tParticleAtt);
		else
			tParticleAtt->_isAlive = false;
	}

}

HRESULT CInstanceEffect_Suck::Initialize_Child_Clone()
{
	m_vecParticleAttribute.clear();

	m_tInstanceDesc.fMaxBoundaryRadius = 99999.f;

	INSTANCEATT part;
	_Vector vRight, vUp, vLook;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	if (m_tInstanceDesc.bBillboard)
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			part._LocalMatirx = XMMatrixIdentity();
			ResetParticle(&part);
			part._age = part._lifeTime* pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; i++)
		{

			ResetParticle(&part);

			vUp = part._velocity.Get_Nomalize();
			vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), vUp));
			vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[0])), vRight);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[1])), vUp);
			XMStoreFloat4(((_float4*)(&part._LocalMatirx.m[2])), vLook);

			part._age = part._lifeTime * pUtil->RandomFloat(-0.5f, 0.f);
			m_vecParticleAttribute.push_back(part);
		}
	}


	return S_OK;
}

_int CInstanceEffect_Suck::Update(_double fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta)) return -1;
	return _int();
}

_int CInstanceEffect_Suck::LateUpdate(_double fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta)) return -1;
	return _int();
}

CInstanceEffect_Suck * CInstanceEffect_Suck::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CInstanceEffect_Suck* pInstance = new CInstanceEffect_Suck(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Fail to Create CInstanceEffect_Suck");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInstanceEffect_Suck::Clone(void * pArg)
{
	CInstanceEffect_Suck* pInstance = new CInstanceEffect_Suck(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CInstanceEffect_Suck");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion