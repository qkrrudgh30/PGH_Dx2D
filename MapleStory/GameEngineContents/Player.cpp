#include "PreCompile.h"
#include "Player.h"
#include "GlobalContentsValue.h"
#include "ContentsLevel.h"
#include "PixelCollisionMap.h"
#include "RigidBody.h"

#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Player* Player::spPlayer = nullptr;

Player::Player() 
	: mpParentLevel(nullptr)
	, mf4PixelData()
	, StateManager()
	, mbOnLadder(false)
{
	mfSpeed = 400.f;
	spPlayer = this;
}

Player::~Player() 
{
}

void Player::Start()
{
	mpRenderer = CreateComponent<GameEngineTextureRenderer>();
	mfWidth = 83.f;
	mfHeight = 85.f;
	mpRenderer->GetTransform().SetLocalScale({ mfWidth, mfHeight, 1.f, 1.f });
	// mpRenderer->ScaleToTexture();
	mpRenderer->CreateFrameAnimation("CharacterAlert", FrameAnimation_DESC("CharacterAlert.png", 0, 4, 0.5f));		
	mpRenderer->CreateFrameAnimation("CharacterJump", FrameAnimation_DESC("CharacterJump.png", 0, 0, 0.5f));
	mpRenderer->CreateFrameAnimation("CharacterLadderIdle", FrameAnimation_DESC("CharacterLadderIdle.png", 0, 0, 0.5f));
	mpRenderer->CreateFrameAnimation("CharacterLadderMove", FrameAnimation_DESC("CharacterLadderMove.png", 0, 1, 0.5f));
	mpRenderer->CreateFrameAnimation("CharacterStab", FrameAnimation_DESC("CharacterStab.png", 0, 2, 0.2f));
	mpRenderer->CreateFrameAnimation("CharacterStabF", FrameAnimation_DESC("CharacterStabF.png", 0, 3, 0.2f));
	mpRenderer->CreateFrameAnimation("CharacterStand", FrameAnimation_DESC("CharacterStand.png", 0, 4, 0.5f));		
	mpRenderer->CreateFrameAnimation("CharacterSwing", FrameAnimation_DESC("CharacterSwing.png", 0, 2, 0.2f));		
	mpRenderer->CreateFrameAnimation("CharacterSwingF", FrameAnimation_DESC("CharacterSwingF.png", 0, 3, 0.2f));		
	mpRenderer->CreateFrameAnimation("CharacterWalk", FrameAnimation_DESC("CharacterWalk.png", 0, 5, 0.2f));
	mpRenderer->CreateFrameAnimation("CharacterDead", FrameAnimation_DESC("CharacterDead.png", 0, 0, 0.2f));
	mpRenderer->CreateFrameAnimationFolder("WarriorLeap", FrameAnimation_DESC("WarriorLeap", 0.1f));
	mpRenderer->SetPivot(PIVOTMODE::BOT);

	SetGround(false);
	RigidBody* rigid = CreateComponent<RigidBody>();

	StateManager.CreateStateMember("Stand", this, &Player::StandUpdate, &Player::StandStart, &Player::StandEnd);
	StateManager.CreateStateMember("Walk", this, &Player::WalkUpdate, &Player::WalkStart, &Player::WalkEnd);
	StateManager.CreateStateMember("Dead", this, &Player::DeadUpdate, &Player::DeadStart, &Player::DeadEnd);
	StateManager.CreateStateMember("Attack1", this, &Player::Attack1Update, &Player::Attack1Start, &Player::Attack1End);
	StateManager.CreateStateMember("Attack2", this, &Player::Attack2Update, &Player::Attack2Start, &Player::Attack2End);
	StateManager.CreateStateMember("Alert", this, &Player::AlertUpdate, &Player::AlertStart, &Player::AlertEnd);
	StateManager.CreateStateMember("Jump", this, &Player::JumpUpdate, &Player::JumpStart, &Player::JumpEnd);
	StateManager.CreateStateMember("LadderIdle", this, &Player::LadderIdleUpdate, &Player::LadderIdleStart, &Player::LadderIdleEnd);
	StateManager.CreateStateMember("LadderMove", this, &Player::LadderMoveUpdate, &Player::LadderMoveStart, &Player::LadderMoveEnd);
	StateManager.CreateStateMember("FinalAttack1", this, &Player::FinalAttack1Update, &Player::FinalAttack1Start, &Player::FinalAttack1End);
	StateManager.CreateStateMember("FinalAttack2", this, &Player::FinalAttack2Update, &Player::FinalAttack2Start, &Player::FinalAttack2End);
	StateManager.ChangeState("Stand");
}

