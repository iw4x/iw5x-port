#include <std_include.hpp>

#include "iweapon.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include <module/console.hpp>

namespace asset_dumpers
{

#define CONVERT_SOUND(name)\
if (asset->##name)\
target->##name = exporter::convert(game::native::ASSET_TYPE_SOUND, { asset->##name }).sound

#define CONVERT_FX(name)\
if (asset->##name)\
target->##name = exporter::convert(game::native::ASSET_TYPE_FX, { asset->##name }).fx

#define CONVERT_MATERIAL(name)\
if (asset->##name)\
target->##name = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { asset->##name }).material

#define CONVERT_XMODEL(name)\
if (asset->##name)\
target->##name = exporter::convert(game::native::ASSET_TYPE_XMODEL, { asset->##name }).model

#define COPY_ARRAY(member, type)\
std::memcpy(target->##member, asset->##member, ARRAYSIZE(asset->##member) * sizeof(type));

	iw4::native::WeaponCompleteDef* iweapon::convert(const game::native::WeaponCompleteDef* asset)
	{		
		iw4::native::WeaponCompleteDef* target = local_allocator.allocate<iw4::native::WeaponCompleteDef>();

		std::string internal_name = asset->szInternalName;
		target->szInternalName = internal_name.starts_with("iw5_") ?
			asset->szInternalName :
			local_allocator.duplicate_string("iw5_"s + asset->szInternalName);

		if (asset->szAltWeaponName && strnlen(asset->szAltWeaponName, 1) > 0)
		{
			target->szAltWeaponName = local_allocator.duplicate_string("iw5_"s + asset->szAltWeaponName);
		}

		target->hideTags = asset->hideTags;
		target->szXAnims = asset->szXAnims;
		target->szDisplayName = asset->szDisplayName;
		target->fAdsZoomFov = asset->fAdsZoomFov;
		target->iAdsTransInTime = asset->iAdsTransInTime;
		target->iAdsTransOutTime = asset->iAdsTransOutTime;
		target->iClipSize = asset->iClipSize;
		target->impactType = asset->impactType;
		target->iFireTime = asset->iFireTime;
		target->dpadIconRatio = asset->dpadIconRatio;
		target->penetrateMultiplier = asset->penetrateMultiplier;
		target->fAdsViewKickCenterSpeed = asset->fAdsViewKickCenterSpeed;
		target->fHipViewKickCenterSpeed = asset->fHipViewKickCenterSpeed;
		target->iAltRaiseTime = asset->iAltRaiseTime;
		target->fireAnimLength = asset->fireAnimLength;
		target->iFirstRaiseTime = asset->iFirstRaiseTime;
		target->ammoDropStockMax = asset->ammoDropStockMax;
		target->adsDofStart = asset->adsDofStart;
		target->adsDofEnd = asset->adsDofEnd;
		target->motionTracker = asset->motionTracker;
		target->enhanced = asset->enhanced;
		target->dpadIconShowsAmmo = asset->dpadIconShowsAmmo;

		if (asset->killIcon)
		{
			target->killIcon = exporter::convert(game::native::XAssetType::ASSET_TYPE_MATERIAL, { asset->killIcon }).material;
		}

		if (asset->dpadIcon)
		{
			target->dpadIcon = exporter::convert(game::native::XAssetType::ASSET_TYPE_MATERIAL, { asset->dpadIcon }).material;
		}

		target->weapDef = convert(asset->weapDef);

		if (asset->scopes)
		{
		if (std::string(asset->szInternalName).contains("pecheneg_mp"s))
		{
			printf("");
		}
			const auto scope = *asset->scopes;
			if (scope)
			{
				for (size_t i = 0; i < 16; i++)
				{
					if (target->weapDef->gunXModel[i])
					{
						const auto converted_world = exporter::convert(game::native::XAssetType::ASSET_TYPE_XMODEL, { scope->worldModels[i] }).model; 
						const auto converted_view = exporter::convert(game::native::XAssetType::ASSET_TYPE_XMODEL, { scope->viewModels[i] }).model;  
						//const auto converted_reticle = exporter::convert(game::native::XAssetType::ASSET_TYPE_XMODEL, { scope->reticleViewModels[i] }).model; 

						target->weapDef->gunXModel[i] = append(target->weapDef->gunXModel[i], converted_view);
						target->weapDef->worldModel[i] = append(target->weapDef->worldModel[i], converted_world);
					}
				}
			}
		}

		return target;
	}

	iw4::native::WeaponDef* iweapon::convert(const game::native::WeaponDef* asset)
	{
		iw4::native::WeaponDef* target = local_allocator.allocate<iw4::native::WeaponDef>();

		CONVERT_XMODEL(handXModel);
		CONVERT_FX(viewFlashEffect);
		CONVERT_FX(worldFlashEffect);
		CONVERT_SOUND(pickupSound);
		CONVERT_SOUND(pickupSoundPlayer);
		CONVERT_SOUND(ammoPickupSound);
		CONVERT_SOUND(ammoPickupSoundPlayer);
		CONVERT_SOUND(projectileSound);
		CONVERT_SOUND(pullbackSound);
		CONVERT_SOUND(pullbackSoundPlayer);
		CONVERT_SOUND(fireSound);
		CONVERT_SOUND(fireSoundPlayer);
		CONVERT_SOUND(fireSoundPlayerAkimbo);
		CONVERT_SOUND(fireLoopSound);
		CONVERT_SOUND(fireLoopSoundPlayer);
		CONVERT_SOUND(fireStopSound);
		CONVERT_SOUND(fireStopSoundPlayer);
		CONVERT_SOUND(fireLastSound);
		CONVERT_SOUND(fireLastSoundPlayer);
		CONVERT_SOUND(emptyFireSound);
		CONVERT_SOUND(emptyFireSoundPlayer);
		CONVERT_SOUND(meleeSwipeSound);
		CONVERT_SOUND(meleeSwipeSoundPlayer);
		CONVERT_SOUND(meleeHitSound);
		CONVERT_SOUND(meleeMissSound);
		CONVERT_SOUND(rechamberSound);
		CONVERT_SOUND(rechamberSoundPlayer);
		CONVERT_SOUND(reloadSound);
		CONVERT_SOUND(reloadSoundPlayer);
		CONVERT_SOUND(reloadEmptySound);
		CONVERT_SOUND(reloadEmptySoundPlayer);
		CONVERT_SOUND(reloadStartSound);
		CONVERT_SOUND(reloadStartSoundPlayer);
		CONVERT_SOUND(reloadEndSound);
		CONVERT_SOUND(reloadEndSoundPlayer);
		CONVERT_SOUND(detonateSound);
		CONVERT_SOUND(detonateSoundPlayer);
		CONVERT_SOUND(nightVisionWearSound);
		CONVERT_SOUND(nightVisionWearSoundPlayer);
		CONVERT_SOUND(nightVisionRemoveSound);
		CONVERT_SOUND(nightVisionRemoveSoundPlayer);
		CONVERT_SOUND(altSwitchSound);
		CONVERT_SOUND(altSwitchSoundPlayer);
		CONVERT_SOUND(raiseSound);
		CONVERT_SOUND(raiseSoundPlayer);
		CONVERT_SOUND(firstRaiseSound);
		CONVERT_SOUND(firstRaiseSoundPlayer);
		CONVERT_SOUND(putawaySound);
		CONVERT_SOUND(putawaySoundPlayer);
		CONVERT_SOUND(scanSound);

		if (asset->bounceSound)
		{
			target->bounceSound = local_allocator.allocate_array<iw4::native::snd_alias_list_t*>(31);
			for (size_t i = 0; i < 31; i++)
			{
				if (asset->bounceSound[i])
				{
					target->bounceSound[i] = exporter::convert(game::native::ASSET_TYPE_SOUND, { asset->bounceSound[i] }).sound;
				}
			}
		}

		CONVERT_FX(viewShellEjectEffect);
		CONVERT_FX(worldShellEjectEffect);
		CONVERT_FX(viewLastShotEjectEffect);
		CONVERT_FX(worldLastShotEjectEffect);
		CONVERT_FX(projIgnitionEffect);
		CONVERT_FX(projTrailEffect);
		CONVERT_FX(projBeaconEffect);
		CONVERT_FX(projExplosionEffect);
		CONVERT_FX(projDudEffect);
		CONVERT_FX(turretOverheatEffect);

		CONVERT_MATERIAL(reticleCenter);
		CONVERT_MATERIAL(reticleSide);
		CONVERT_MATERIAL(hudIcon);
		CONVERT_MATERIAL(pickupIcon);
		CONVERT_MATERIAL(ammoCounterIcon);

		if (asset->overlay.shader)
			target->overlayMaterial = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { asset->overlay.shader }).material;

		if (asset->overlay.shaderLowRes)
			target->overlayMaterialLowRes = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { asset->overlay.shaderLowRes }).material;

		if (asset->overlay.shaderEMP)
			target->overlayMaterialEMP = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { asset->overlay.shaderEMP }).material;

		if (asset->overlay.shaderEMPLowRes)
			target->overlayMaterialEMPLowRes = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { asset->overlay.shaderEMPLowRes }).material;

		target->worldModel = local_allocator.allocate_array<iw4::native::XModel*>(16);
		target->gunXModel = local_allocator.allocate_array<iw4::native::XModel*>(16);
		for (size_t i = 0; i < 16; i++)
		{
			if (asset->worldModel[i])
			{
				target->worldModel[i] = exporter::convert(game::native::ASSET_TYPE_XMODEL, { asset->worldModel[i] }).model;
			}

			if (asset->gunXModel[i])
			{
				target->gunXModel[i] = exporter::convert(game::native::ASSET_TYPE_XMODEL, { asset->gunXModel[i] }).model;
			}
		}

		CONVERT_XMODEL(worldClipModel);
		CONVERT_XMODEL(rocketModel);
		CONVERT_XMODEL(knifeModel);
		CONVERT_XMODEL(worldKnifeModel);
		CONVERT_XMODEL(projectileModel);

		CONVERT_SOUND(projExplosionSound);
		CONVERT_SOUND(projDudSound);
		CONVERT_SOUND(projIgnitionSound);
		CONVERT_SOUND(turretBarrelSpinMaxSnd);
		CONVERT_SOUND(missileConeSoundAlias);
		CONVERT_SOUND(missileConeSoundAliasAtBase);
		CONVERT_SOUND(turretOverheatSound);

		for (size_t i = 0; i < 4; i++)
		{
			CONVERT_SOUND(turretBarrelSpinUpSnd[i]);
			CONVERT_SOUND(turretBarrelSpinDownSnd[i]);
		}

		COPY_ARRAY(vStandMove, float);
		COPY_ARRAY(vStandRot, float);
		COPY_ARRAY(strafeMove, float);
		COPY_ARRAY(strafeRot, float);
		COPY_ARRAY(vDuckedOfs, float);
		COPY_ARRAY(vDuckedMove, float);
		COPY_ARRAY(vDuckedRot, float);
		COPY_ARRAY(vProneOfs, float);
		COPY_ARRAY(vProneMove, float);
		COPY_ARRAY(vProneRot, float);
		COPY_ARRAY(vProjectileColor, float);
		COPY_ARRAY(minTurnSpeed, float);
		COPY_ARRAY(maxTurnSpeed, float);
		COPY_ARRAY(fOOPosAnimLength, float);

		target->szOverlayName = asset->szOverlayName;
		target->szXAnimsRightHanded = asset->szXAnimsRightHanded;
		target->szXAnimsLeftHanded = asset->szXAnimsLeftHanded;
		target->szModeName = asset->szModeName;
		target->notetrackSoundMapKeys = asset->notetrackSoundMapKeys;
		target->notetrackSoundMapValues = asset->notetrackSoundMapValues;
		target->notetrackRumbleMapKeys = asset->notetrackRumbleMapKeys;
		target->notetrackRumbleMapValues = asset->notetrackRumbleMapValues;
		target->playerAnimType = asset->playerAnimType;
		target->weapType = static_cast<iw4::native::weapType_t>(
			asset->weapType == game::native::weapType_t::WEAPTYPE_NONE ?
			iw4::native::weapType_t::WEAPTYPE_BULLET :
			asset->weapType - 1
			);
		target->weapClass = asset->weapClass;
		target->penetrateType = asset->penetrateType;
		target->inventoryType = asset->inventoryType;
		target->fireType = asset->fireType;
		target->offhandClass = asset->offhandClass;
		target->stance = asset->stance;
		target->iReticleCenterSize = asset->iReticleCenterSize;
		target->iReticleSideSize = asset->iReticleSideSize;
		target->iReticleMinOfs = asset->iReticleMinOfs;
		target->activeReticleType = asset->activeReticleType;

		target->fPosMoveRate = asset->fPosMoveRate;
		target->fPosProneMoveRate = asset->fPosProneMoveRate;
		target->fStandMoveMinSpeed = asset->fStandMoveMinSpeed;
		target->fDuckedMoveMinSpeed = asset->fDuckedMoveMinSpeed;
		target->fProneMoveMinSpeed = asset->fProneMoveMinSpeed;
		target->fPosRotRate = asset->fPosRotRate;
		target->fPosProneRotRate = asset->fPosProneRotRate;
		target->fStandRotMinSpeed = asset->fStandRotMinSpeed;
		target->fDuckedRotMinSpeed = asset->fDuckedRotMinSpeed;
		target->fProneRotMinSpeed = asset->fProneRotMinSpeed;
		target->hudIconRatio = asset->hudIconRatio;
		target->pickupIconRatio = asset->pickupIconRatio;
		target->ammoCounterIconRatio = asset->ammoCounterIconRatio;
		target->ammoCounterClip = asset->ammoCounterClip;
		target->iStartAmmo = asset->iStartAmmo;
		target->szAmmoName = asset->szAmmoName;
		target->iAmmoIndex = asset->iAmmoIndex;
		target->szClipName = asset->szClipName;
		target->iClipIndex = asset->iClipIndex;
		target->iMaxAmmo = asset->iMaxAmmo;
		target->shotCount = asset->shotCount;
		target->szSharedAmmoCapName = asset->szSharedAmmoCapName;
		target->iSharedAmmoCapIndex = asset->iSharedAmmoCapIndex;
		target->iSharedAmmoCap = asset->iSharedAmmoCap;
		target->damage = asset->damage;
		target->playerDamage = asset->playerDamage;
		target->iMeleeDamage = asset->iMeleeDamage;
		target->iDamageType = asset->iDamageType;
		target->iFireDelay = asset->stateTimers.iFireDelay;
		target->iMeleeDelay = asset->stateTimers.iMeleeDelay;
		target->meleeChargeDelay = asset->stateTimers.meleeChargeDelay;
		target->iDetonateDelay = asset->stateTimers.iDetonateDelay;
		target->iRechamberTime = asset->stateTimers.iRechamberTime;
		target->rechamberTimeOneHanded = asset->stateTimers.rechamberTimeOneHanded;
		target->iRechamberBoltTime = asset->stateTimers.iRechamberBoltTime;
		target->iHoldFireTime = asset->stateTimers.iHoldFireTime;
		target->iDetonateTime = asset->stateTimers.iDetonateTime;
		target->iMeleeTime = asset->stateTimers.iMeleeTime;
		target->meleeChargeTime = asset->stateTimers.meleeChargeTime;
		target->iReloadTime = asset->stateTimers.iReloadTime;
		target->reloadShowRocketTime = asset->stateTimers.reloadShowRocketTime;
		target->iReloadEmptyTime = asset->stateTimers.iReloadEmptyTime;
		target->iReloadAddTime = asset->stateTimers.iReloadAddTime;
		target->iReloadStartTime = asset->stateTimers.iReloadStartTime;
		target->iReloadStartAddTime = asset->stateTimers.iReloadStartAddTime;
		target->iReloadEndTime = asset->stateTimers.iReloadEndTime;
		target->iDropTime = asset->stateTimers.iDropTime;
		target->iRaiseTime = asset->stateTimers.iRaiseTime;
		target->iAltDropTime = asset->stateTimers.iAltDropTime;
		target->quickDropTime = asset->stateTimers.quickDropTime;
		target->quickRaiseTime = asset->stateTimers.quickRaiseTime;
		target->iBreachRaiseTime = asset->stateTimers.iBreachRaiseTime;
		target->iEmptyRaiseTime = asset->stateTimers.iEmptyRaiseTime;
		target->iEmptyDropTime = asset->stateTimers.iEmptyDropTime;
		target->sprintInTime = asset->stateTimers.sprintInTime;
		target->sprintLoopTime = asset->stateTimers.sprintLoopTime;
		target->sprintOutTime = asset->stateTimers.sprintOutTime;
		target->stunnedTimeBegin = asset->stateTimers.stunnedTimeBegin;
		target->stunnedTimeLoop = asset->stateTimers.stunnedTimeLoop;
		target->stunnedTimeEnd = asset->stateTimers.stunnedTimeEnd;
		target->nightVisionWearTime = asset->stateTimers.nightVisionWearTime;
		target->nightVisionWearTimeFadeOutEnd = asset->stateTimers.nightVisionWearTimeFadeOutEnd;
		target->nightVisionWearTimePowerUp = asset->stateTimers.nightVisionWearTimePowerUp;
		target->nightVisionRemoveTime = asset->stateTimers.nightVisionRemoveTime;
		target->nightVisionRemoveTimePowerDown = asset->stateTimers.nightVisionRemoveTimePowerDown;
		target->nightVisionRemoveTimeFadeInStart = asset->stateTimers.nightVisionRemoveTimeFadeInStart;
		target->fuseTime = asset->stateTimers.fuseTime;
		target->aiFuseTime = asset->stateTimers.aiFuseTime;
		target->autoAimRange = asset->autoAimRange;
		target->aimAssistRange = asset->aimAssistRange;
		target->aimAssistRangeAds = asset->aimAssistRangeAds;
		target->aimPadding = asset->aimPadding;
		target->enemyCrosshairRange = asset->enemyCrosshairRange;
		target->moveSpeedScale = asset->moveSpeedScale;
		target->adsMoveSpeedScale = asset->adsMoveSpeedScale;
		target->sprintDurationScale = asset->sprintDurationScale;
		target->fAdsZoomInFrac = asset->fAdsZoomInFrac;
		target->fAdsZoomOutFrac = asset->fAdsZoomOutFrac;
		target->overlayReticle = asset->overlay.reticle;
		target->overlayInterface = asset->overlayInterface;
		target->overlayWidth = asset->overlay.width;
		target->overlayHeight = asset->overlay.height;
		target->overlayWidthSplitscreen = asset->overlay.widthSplitscreen;
		target->overlayHeightSplitscreen = asset->overlay.heightSplitscreen;
		target->fAdsBobFactor = asset->fAdsBobFactor;
		target->fAdsViewBobMult = asset->fAdsViewBobMult;
		target->fHipSpreadStandMin = asset->fHipSpreadStandMin;
		target->fHipSpreadDuckedMin = asset->fHipSpreadDuckedMin;
		target->fHipSpreadProneMin = asset->fHipSpreadProneMin;
		target->hipSpreadStandMax = asset->hipSpreadStandMax;
		target->hipSpreadDuckedMax = asset->hipSpreadDuckedMax;
		target->hipSpreadProneMax = asset->hipSpreadProneMax;
		target->fHipSpreadDecayRate = asset->fHipSpreadDecayRate;
		target->fHipSpreadFireAdd = asset->fHipSpreadFireAdd;
		target->fHipSpreadTurnAdd = asset->fHipSpreadTurnAdd;
		target->fHipSpreadMoveAdd = asset->fHipSpreadMoveAdd;
		target->fHipSpreadDuckedDecay = asset->fHipSpreadDuckedDecay;
		target->fHipSpreadProneDecay = asset->fHipSpreadProneDecay;
		target->fHipReticleSidePos = asset->fHipReticleSidePos;
		target->fAdsIdleAmount = asset->fAdsIdleAmount;
		target->fHipIdleAmount = asset->fHipIdleAmount;
		target->adsIdleSpeed = asset->adsIdleSpeed;
		target->hipIdleSpeed = asset->hipIdleSpeed;
		target->fIdleCrouchFactor = asset->fIdleCrouchFactor;
		target->fIdleProneFactor = asset->fIdleProneFactor;
		target->fGunMaxPitch = asset->fGunMaxPitch;
		target->fGunMaxYaw = asset->fGunMaxYaw;
		target->swayMaxAngle = asset->swayMaxAngle;
		target->swayLerpSpeed = asset->swayLerpSpeed;
		target->swayPitchScale = asset->swayPitchScale;
		target->swayYawScale = asset->swayYawScale;
		target->swayHorizScale = asset->swayHorizScale;
		target->swayVertScale = asset->swayVertScale;
		target->swayShellShockScale = asset->swayShellShockScale;
		target->adsSwayMaxAngle = asset->adsSwayMaxAngle;
		target->adsSwayLerpSpeed = asset->adsSwayLerpSpeed;
		target->adsSwayPitchScale = asset->adsSwayPitchScale;
		target->adsSwayYawScale = asset->adsSwayYawScale;
		target->adsSwayHorizScale = asset->adsSwayHorizScale;
		target->adsSwayVertScale = asset->adsSwayVertScale;
		target->adsViewErrorMin = asset->adsViewErrorMin;
		target->adsViewErrorMax = asset->adsViewErrorMax;
		//game::native::PhysCollmap* physCollmap;
		target->dualWieldViewModelOffset = asset->dualWieldViewModelOffset;
		target->killIconRatio = asset->killIconRatio;
		target->iReloadAmmoAdd = asset->iReloadAmmoAdd;
		target->iReloadStartAdd = asset->iReloadStartAdd;
		target->ammoDropStockMin = asset->ammoDropStockMin;
		target->ammoDropClipPercentMin = asset->ammoDropClipPercentMin;
		target->ammoDropClipPercentMax = asset->ammoDropClipPercentMax;
		target->iExplosionRadius = asset->iExplosionRadius;
		target->iExplosionRadiusMin = asset->iExplosionRadiusMin;
		target->iExplosionInnerDamage = asset->iExplosionInnerDamage;
		target->iExplosionOuterDamage = asset->iExplosionOuterDamage;
		target->damageConeAngle = asset->damageConeAngle;
		target->bulletExplDmgMult = asset->bulletExplDmgMult;
		target->bulletExplRadiusMult = asset->bulletExplRadiusMult;
		target->iProjectileSpeed = asset->iProjectileSpeed;
		target->iProjectileSpeedUp = asset->iProjectileSpeedUp;
		target->iProjectileSpeedForward = asset->iProjectileSpeedForward;
		target->iProjectileActivateDist = asset->iProjectileActivateDist;
		target->projLifetime = asset->projLifetime;
		target->timeToAccelerate = asset->timeToAccelerate;
		target->projectileCurvature = asset->projectileCurvature;
		target->projExplosion = asset->projExplosion;
		target->stickiness = asset->stickiness;
		target->lowAmmoWarningThreshold = asset->lowAmmoWarningThreshold;
		target->ricochetChance = asset->ricochetChance;
		target->parallelBounce = asset->parallelBounce;
		target->perpendicularBounce = asset->perpendicularBounce;
		target->guidedMissileType = asset->guidedMissileType;
		target->maxSteeringAccel = asset->maxSteeringAccel;
		target->projIgnitionDelay = asset->projIgnitionDelay;
		target->fAdsAimPitch = asset->fAdsAimPitch;
		target->fAdsCrosshairInFrac = asset->fAdsCrosshairInFrac;
		target->fAdsCrosshairOutFrac = asset->fAdsCrosshairOutFrac;
		target->adsGunKickReducedKickBullets = asset->adsGunKickReducedKickBullets;
		target->adsGunKickReducedKickPercent = asset->adsGunKickReducedKickPercent;
		target->fAdsGunKickPitchMin = asset->fAdsGunKickPitchMin;
		target->fAdsGunKickPitchMax = asset->fAdsGunKickPitchMax;
		target->fAdsGunKickYawMin = asset->fAdsGunKickYawMin;
		target->fAdsGunKickYawMax = asset->fAdsGunKickYawMax;
		target->fAdsGunKickAccel = asset->fAdsGunKickAccel;
		target->fAdsGunKickSpeedMax = asset->fAdsGunKickSpeedMax;
		target->fAdsGunKickSpeedDecay = asset->fAdsGunKickSpeedDecay;
		target->fAdsGunKickStaticDecay = asset->fAdsGunKickStaticDecay;
		target->fAdsViewKickPitchMin = asset->fAdsViewKickPitchMin;
		target->fAdsViewKickPitchMax = asset->fAdsViewKickPitchMax;
		target->fAdsViewKickYawMin = asset->fAdsViewKickYawMin;
		target->fAdsViewKickYawMax = asset->fAdsViewKickYawMax;
		target->fAdsViewScatterMin = asset->fAdsViewScatterMin;
		target->fAdsViewScatterMax = asset->fAdsViewScatterMax;
		target->fAdsSpread = asset->fAdsSpread;
		target->hipGunKickReducedKickBullets = asset->hipGunKickReducedKickBullets;
		target->hipGunKickReducedKickPercent = asset->hipGunKickReducedKickPercent;
		target->fHipGunKickPitchMin = asset->fHipGunKickPitchMin;
		target->fHipGunKickPitchMax = asset->fHipGunKickPitchMax;
		target->fHipGunKickYawMin = asset->fHipGunKickYawMin;
		target->fHipGunKickYawMax = asset->fHipGunKickYawMax;
		target->fHipGunKickAccel = asset->fHipGunKickAccel;
		target->fHipGunKickSpeedMax = asset->fHipGunKickSpeedMax;
		target->fHipGunKickSpeedDecay = asset->fHipGunKickSpeedDecay;
		target->fHipGunKickStaticDecay = asset->fHipGunKickStaticDecay;
		target->fHipViewKickPitchMin = asset->fHipViewKickPitchMin;
		target->fHipViewKickPitchMax = asset->fHipViewKickPitchMax;
		target->fHipViewKickYawMin = asset->fHipViewKickYawMin;
		target->fHipViewKickYawMax = asset->fHipViewKickYawMax;
		target->fHipViewScatterMin = asset->fHipViewScatterMin;
		target->fHipViewScatterMax = asset->fHipViewScatterMax;
		target->fightDist = asset->fightDist;
		target->maxDist = asset->maxDist;
		//float(*originalAccuracyGraphKnots[2])[2];
		//unsigned __int16 originalAccuracyGraphKnotCount[2];
		target->iPositionReloadTransTime = asset->iPositionReloadTransTime;
		target->leftArc = asset->leftArc;
		target->rightArc = asset->rightArc;
		target->topArc = asset->topArc;
		target->bottomArc = asset->bottomArc;
		target->accuracy = asset->accuracy;
		target->aiSpread = asset->aiSpread;
		target->playerSpread = asset->playerSpread;
		target->pitchConvergenceTime = asset->pitchConvergenceTime;
		target->yawConvergenceTime = asset->yawConvergenceTime;
		target->suppressTime = asset->suppressTime;
		target->maxRange = asset->maxRange;
		target->fAnimHorRotateInc = asset->fAnimHorRotateInc;
		target->fPlayerPositionDist = asset->fPlayerPositionDist;
		target->szUseHintString = asset->szUseHintString;
		target->dropHintString = asset->dropHintString;
		target->iUseHintStringIndex = asset->iUseHintStringIndex;
		target->dropHintStringIndex = asset->dropHintStringIndex;
		target->horizViewJitter = asset->horizViewJitter;
		target->vertViewJitter = asset->vertViewJitter;
		target->scanSpeed = asset->scanSpeed;
		target->scanAccel = asset->scanAccel;
		target->scanPauseTime = asset->scanPauseTime;
		target->szScript = asset->szScript;
		target->minDamage = asset->minDamage;
		target->minPlayerDamage = asset->minPlayerDamage;
		target->fMaxDamageRange = asset->fMaxDamageRange;
		target->fMinDamageRange = asset->fMinDamageRange;
		target->destabilizationRateTime = asset->destabilizationRateTime;
		target->destabilizationCurvatureMax = asset->destabilizationCurvatureMax;
		target->destabilizeDistance = asset->destabilizeDistance;
		target->locationDamageMultipliers = asset->locationDamageMultipliers;
		target->fireRumble = asset->fireRumble;
		target->meleeImpactRumble = asset->meleeImpactRumble;

		if (asset->tracerType)
		{
			target->tracerType = exporter::convert(game::native::XAssetType::ASSET_TYPE_TRACER, { asset->tracerType }).tracerDef;
		}

		target->turretScopeZoomRate = asset->turretScopeZoomRate;
		target->turretScopeZoomMin = asset->turretScopeZoomMin;
		target->turretScopeZoomMax = asset->turretScopeZoomMax;
		target->turretOverheatUpRate = asset->turretOverheatUpRate;
		target->turretOverheatDownRate = asset->turretOverheatDownRate;
		target->turretOverheatPenalty = asset->turretOverheatPenalty;
		target->turretBarrelSpinRumble = asset->turretBarrelSpinRumble;
		target->turretBarrelSpinSpeed = asset->turretBarrelSpinSpeed;
		target->turretBarrelSpinUpTime = asset->turretBarrelSpinUpTime;
		target->turretBarrelSpinDownTime = asset->turretBarrelSpinDownTime;
		target->missileConeSoundRadiusAtTop = asset->missileConeSoundRadiusAtTop;
		target->missileConeSoundRadiusAtBase = asset->missileConeSoundRadiusAtBase;
		target->missileConeSoundHeight = asset->missileConeSoundHeight;
		target->missileConeSoundOriginOffset = asset->missileConeSoundOriginOffset;
		target->missileConeSoundVolumescaleAtCore = asset->missileConeSoundVolumescaleAtCore;
		target->missileConeSoundVolumescaleAtEdge = asset->missileConeSoundVolumescaleAtEdge;
		target->missileConeSoundVolumescaleCoreSize = asset->missileConeSoundVolumescaleCoreSize;
		target->missileConeSoundPitchAtTop = asset->missileConeSoundPitchAtTop;
		target->missileConeSoundPitchAtBottom = asset->missileConeSoundPitchAtBottom;
		target->missileConeSoundPitchTopSize = asset->missileConeSoundPitchTopSize;
		target->missileConeSoundPitchBottomSize = asset->missileConeSoundPitchBottomSize;
		target->missileConeSoundCrossfadeTopSize = asset->missileConeSoundCrossfadeTopSize;
		target->missileConeSoundCrossfadeBottomSize = asset->missileConeSoundCrossfadeBottomSize;
		target->sharedAmmo = asset->sharedAmmo;
		target->lockonSupported = asset->lockonSupported;
		target->requireLockonToFire = asset->requireLockonToFire;
		target->bigExplosion = asset->bigExplosion;
		target->noAdsWhenMagEmpty = asset->noAdsWhenMagEmpty;
		target->avoidDropCleanup = asset->avoidDropCleanup;
		target->inheritsPerks = asset->inheritsPerks;
		target->crosshairColorChange = asset->crosshairColorChange;
		target->bRifleBullet = asset->bRifleBullet;
		target->armorPiercing = asset->armorPiercing;
		target->bBoltAction = asset->bBoltAction;
		target->aimDownSight = asset->aimDownSight;
		target->bRechamberWhileAds = asset->bRechamberWhileAds;
		target->bBulletExplosiveDamage = asset->bBulletExplosiveDamage;
		target->bCookOffHold = asset->bCookOffHold;
		target->bClipOnly = asset->bClipOnly;
		target->noAmmoPickup = asset->noAmmoPickup;
		target->adsFireOnly = asset->adsFireOnly;
		target->cancelAutoHolsterWhenEmpty = asset->cancelAutoHolsterWhenEmpty;
		target->disableSwitchToWhenEmpty = asset->disableSwitchToWhenEmpty;
		target->suppressAmmoReserveDisplay = asset->suppressAmmoReserveDisplay;
		target->laserSightDuringNightvision = asset->laserSightDuringNightvision;
		target->markableViewmodel = asset->markableViewmodel;
		target->noDualWield = asset->noDualWield;
		target->flipKillIcon = asset->flipKillIcon;
		target->bNoPartialReload = asset->bNoPartialReload;
		target->bSegmentedReload = asset->bSegmentedReload;
		target->blocksProne = asset->blocksProne;
		target->silenced = asset->silenced;
		target->isRollingGrenade = asset->isRollingGrenade;
		target->projExplosionEffectForceNormalUp = asset->projExplosionEffectForceNormalUp;
		target->bProjImpactExplode = asset->bProjImpactExplode;
		target->stickToPlayers = asset->stickToPlayers;
		target->hasDetonator = asset->hasDetonator;
		target->disableFiring = asset->disableFiring;
		target->timedDetonation = asset->timedDetonation;
		target->rotate = asset->rotate;
		target->holdButtonToThrow = asset->holdButtonToThrow;
		target->freezeMovementWhenFiring = asset->freezeMovementWhenFiring;
		target->thermalScope = asset->thermalScope;
		target->altModeSameWeapon = asset->altModeSameWeapon;
		target->turretBarrelSpinEnabled = asset->turretBarrelSpinEnabled;
		target->missileConeSoundEnabled = asset->missileConeSoundEnabled;
		target->missileConeSoundPitchshiftEnabled = asset->missileConeSoundPitchshiftEnabled;
		target->missileConeSoundCrossfadeEnabled = asset->missileConeSoundCrossfadeEnabled;
		target->offhandHoldIsCancelable = asset->offhandHoldIsCancelable;

		return target;
	}

	iw4::native::XModel* iweapon::append(const iw4::native::XModel* a, const iw4::native::XModel* b)
	{
		iw4::native::XModel* output = local_allocator.allocate<iw4::native::XModel>();

		std::memcpy(output, a, sizeof(iw4::native::XModel));
		output->numBones += b->numBones;
		output->numRootBones += b->numRootBones;
		output->numsurfs += b->numsurfs;

		// If this doesn't match, we're fucked lol
		assert(output->scale == b->scale);

		// Bones
		output->boneNames = local_allocator.allocate_array<uint16_t>(output->numBones);
		std::memcpy(output->boneNames, a->boneNames, a->numBones * sizeof(uint16_t));
		std::memcpy(&output->boneNames[a->numBones], b->boneNames, b->numBones * sizeof(uint16_t));

		const auto a_bones_minus_root = a->numBones - a->numRootBones;
		const auto b_bones_minus_root = b->numBones - b->numRootBones;
		const auto output_bones_minus_root = output->numBones - output->numRootBones;
		assert(output_bones_minus_root == a_bones_minus_root + b_bones_minus_root);

		output->parentList = local_allocator.allocate_array<unsigned char>(output_bones_minus_root);
		std::memcpy(output->parentList, a->parentList, a_bones_minus_root);
		std::memcpy(&output->parentList[a_bones_minus_root], b->parentList, b_bones_minus_root);

		output->quats =local_allocator.allocate_array<int16_t>(output_bones_minus_root * 4);
		std::memcpy(output->quats, a->quats, a_bones_minus_root * 4 * sizeof(int16_t));
		std::memcpy(&output->quats[a_bones_minus_root * 4], b->quats, b_bones_minus_root * 4 * sizeof(int16_t));

		output->trans = local_allocator.allocate_array<float>(output_bones_minus_root * 3);
		std::memcpy(output->trans, a->trans, a_bones_minus_root * 3 * sizeof(float));
		std::memcpy(&output->trans[a_bones_minus_root * 3], b->trans, b_bones_minus_root * 3);

		output->partClassification = local_allocator.allocate_array<unsigned char>(output->numBones);
		std::memcpy(output->partClassification, a->partClassification, a->numBones);
		std::memcpy(&output->partClassification[a->numBones], b->partClassification, b->numBones);

		output->baseMat = local_allocator.allocate_array<game::native::DObjAnimMat>(output->numBones);
		std::memcpy(output->baseMat, a->baseMat, a->numBones * sizeof(game::native::DObjAnimMat));
		std::memcpy(&output->baseMat[a->numBones], b->baseMat, b->numBones * sizeof(game::native::DObjAnimMat));
	
		output->boneInfo = local_allocator.allocate_array<game::native::XBoneInfo>(output->numBones);
		std::memcpy(output->boneInfo, a->boneInfo, a->numBones * sizeof(game::native::XBoneInfo));
		std::memcpy(&output->boneInfo[a->numBones], b->boneInfo, b->numBones * sizeof(game::native::XBoneInfo));

		//// Surfaces
		output->materialHandles = local_allocator.allocate_array<iw4::native::Material*>(output->numsurfs);
		std::memcpy(output->materialHandles, a->materialHandles, a->numsurfs * sizeof(iw4::native::Material*));
		std::memcpy(&output->materialHandles[a->numsurfs], b->materialHandles, b->numsurfs * sizeof(iw4::native::Material*));
	
		// Lods
		for (size_t i = 0; i < a->numLods; i++)
		{
			const auto a_lod = &a->lodInfo[i];
			const auto b_lod = i < b->numLods ? &b->lodInfo[i] : &b->lodInfo[b->numLods];

			std::memcpy(&output->lodInfo[i], a_lod, sizeof(iw4::native::XModelLodInfo));

			const auto output_lod = &output->lodInfo[i];
			
			// Surfaces
			output_lod->numsurfs = a_lod->numsurfs + b_lod->numsurfs;
			output_lod->surfIndex = a_lod->surfIndex;
			if (i > 0)
			{
				output_lod->surfIndex += output->lodInfo[i - 1].numsurfs - a->lodInfo[i - 1].numsurfs;
			}

			output_lod->surfs = local_allocator.allocate_array<iw4::native::XSurface>(output_lod->numsurfs);
			std::memcpy(output_lod->surfs, a_lod->surfs, a_lod->numsurfs * sizeof(iw4::native::XSurface));
			std::memcpy(&output_lod->surfs[a_lod->numsurfs], b_lod->surfs, b_lod->numsurfs * sizeof(iw4::native::XSurface));

			// ModelSurf
			output_lod->modelSurfs = local_allocator.allocate<iw4::native::XModelSurfs>();
			std::memcpy(output_lod->modelSurfs, a_lod->modelSurfs, sizeof(iw4::native::XModelSurfs));

			output_lod->modelSurfs->surfaces = output_lod->surfs;

			assert(output_lod->surfs);
		}

		output->collSurfs = local_allocator.allocate_array<game::native::XModelCollSurf_s>(output->numCollSurfs);
		for (size_t i = 0; i < output->numCollSurfs; i++)
		{
			const auto a_col = &a->collSurfs[i];
			const auto b_col = i < b->numCollSurfs ? &b->collSurfs[i] : &b->collSurfs[b->numCollSurfs-1];
			const auto output_col = &output->collSurfs[i];

			std::memcpy(output_col, a_col, sizeof(game::native::XModelCollSurf_s));

			output_col->numCollTris = a_col->numCollTris + b_col->numCollTris;
			output_col->collTris = local_allocator.allocate_array<game::native::XModelCollTri_s>(output_col->numCollTris);
			std::memcpy(output_col->collTris, a_col->collTris, a_col->numCollTris * sizeof(game::native::XModelCollTri_s));
			std::memcpy(&output_col->collTris[a_col->numCollTris], b_col->collTris, b_col->numCollTris * sizeof(game::native::XModelCollTri_s));

			printf("");

			// TODO BOUNDS
		}

		return output;
	}



#undef CONVERT_SOUND
#undef CONVERT_FX
#undef CONVERT_MATERIAL
#undef CONVERT_XMODEL

	void iweapon::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		if (!header.data)
		{
			return;
		}

		out.weapon = convert(header.weapon);
	}

