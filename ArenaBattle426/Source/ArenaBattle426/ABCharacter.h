// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle426.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE426_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override final;

	enum class EControlMode
	{
		GTA, 
		DIABLO,
	};

	void SetControlMode(EControlMode ControlMode);
	//EControlMode CurrentControlMode = EControlMode::DIABLO;
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove;// = FVector::ZeroVector;
	
	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override final;
	virtual void PostInitializeComponents() override final;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override final;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;


	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruted);
	//FScriptDelegate OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruted);
	
	void AttackStartComboState();
	void AttackEndComboState();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY()
	class UABAnimInstance* ABAnim;
};
