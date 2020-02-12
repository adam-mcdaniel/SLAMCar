// PX2EffectableController.hpp

#ifndef PX2PARTICLEEMITTERCONTROLLER_HPP
#define PX2PARTICLEEMITTERCONTROLLER_HPP

#include "PX2EffectPre.hpp"
#include "PX2ParticleEmitterController.hpp"
#include "PX2ParticleObject.hpp"
#include "PX2TRecyclingArray.hpp"
#include "PX2EffectableController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ParticleEmitterController : public EffectableController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ParticleEmitterController);

	public:
		ParticleEmitterController ();
		virtual ~ParticleEmitterController ();

		virtual void Reset ();

		void SetMaxNumParticles (int num);
		int GetMaxNumParticles () const;
		TRecyclingArray<ParticleObject>* &GetArray ();

		// ��
		virtual bool Update (double applicationTime, double elapsedTime);

	protected:
		void NewAParticle (float ctrlTime);
		virtual void OnNewAEffectObject (EffectObject *obj);

		TRecyclingArray<ParticleObject> *mParticleArray;
		int mMaxNumParticles; // ��Ҫ�־û�

		int mCreatedParticlesQuantity; //< �ܹ���������������(����+����ȥ)
		float mNumNewParticlesExcess;
	};

	PX2_REGISTER_STREAM(ParticleEmitterController);
	typedef Pointer0<ParticleEmitterController> ParticleEmitterControllerPtr;
#include "PX2ParticleEmitterController.inl"

}

#endif