	void iweapon::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_WEAPON, header.data);
		assert(result);
	}


	iweapon::iweapon()
	{
		command::add("dumpWeapon", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				if ("*"s == params[1])
				{
					std::vector<std::string> names{};

					game::native::DB_EnumXAssets(game::native::ASSET_TYPE_WEAPON, [](game::native::XAssetHeader header, void* data) {
						auto names_ptr = reinterpret_cast<std::vector<std::string>*>(data);
						names_ptr->push_back(header.weapon->szInternalName);
						}, &names, false);

					for (const auto& name : names)
					{
						const auto entry = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_WEAPON, name.data(), 0);
						iw4::native::XAssetHeader output{};
						convert(entry, output);
						write(output);

					}

					return;
				}

				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_WEAPON, params[1]);

				if (entry)
				{
					convert_and_write(entry->asset.header);
					exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_WEAPON, entry->asset.header.weapon->szInternalName);
				}
				else
				{
					console::warn("i cannot find %s! :(\n ", params.get(1));
				}
			});
	}


	//	void IWeapon::DifferentiateWeapon(Game::IW4::WeaponCompleteDef* weapon)
	//	{
	//		for (size_t i = 0; i < 16; i++)
	//		{
	//			if (weapon->weapDef->gunXModel[i])
	//			{
	//				DifferentiateModel(weapon->weapDef->gunXModel[i]);
	//			}
	//
	//			if (weapon->weapDef->worldModel[i])
	//			{
	//				DifferentiateModel(weapon->weapDef->worldModel[i]);
	//			}
	//		}
	//	}
	//
	//	void IWeapon::DifferentiateModel(Game::IW4::XModel* model)
	//	{
	//		const auto surfCount = model->numsurfs;
	//		for (size_t surfIndex = 0; surfIndex < surfCount; surfIndex++)
	//		{
	//			const auto material = model->materialHandles[surfIndex];
	//			std::string name = material->name;
	//
	//			if (name.ends_with(IGfxImage::imageSuffix))
	//			{
	//				continue;
	//			}
	//
	//			material->name = LocalAllocator.DuplicateString(std::format("{}{}", material->name, IGfxImage::imageSuffix));
	//
	//			for (char textureIndex = 0; textureIndex < material->textureCount; textureIndex++)
	//			{
	//				const auto texture = &material->textureTable[textureIndex];
	//				const std::string textureName = texture->u.image->name;
	//
	//				if (textureName.ends_with(IGfxImage::imageSuffix))
	//				{
	//					continue;
	//				}
	//
	//				const auto newName = LocalAllocator.DuplicateString(std::format("{}{}", textureName, IGfxImage::imageSuffix));
	//
	//				IGfxImage::AddRename(textureName, newName);
	//
	//				texture->u.image->name = newName;
	//			}
	//		}
	//	}
}
