#include "Quest/PAQuestComponent.h"

UPAQuestComponent::UPAQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    bQuestCompleted = false;
}

bool UPAQuestComponent::IsQuestCompleted() const
{
    return bQuestCompleted;
}

FString UPAQuestComponent::GetObjectiveText() const
{
    return bQuestCompleted
        ? TEXT("Quest erledigt: Du hast mit dem Dorfältesten gesprochen.")
        : TEXT("Quest: Finde den Dorfältesten und drücke E zur Interaktion.");
}

void UPAQuestComponent::HandleTalkedToElder()
{
    bQuestCompleted = true;
}
