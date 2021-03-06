#pragma once

#include <AzCore/Math/Transform.h>
#include <AzCore/Math/Quaternion.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <GameplayEventBus.h>
#include <LmbrCentral/Physics/PhysicsComponentBus.h>

#include <DragonCrashController/DragonCrashControllerBus.h>
#include <LmbrCentral/Scripting/SpawnerComponentBus.h>


namespace DragonCrashController
{
	class DragonCrashControllerComponent
		: public AZ::Component
		, protected DragonCrashControllerRequestBus::Handler
		, public AZ::TickBus::Handler
		, public LmbrCentral::PhysicsComponentNotificationBus::Handler
		, public LmbrCentral::SpawnerComponentNotificationBus::Handler
		, public AZ::GameplayNotificationBus<float>::MultiHandler
	{
	public:
		AZ_COMPONENT(DragonCrashControllerComponent, "{13263517-6125-4A1E-B316-6CF810D340CE}");
		static void Reflect(AZ::ReflectContext* context);
		
		DragonCrashControllerComponent();
		~DragonCrashControllerComponent();

		// AZ::Component
		void Init() override;
		void Activate() override;
		void Deactivate() override;

		// TickBus
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		// PhysicsComponentNotificationBus
		void OnCollision(const Collision& collision) override;

		// SpawnerComponentNotificationBus
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& ticket, const AZ::EntityId& spawnedEntity) override;

		// GameplayNotificationBus
		void OnGameplayEventAction(const float& value) override;
		void OnGameplayEventFailed() override;

		// Controller Bus
		//int getState() override { return m_currentState; }

		void Spawn(AZ::Transform spawnTransform);
		void Kill();

	private:
		void TickFlight(float deltaTime, std::stringstream &debug);
		void TickHover(float deltaTime, std::stringstream &debug);
		void TickMovement(float deltaTime, std::stringstream &debug);
		void TickDragonCrash(float deltaTime, std::stringstream &debug);
		void TickShield(float deltaTime, std::stringstream &debug);
		void TickAttack(float deltaTime, std::stringstream &debug);
		void TickDead(float deltaTime, std::stringstream &debug);

		void CollisionDragon(Collision collision);
		void CollisionAttack(Collision collision);
		void CollisionCrystal(Collision collision);
		void CollisionOther(Collision collision);

		// Editor-exposed settings
			// General
			float m_healthMax;
			float m_respawnTime;
			float m_pitchTurnLimit;

			// Flight mode
			float m_flightAcceleration;
			float m_flightMaxSpeed;
			float m_flightAscendAngle;
			float m_flightYawTurnSpeed;
			float m_flightPitchTurnSpeed;

			// Hover mode
			float m_hoverSpeed;
			float m_hoverAscendSpeed;
			float m_hoverYawTurnSpeed;
			float m_hoverPitchTurnSpeed;
		
			// Boosting and energy
			float m_boostAcceleration;
			float m_boostMaxSpeed;
			float m_boostTurnFactor;
			float m_energyMax;
			float m_energyRechargeTime;
			float m_energyRechargeDelay;

			// Projectile attacks
			AZ::EntityId m_attackSpawnLocation;
			AZ::Data::Asset<AZ::DynamicPrefabAsset> m_primaryAttackSlice;
			AZ::Data::Asset<AZ::DynamicPrefabAsset> m_secondaryAttackSlice;
			float m_primaryAttackCooldown;
			float m_secondaryAttackCooldown;

		// Inputs
		float m_inputMainYaw;
		float m_inputMainPitch;
		float m_inputSecondaryYaw;
		float m_inputSecondaryPitch;
		float m_inputAscend;
		bool m_inputModeSwitch;
		bool m_inputBoost;
		bool m_inputShield;
		bool m_inputAim;
		bool m_inputMainFire;
		bool m_inputSecondaryFire;

		// State values
		int m_currentState;
		float m_healthCurrent;
		AZ::Transform m_respawnTransform;
		float m_respawnTimer;
		float m_modeSwitchTimer;
		float m_yaw;
		float m_pitch;
		bool m_isBoosting;
		bool m_isEnergyExhausted;
		float m_energyRemaining;
		float m_energyRechargeTimer;
		int m_crystalsCollected;
		float m_primaryAttackTimer;
		float m_secondaryAttackTimer;

		// UI values
		AZ::EntityId m_uiCanvasId;
		AZ::EntityId m_uiStatusId;
		AZ::EntityId m_uiInfoId;
	};
}