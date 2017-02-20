// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESPlayerCharacter.h"

void AGrim_ESPlayerCharacter::RotateToCursor()
{
	// Rotates character toward cursor location, used for attacks
	// TODO rework system to add delay to character turn
	if (_playerController) {
		auto newRotation = _playerController->CalculateRotationToCursor(GetActorLocation());
		SetActorRotation(newRotation);
	}
}

AGrim_ESPlayerCharacter::AGrim_ESPlayerCharacter()
{
	// Set up components
	FallDamageComponent = CreateDefaultSubobject<UGrim_ESFallDamageComponent>(TEXT("FallDamageComponent"));
	FallDamageComponent->SetDamageComponent(DamageComponent);

	PlayerMovementComponent = CreateDefaultSubobject<UGrim_ESPlayerMovementComponent>(TEXT("MovementComponent"));
	PlayerMovementComponent->SetFallDamageComponent(FallDamageComponent);
	PlayerMovementComponent->SetStaminaComponent(StaminaComponent);

	MovementComponent = PlayerMovementComponent;

	PlayerAttackComponent = CreateDefaultSubobject<UGrim_ESPlayerAttackComponent>(TEXT("AttackComponent"));

	AttackComponent = PlayerAttackComponent;

	CameraComponent = CreateDefaultSubobject<UGrim_ESCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set default variable values
	attackNum = 0;
	canAttack = true;
	bAttacking = false;
	bHeavyAttacking = false;
	bSprinting = false;
}

void AGrim_ESPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	_playerController = Cast<AGrim_ESPlayerController>(Controller);

	if (!PauseMenuClass) {
		return;
	}

	// Adds pause menu widget
	PauseMenu = CreateWidget<UUserWidget>(_playerController, PauseMenuClass);
	PauseMenu->AddToViewport();
}

void AGrim_ESPlayerCharacter::Tick(float deltaTime)
{
	// Rotates player to cursor if they are attacking
	if (bAttacking || bHeavyAttacking)
	{
		RotateToCursor();
	}

	// Start stamina regen if stamina is zero or less and another action is not currently ongoing (fixed regen not working if sprint button was held)
	if (StaminaComponent->StaminaConfig.stamina <= 0 && (StaminaComponent->GetTicking() || bSprinting == true))
	{
		bSprinting = false;;
		StaminaComponent->StartTick();
	}
}

void AGrim_ESPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CameraComponent->SetPlayerSilhouette(PlayerSilhouetteMaterial);
}

void AGrim_ESPlayerCharacter::SetupPlayerInputComponent(UInputComponent * InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	PlayerMovementComponent->SetInputComponent(InputComponent);
	PlayerAttackComponent->SetInputComponent(InputComponent);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AGrim_ESPlayerCharacter::Attack);
	InputComponent->BindAction("HeavyAttack", IE_Pressed, this, &AGrim_ESPlayerCharacter::HeavyAttack);

	//InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AGrim_ESPlayerCharacter::Pause);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AGrim_ESPlayerCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AGrim_ESPlayerCharacter::StopSprinting);
}

//-----------------------------------------------------------------------------------------------------Attack Functions-----------------------------------------------------------------------------------------------------

void AGrim_ESPlayerCharacter::Attack()
{
	if (CanJump() && StaminaComponent->StaminaConfig.stamina > 0 && !bAttacking && !bHeavyAttacking)
	{
		bAttacking = true;
		if (canAttack) // Is an attack curently underway? Reset by the ResetAttack function
		{
			StaminaComponent->SubtractStamina(20);
			canAttack = false;
			if (attackNum == 0) // If first attack in sequence. Reset by AttackFinished function if a new attack isn't started.
			{
				attackNum++;
				StaminaComponent->StopTick();
				GetWorldTimerManager().SetTimer(ResetAttackTimerHandle, this, &AGrim_ESPlayerCharacter::ResetAttack, 0.55f, false); // Point at which the player can start another attack.
				// Player has the amount of time between the attack reset and the attack finished to execute the next attack in the string. Otherwise, string will reset.
				GetWorldTimerManager().SetTimer(LightAttackFinishedTimerHandle, this, &AGrim_ESPlayerCharacter::AttackFinished, 0.7333f, false); // Length of the attack anim
				GetWorldTimerManager().SetTimer(StaminaTimerHandle, StaminaComponent, &UGrim_ESStaminaComponent::StartTick, 1.4333f, false); // 0.7 seconds after the attack anim
			}
			else if (attackNum == 1) // If second attack in sequence
			{
				attackNum++;
				StaminaComponent->StopTick();
				GetWorldTimerManager().SetTimer(ResetAttackTimerHandle, this, &AGrim_ESPlayerCharacter::ResetAttack, 0.46f, false);
				GetWorldTimerManager().SetTimer(LightAttackFinishedTimerHandle, this, &AGrim_ESPlayerCharacter::AttackFinished, 0.6333f, false);
				GetWorldTimerManager().SetTimer(StaminaTimerHandle, StaminaComponent, &UGrim_ESStaminaComponent::StartTick, 1.3333f, false);
			}
			else if (attackNum == 2) // If third attack in sequence
			{
				attackNum++;
				StaminaComponent->StopTick();
				GetWorldTimerManager().SetTimer(ResetAttackTimerHandle, this, &AGrim_ESPlayerCharacter::ResetAttack, 0.28f, false);
				GetWorldTimerManager().SetTimer(LightAttackFinishedTimerHandle, this, &AGrim_ESPlayerCharacter::AttackFinished, 0.28f, false); // End attack early to reset the string
				GetWorldTimerManager().SetTimer(StaminaTimerHandle, StaminaComponent, &UGrim_ESStaminaComponent::StartTick, 1.259f, false);
			}
		}
		GetWorldTimerManager().SetTimer(PlayerTimerHandle, this, &AGrim_ESPlayerCharacter::StopAttack, 0.154f, false);
	}
}

