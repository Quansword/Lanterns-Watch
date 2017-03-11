// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "Grim_ES/Components/Character/Grim_ESHealthComponent.h"
#include "Grim_ES/Components/Character/Grim_ESDamageComponent.h"
#include "Grim_ESDummy.generated.h"

UCLASS()
class GRIM_ES_API AGrim_ESDummy : public APawn
{
	GENERATED_BODY()

	void Regen();

	float deltaTick;
	float cooldown;
	float prevHealth;

public:
	// Sets default values for this pawn's properties
	AGrim_ESDummy();

	UPROPERTY(BlueprintReadOnly, Category = Stats)
	class UGrim_ESHealthComponent* HealthComponent;
	class UGrim_ESDamageComponent* DamageComponent;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
