// Copyright Unchained.

#pragma once

#include "CoreMinimal.h"
#include "ExternalAIIntegrationTypes.h"
#include "Components/ActorComponent.h"
#include "ExternalAIConsumerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnExternalAIConsumerUpdated,
	const FExternalAIResponse&, Response,
	bool, bSuccess,
	const FString&, ErrorMessage);

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class UNCHAINED_API UExternalAIConsumerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExternalAIConsumerComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External AI")
	FString NpcIdOverride;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "External AI")
	FString LastRequestId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "External AI")
	FString CurrentMood;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "External AI")
	float CurrentThreatScore = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "External AI")
	FString LastRecommendedAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "External AI")
	FString LastReplyText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "External AI")
	FString LastErrorMessage;

	UPROPERTY(BlueprintAssignable, Category = "External AI")
	FOnExternalAIConsumerUpdated OnAIStateUpdated;

	UFUNCTION(BlueprintCallable, Category = "Unchained|External AI")
	FString RequestAIUpdate(int32 PlayerReputation, const FString& Location, bool bActiveAlert, const FString& AdditionalContextJson);

	UFUNCTION(BlueprintCallable, Category = "Unchained|External AI")
	void ApplyAIResponse(const FExternalAIResponse& Response);

private:
	UFUNCTION()
	void HandleSubsystemResponse(const FString& RequestId, const FExternalAIResponse& Response, bool bSuccess, const FString& ErrorMessage);

	FString ResolveNpcId() const;
	FString ResolveLocationString(const FString& ExplicitLocation) const;
};
