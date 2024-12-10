// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/AbilityDetailStruct.h"
#include "AbilityActorBase.generated.h"

class AGravityGunTestCharacter;

UCLASS(Blueprintable)
class GRAVITYGUNTEST_API AAbilityActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityActorBase();

	UPROPERTY(BlueprintReadWrite,EditAnywhere , Category = "Ability Detail")
	FAbilityDetailInfo AbilityDetailInfo;

	UPROPERTY()
	TObjectPtr<AGravityGunTestCharacter> Character;

	/** Static mesh component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
