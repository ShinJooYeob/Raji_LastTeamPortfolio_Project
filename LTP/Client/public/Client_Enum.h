#pragma once

enum ECameraMode
{
	CAM_MODE_FREE, CAM_MODE_NOMAL
};

enum UPGRADEID
{
	Upgrade_Vopal, Upgrade_PaperGrinder, Upgrade_Horse, Upgrade_Teapot, Upgrade_End
};

enum SCENEID
{
	SCENE_STATIC,
	SCENE_LOBY,
	SCENE_LOADING,
	SCENE_STAGESELECT,
	SCENE_STAGE1,
	SCENE_STAGE2,
	SCENE_STAGE3,
	SCENE_STAGE4,
	SCENE_STAGE5,
	SCENE_STAGE6,
	SCENE_STAGE7,
	SCENE_BOSS,
	SCENE_ENDING,

	SCENE_EDIT,
	SCENE_END
};
static const char* Tag_ScenenName(SCENEID eTag)
{
	switch (eTag)
	{
	case SCENE_STATIC: 
		return "SCENE_STATIC";
	case SCENE_LOBY:
		return "SCENE_LOBY";
	case SCENE_LOADING:
		return "SCENE_LOADING";
	case SCENE_STAGESELECT:
		return "SCENE_STAGESELECT";
	case SCENE_STAGE1:
		return "SCENE_STAGE1";
	case SCENE_STAGE2:
		return "SCENE_STAGE2";
	case SCENE_STAGE3:
		return "SCENE_STAGE3";
	case SCENE_STAGE4:
		return "SCENE_STAGE4";
	case SCENE_STAGE5:
		return "SCENE_STAGE5";
	case SCENE_STAGE6:
		return "SCENE_STAGE6";
	case SCENE_STAGE7:
		return "SCENE_STAGE7";
	case SCENE_BOSS:
		return "SCENE_BOSS";
	case SCENE_ENDING:
		return "SCENE_ENDING";
	case SCENE_EDIT:
		return "SCENE_EDIT";
	default:
		MSGBOX("Wrong Type SceneNumber");
		return nullptr;
		break;
	}

}

#define  TAG_SCENE Tag_ScenenName





enum OBJECTPROTOTYPEID
{
	Prototype_Camera_Main,
	Prototype_Player,
	Prototype_PlayerWeapon_Spear,
	Prototype_PlayerWeapon_Bow,
	Prototype_PlayerWeapon_Sword,

	Prototype_SkyBox,

	Prototype_UIImage,

	Prototype_Rect,
	Prototype_Cube,
	Prototype_Terrain,


	Prototype_StaticMapObject,


	Prototype_TestObject,
	Prototype_EditorCursor,
	Prototype_EditorTerrain,
	Prototype_Camera_Editor,
	Prototype_TestObject_Himeko,
	Prototype_NaviPoint,


	Prototype_Object_Monster_Mahinasura_Minion,
	Prototype_Object_Monster_Mahinasura_Leader,
	Prototype_Object_Monster_Vayusura_Minion,
	Prototype_Object_Monster_Vayusura_Leader,
	Prototype_Obejct_Monster_Tezabsura_Minion,
	Prototype_Obejct_Monster_Tezabsura_Purple,


	//JJB
	Prototype_Object_Boss_Rangda,
	Prototype_Object_Boss_Chiedtian,
	Prototype_Object_Boss_ChiedtianWeapon,
	Prototype_Object_Effect_MagicCircle,

	//Monster_Bullet
	Prototype_Object_Monster_Bullet_Universal,

	Object_Prototype_End
};



