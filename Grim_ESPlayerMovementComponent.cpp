// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESPlayerMovementComponent.h"

UGrim_ESPlayerMovementComponent::UGrim_ESPlayerMovementComponent()
{
	_movementSpeed = 500.0f;
	_sprintSpeed = 900.0f;

	deltaTick = 0;

	bWantsBeginPlay = true;
}

void UGrim_ESPlayerMovementComponent::Initialize(ACharacter * character)
{
	Super::Initialize(character);

	_characterMovement->GravityScale = 2.0;
	_characterMovement->MaxWalkSpeed = _movementSpeed;
	_characterMovement->bOrientRotationToMovement = true; // Rotate character to moving direction
	_characterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	_characterMovement->JumpZVelocity = 700.f;
	_characterMovement->AirControl = 0.2f;

	if (!_inputComponent)
	{
		return;
	}

	_inputComponent->BindAxis("MoveForward", this, &UGrim_ESPlayerMovementComponent::MoveForward);
	_inputComponent->BindAxis("MoveRight", this, &UGrim_ESPlayerMovementComponent::MoveRight);

	_inputComponent->BindAction("Jump", IE_Pressed, this, &UGrim_ESPlayerMovementComponent::Jump);
	_inputComponent->BindAction("Jump", IE_Released, _character, &ACharacter::StopJumping);
}

void UGrim_ESPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	deltaTick = DeltaTime;

	if (!_initialized)
		return;

	if (_fallStartPosition.IsZero() && _characterMovement->IsFalling())
		_fallStartPosition = _character->GetActorLocation();

	if (!_character->IsPlayingRootMotion() && CanJump() && getSprinting())
	{
		if (_character->GetVelocity() != FVector(0, 0, 0))
		{
			_staminaComponent->StopTick();
			_staminaComponent->SubtractStamina(20 * deltaTick);
		}
		else
		{
			_staminaComponent->StartTick();
		}
	}
}

void UGrim_ESPlayerMovementComponent::Jump()
{
	if (!_character->IsPlayingRootMotion() && CanJump() && _staminaComponent->StaminaConfig.stamina > 0)
	{
		_character->bPressedJump = true;
		_character->JumpKeyHoldTime = 0.0f;

		if (!getSprinting() || _character->GetVelocity() == FVector(0, 0, 0))
			_staminaComponent->SubtractStamina(5);
		else
			_staminaComponent->SubtractStamina(10);

		_staminaComponent->StopTick();
	}
}

void UGrim_ESPlayerMovementComponent::Landed(const FHitResult & HitResult)
{
	Super::Landed(HitResult);
	
	auto currentHeight = _character->GetActorLocation().Z;
	_fallDamageComponent->ApplyFallDamage(_fallStartPosition.Z, currentHeight);
	_fallStartPosition = FVector::ZeroVector;
	if (!getSprinting())
		_staminaComponent->StartTick();
}

//-----------------------------------------------------------------------------------------------------Sprint Functions-----------------------------------------------------------------------------------------------------

void UGrim_ESPlayerMovementComponent::Sprint()
{
	setSprinting(true);
	_characterMovement->MaxWalkSpeed = _sprintSpeed;
	_staminaComponent->StopTick();
	// Parts needed to be moved to Grim_ESPlayerCharacter in order to use timers for stamina regeneration delay
}

void UGrim_ESPlayerMovementComponent::StopSprinting()
{
	setSprinting(false);
	_characterMovement->MaxWalkSpeed = _movementSpeed;
	// Parts needed to be moved to Grim_ESPlayerCharacter in order to use timers for stamina regeneration delay
}

//-----------------------------------------------------------------------------------------------------Initializing Functions-----------------------------------------------------------------------------------------------------

void UGrim_ESPlayerMovementComponent::SetInputComponent(UInputComponent * InputComponent)
{
	_inputComponent = InputComponent;
}

void UGrim_ESPlayerMovementComponent::SetFallDamageComponent(UGrim_ESFallDamageComponent* FallDamageComponent)
{
	_fallDamageComponent = FallDamageComponent;
}

void UGrim_ESPlayerMovementComponent::SetStaminaComponent(UGrim_ESStaminaComponent* StaminaComponent)
{
	_staminaComponent = StaminaComponent;
}
