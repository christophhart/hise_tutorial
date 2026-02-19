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

namespace unisono_test_impl
{
// ==============================| Node & Parameter type declarations |==============================

template <int NV>
using oscillator_t = wrap::no_data<core::oscillator<NV>>;
DECLARE_PARAMETER_RANGE_SKEW(clone_cable_modRange, 
                             0.909091, 
                             1.1, 
                             0.93424);

template <int NV>
using clone_cable_mod = parameter::from0To1<oscillator_t<NV>, 
                                            2, 
                                            clone_cable_modRange>;

template <int NV>
using clone_cable_t = control::clone_cable<parameter::cloned<clone_cable_mod<NV>>, 
                                           duplilogic::spread>;

template <int NV>
using clone_cable1_t = control::clone_cable<parameter::cloned<parameter::plain<oscillator_t<NV>, 4>>, 
                                            duplilogic::random>;

DECLARE_PARAMETER_RANGE(clone_cable2_modRange, 
                        -1., 
                        1.);

template <int NV>
using clone_cable2_mod = parameter::from0To1<jdsp::jpanner<NV>, 
                                             0, 
                                             clone_cable2_modRange>;

template <int NV>
using clone_cable2_t = control::clone_cable<parameter::cloned<clone_cable2_mod<NV>>, 
                                            duplilogic::spread>;

template <int NV>
using clone_cable3_t = control::clone_cable<parameter::cloned<parameter::plain<oscillator_t<NV>, 5>>, 
                                            duplilogic::ducker>;

template <int NV>
using offline_t_ = container::chain<parameter::empty, 
                                    wrap::fix<2, clone_cable_t<NV>>, 
                                    clone_cable1_t<NV>, 
                                    clone_cable2_t<NV>, 
                                    clone_cable3_t<NV>>;

template <int NV>
using offline_t = wrap::offline<offline_t_<NV>>;

template <int NV>
using clone_child_t = container::chain<parameter::empty, 
                                       wrap::fix<2, oscillator_t<NV>>, 
                                       jdsp::jpanner<NV>>;
template <int NV>
using clone_t = wrap::clonecopy<clone_child_t<NV>, 16>;

namespace unisono_test_t_parameters
{
// Parameter list for unisono_test_impl::unisono_test_t --------------------------------------------

template <int NV>
using NumVoices = parameter::chain<ranges::Identity, 
                                   parameter::plain<unisono_test_impl::clone_t<NV>, 0>, 
                                   parameter::plain<unisono_test_impl::clone_cable_t<NV>, 0>, 
                                   parameter::plain<unisono_test_impl::clone_cable1_t<NV>, 0>, 
                                   parameter::plain<unisono_test_impl::clone_cable2_t<NV>, 0>, 
                                   parameter::plain<unisono_test_impl::clone_cable3_t<NV>, 0>>;

template <int NV>
using Detune = parameter::plain<unisono_test_impl::clone_cable_t<NV>, 
                                1>;
template <int NV>
using Spread = parameter::plain<unisono_test_impl::clone_cable2_t<NV>, 
                                1>;
template <int NV>
using unisono_test_t_plist = parameter::list<NumVoices<NV>, 
                                             Detune<NV>, 
                                             Spread<NV>>;
}

template <int NV>
using unisono_test_t_ = container::chain<unisono_test_t_parameters::unisono_test_t_plist<NV>, 
                                         wrap::fix<2, offline_t<NV>>, 
                                         clone_t<NV>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public unisono_test_impl::unisono_test_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(unisono_test);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(52)
		{
			0x005C, 0x0000, 0x0000, 0x754E, 0x566D, 0x696F, 0x6563, 0x0073, 
            0x0000, 0x3F80, 0x0000, 0x4180, 0x0000, 0x4100, 0x0000, 0x3F80, 
            0x0000, 0x3F80, 0x005C, 0x0001, 0x0000, 0x6544, 0x7574, 0x656E, 
            0x0000, 0x0000, 0x0000, 0x8000, 0x8B3F, 0x9B54, 0x003D, 0x8000, 
            0x003F, 0x0000, 0x5C00, 0x0200, 0x0000, 0x5300, 0x7270, 0x6165, 
            0x0064, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x3F80, 0x0000, 0x0000, 0x0000
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
		// Node References -------------------------------------------------------------------------
		
		auto& offline = this->getT(0);              // unisono_test_impl::offline_t<NV>
		auto& clone_cable = this->getT(0).getT(0);  // unisono_test_impl::clone_cable_t<NV>
		auto& clone_cable1 = this->getT(0).getT(1); // unisono_test_impl::clone_cable1_t<NV>
		auto& clone_cable2 = this->getT(0).getT(2); // unisono_test_impl::clone_cable2_t<NV>
		auto& clone_cable3 = this->getT(0).getT(3); // unisono_test_impl::clone_cable3_t<NV>
		auto& clone = this->getT(1);                // unisono_test_impl::clone_t<NV>                // unisono_test_impl::clone_child_t<NV>
		auto oscillator = this->getT(1).getT(0);    // unisono_test_impl::oscillator_t<NV>
		auto jpanner = this->getT(1).getT(1);       // jdsp::jpanner<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		auto& NumVoices_p = this->getParameterT(0);
		NumVoices_p.connectT(0, clone);        // NumVoices -> clone::NumClones
		NumVoices_p.connectT(1, clone_cable);  // NumVoices -> clone_cable::NumClones
		NumVoices_p.connectT(2, clone_cable1); // NumVoices -> clone_cable1::NumClones
		NumVoices_p.connectT(3, clone_cable2); // NumVoices -> clone_cable2::NumClones
		NumVoices_p.connectT(4, clone_cable3); // NumVoices -> clone_cable3::NumClones
		
		this->getParameterT(1).connectT(0, clone_cable); // Detune -> clone_cable::Value
		
		this->getParameterT(2).connectT(0, clone_cable2); // Spread -> clone_cable2::Value
		
		// Modulation Connections ------------------------------------------------------------------
		
		clone_cable.getWrappedObject().getParameter().connectT(0, oscillator);  // clone_cable -> oscillator::FreqRatio
		clone_cable1.getWrappedObject().getParameter().connectT(0, oscillator); // clone_cable1 -> oscillator::Phase
		clone_cable2.getWrappedObject().getParameter().connectT(0, jpanner);    // clone_cable2 -> jpanner::Pan
		clone_cable3.getWrappedObject().getParameter().connectT(0, oscillator); // clone_cable3 -> oscillator::Gain
		
		// Default Values --------------------------------------------------------------------------
		
		;                                 // clone_cable::NumClones is automated
		;                                 // clone_cable::Value is automated
		clone_cable.setParameterT(2, 1.); // control::clone_cable::Gamma
		
		;                                   // clone_cable1::NumClones is automated
		clone_cable1.setParameterT(1, 0.5); // control::clone_cable::Value
		clone_cable1.setParameterT(2, 0.);  // control::clone_cable::Gamma
		
		;                                  // clone_cable2::NumClones is automated
		;                                  // clone_cable2::Value is automated
		clone_cable2.setParameterT(2, 0.); // control::clone_cable::Gamma
		
		;                                        // clone_cable3::NumClones is automated
		clone_cable3.setParameterT(1, 0.);       // control::clone_cable::Value
		clone_cable3.setParameterT(2, 0.792883); // control::clone_cable::Gamma
		
		;                           // clone::NumClones is automated
		clone.setParameterT(1, 2.); // container::clone::SplitSignal
		
		oscillator.setParameterT(0, 1.);   // core::oscillator::Mode
		oscillator.setParameterT(1, 220.); // core::oscillator::Frequency
		;                                  // oscillator::FreqRatio is automated
		oscillator.setParameterT(3, 1.);   // core::oscillator::Gate
		;                                  // oscillator::Phase is automated
		;                                  // oscillator::Gain is automated
		
		;                             // jpanner::Pan is automated
		jpanner.setParameterT(1, 1.); // jdsp::jpanner::Rule
		
		this->setParameterT(0, 8.);
		this->setParameterT(1, 0.0758448);
		this->setParameterT(2, 1.);
		this->setExternalData({}, -1);
	}
	~instance() override
	{
		// Cleanup external data references --------------------------------------------------------
		
		this->setExternalData({}, -1);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool isProcessingHiseEvent() { return true; };
	
	static constexpr bool hasTail() { return true; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(1).getT(0).setExternalData(b, index); // unisono_test_impl::oscillator_t<NV>
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
using unisono_test = wrap::node<unisono_test_impl::instance<NV>>;
}


