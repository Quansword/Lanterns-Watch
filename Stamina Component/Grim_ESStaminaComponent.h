// Copyright 2015-2017 Molotov Industries, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Grim_ESStaminaComponent.generated.h"

USTRUCT() // Here are the different stats that a character has, we set them when we find out what type of class they are.
struct FStaminaData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float maxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float staminaRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float staminaRegen;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GRIM_ES_API UGrim_ESStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	void RegenStamina();

	float deltaTick;

public:
	// Sets default values for this component's properties
	UGrim_ESStaminaComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddStamina(const float staminaToAdd);

	void SubtractStamina(const float staminaToSubtract);

	void StartTick();

	void StopTick();

	bool GetTicking();

	UPROPERTY(BlueprintReadOnly, Category = Health)
	FStaminaData StaminaConfig;

	bool zeroStamina;
};
