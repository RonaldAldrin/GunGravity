// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActor/AbilityActorBase.h"
#include "AbilitySmokeGrenade.generated.h"

class UProjectileMovementComponent;

/**
 * 
 */
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GRAVITYGUNTEST_API AAbilitySmokeGrenade : public AAbilityActorBase
{
	GENERATED_BODY()



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

	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Niagara")
	TObjectPtr<UNiagaraSystem> SmokeSystem;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> Smoke;
};
