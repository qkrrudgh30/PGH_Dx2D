#include "PreCompile.h"
#include "TempleOfTime2.h"
#include "GlobalContentsValue.h"
#include "Player.h"
#include "Temple2BackGround.h"
#include "Temple2Tile.h"
#include "Temple2Cloud.h"
#include "PixelCollisionMap.h"
#include "Portal.h"
#include "Veil.h"
#include "Temple2Monster.h"
#include "StateBar.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineWindow.h>

TempleOfTime2::TempleOfTime2() 
{
}

TempleOfTime2::~TempleOfTime2() 
{
}

void TempleOfTime2::OnEvent()
{
	Veil::SetVeilEffect(VEIL_EFFECT::FADE_IN);
}

void TempleOfTime2::Start()
{
	mpCamera = GetMainCameraActor();
	mpCamera->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	mpCamera->GetTransform().SetWorldPosition({ 0.0f, 0.0f, -500.0f });
	// GetMainCameraActor()->FreeCameraModeOnOff();

	SetMapSize(float4{ 1982.f, 846.f });
	mpBackGround = CreateActor<Temple2BackGround>(OBJECTORDER::UI);
	mpTile = CreateActor<Temple2Tile>(OBJECTORDER::UI);
	mpCloud = CreateActor<Temple2Cloud>(OBJECTORDER::UI);
	mpPCMap = CreateActor<PixelCollisionMap>(OBJECTORDER::UI);
	mpPCMap->SetInfo("Temple2TileP.png");

	float4 CenterPointOfMap = float4{ GetMapSize().x / 2.f, -(GetMapSize().y / 2.f), 0.f, 0.f };
	mpBackGround->GetTransform().SetWorldMove(CenterPointOfMap);
	mpTile->GetTransform().SetWorldMove(CenterPointOfMap);
	mpCloud->GetTransform().SetWorldMove(CenterPointOfMap);
	mpPCMap->GetTransform().SetWorldMove(CenterPointOfMap);
	
	mpPlayer = CreateActor<Player>(OBJECTORDER::Character);
	float4 StartPoint = float4{ 165.f, -660.f, 0.f, 0.f };
	mpPlayer->GetTransform().SetWorldMove(StartPoint);
	
	Temple2Monster* mpMonster = CreateActor<Temple2Monster>(OBJECTORDER::Mob);
	StartPoint.x += 200.f;
	mpMonster->GetTransform().SetWorldMove(StartPoint);

	mpPortalToPrevious = CreateActor<Portal>(OBJECTORDER::UI);
	mpPortalToNext = CreateActor<Portal>(OBJECTORDER::UI);

	mpStateBar = CreateActor<StateBar>(OBJECTORDER::UI);

	mpVeil = CreateActor<Veil>(OBJECTORDER::UI);
}

void TempleOfTime2::Update(float _DeltaTime)
{
	mpPortalToPrevious->GetTransform().SetWorldPosition(float4{ 165.f, -660.f, OBJECTORDER::Character, 1.f });
	mpPortalToNext->GetTransform().SetWorldPosition(float4{ 1860.f, -660.f, OBJECTORDER::Character, 1.f });
	if (true == ContentsCore::IsCameraFollowingOn())
	{
		mpCamera->GetTransform().SetWorldPosition(mpPlayer->GetTransform().GetLocalPosition());
	}

	if (true == GameEngineInput::GetInst()->IsPress("CamLeft"))
	{
		GetMainCameraActorTransform().SetWorldMove(-GetMainCameraActorTransform().GetRightVector() * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamRight"))
	{
		GetMainCameraActorTransform().SetWorldMove(GetMainCameraActorTransform().GetRightVector() * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamUp"))
	{
		GetMainCameraActorTransform().SetWorldMove(GetMainCameraActorTransform().GetUpVector() * 100 * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("CamDown"))
	{
		GetMainCameraActorTransform().SetWorldMove(-GetMainCameraActorTransform().GetUpVector() * 100 * _DeltaTime);
	}

	LimitCameraMoving(GetMapSize());
	float4 pos = mpCamera->GetTransform().GetWorldPosition();
	pos.z = OBJECTORDER::Alpha;
	mpVeil->GetTransform().SetWorldPosition(pos);

	if (PortalCollisionType::NEXT == IsPortalCollided())
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
		{
			mfVeilStartSecond = GetAccTime();
			Veil::SetVeilEffect(VEIL_EFFECT::FADE_OUT);
		}
	}
	if (PortalCollisionType::PREVIOUS == IsPortalCollided())
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
		{
			mfVeilStartSecond = GetAccTime();
			Veil::SetVeilEffect(VEIL_EFFECT::FADE_OUT);
		}
	}

	if (-1.f != mfVeilStartSecond && 1.f <= GetAccTime() - mfVeilStartSecond)
	{
		mfVeilStartSecond = -1.f;
		if (PortalCollisionType::PREVIOUS == IsPortalCollided())
		{
			GEngine::ChangeLevel("TempleOfTime1");
		}
		if (PortalCollisionType::NEXT == IsPortalCollided())
		{
			GEngine::ChangeLevel("TempleOfTime3");
		}
	}

	if (true == GameEngineInput::GetInst()->IsDown("PCMapOnOffToggle"))
	{
		mpPCMap->GetRenderer()->OnOffSwitch();
	}

	// GameEngineDebug::OutPutString(std::to_string(mpPlayer->GetTransform().GetLocalPosition().x) + "  " + std::to_string(mpPlayer->GetTransform().GetLocalPosition().y));
}

void TempleOfTime2::End()
{
}