void Player::Update(float _DeltaTime)
{
	// Check Walk
	// Check Jump
	// Check Attack1
	// Check Attack2
	// Check Alert
	// Check FinalAttack1
	// Check FinalAttack2
	// Check LadderStand
	// Check LadderMove
	// Check Dead
	StateManager.Update(_DeltaTime);

	if (true == GameEngineInput::GetInst()->IsPress("PlayerDoubleJump"))
	{
		mpRenderer->ChangeFrameAnimation("WarriorLeap");
	}

	// mpParentLevel = GetLevel<ContentsLevel>();
	if (nullptr != mpParentLevel)
	{
		mf4PixelData = mpParentLevel->GetPCMap()->GetRenderer()->GetCurTexture()->GetPixel(GetTransform().GetWorldPosition().x, -(GetTransform().GetWorldPosition().y + 1));
		float temp = mf4PixelData.r;
		mf4PixelData.r = mf4PixelData.b;
		mf4PixelData.b = temp;
	}

	if (true == mf4PixelData.CompareInt4D(float4::MAGENTA) || true == mf4PixelData.CompareInt4D(float4::CYAN)) // �Ʒ��ٴ�, ���ٴ�
	{
		mbOnGround = true;
	}
	else
	{
		mbOnGround = false;
	}

	if (true == mf4PixelData.CompareInt4D(float4::YELLOW))
	{
		mbOnLadder = true;
	}
	else
	{
		mbOnLadder = false;
	}
}

void Player::End()
{
}

void Player::StandStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 83.f, 85.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterStand");
}

void Player::StandUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// [D]Jump
	if (true == GameEngineInput::GetInst()->IsDown("PlayerJump"))
	{
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * 90.f);
		StateManager.ChangeState("Jump");
		return;
	}
	
	// [D]Walk
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		mpRenderer->GetTransform().PixLocalPositiveX();
		StateManager.ChangeState("Walk");
		return;
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		mpRenderer->GetTransform().PixLocalNegativeX();
		StateManager.ChangeState("Walk");
		return;
	}

	// [D]Alert
	// [S]Attack1
	if (true == GameEngineInput::GetInst()->IsPress("PlayerAttack1"))
	{
		StateManager.ChangeState("Attack1");
		return;
	}
	// [S]Attack2
	if (true == GameEngineInput::GetInst()->IsPress("PlayerAttack2"))
	{
		StateManager.ChangeState("Attack2");
		return;
	}
}

void Player::WalkStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 84.f, 85.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterWalk");
}

void Player::WalkUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// [D]Stand
	if (true == GameEngineInput::GetInst()->IsFree("PlayerLeft") && true == GameEngineInput::GetInst()->IsFree("PlayerRight")) 
	{ 
		StateManager.ChangeState("Stand"); 
		return;
	}
	
	// [D]Jump
	if (true == GameEngineInput::GetInst()->IsDown("PlayerJump"))
	{
		if (0 <= PreviousDirection().x) { GetTransform().SetWorldMove(GetTransform().GetUpVector() * 40.f + GetTransform().GetLeftVector() * 40.f); }
		else { GetTransform().SetWorldMove(GetTransform().GetUpVector() * 40.f + GetTransform().GetRightVector() * 40.f); }
		StateManager.ChangeState("Jump");
		return;
	}

	// [D]Alert
	// [D]Attack1
	// [D]Attack2

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		mpRenderer->GetTransform().PixLocalPositiveX();
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * mfSpeed * _DeltaTime);
		return;
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		mpRenderer->GetTransform().PixLocalNegativeX();
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * mfSpeed * _DeltaTime);
		return;
	}
}

void Player::DeadStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 39.f, 63.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterDead");
	// Start falling tomb effect.
}

void Player::DeadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// Wait 10 Seconds.
}

void Player::DeadEnd(const StateInfo& _Info)
{
	// Go to login level.
}

void Player::Attack1Start(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 140.f, 68.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterStab");
}

void Player::Attack1Update(float _DeltaTime, const StateInfo& _Info)
{
	// [D]Walk
	// [D]Alert
	// [S]FinalAttack1
}

void Player::Attack2Start(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 198.f, 128.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterSwing");
}

void Player::Attack2Update(float _DeltaTime, const StateInfo& _Info)
{
	// [D]Walk
	// [D]Alert
	// [S]FinalAttack2
}

void Player::AlertStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 86.f, 78.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterAlert");
}

void Player::AlertUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// [D]Stand
	// [D]Jump
	// [D]Walk
	// [D]Attack1
	// [D]Attack2
	// [S]Dead
}

void Player::JumpStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 83.f, 80.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterJump");
}

void Player::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	

	// [D]Stand
	if (true == mbOnGround) 
	{ 
		StateManager.ChangeState("Stand"); 
		return;
	}
	// [D]Alert
	// [D]Ladder
	if (true == mbOnLadder && true == GameEngineInput::GetInst()->IsPress("PlayerUp")) 
	{ 
		StateManager.ChangeState("LadderIdle"); 
		return; 
	}
}

void Player::LadderIdleStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 42.f, 64.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterLadderIdle");
}

void Player::LadderIdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// [D]Jump
	// [D]Stand
	// [D]Alert
}

void Player::LadderMoveStart(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 42.f, 64.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterLadderMove");
}

void Player::LadderMoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void Player::FinalAttack1Start(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 152.f, 99.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterStabF");
}

void Player::FinalAttack1Update(float _DeltaTime, const StateInfo& _Info)
{
	// [S]Stand
	// [S]Alert
}

void Player::FinalAttack2Start(const StateInfo& _Info)
{
	float4 fPreviousScale = PreviousDirection();
	mpRenderer->GetTransform().SetWorldScale(fPreviousScale * float4{ 194.f, 108.f, 1.f, 1.f });
	mpRenderer->ChangeFrameAnimation("CharacterSwingF");
}

void Player::FinalAttack2Update(float _DeltaTime, const StateInfo& _Info)
{
	// [S]Stand
	// [S]Alert
}
