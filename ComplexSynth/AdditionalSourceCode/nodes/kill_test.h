#pragma once

// These will improve the readability of the connection definition

#define getT(Idx) template get<Idx>()
#define connectT(Idx, target) template connect<Idx>(target)
#define getParameterT(Idx) template getParameter<Idx>()
#define setParameterT(Idx, value) template setParameter<Idx>(value)
#define setParameterWT(Idx, value) template setWrapParameter<Idx>(value)
using namespace scriptnode;
using namespace snex;
using namespace snex::Types;

namespace kill_test_impl
{
// ======================| Node & Parameter type declarations |======================

template <int NV>
using oscillator_t = wrap::no_data<core::oscillator<NV>>;

template <int NV>
using kill_test_t_ = container::chain<parameter::empty, 
                                      wrap::fix<2, oscillator_t<NV>>, 
                                      envelope::voice_manager>;

// =========================| Root node initialiser class |=========================

template <int NV> struct instance: public kill_test_impl::kill_test_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(kill_test);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(2)
		{
			0x0000, 0x0000
		};
		SNEX_METADATA_ENCODED_MOD_INFO(17)
		{
			0x003A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000
		};
	};
	
	instance()
	{
		// Node References ---------------------------------------------------------
		
		auto& oscillator = this->getT(0);    // kill_test_impl::oscillator_t<NV>
		auto& voice_manager = this->getT(1); // envelope::voice_manager
		
		// Default Values ----------------------------------------------------------
		
		oscillator.setParameterT(0, 1.);   // core::oscillator::Mode
		oscillator.setParameterT(1, 220.); // core::oscillator::Frequency
		oscillator.setParameterT(2, 1.);   // core::oscillator::FreqRatio
		oscillator.setParameterT(3, 1.);   // core::oscillator::Gate
		oscillator.setParameterT(4, 0.);   // core::oscillator::Phase
		oscillator.setParameterT(5, 1.);   // core::oscillator::Gain
		
		voice_manager.setParameterT(0, 0.); // envelope::voice_manager::KillVoice
		
		this->setExternalData({}, -1);
	}
	~instance() override
	{
		// Cleanup external data references ----------------------------------------
		
		this->setExternalData({}, -1);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool isProcessingHiseEvent() { return true; };
	
	static constexpr bool hasTail() { return true; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections -----------------------------------------------
		
		this->getT(0).setExternalData(b, index); // kill_test_impl::oscillator_t<NV>
	}
};
}

#undef getT
#undef connectT
#undef setParameterT
#undef setParameterWT
#undef getParameterT
// ==============================| Public Definition |==============================

namespace project
{
// polyphonic template declaration

template <int NV>
using kill_test = wrap::node<kill_test_impl::instance<NV>>;
}