static const _tchar* Tag_Object_Prototype(OBJECTPROTOTYPEID eTag)
{
	switch (eTag)
	{


	case Prototype_Camera_Main:
		return TEXT("Prototype_Camera_Main");
		break;
	case Prototype_SkyBox:
		return TEXT("Prototype_SkyBox");
		break;
	case Prototype_UIImage:
		return TEXT("Prototype_UI_Image");
		break;


	case Prototype_Rect:
		return TEXT("Prototype_Rect");
		break;
	case Prototype_Cube:
		return TEXT("Prototype_Cube");
		break;
	case Prototype_Terrain:
		return TEXT("Prototype_Terrain");
		break;
	case Prototype_Player:
		return TEXT("Prototype_Player");
		break;
	case Prototype_PlayerWeapon_Spear:
		return TEXT("Prototype_PlayerWeapon_Spear");
		break;
	case Prototype_PlayerWeapon_Bow:
		return TEXT("Prototype_PlayerWeapon_Bow");
		break;
	case Prototype_PlayerWeapon_Sword:
		return TEXT("Prototype_PlayerWeapon_Sword");
		break;

	case Prototype_StaticMapObject:
		return TEXT("Prototype_StaticMapObject");
		break;
		//////////////////////////////////////////////////////////////////////////

	case Prototype_TestObject:
		return TEXT("Prototype_TestObject");
		break;
	case Prototype_EditorCursor:
		return TEXT("Prototype_EditorCursor");
		break;
	case Prototype_EditorTerrain:
		return TEXT("Prototype_EditorTerrain");
		break;
	case Prototype_Camera_Editor:
		return TEXT("Prototype_Camera_Editor");
		break;

	case Prototype_TestObject_Himeko: 
		return TEXT("Prototype_TestObject_Himeko");
		break;

	case Prototype_NaviPoint:
		return TEXT("Prototype_NaviPoint");
		break;

	case Prototype_Object_Monster_Mahinasura_Minion:
		return TEXT("Prototype_Object_Monster_Mahinasura_Minion");

	case Prototype_Object_Monster_Mahinasura_Leader:
		return TEXT("Prototype_Object_Monster_Mahinasura_Leader");

	case Prototype_Object_Monster_Vayusura_Minion:
		return TEXT("Prototype_Object_Monster_Vayusura_Minion");

	case Prototype_Object_Monster_Vayusura_Leader:
		return TEXT("Prototype_Object_Monster_Vayusura_Leader");

	case Prototype_Obejct_Monster_Tezabsura_Minion:
		return TEXT("Prototype_Object_Monster_Tezabsura_Minion");

	case Prototype_Obejct_Monster_Tezabsura_Purple:
		return TEXT("Prototype_Object_Monster_Tezabsura_Purple");

	case Prototype_Object_Boss_Rangda:
		return TEXT("Prototype_Object_Boss_Rangda");
		break;

	case Prototype_Object_Boss_Chiedtian:
		return TEXT("Prototype_Object_Boss_Chiedtian");
		break;

	case Prototype_Object_Boss_ChiedtianWeapon:
		return TEXT("Prototype_Object_Boss_ChiedtianWeapon");
		break;

	case Prototype_Object_Effect_MagicCircle:
		return TEXT("Prototype_Object_Effect_MagicCircle");

	case Prototype_Object_Monster_Bullet_Universal:
		return TEXT("Prototype_Object_Monster_Bullet_Universal");
	default:
		MSGBOX("Wrong Type Object Prototype");
		return nullptr;
		break;
	}


}
#define  TAG_OP Tag_Object_Prototype

//
//enum MESHTYPEID
//{
//	Mesh_None,
//	Mesh_Player,
//	Mesh_Monster,
//
//
//
//	Mesh_AlgaeRock_Ledge,
//	Mesh_AlgaeRock_Pillar,
//	Mesh_AlgaeRock_Wall,
//
//	MeshID_End
//};
//
//
//
//static const _tchar* Tag_MeshID(MESHTYPEID eTag)
//{
//	switch (eTag)
//	{
//	case Mesh_None:
//		return TEXT("Mesh_None");
//		break;
//
//	case Mesh_Monster:
//		return TEXT("Monster");
//		break;
//	case Mesh_Player:
//		return TEXT("Player");
//		break;
//
//
//
//
//
//
//
//
//		//////////////////////////////////////////////////////////////////////////
//	case Mesh_AlgaeRock_Ledge:
//		return TEXT("Mesh_AlgaeRock_Ledge");
//		break;
//	case Mesh_TestMesh2:
//		return TEXT("TestMesh2");
//		break;
//	case Mesh_TestMesh3:
//		return TEXT("TestMesh3");
//		break;
//	case Mesh_TestMesh4:
//		return TEXT("TestMesh4");
//		break;
//
//	default:
//		MSGBOX("Wrong Type Mesh");
//		return nullptr;
//		break;
//	}
//
//
//}
//#define  TAG_MESH Tag_MeshID

