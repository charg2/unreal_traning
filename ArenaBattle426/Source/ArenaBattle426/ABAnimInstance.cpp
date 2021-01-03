// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"


UABAnimInstance::UABAnimInstance() : CurrentPawnSpeed{ 0.0f }, IsInAir{ false }
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE{TEXT("AnimMontage'/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage'")};
	if (true == ATTACK_MONTAGE.Succeeded() )
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (true == ::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size(); // 속력을 가져와서 저장.
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}


void UABAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	//Montage_Play(AttackMontage, 1.0f);

	ABCHECK(Montage_IsPlaying(AttackMontage));
	//ABLOG(Warning, TEXT("JumpToAttackMontageSection( %d )"), NewSection );

	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}


void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}


void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	ABLOG_S(Warning);
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);

	//ABLOG(Warning, TEXT("Attack%d"), Section );

	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}