void AGrim_ESPlayerCharacter::HeavyAttack()
{
	if (CanJump() && StaminaComponent->StaminaConfig.stamina > 0 && !bAttacking && !bHeavyAttacking)
	{
		bHeavyAttacking = true;
		if (canAttack && attackNum < 2) // Is an attack curently underway and is the string on the first two attacks? Reset by the ResetAttack function
		{
			StaminaComponent->SubtractStamina(35);
			canAttack = false;
			if (attackNum == 0) // If first attack in sequence. Reset by AttackFinished function if a new attack isn't started.
			{
				attackNum++;
				StaminaComponent->StopTick();
				GetWorldTimerManager().SetTimer(ResetAttackTimerHandle, this, &AGrim_ESPlayerCharacter::ResetAttack, 0.66f, false); // Point at which the player can start another attack.
				// Player has the amount of time between the attack reset and the attack finished to execute the next attack in the string. Otherwise, string will reset.
				GetWorldTimerManager().SetTimer(LightAttackFinishedTimerHandle, this, &AGrim_ESPlayerCharacter::AttackFinished, 0.88f, false); // Length of the attack anim
				GetWorldTimerManager().SetTimer(StaminaTimerHandle, StaminaComponent, &UGrim_ESStaminaComponent::StartTick, 1.58f, false); // 0.7 seconds after the attack anim
			}
			else if (attackNum == 1) // If second attack in sequence
			{
				attackNum++;
				StaminaComponent->StopTick();
				GetWorldTimerManager().SetTimer(ResetAttackTimerHandle, this, &AGrim_ESPlayerCharacter::ResetAttack, 0.648f, false);
				GetWorldTimerManager().SetTimer(LightAttackFinishedTimerHandle, this, &AGrim_ESPlayerCharacter::AttackFinished, 0.8f, false);
				GetWorldTimerManager().SetTimer(StaminaTimerHandle, StaminaComponent, &UGrim_ESStaminaComponent::StartTick, 1.5f, false);
			}
		}
		GetWorldTimerManager().SetTimer(PlayerTimerHandle, this, &AGrim_ESPlayerCharacter::StopAttack, 0.2f, false);
	}
}

void AGrim_ESPlayerCharacter::StopAttack()
{
	bAttacking = false;
	bHeavyAttacking = false;
}

void AGrim_ESPlayerCharacter::ResetAttack()
{
	canAttack = true;
}

void AGrim_ESPlayerCharacter::AttackFinished()
{
	if (attackNum == 1 || attackNum == 2 || attackNum == 3)
	{
		attackNum = 0;
	}
}

//-----------------------------------------------------------------------------------------------------Sprint Functions-----------------------------------------------------------------------------------------------------

void AGrim_ESPlayerCharacter::Sprint() // Call StartSprint() function
{
	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &AGrim_ESPlayerCharacter::StartSprint, 0.1f, false);
}

void AGrim_ESPlayerCharacter::StopSprinting()
{
	GetWorldTimerManager().ClearTimer(MovementTimerHandle); // Stops sprint from restarting from an existing timer
	PlayerMovementComponent->StopSprinting();
	bSprinting = PlayerMovementComponent->getSprinting();
	if (!IsPlayingRootMotion() && CanJump() && !bAttacking && !bHeavyAttacking)
	{
		// This timer could not be used in a component, which is why sprint was moved here
		GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &AGrim_ESPlayerCharacter::StartStaminaTick, 0.3, false);
	}
}

void AGrim_ESPlayerCharacter::StartSprint() // Call Sprint function in the player movement component, this is done for the purpose of using timers for stamina regen delay
{
	if (!IsPlayingRootMotion() && CanJump() && !bAttacking && !bHeavyAttacking && StaminaComponent->StaminaConfig.stamina > 0)
	{
		PlayerMovementComponent->Sprint();
		bSprinting = PlayerMovementComponent->getSprinting();
	}
}

void AGrim_ESPlayerCharacter::StartStaminaTick()
{
	// Stamina component tick starts stamina regeneration
	StaminaComponent->StartTick();
}