// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"



UABAnimInstance::UABAnimInstance() : CurrentPawnSpeed{ 0.0f }, IsInAir{ false }
{}

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
