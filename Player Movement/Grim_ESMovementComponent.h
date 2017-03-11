// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Grim_ES/Components/Character/Player/Grim_ESFallDamageComponent.h"
#include "Components/Character/Grim_ESStaminaComponent.h"
#include "Grim_ESMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIM_ES_API UGrim_ESMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	bool bSprinting;

protected:
	bool _initialized;
	class ACharacter* _character;
	class UCharacterMovementComponent* _characterMovement;
	class UGrim_ESStaminaComponent* _staminaComponent;
	float _movementSpeed;
	float _sprintSpeed;

	bool CanJump();

public:	
	// Sets default values for this component's properties
	UGrim_ESMovementComponent();

	virtual void Initialize(ACharacter* character);

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Sprint();

	void StopSprinting();

	virtual void Landed(const FHitResult& HitResult);

	bool getSprinting();

	void setSprinting(bool sprintSet);
};
