#include "PreCompile.h"
#include "Player.h"
#include <GameEngineBase/GameEngineInput.h>
#include <iostream>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineContents/GlobalContentsValue.h>
#include "ScoreTestComponent.h"
#include <GameEngineCore/GameEngineDefaultRenderer.h>

Player::Player() 
	: Speed(50.0f)
{
}

Player::~Player() 
{
}

GameEngineRenderer* CurRenderer;
GameEngineRenderer* ChildRenderer;
GameEngineRenderer* ChildRenderer2;


void Player::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("PlayerLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("PlayerLeft", VK_NUMPAD4);
		GameEngineInput::GetInst()->CreateKey("PlayerRight", VK_NUMPAD6);
		GameEngineInput::GetInst()->CreateKey("PlayerUp", VK_NUMPAD9);
		GameEngineInput::GetInst()->CreateKey("PlayerDown", VK_NUMPAD7);
		GameEngineInput::GetInst()->CreateKey("PlayerForward", VK_NUMPAD8);
		GameEngineInput::GetInst()->CreateKey("PlayerBack", VK_NUMPAD5);
		GameEngineInput::GetInst()->CreateKey("Rot+", VK_NUMPAD1);
		GameEngineInput::GetInst()->CreateKey("Rot-", VK_NUMPAD2);
	}

	GetTransform().SetLocalScale({1, 1, 1});

	ScoreTestComponent* ScoreCom = CreateComponent<ScoreTestComponent>();

	{
		CurRenderer = CreateComponent<GameEngineDefaultRenderer>();
		CurRenderer->GetTransform().SetLocalScale({ 100, 100, 100 });
	}

	//ScoreCom->SetParent(CurRenderer);

	{
		ChildRenderer = CreateComponent<GameEngineDefaultRenderer>();
		ChildRenderer->SetParent(CurRenderer);
		ChildRenderer->GetTransform().SetWorldPosition({ 150.0f, 100.0f, 0.0f });
	}

	{
		ChildRenderer2 = CreateComponent<GameEngineDefaultRenderer>();
		ChildRenderer2->SetParent(ChildRenderer);
		ChildRenderer2->GetTransform().SetWorldPosition({ 250.0f, 100.0f, 0.0f });
	}
}


Monster* TestMonsterObject = nullptr;


void Player::Update(float _DeltaTime)
{
	if (nullptr != ChildRenderer && true == ChildRenderer->IsDeath())
	{
		ChildRenderer = nullptr;
	}

	if (nullptr != ChildRenderer)
	{
		std::list<Monster*> MonsterList = GetLevel()->GetConvertToGroup<Monster>(OBJECTORDER::Monster);
		for (Monster* MonsterObject : MonsterList)
		{
			if (GameEngineTransform::OBBToOBB(ChildRenderer->GetTransform(), MonsterObject->GetTransform()))
			{
				ChildRenderer->Death();
				//TestMonsterObject = MonsterObject;
				//MonsterObject->Death();
			}
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
	{
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		GetTransform().SetWorldMove(GetTransform().GetDownVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerForward"))
	{
		GetTransform().SetWorldMove(GetTransform().GetForwardVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerBack"))
	{
		GetTransform().SetWorldMove(GetTransform().GetBackVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::GetInst()->IsPress("Rot+"))
	{
		CurRenderer->GetTransform().SetLocalRotate({ 0.0f, 0.0f, 360.0f * _DeltaTime });
	}
	if (true == GameEngineInput::GetInst()->IsPress("Rot-"))
	{
		CurRenderer->GetTransform().SetLocalRotate({ 0.0f, 0.0f, -360.0f * _DeltaTime });
	}

	// GlobalContentsValue::Actors::TestMonster

	// ChildRenderer->GetTransform().SetWorldPosition({ 150.0f, 100.0f, 30.0f });

}