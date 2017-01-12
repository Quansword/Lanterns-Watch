// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESStaminaComponent.h"

void UGrim_ESStaminaComponent::RegenStamina()
{
	if (StaminaConfig.stamina <= 0 && !zeroStamina)
	{
		zeroStamina = true;
		StaminaConfig.staminaRegenRate = StaminaConfig.staminaRegenRate * 0.85;
	}

	AddStamina(StaminaConfig.staminaRegen * StaminaConfig.staminaRegenRate * deltaTick);

	if (StaminaConfig.stamina >= StaminaConfig.maxStamina && zeroStamina)
	{
		zeroStamina = false;
		StaminaConfig.staminaRegenRate = StaminaConfig.staminaRegenRate / 0.85;
	}
}

// Sets default values for this component's properties
UGrim_ESStaminaComponent::UGrim_ESStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	zeroStamina = false;
	deltaTick = 0;
}

// Called when the game starts
void UGrim_ESStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGrim_ESStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	deltaTick = DeltaTime;
	RegenStamina(); // TODO: add something here to only update on the regen rate interval

	if (StaminaConfig.stamina >= StaminaConfig.maxStamina)
	{
		StopTick();
	}
}

void UGrim_ESStaminaComponent::AddStamina(const float staminaToAdd)
{
	auto currentStamina = StaminaConfig.stamina;
	auto resultStamina = currentStamina + staminaToAdd;

	if (resultStamina > StaminaConfig.maxStamina)
		resultStamina = StaminaConfig.maxStamina;
	
	if (currentStamina <= 0 && staminaToAdd < 0)
		resultStamina = currentStamina;

	StaminaConfig.stamina = resultStamina;
}

void UGrim_ESStaminaComponent::SubtractStamina(const float staminaToSubtract)
{
	AddStamina(-staminaToSubtract);
}

void UGrim_ESStaminaComponent::StartTick()
{
	SetComponentTickEnabled(true);
}

void UGrim_ESStaminaComponent::StopTick()
{
	SetComponentTickEnabled(false);
}

bool UGrim_ESStaminaComponent::GetTicking()
{
	return IsComponentTickEnabled();
}