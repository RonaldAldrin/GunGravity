// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActor/AbilityActorBase.h"
#include "AbilitySmokeGrenade.generated.h"

class UProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class GRAVITYGUNTEST_API AAbilitySmokeGrenade : public AAbilityActorBase
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
public:

	AAbilitySmokeGrenade();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	virtual void BeginPlay() override;
};
