#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class InventoryInfo
{
	InventorySize = 24,
	InventoryWidth = 4,
	InventoryHeight = 6,
	ItemWidth = 32,
	ItemHeight = 32,
	ItemWidthOffset = 10,
	ItemHeightOffset = 10,
};

class ItemInfo
{
public:
	int meItemType;
	unsigned int muItemCount;
};

// ���� :
class GameEngineTextureRenderer;
class GameEngineUIRenderer;
class ETCMenuButton;
class GameEngineFontRenderer;
class Inventory : public GameEngineActor
{
private:
	static Inventory* spInventory;
	float mfWidth;
	float mfHeight;
	static Player* spPlayer;
	GameEngineTextureRenderer* mpRenderer;
	GameEngineUIRenderer* mpETCRenderer;
	GameEngineUIRenderer* mpConsumptionRenderer;
	float4 mf4TitlePos;
	float4 mf4MousePos;
	float4 mf4MouseDownPos;
	float4 mf4MouseDownInvenPos;
	float4 mf4MouseUpPos;
	float4 mf4MouseUpInvenPos;
	bool   mbOnTitle;
	bool   mbOnDrag;
	unsigned int muAccMeso;
	float4 mf4MesoFontPosition;
	GameEngineFontRenderer* mpFontRenderer;
	std::vector<std::pair<GameEngineUIRenderer*, ItemInfo>> mvItemsVector;
	unsigned int muItemsIndex;

public:
	// constrcuter destructer
	Inventory();
	~Inventory();

	// delete Function
	Inventory(const Inventory& _Other) = delete;
	Inventory(Inventory&& _Other) noexcept = delete;
	Inventory& operator=(const Inventory& _Other) = delete;
	Inventory& operator=(Inventory&& _Other) noexcept = delete;

	static Inventory* GetInventory() { return spInventory; }

	int FindItem(int _nObjectOrder);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() {}

private:

};
