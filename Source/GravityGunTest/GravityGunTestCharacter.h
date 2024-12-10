// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GravityGunTestCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class AAbilityActorBase;
class AAbilitySmokeGrenade;
class AAbilitySummonTotem;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkill1UseSignature,bool,UsingSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkill2UseSignature, bool, UsingSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveSignature);

UCLASS(config=Game)
class AGravityGunTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction2;

	
public:
	AGravityGunTestCharacter();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnObjectiveSignature OnObjective;

protected:

	/** smoke grenade class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = AbilityClass)
	TSubclassOf<AAbilityActorBase> AbilitySmokeGrenadeClass;

	UPROPERTY()
	TObjectPtr<AAbilityActorBase> SmokeGrenade;

	/** summon totem class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = AbilityClass)
	TSubclassOf<AAbilityActorBase> AbilitySummonTotemClass;

	UPROPERTY()
	TObjectPtr<AAbilityActorBase> SummonTotem;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnSkill1UseSignature OnSkill1Use;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSkill2UseSignature OnSkill2Use;

	bool bPressedSkill1 = false;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category ="SkillTimerHandle")
	FTimerHandle Skill1TimerHandle;

	bool bPressedSkill2 = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "SkillTimerHandle")
	FTimerHandle Skill2TimerHandle;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void UseSkill1();

	void UseSkill2();

	UFUNCTION()
	void SkillCooldown1();

	UFUNCTION()
	void SkillCooldown2();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetCooldownPercentage(FTimerHandle SkillTimerHandle);

	void SpawnSkill(UWorld* World,TSubclassOf<AAbilityActorBase> SkilltoSpawnClass, AAbilityActorBase* SkilltoSpawn,const FVector Location,const FRotator Rotation);

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

