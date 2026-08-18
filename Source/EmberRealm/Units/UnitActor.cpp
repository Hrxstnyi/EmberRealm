// Copyright EmberRealm. All Rights Reserved.
#include "Units/UnitActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

AUnitActor::AUnitActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(60, 8));
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateHealthDisplay();
}

void AUnitActor::SetFactionColor(EFactionType Faction)
{
	FLinearColor Color;
	switch (Faction)
	{
	case EFactionType::Garrison:	Color = FLinearColor(0.2f, 0.4f, 0.8f); break; // 蓝
	case EFactionType::Contract:	Color = FLinearColor(0.8f, 0.6f, 0.1f); break; // 金
	case EFactionType::Autonomy:	Color = FLinearColor(0.2f, 0.7f, 0.3f); break; // 绿
	case EFactionType::Insider:		Color = FLinearColor(0.6f, 0.1f, 0.6f); break; // 紫
	default: Color = FLinearColor::Gray;
	}

	if (MeshComponent)
	{
		UMaterialInstanceDynamic* DynMat = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMat) DynMat->SetVectorParameterValue(TEXT("BaseColor"), Color);
	}
}

void AUnitActor::UpdateHealthDisplay()
{
	// 生命值条更新（可在UMG中绑定）
}

void AUnitActor::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;
	if (MeshComponent)
	{
		// 选中高亮：放大或发光
		float Scale = bSelected ? 1.2f : 1.0f;
		MeshComponent->SetWorldScale3D(FVector(Scale));
	}
}
