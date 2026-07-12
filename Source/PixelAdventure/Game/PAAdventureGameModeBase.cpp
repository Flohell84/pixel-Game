#include "Game/PAAdventureGameModeBase.h"
#include "Game/PAPixelCharacter.h"

APAAdventureGameModeBase::APAAdventureGameModeBase()
{
    DefaultPawnClass = APAPixelCharacter::StaticClass();
}