enum LAYERID
{
	Layer_Camera_Main,
	Layer_Camera_Editor,
	Layer_ScreenEffect,
	Layer_SkyBox,
	Layer_Player,
	Layer_PlayerWeapon,
	Layer_Bullet,
	Layer_ClockBomb,

	Layer_Monster,
	Layer_MonsterBullet,
	Layer_MonsterBullet1,
	Layer_MonsterBullet2,
	Layer_Particle,

	Layer_Terrain,
	Layer_WireTerrain,


	Layer_JumpPad,
	Layer_SteamPad,
	Layer_HiddenPad,
	Layer_TeethObj,
	Layer_RoseObj,
	Layer_Breakable,
	Layer_BreakablePiece,
	Layer_SlideObj,

	Layer_EscalatorPad,
	Layer_ButtonPad,
	Layer_ShrinkFlower,
	Layer_Vent,
	Layer_MazeDoor,
	Layer_ShapeMemoryPad,
	Layer_Presser,



	Layer_StaticMapObj,
	Layer_TriggerCollider,


	Layer_UI_GamePlay,
	Layer_NPC,



	Layer_UI_IMG,
	Layer_UI_BTN,
	Layer_UI_SLIDE,
	Layer_UI_INPUT,

	Layer_TestObject,
	Layer_TestEffect,

};

static const _tchar* Tag_Layer(LAYERID eTag)
{
	switch (eTag)
	{
		
	case Layer_NPC:
		return TEXT("Layer_NPC");
		break;

	case Layer_Camera_Main:
		return TEXT("Layer_Camera_Main");
		break;
	case Layer_Camera_Editor:
		return TEXT("Layer_Camera_Editor");
		break;
	case Layer_ScreenEffect:
		return TEXT("Layer_ScreenEffect");
		break;
	case Layer_TriggerCollider:
		return TEXT("Layer_TriggerCollider");
		break;
		
		

	case Layer_Bullet:
		return TEXT("Layer_Bullet");
		break;
	case Layer_SkyBox:
		return TEXT("Layer_SkyBox");
		break;
	case Layer_Terrain:
		return TEXT("Layer_Terrain");
		break;
	case Layer_WireTerrain:
		return TEXT("Layer_WireTerrain");
		break;
	case Layer_Particle:
		return TEXT("Layer_Particle");
		break;
	case Layer_ClockBomb:
		return TEXT("Layer_ClockBomb");
		break;
		

	case Layer_Monster:
		return TEXT("Layer_Monster");
		break;

	case Layer_MonsterBullet:
		return TEXT("Layer_MonsterBullet");
		break;
	case Layer_MonsterBullet1:
		return TEXT("Layer_MonsterBullet1");
		break;
	case Layer_MonsterBullet2:
		return TEXT("Layer_MonsterBullet2");
		break;


	case Layer_Player:
		return TEXT("Layer_Player");
		break;
	case Layer_PlayerWeapon:
		return TEXT("Layer_PlayerWeapon");
		break;
	case Layer_JumpPad:
		return TEXT("Layer_JumpPad");
		break;
	case Layer_SteamPad:
		return TEXT("Layer_SteamPad");
		break;
	case Layer_HiddenPad:
		return TEXT("Layer_HiddenPad");
		break;
	case Layer_TeethObj:
		return TEXT("Layer_TeethObj");
		break;
	case Layer_RoseObj:
		return TEXT("Layer_RoseObj");
		break;
	case Layer_Breakable:
		return TEXT("Layer_Breakable");
		break;
	case Layer_BreakablePiece:
		return TEXT("Layer_BreakablePiece");
		break;
	case Layer_SlideObj:
		return TEXT("Layer_SlideObj");
		break;
	case Layer_EscalatorPad:
		return TEXT("Layer_EscalatorPad");
		break;
	case Layer_ButtonPad:
		return TEXT("Layer_ButtonPad");
		break;
	case Layer_ShrinkFlower:
		return TEXT("Layer_ShrinkFlower");
		break;
	case Layer_Vent:
		return TEXT("Layer_Vent");
		break;
	case Layer_MazeDoor:
		return TEXT("Layer_MazeDoor");
		break;
	case Layer_ShapeMemoryPad:
		return TEXT("Layer_ShapeMemoryPad");
		break;
	case Layer_Presser:
		return TEXT("Layer_Presser");
		break;




	case Layer_StaticMapObj:
		return TEXT("Layer_StaticMapObj");
		break;

	case Layer_UI_GamePlay:
		return TEXT("Layer_UI_GamePlay");
		break;

	case Layer_UI_IMG:
		return TEXT("Layer_UI_IMG");
		break;
	case Layer_UI_BTN:
		return TEXT("Layer_UI_BTN");
		break;
	case Layer_UI_SLIDE:
		return TEXT("Layer_UI_SLIDE");
		break;
	case Layer_UI_INPUT:
		return TEXT("Layer_UI_INPUT");
		break;

		//////////////////////////////////////////////////////////////////////////

	case Layer_TestObject:
		return TEXT("Layer_TestObject");
		break;

	case Layer_TestEffect:
		return TEXT("Layer_TestEffect");
		break;
		
	default:
		MSGBOX("Wrong Type Layer");
		return nullptr;
		break;
	}


}
#define  TAG_LAY Tag_Layer

