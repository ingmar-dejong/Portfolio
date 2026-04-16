// Copyright Unchained.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ExternalAISettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "External AI"))
class UNCHAINED_API UExternalAISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Game"); }

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Connection")
	FString EndpointUrl = TEXT("http://127.0.0.1:8080/api/npc/decision");

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Connection", meta = (ClampMin = "0.1"))
	float RequestTimeoutSeconds = 5.0f;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Testing")
	bool bUseMockResponses = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Testing")
	bool bLogPayloads = true;
};
