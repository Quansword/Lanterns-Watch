// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESMovementComponent.h"


bool UGrim_ESMovementComponent::CanJump()
{
	if (!_character->IsPlayingRootMotion())
	{
		return _character->CanJump(); // Doesn't cover root motion animations
	}
	else
		return false;
}

// Sets default values for this component's properties
UGrim_ESMovementComponent::UGrim_ESMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGrim_ESMovementComponent::Initialize(ACharacter* character)
{
	_initialized = true;
	_character = character;
	_characterMovement = _character->GetCharacterMovement();
	_characterMovement->RotationRate = FRotator(0, 345, 0);
	_characterMovement->AirControl = 0.4;
}


// Called when the game starts
void UGrim_ESMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UGrim_ESMovementComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!_initialized) 
		return;

	// Disables sprinting if stamina is 0 or less
	if (_staminaComponent->StaminaConfig.stamina <= 0)
	{
		bSprinting = false;
		_characterMovement->MaxWalkSpeed = _movementSpeed;
	}
}

// Handles forward and backward movement
void UGrim_ESMovementComponent::MoveForward(float Value)
{
	if (!_initialized)
		return;

	if ((!_character->Controller) || (Value == 0.0f))
	{
		return;
	}

	// Sets different rotation rates for when the player is on the ground vs in the air
	if (!CanJump())
	{
		_characterMovement->RotationRate = FRotator(0, 115, 0);
	}
	else
	{
		_characterMovement->RotationRate = FRotator(0, 345, 0);
	}

	// find out which way is forward
	const FRotator Rotation = _character->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	_character->AddMovementInput(Direction, Value);
}

// Handles right and left movement
void UGrim_ESMovementComponent::MoveRight(float Value)
{
	if (!_initialized)
		return;

	if ((!_character->Controller) || (Value == 0.0f))
	{
		return;
	}

	// Sets different rotation rates for when the player is on the ground vs in the air
	if (!CanJump())
	{
		_characterMovement->RotationRate = FRotator(0, 115, 0);
	}
	else
	{
		_characterMovement->RotationRate = FRotator(0, 345, 0);
	}

	// find out which way is right
	const FRotator Rotation = _character->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	_character->AddMovementInput(Direction, Value);
}

void UGrim_ESMovementComponent::Sprint()
{
	// Checks if the amount of stamina is greater than 0 and enables sprinting if it is
	if (_staminaComponent->StaminaConfig.stamina > 0)
	{
		bSprinting = true;
		_characterMovement->MaxWalkSpeed = _sprintSpeed;
	}
}

void UGrim_ESMovementComponent::StopSprinting()
{
	bSprinting = false;
	_characterMovement->MaxWalkSpeed = _movementSpeed;
}

void UGrim_ESMovementComponent::Landed(const FHitResult & HitResult)
{
	// Currently unused function for what happens to player upon landing from a fall, handled in Grim_ESPlayerMovementComponent.cpp
}

bool UGrim_ESMovementComponent::getSprinting()
{
	return bSprinting;
}

void UGrim_ESMovementComponent::setSprinting(bool sprintSet)
{
	bSprinting = sprintSet;
}
