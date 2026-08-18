// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UERWeaponComponent;

/**
 * FPS角色基类
 * 用于精英小队特种行动模式
 * 塔科夫式硬核真实：部位伤害、弹药管理、极简UI
 */
UCLASS()
class EMBERREALM_API AFPSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacterBase();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** 第一人称相机 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "相机")
	UCameraComponent* FPCamera;

	/** 武器组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "武器")
	UERWeaponComponent* WeaponComponent;

	/** 当前生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "状态")
	float CurrentHealth = 100.0f;

	/** 最大生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "状态")
	float MaxHealth = 100.0f;

	/** 是否正在瞄准 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "状态")
	bool bIsAiming = false;

	/** 是否正在冲刺 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "状态")
	bool bIsSprinting = false;

protected:
	/** 移动输入 */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	/** 动作输入 */
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void StartCrouch();
	void StopCrouch();
	void StartFire();
	void StopFire();
	void StartAim();
	void StopAim();
	void Reload();
	void Interact();
};
