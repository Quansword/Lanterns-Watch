// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESStaminaComponent.h"

void UGrim_ESStaminaComponent::RegenStamina()
{
	if (StaminaConfig.stamina <= 0 && !zeroStamina) // Slows down stamina regen rate when 0 stamina is reached, reset when stamina is fully regained
	{
		zeroStamina = true;
		StaminaConfig.staminaRegenRate = StaminaConfig.staminaRegenRate * 0.85;
	}

	AddStamina(StaminaConfig.staminaRegen * StaminaConfig.staminaRegenRate * deltaTick); // Adds stamina, uses deltaTick to keep regen even across all tick rates

	if (StaminaConfig.stamina >= StaminaConfig.maxStamina && zeroStamina) // Resets stamin regen rate if it was slowed down
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

	deltaTick = DeltaTime; // Keep track of how much time has passed since the last tick
	RegenStamina();

	// Stops ticking if stamina is full
	if (StaminaConfig.stamina >= StaminaConfig.maxStamina)
	{
		StopTick();
	}
}

void UGrim_ESStaminaComponent::AddStamina(const float staminaToAdd)
{
	auto currentStamina = StaminaConfig.stamina;
	auto resultStamina = currentStamina + staminaToAdd;

	if (resultStamina > StaminaConfig.maxStamina) // Keep stamina at the maximum if it tries to go over
		resultStamina = StaminaConfig.maxStamina;
	
	if (currentStamina <= 0 && staminaToAdd < 0) // Doesn't stop stamina from going under 0, but does stop it from going further. Used if some effect drains stamina.
		resultStamina = currentStamina;

	StaminaConfig.stamina = resultStamina; // New stamina
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
	// Relays state of ticking to other components if called
	return IsComponentTickEnabled();
}