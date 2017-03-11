// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESHealthComponent.h"


void UGrim_ESHealthComponent::RegenHealth()
{
	AddHealth(HealthConfig.healthRegen * deltaTick); // Adds health to character the component is tied to, uses deltaTick to keep regen even across all tick rates
}

// Sets default values for this component's properties
UGrim_ESHealthComponent::UGrim_ESHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	deltaTick = 0;
}


// Called when the game starts
void UGrim_ESHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGrim_ESHealthComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	deltaTick = DeltaTime; // Keep track of how much time has passed since the last tick
	RegenHealth(); // Regen more health

	if (HealthConfig.health >= HealthConfig.maxHealth) // Stop the tick if health is full to improve performance
	{
		StopTick();
	}
}

void UGrim_ESHealthComponent::AddHealth(const float healthToAdd)
{
	auto currentHealth = HealthConfig.health;
	auto resultHealth = currentHealth + healthToAdd;

	if (resultHealth >= HealthConfig.maxHealth)
	{
		resultHealth = HealthConfig.maxHealth;	// Keep health at the maximum if it tries to go over	
		StopTick(); // Stop the tick if health is full to improve performance
	}
	else if (resultHealth < 0) 
	{
		resultHealth = 0; // Keeps health at 0 if it tries to go under
		StopTick();

		auto actor = GetOwner();
		UnitKilledDelegate().ExecuteIfBound(actor);
	}

	HealthConfig.health = resultHealth; // Set new health
}

// Add negative value to health
void UGrim_ESHealthComponent::SubtractHealth(const float healthToSubtract)
{
	AddHealth(-healthToSubtract);
}

// Restarts tick
void UGrim_ESHealthComponent::StartTick()
{
	SetComponentTickEnabled(true);
}

// Stops tick
void UGrim_ESHealthComponent::StopTick()
{
	SetComponentTickEnabled(false);
}

// Relays state of ticking to other components if called
bool UGrim_ESHealthComponent::GetTicking()
{
	return IsComponentTickEnabled();
}

UGrim_ESHealthComponent::ActorDelegate UGrim_ESHealthComponent::UnitKilledDelegate()
{
	return UnitKilled;
}
