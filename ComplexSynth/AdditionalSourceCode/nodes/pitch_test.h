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

namespace pitch_test_impl
{
// ==============================| Node & Parameter type declarations |==============================

using pitch_mod_t_index = runtime_target::indexers::fix_hash<90001>;

template <int NV>
using oscillator_t = wrap::no_data<core::oscillator<NV>>;
template <int NV>
using pitch_mod_t = wrap::mod<parameter::plain<oscillator_t<NV>, 2>, 
                              wrap::no_data<core::pitch_mod<NV, pitch_mod_t_index>>>;
using extra_mod_t_index = runtime_target::indexers::fix_hash<5000>;
using extra_mod_t_config = modulation::config::constant<false, 
                                                        modulation::TargetMode::Raw>;

DECLARE_PARAMETER_RANGE_SKEW(extra_mod_modRange, 
                             20., 
                             20000., 
                             0.229905);

template <int NV>
using extra_mod_mod = parameter::from0To1<filters::svf<NV>, 
                                          0, 
                                          extra_mod_modRange>;

template <int NV>
using extra_mod_t = wrap::mod<extra_mod_mod<NV>, 
                              wrap::no_data<core::extra_mod<NV, extra_mod_t_index, extra_mod_t_config>>>;

using simple_ar_multimod = parameter::list<parameter::empty, parameter::empty>;

template <int NV>
using simple_ar_t = wrap::no_data<envelope::simple_ar<NV, simple_ar_multimod>>;

template <int NV>
using fix8_block_t_ = container::chain<parameter::empty, 
                                       wrap::fix<2, pitch_mod_t<NV>>, 
                                       oscillator_t<NV>, 
                                       extra_mod_t<NV>, 
                                       filters::svf<NV>, 
                                       simple_ar_t<NV>>;

template <int NV>
using fix8_block_t = wrap::fix_block<8, fix8_block_t_<NV>>;

template <int NV>
using midichain_t_ = container::chain<parameter::empty, 
                                      wrap::fix<2, fix8_block_t<NV>>>;

template <int NV>
using midichain_t = wrap::event<midichain_t_<NV>>;

template <int NV>
using pitch_test_t_ = container::chain<parameter::empty, 
                                       wrap::fix<2, math::clear<NV>>, 
                                       midichain_t<NV>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public pitch_test_impl::pitch_test_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(pitch_test);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(16)
		{
			0x005C, 0x0000, 0x0000, 0x7246, 0x7165, 0x0000, 0x0000, 0x0000, 
            0x8000, 0x003F, 0x8000, 0x003F, 0x8000, 0x003F, 0x0000, 0x0000
		};
		SNEX_METADATA_ENCODED_MOD_INFO(17)
		{
			0x003A, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000
		};
	};
	
	instance()
	{
		// Node References -------------------------------------------------------------------------
		
		auto& clear = this->getT(0);                      // math::clear<NV>
		auto& midichain = this->getT(1);                  // pitch_test_impl::midichain_t<NV>
		auto& fix8_block = this->getT(1).getT(0);         // pitch_test_impl::fix8_block_t<NV>
		auto& pitch_mod = this->getT(1).getT(0).getT(0);  // pitch_test_impl::pitch_mod_t<NV>
		auto& oscillator = this->getT(1).getT(0).getT(1); // pitch_test_impl::oscillator_t<NV>
		auto& extra_mod = this->getT(1).getT(0).getT(2);  // pitch_test_impl::extra_mod_t<NV>
		auto& svf = this->getT(1).getT(0).getT(3);        // filters::svf<NV>
		auto& simple_ar = this->getT(1).getT(0).getT(4);  // pitch_test_impl::simple_ar_t<NV>
		
		// Modulation Connections ------------------------------------------------------------------
		
		pitch_mod.getParameter().connectT(0, oscillator); // pitch_mod -> oscillator::FreqRatio
		extra_mod.getParameter().connectT(0, svf);        // extra_mod -> svf::Frequency
		auto& simple_ar_p = simple_ar.getWrappedObject().getParameter();
		
		// Default Values --------------------------------------------------------------------------
		
		clear.setParameterT(0, 0.); // math::clear::Value
		
		pitch_mod.setParameterT(0, 0.); // core::pitch_mod::ProcessSignal
		
		oscillator.setParameterT(0, 1.);   // core::oscillator::Mode
		oscillator.setParameterT(1, 220.); // core::oscillator::Frequency
		;                                  // oscillator::FreqRatio is automated
		oscillator.setParameterT(3, 1.);   // core::oscillator::Gate
		oscillator.setParameterT(4, 0.);   // core::oscillator::Phase
		oscillator.setParameterT(5, 1.);   // core::oscillator::Gain
		
		extra_mod.setParameterT(0, 0.); // core::extra_mod::Index
		extra_mod.setParameterT(1, 0.); // core::extra_mod::ProcessSignal
		
		;                           // svf::Frequency is automated
		svf.setParameterT(1, 1.);   // filters::svf::Q
		svf.setParameterT(2, 0.);   // filters::svf::Gain
		svf.setParameterT(3, 0.01); // filters::svf::Smoothing
		svf.setParameterT(4, 0.);   // filters::svf::Mode
		svf.setParameterT(5, 1.);   // filters::svf::Enabled
		
		simple_ar.setParameterT(0, 10.); // envelope::simple_ar::Attack
		simple_ar.setParameterT(1, 10.); // envelope::simple_ar::Release
		simple_ar.setParameterT(2, 0.);  // envelope::simple_ar::Gate
		simple_ar.setParameterT(3, 0.);  // envelope::simple_ar::AttackCurve
		
		this->setParameterT(0, 1.);
		this->setExternalData({}, -1);
	}
	~instance() override
	{
		// Cleanup external data references --------------------------------------------------------
		
		this->setExternalData({}, -1);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool isProcessingHiseEvent() { return true; };
	
	static constexpr bool hasTail() { return false; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
	
	void connectToRuntimeTarget(bool addConnection, const runtime_target::connection& c)
	{
		// Runtime target Connections --------------------------------------------------------------
		
		this->getT(1).getT(0).getT(0).connectToRuntimeTarget(addConnection, c); // pitch_test_impl::pitch_mod_t<NV>
		this->getT(1).getT(0).getT(2).connectToRuntimeTarget(addConnection, c); // pitch_test_impl::extra_mod_t<NV>
	}
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(1).getT(0).getT(0).setExternalData(b, index); // pitch_test_impl::pitch_mod_t<NV>
		this->getT(1).getT(0).getT(1).setExternalData(b, index); // pitch_test_impl::oscillator_t<NV>
		this->getT(1).getT(0).getT(2).setExternalData(b, index); // pitch_test_impl::extra_mod_t<NV>
		this->getT(1).getT(0).getT(4).setExternalData(b, index); // pitch_test_impl::simple_ar_t<NV>
	}
};
}

#undef getT
#undef connectT
#undef setParameterT
#undef setParameterWT
#undef getParameterT
// ======================================| Public Definition |======================================

namespace project
{
// polyphonic template declaration

template <int NV>
using pitch_test = wrap::node<pitch_test_impl::instance<NV>>;
}


