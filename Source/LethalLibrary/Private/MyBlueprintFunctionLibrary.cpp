// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

bool UMyBlueprintFunctionLibrary::ListAllBlueprintsInPath(FName Path, TArray<UClass*>& Result, UClass* Class)
{
	auto Library = UObjectLibrary::CreateLibrary(Class, true, GIsEditor);
	Library->LoadBlueprintAssetDataFromPath(Path.ToString());

	TArray<FAssetData> Assets;
	Library->GetAssetDataList(Assets);

	for (auto& Asset : Assets)
	{
		UBlueprint* bp = Cast<UBlueprint>(Asset.GetAsset());
		if (bp)
		{
			auto gc = bp->GeneratedClass;
			if (gc)
			{
				Result.Add(gc);
			}
		}
		else
		{
			auto GeneratedClassName = (Asset.AssetName.ToString() + "_C");

			UClass* Clazz = FindObject<UClass>(Asset.GetPackage(), *GeneratedClassName);
			if (Clazz)
			{
				Result.Add(Clazz);
			}
			else
			{
				UObjectRedirector* RenamedClassRedirector = FindObject<UObjectRedirector>(Asset.GetPackage(), *GeneratedClassName);
				if (RenamedClassRedirector)
				{
					Result.Add(CastChecked<UClass>(RenamedClassRedirector->DestinationObject));
				}
			}

		}
	}

	return true;
}