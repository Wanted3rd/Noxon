#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFinder, Warning, All);

#define __STRINGIFY(x)__ #x
#define STRINGIFY(x) __STRINGIFY__(x)

namespace FinderHelper
{
	template<typename T>
	T* FindAssetFromConstructor(FString Path)
	{
		ConstructorHelpers::FObjectFinder<T> TempAsset(*Path);
		if (TempAsset.Succeeded())
		{
			return TempAsset.Object;
		}

		FString templateName = "Nulllllll";
		if constexpr (TIsDerivedFrom<T, UObject>::Value)
		{
			templateName = T::StaticClass()->GetName();
		}
		else if (FString(typeid(T)) != "")
		{
			templateName = FString(typeid(T));
		}
		UE_LOG(LogFinder, Warning, TEXT("Can't Find Class %s"), *templateName);
		
		return nullptr;
	}

	template<typename T>
	TSubclassOf<T> GetClassFromConstructor(FString Path)
	{
		ConstructorHelpers::FClassFinder<T> TempClass(*Path);
		
		if (TempClass.Succeeded())
		{
			return TempClass.Class;
		}

		FString templateName = "Nulllllll";
		if constexpr (TIsDerivedFrom<T, UObject>::Value)
		{
			templateName = T::StaticClass()->GetName();
		}
		else if (FString(typeid(T)) != "")
		{
			templateName = FString(typeid(T));
		}
		UE_LOG(LogFinder, Warning, TEXT("Can't Find Class %s"), *templateName);
		
		return nullptr;
	}
};

#define BASE_SKM FinderHelper::FindAssetFromConstructor<USkeletalMesh>("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'")
#define BASE_CUBE FinderHelper::FindAssetFromConstructor<UStaticMesh>("/Script/Engine.StaticMesh'/Game/Assets/BaseAssets/Shapes/Shape_Cube.Shape_Cube'")
#define BASE_CYLINDER FinderHelper::FindAssetFromConstructor<UStaticMesh>("/Script/Engine.StaticMesh'/Game/Assets/BaseAssets/Shapes/Shape_Cylinder.Shape_Cylinder'")
#define BASE_SPHERE FinderHelper::FindAssetFromConstructor<UStaticMesh>("/Script/Engine.StaticMesh'/Game/Assets/BaseAssets/Shapes/Shape_Sphere.Shape_Sphere'")
#define BASE_CAPSULE FinderHelper::FindAssetFromConstructor<UStaticMesh>("/Script/Engine.StaticMesh'/Game/Assets/BaseAssets/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'")
#define BASE_PLANE FinderHelper::FindAssetFromConstructor<UStaticMesh>("/Script/Engine.StaticMesh'/Game/Assets/BaseAssets/Shapes/Shape_Plane.Shape_Plane'")
#define BASE_LOGO FinderHelper::FindAssetFromConstructor<UTexture2D>("/Script/Engine.Texture2D'/Game/Assets/BaseAssets/UnrealLogo.UnrealLogo'")
#define BASE_SOUND_FIRE FinderHelper::FindAssetFromConstructor<USoundCue>("/Script/Engine.SoundCue'/Game/Assets/BaseAssets/Audio/Fire01_Cue.Fire01_Cue'")
#define BASE_SOUND_MUSIC FinderHelper::FindAssetFromConstructor<USoundCue>("/Script/Engine.SoundCue'/Game/Assets/BaseAssets/Audio/Starter_Music_Cue.Starter_Music_Cue'")