enum COMPONENTPROTOTYPEID
{
	/////////////////기본 컴포넌트
	Prototype_Renderer,
	Prototype_Transform,
	Prototype_Collider,
	Prototype_Trail,
	Prototype_Navigation,
	Prototype_Shader_VT,
	Prototype_Shader_VCT,
	Prototype_Shader_VNT,
	Prototype_Shader_VNCT,
	Prototype_Shader_VAM,
	Prototype_Shader_VNAM,
	Prototype_Shader_VTXPOINTINST,

	//////////////버퍼////////////////////////////////////////////////////////////
	Prototype_VIBuffer_Rect,
	Prototype_VIBuffer_Cube,
	Prototype_VIBuffer_Terrain,
	Prototype_VIBuffer_Terrain_Edit,
	Prototype_VIBuffer_Point_Instance,

	//////StaticMesh//////////None하고 알게락 레지 사이에다가만 넣을 것//////////////////////////////////////////////////////////
	Prototype_Mesh_None,
	Prototype_Mesh_TEST_STATIC,



	Prototype_Mesh_AlgaeRock_Ledge,
	//////다이나믹Mesh///////////////플레이어하고 테스트 오브젝트 사이에다가만 넣을 것/////////////////////////////////////////////////////
	Prototype_Mesh_Player,
	Prototype_Mesh_PlayerWeapon_Spear,
	Prototype_Mesh_PlayerWeapon_Bow,
	Prototype_Mesh_PlayerWeapon_Sword,
	Prototype_Mesh_Monster_Mahinasura_Minion,
	Prototype_Mesh_Monster_Mahinasura_Leader,
	Prototype_Mesh_Monster_Vayusura_Minion,
	Prototype_Mesh_Monster_Vayusura_Leader,
	Prototype_Mesh_Monster_Tezabsura_Minion,
	Prototype_Mesh_Monster_Tezabsura_Purple,
	Prototype_Mesh_Monster_Ninjasura,
	Prototype_Mesh_Monster_Jalsura,
	Prototype_Mesh_Monster_Gadasura_Black,
	Prototype_Mesh_Monster_Gadasura_Rage,
	Prototype_Mesh_Monster_Gadasura_Rage_Hollogram,


	Prototype_Mesh_Boss_Rangda,
	Prototype_Mesh_Boss_Chieftian,
	Prototype_Mesh_Boss_ChieftianWeapon,


	Prototype_Mesh_SkyBox,
	Prototype_Mesh_TestObject_Himeko,
	Prototype_Mesh_TestObject,
	/////////텍스쳐/////////////////////////////////////////////////////////////////

