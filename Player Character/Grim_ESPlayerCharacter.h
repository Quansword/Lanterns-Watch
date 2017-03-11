// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#pragma once

#include "Pawns/Grim_ESCharacterBase.h"
#include "Grim_ES/Components/Character/Player/Grim_ESPlayerAttackComponent.h"
#include "Grim_ES/Components/Character/Player/Grim_ESPlayerMovementComponent.h"
#include "Grim_ES/Components/Character/Player/Grim_ESFallDamageComponent.h"
#include "Grim_ES/Components/Character/Player/Grim_ESCameraComponent.h"
#include "Grim_ES/Grim_ESPlayerController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Grim_ESPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GRIM_ES_API AGrim_ESPlayerCharacter : public AGrim_ESCharacterBase
{
	GENERATED_BODY()

	void RotateToCursor();

	AGrim_ESPlayerController* _playerController;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menus")
	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menus")
	class UUserWidget* PauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	bool bAttacking; // Is the player attacking?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	bool bHeavyAttacking; // Is the player attacking with a heavy attack?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	bool bSprinting; // Is the player sprinting?

	// Attack Functions
	int32 attackNum;
	bool canAttack;
	void AttackFinished();
	void ResetAttack();

	// Sprint functions
	void StartSprint();
	void StartStaminaTick();

public:
	AGrim_ESPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Attack Functions
	void Attack();
	void HeavyAttack();
	void StopAttack();

	// Sprint functions
	void Sprint();
	void StopSprinting();

	void Pause();

	class UGrim_ESPlayerAttackComponent* PlayerAttackComponent;
	class UGrim_ESPlayerMovementComponent* PlayerMovementComponent;
	class UGrim_ESFallDamageComponent* FallDamageComponent;
	class UGrim_ESCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = PostProcess) // Post processing material giving the player a silhouette through walls
	UMaterial* PlayerSilhouetteMaterial;

private:
	FTimerHandle PlayerTimerHandle;
	FTimerHandle ResetAttackTimerHandle;
	FTimerHandle LightAttackFinishedTimerHandle;
	FTimerHandle StaminaTimerHandle;
	FTimerHandle MovementTimerHandle;
};
