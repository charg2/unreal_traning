// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle426.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE426_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override final;

	void PlayAttackMontage();

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed; // 애니메이션 블루프린트의 애님 그래프에서 사용

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