	Prototype_Texture_Player,
	Prototype_Texture_SkyBox,
	Prototype_Texture_EditScene,
	Prototype_Texture_Terrain,
	Prototype_Texture_Edit_Terrain,
	Prototype_Texture_DefaultUI,
	Prototype_Texture_InstanceParticleTex,
	Prototype_Texture_ScreenEffectUI,
	Prototype_Texture_PlayerEffect,
	Prototype_Texture_DissolveEffect,
	Prototype_Texture_GamePlayScene,
	Prototype_Texture_LoadingScene,
	Prototype_Texture_LobyScene,
	Prototype_Texture_SettingScene,
	Prototype_Texture_MskTex,
	Prototype_Texture_PauseUI,
	Prototype_Texture_TestEffect,


};
static const _tchar* Tag_Component_Prototype(COMPONENTPROTOTYPEID eTag)
{
	switch (eTag)
	{

	case Prototype_Renderer:
		return TEXT("Prototype_Component_Renderer");
		break;
	case Prototype_Transform:
		return TEXT("Prototype_Component_Transform");
		break;
	case Prototype_Collider:
		return TEXT("Prototype_Component_Collider");
		break;
	case Prototype_Trail:
		return TEXT("Prototype_Component_Trail");
		break;

	case Prototype_Navigation:
		return TEXT("Prototype_Component_Navigation");
		break;

	case Prototype_Shader_VT:
		return TEXT("Prototype_Component_Shader_VTXTEX");
		break;
	case Prototype_Shader_VCT:
		return TEXT("Prototype_Component_Shader_VTXCUBETEX");
		break;

	case Prototype_Shader_VNT:
		return TEXT("Prototype_Component_Shader_VTXNORMTEX");
		break;

	case Prototype_Shader_VNCT:
		return TEXT("Prototype_Component_Shader_VTXNORMCUBETEX");
		break;
	case Prototype_Shader_VAM:
		return TEXT("Prototype_Component_Shader_VTXANIMMODEL");
		break;
	case Prototype_Shader_VNAM:
		return TEXT("Prototype_Component_Shader_VTXNONANIMMODEL");
		break;
	case Prototype_Shader_VTXPOINTINST:
		return TEXT("Prototype_Component_Shader_VTXPOINTINST");
		break;
	case Prototype_VIBuffer_Rect:
		return TEXT("Prototype_Component_VIBuffer_Rect");
		break;

	case Prototype_VIBuffer_Cube:
		return TEXT("Prototype_Component_VIBuffer_Cube");
		break;

	case Prototype_VIBuffer_Point_Instance:
		return TEXT("Prototype_VIBuffer_Point_Instance");
		break;
		

		//메쉬////////////////////////////////////////////////////////////////////////

	case Prototype_Mesh_None:
		return TEXT("Prototype_Mesh_None");
		break;
		
	case Prototype_Mesh_TEST_STATIC:
		return TEXT("Prototype_Mesh_TEST_STATIC");

	case Prototype_Mesh_AlgaeRock_Ledge:
		return TEXT("Prototype_Mesh_AlgaeRock_Ledge");
		break;
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		

	case Prototype_Mesh_Player:
		return TEXT("Player.fbx");
		break;

	case Prototype_Mesh_PlayerWeapon_Spear:
		return TEXT("PlayerSpear.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Bow:
		return TEXT("PlayerBow.fbx");
		break;
	case Prototype_Mesh_PlayerWeapon_Sword:
		return TEXT("PlayerSword.fbx");
		break;

	case Prototype_Mesh_Monster_Mahinasura_Minion:
		return TEXT("Monster_Mahinasura_Minion.fbx");
		break;

	case Prototype_Mesh_Monster_Mahinasura_Leader:
		return TEXT("Monster_Mahinasura_Leader.fbx");
		break;

	case Prototype_Mesh_Monster_Vayusura_Minion:
		return TEXT("Monster_Vayusura_Minion.fbx");
		break;

	case Prototype_Mesh_Monster_Vayusura_Leader:
		return TEXT("Monster_Vayusura_Leader.fbx");
		break;

	case Prototype_Mesh_Monster_Tezabsura_Minion:
		return TEXT("Monster_Tezabsura_Minion.fbx");
		break;

	case Prototype_Mesh_Monster_Tezabsura_Purple:
		return TEXT("Monster_Tezabsura_Purple.fbx");
		break;

	case Prototype_Mesh_Monster_Ninjasura:
		return TEXT("Monster_Ninjasura_GLTF.fbx");
		break;
	case Prototype_Mesh_Monster_Jalsura:
		return TEXT("Monster_Jalsura.fbx");
		break;

	case Prototype_Mesh_Monster_Gadasura_Black:
		return TEXT("Monster_Gadasura_Black.fbx");
		break;
	case Prototype_Mesh_Monster_Gadasura_Rage:
		return TEXT("Monster_Gadasura_Rage.fbx");
		break;
	case Prototype_Mesh_Monster_Gadasura_Rage_Hollogram:
		return TEXT("Monster_Gadasura_Rage_Hollogram.fbx");
		break;
		
	case Prototype_Mesh_Boss_Rangda:
		return TEXT("Boss_Randa.fbx");
		break;

	case Prototype_Mesh_Boss_Chieftian:
		return TEXT("Boss_Chiedtian.fbx");
		break;

	case Prototype_Mesh_Boss_ChieftianWeapon:
		return TEXT("Chieft_Weapon.fbx");
		break;

	case Prototype_Mesh_TestObject:
		return TEXT("Prototype_Mesh_TestObject");
		break;

	case Prototype_Mesh_TestObject_Himeko:
		return TEXT("Prototype_Mesh_TestObject_Himeko");
		break;

	case Prototype_Mesh_SkyBox:
		return TEXT("Prototype_Mesh_SkyBox");
		break;


		/////////////////텍스처////////////////////////////////////////////////////////


	case Prototype_Texture_Player:
		return TEXT("Prototype_Component_Texture_Player");
		break;
	case Prototype_Texture_PlayerEffect:
		return TEXT("Prototype_Texture_PlayerEffect");
		break;
	case Prototype_Texture_DissolveEffect:
		return TEXT("Prototype_Texture_DissolveEffect");
		break;
	case Prototype_Texture_ScreenEffectUI:
		return TEXT("Prototype_Texture_ScreenEffectUI");
		break;
		
	case Prototype_Texture_SkyBox:
		return TEXT("Prototype_Component_Texture_SkyBox");
		break;

	case Prototype_Texture_EditScene:
		return TEXT("Prototype_Component_Texture_EditScene");

		break;
	case 	Prototype_Texture_Terrain:
		return TEXT("Prototype_Component_Texture_Terrain");

		break;
	case 	Prototype_Texture_Edit_Terrain:
		return TEXT("Prototype_Texture_Edit_Terrain");

	case 	Prototype_Texture_DefaultUI:
		return TEXT("Prototype_Texture_DefaultUI");
		break;


	case 	Prototype_Texture_GamePlayScene:
		return TEXT("Prototype_Texture_GamePlayScene");
		break;

	case 	Prototype_Texture_LoadingScene:
		return TEXT("Prototype_Texture_LoadingScene");
		break;

	case 	Prototype_Texture_LobyScene:
		return TEXT("Prototype_Texture_LobyScene");
		break;

		case
			Prototype_Texture_SettingScene:
				return TEXT("Prototype_Texture_SettingScene");
				break;

		case Prototype_Texture_MskTex:
			return TEXT("Prototype_Texture_MskTex");
			break;
		case Prototype_Texture_InstanceParticleTex:
			return TEXT("Prototype_Texture_InstanceParticleTex");
			break;
		case Prototype_Texture_PauseUI:
			return TEXT("Prototype_Texture_PauseUI");
			break;

		case Prototype_Texture_TestEffect:
			return TEXT("Prototype_Texture_TestEffect");
			break;
			
				//메쉬////////////////////////////////////////////////////////////////////////







				break;
		case 	Prototype_VIBuffer_Terrain:
			return TEXT("Prototype_Component_VIBuffer_Terrain");
			break;
		case 	Prototype_VIBuffer_Terrain_Edit:
			return TEXT("Prototype_Component_VIBuffer_Terrain_Edit");
			break;



			//////////////////////////////////////////////////////////////////////////
		default:
			MSGBOX("Wrong Type Layer");
			return nullptr;
			break;
	}


}

#define  TAG_CP Tag_Component_Prototype

enum COMPONENTID
{
	Com_Renderer,
	Com_VIBuffer,
	Com_Transform,
	Com_SubTransform,
	Com_Texture,
	Com_Inventory,
	Com_Shader,
	Com_ShaderSub,
	Com_Model,
	Com_SubModel,
	Com_Collider,
	Com_SwordTrail,
	Com_SubSwordTrail,
	Com_Navaigation
};
static const _tchar* Tag_Component(COMPONENTID eTag)
{
	switch (eTag)
	{
	case Com_Renderer:
		return TEXT("Com_Renderer");
		break;
	case Com_VIBuffer:
		return TEXT("Com_VIBuffer");
		break;
	case Com_Transform:
		return TEXT("Com_Transform");
		break;
	case Com_SubTransform:
		return TEXT("Com_SubTransform");
		break;
		
	case Com_Texture:
		return TEXT("Com_Texture");
		break;
	case Com_Inventory:
		return TEXT("Com_Inventory");
		break;
	case Com_Shader:
		return TEXT("Com_Shader");
		break;
	case Com_ShaderSub:
		return TEXT("Com_ShaderSub");
		break;
	case Com_Model:
		return TEXT("Com_Model");
		break;
	case Com_SubModel:
		return TEXT("Com_SubModel");
		break;

	case Com_Collider:
		return TEXT("Com_Collider");
		break;

	case Com_SwordTrail:
		return TEXT("Com_SwordTrail");
		break;
	case Com_SubSwordTrail:
		return TEXT("Com_SubSwordTrail");
		break;
	case Com_Navaigation:
		return TEXT("Com_Navigation");
		break;
		
		

		//////////////////////////////////////////////////////////////////////////
	default:
		MSGBOX("Wrong Type Component");
		return nullptr;
		break;
	}


}
#define  TAG_COM Tag_Component



static const char* Tag_ModelTextureType(_uint eTag)
{
	switch (eTag)
	{
	case aiTextureType_NONE:		return "None";
		break;
	case aiTextureType_DIFFUSE:	return "g_DiffuseTexture";
		break;
	case aiTextureType_SPECULAR:return "g_SpecularTexture";
		break;
	case aiTextureType_AMBIENT:return "g_AmbientTexture";
		break;
	case aiTextureType_EMISSIVE:return "g_EmissiveTexture";
		break;
	case aiTextureType_HEIGHT:return "g_HeightTexture";
		break;
	case aiTextureType_NORMALS:return "g_NormalTexture";
		break;
	case aiTextureType_SHININESS:return "g_ShininessTexture";
		break;
	case aiTextureType_OPACITY:return "g_OpacityTexture";
		break;
	case aiTextureType_DISPLACEMENT:return "g_DisplaceTexture";
		break;
	case aiTextureType_LIGHTMAP:return "g_LightMapTexture";
		break;
	case aiTextureType_REFLECTION:return "g_ReflectTexture";
		break;
	case aiTextureType_BASE_COLOR:return "g_BaseColorTexture";
		break;
	case aiTextureType_NORMAL_CAMERA:return "g_NormalCamTexture";
		break;
	case aiTextureType_EMISSION_COLOR:return "g_EmissionColorTexture";
		break;
	case aiTextureType_METALNESS:return "g_MetalTexture";
		break;
	case aiTextureType_DIFFUSE_ROUGHNESS:return "g_DiffuseRoughTexture";
		break;
	case aiTextureType_AMBIENT_OCCLUSION:return "g_AmbientOcculusionTexture";
		break;
	case aiTextureType_UNKNOWN:return "";
		break;
	case _aiTextureType_Force32Bit:return "";
		break;
	default:
		OutputDebugStringW(L"Wrong Type Texture");
		__debugbreak();
		return nullptr;
		break;
	}
}

#define  MODLETEXTYPE Tag_ModelTextureType


enum BULLETMESHID
{
	Prototype_Mesh_Monster_Bullet_Vayusura_Leader,
	Prototype_Mesh_Monster_Bullet_Tezabsura_Minion,
	Prototype_Mesh_Monster_Bullet_Tezabsura_Purple,
	Tezabsura_Purple_Default_BUllet,
	BULLET_END
};

static const _tchar* Tag_ModelMonsterBulletType(_uint eTag)
{
	switch (eTag)
	{
	case Prototype_Mesh_Monster_Bullet_Vayusura_Leader:
		return TEXT("Monster_Bullet_Vayusura_Leader.fbx");
	case Prototype_Mesh_Monster_Bullet_Tezabsura_Minion:
		return TEXT("Monster_Bullet_Tezabsura_Minion.fbx");
	case Prototype_Mesh_Monster_Bullet_Tezabsura_Purple:
		return TEXT("Monster_Bullet_Tezabsura_Purple.fbx");
	default:
		MSGBOX("Not Prototype_Component_Model_Bullet");
		return TEXT("ha! ha!");
	}
}

#define TAG_MONSTER_BULLET Tag_ModelMonsterBulletType