// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
  Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  AActor *triggeredActor = GetAcceptableActor();
  if (triggeredActor != nullptr)
  {
    UPrimitiveComponent *component = Cast<UPrimitiveComponent>(triggeredActor->GetRootComponent());
    if (component != nullptr)
    {
      if (Mover != nullptr)
      {
        component->SetSimulatePhysics(false);
        triggeredActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
      }
      else if (TextRender != nullptr)
      {
        if (TextRender->bHiddenInGame)
        {
          TextRender->SetHiddenInGame(false);
        }
      }
    }

    if (Mover != nullptr)
    {
      Mover->SetShouldMove(true);
    }
  }
  else
  {
    if (Mover != nullptr)
    {
      Mover->SetShouldMove(false);
    }
  }
}

void UTriggerComponent::SetMover(UMover *NewMover)
{
  Mover = NewMover;
}

void UTriggerComponent::SetTextRender(UTextRenderComponent *NewTextRender)
{
  TextRender = NewTextRender;
}

AActor *UTriggerComponent::GetAcceptableActor()
{
  TArray<AActor *> OverlappingActors;
  GetOverlappingActors(OverlappingActors);

  // UE_LOG(LogTemp, Display, TEXT("GetAcceptableActor for %s %i"), *GetOwner()->GetActorNameOrLabel(), OverlappingActors.Num());
  if (OverlappingActors.Num() > 0)
  {
    for (AActor *Actor : OverlappingActors)
    {
      // UE_LOG(LogTemp, Display, TEXT("Overlapping actor %s"), *Actor->GetActorNameOrLabel());
      //    the overlapping actor only works if it has the Trigger tag on it.
      if (Actor->ActorHasTag(TriggerTag) && !Actor->Tags.Contains("Grabbed"))
      {
        // UE_LOG(LogTemp, Display, TEXT("ActorHasTag actor %s %s"), *Actor->GetActorNameOrLabel(), *TriggerTag.ToString());
        return Actor;
      }
    }
  }
  return nullptr;
}
