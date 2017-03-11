// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#include "Grim_ES.h"
#include "Grim_ESDummy.h"


// Sets default values
AGrim_ESDummy::AGrim_ESDummy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets up the dummy's health and regen
	HealthComponent = CreateDefaultSubobject<UGrim_ESHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->HealthConfig.health = 100;
	HealthComponent->HealthConfig.maxHealth = 100;
	HealthComponent->HealthConfig.healthRegen = 50;

	// Set up for damage component
	DamageComponent = CreateDefaultSubobject<UGrim_ESDamageComponent>(TEXT("DamageComponent"));
	DamageComponent->SetHealthComponent(HealthComponent);

	// Set default values for variables
	deltaTick = 0; // Used to store the time since the last tick
	cooldown = 0; // Used to decide if health regen should start. if this gets to 2, then yes
	prevHealth = 100; // Used to store the last known health of the dummy
}

// Called every frame
void AGrim_ESDummy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	deltaTick = DeltaTime;
	// Calls the regen function, which computes whether or not the health regen function needs to be started
	Regen();
}

void AGrim_ESDummy::Regen()
{
	if (HealthComponent->HealthConfig.health < HealthComponent->HealthConfig.maxHealth) // Only proceed if dummy health is below 100
	{
		if (prevHealth == 100) // If the dummy's health has changed from 100, start the cooldown timer and set prevHealth to the dummy's current health
		{
			cooldown = deltaTick;
			prevHealth = HealthComponent->HealthConfig.health;
		}
		else // If the dummy's health was last below 100
		{
			if (prevHealth == HealthComponent->HealthConfig.health) // If the health hasn't changed from the previous tick
			{
				cooldown += deltaTick; // Add the amount of time passed since the last tick to the cooldown
				if (cooldown >= 2) // If the time past since the dummy last took damage is 2 seconds or more, start the health regen handled in the health component
				{
					HealthComponent->StartTick();
				}
			}
			else if (prevHealth > HealthComponent->HealthConfig.health) // If the health is different from the previous tick, reset the cooldown and the prevHealth (the dummy won't heal, but the regen function needs prevHealth to return to 100 to initialize cooldown properly)
			{
				prevHealth = 100;
				cooldown = 0;
			}
		}
	}
	else // Reset cooldown if health is full
	{
		cooldown = 0;
	}
}
