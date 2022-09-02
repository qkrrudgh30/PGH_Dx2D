#include "PreCompile.h"
#include "Item.h"
#include "RigidBody.h"
#include "ContentsLevel.h"
#include "PixelCollisionMap.h"

#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineCollision.h>

Item::Item() 
	: mpRenderer(nullptr)
	, mpCollision(nullptr)
	, mpPlayer(nullptr)
	, mpRigidBody(nullptr)
	, mf4PixelData{}
	, mpParentLevel(nullptr)
	, mbOnCreate(true)
	, mfSpeed(200.f)
	, mfAccTime(0.f)
	, mfBeforeAccTimeForCreate(0.f)
	, mfWidth(0.f)
	, mfHeight(0.f)
	, muAmount(0u)
	, mnOrder()
{
}

Item::~Item() 
{
}

void Item::Start()
{
	mpCollision = CreateComponent<GameEngineCollision>("ItemCollision");
	mpCollision->ChangeOrder(OBJECTORDER::ItemType);
	mpCollision->SetDebugSetting(CollisionType::CT_AABB, float4{ 1.f, 0.f, 0.f, 0.5f });

	mpRenderer = CreateComponent<GameEngineTextureRenderer>();
	mpRenderer->SetScaleModeImage();
	SetGround(true);
	mpRigidBody = CreateComponent<RigidBody>();
	mfBeforeAccTimeForCreate = 0.f;
}

void Item::Update(float _fDeltaTime)
{
	if (nullptr != Player::GetPlayer())
	{
		mpPlayer = Player::GetPlayer();
	}

	SetParentLevel(GetLevel());

	if (nullptr != mpParentLevel)
	{
		mf4PixelData = mpParentLevel->GetPCMap()->GetRenderer()->GetCurTexture()->GetPixelToFloat4(
			static_cast<int>(GetTransform().GetWorldPosition().x),
			static_cast<int>(-(GetTransform().GetWorldPosition().y + 5.f)));
	}

	if (true == mf4PixelData.CompareInt4D(float4::MAGENTA) || true == mf4PixelData.CompareInt4D(float4::CYAN))
	{
		SetGround(true);
	}
	else
	{
		SetGround(false);
	}

	if (true == mbOnCreate)
	{
		SetGround(true);
		GetTransform().SetLocalMove(GetTransform().GetUpVector() * mfSpeed * _fDeltaTime);
	}

	mfAccTime = GetAccTime();
	SetCreatedAfterSecond();
}

void Item::End()
{
}