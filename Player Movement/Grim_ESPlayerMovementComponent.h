// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#pragma once

#include "Components/Character/Grim_ESMovementComponent.h"
#include "Components/Character/Player/Grim_ESFallDamageComponent.h"
#include "GameFramework/Character.h"
#include "Grim_ESPlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GRIM_ES_API UGrim_ESPlayerMovementComponent : public UGrim_ESMovementComponent
{
	GENERATED_BODY()

	class UGrim_ESFallDamageComponent* _fallDamageComponent;
	class UInputComponent* _inputComponent;

	float deltaTick;

	FVector _fallStartPosition; // Position of player at beginning of fall or jump.

public:
	UGrim_ESPlayerMovementComponent();

	virtual void Initialize(ACharacter* character);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Jump();
	
	virtual void Landed(const FHitResult& HitResult) override;
	
	virtual void Sprint();

	virtual void StopSprinting();

	void SetInputComponent(UInputComponent * InputComponent);
	
	void SetFallDamageComponent(UGrim_ESFallDamageComponent* FallDamageComponent);

	void SetStaminaComponent(UGrim_ESStaminaComponent* StaminaComponent);
};
