// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Grim_ESHealthComponent.generated.h"

USTRUCT() // Here are the different stats that a character has, we set them when we find out what type of class they are.
struct FHealthData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float healthRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float healthRegen;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIM_ES_API UGrim_ESHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	void RegenHealth();

	float deltaTick;

	DECLARE_DELEGATE_OneParam(ActorDelegate, AActor*)
	ActorDelegate UnitKilled;

public:	
	// Sets default values for this component's properties
	UGrim_ESHealthComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void AddHealth(const float healthToAdd);

	void SubtractHealth(const float healthToSubtract);

	void StartTick();

	void StopTick();

	bool GetTicking();

	UPROPERTY(BlueprintReadOnly, Category = Health)
	FHealthData HealthConfig;

	ActorDelegate UnitKilledDelegate();
};
