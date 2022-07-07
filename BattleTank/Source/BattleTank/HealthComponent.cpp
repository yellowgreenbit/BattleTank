
#include "HealthComponent.h"

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float TakeDamageValue = DamageData.DamageValue;
	CurrentHealth -= TakeDamageValue;

	if (CurrentHealth <= 0.f) {
		if (OnDie.IsBound())
			OnDie.Broadcast();
	}
	else
	{
		if (OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(TakeDamageValue);
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddHealthValue)
{
	CurrentHealth += AddHealthValue;

	if (CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}