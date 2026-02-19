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

namespace resonator_impl
{
// ==============================| Node & Parameter type declarations |==============================

DECLARE_PARAMETER_RANGE_SKEW(dry_wet_mixer_c0Range, 
                             -100., 
                             0., 
                             5.42227);

template <int NV>
using dry_wet_mixer_c0 = parameter::from0To1<core::gain<NV>, 
                                             0, 
                                             dry_wet_mixer_c0Range>;

template <int NV> using dry_wet_mixer_c1 = dry_wet_mixer_c0<NV>;

template <int NV>
using dry_wet_mixer_multimod = parameter::list<dry_wet_mixer_c0<NV>, dry_wet_mixer_c1<NV>>;

template <int NV>
using dry_wet_mixer_t = control::xfader<dry_wet_mixer_multimod<NV>, 
                                        faders::cosine_half>;

template <int NV>
using dry_path_t = container::chain<parameter::empty, 
                                    wrap::fix<2, dry_wet_mixer_t<NV>>, 
                                    core::gain<NV>>;

template <int NV>
using smoothed_parameter_unscaled_t = wrap::mod<parameter::plain<jdsp::jdelay_cubic<NV>, 1>, 
                                                control::smoothed_parameter_unscaled<NV, smoothers::linear_ramp<NV>>>;
template <int NV>
using converter4_t = control::converter<parameter::plain<smoothed_parameter_unscaled_t<NV>, 0>, 
                                        conversion_logic::freq2ms>;
template <int NV>
using converter2_t = control::converter<parameter::plain<converter4_t<NV>, 0>, 
                                        conversion_logic::midi2freq>;
template <int NV>
using midi1_t = wrap::mod<parameter::plain<converter2_t<NV>, 0>, 
                          control::midi<midi_logic::notenumber<NV>>>;

template <int NV>
using midichain1_t_ = container::chain<parameter::empty, 
                                       wrap::fix<2, midi1_t<NV>>, 
                                       converter2_t<NV>, 
                                       converter4_t<NV>>;

template <int NV>
using midichain1_t = wrap::event<midichain1_t_<NV>>;
template <int NV>
using stereo_frame_cable = cable::frame<NV, 2>;

template <int NV>
using frame2_block1_t_ = container::chain<parameter::empty, 
                                          wrap::fix<2, routing::receive<NV, stereo_frame_cable<NV>>>, 
                                          smoothed_parameter_unscaled_t<NV>, 
                                          filters::one_pole<NV>, 
                                          fx::phase_delay<NV>, 
                                          jdsp::jdelay_cubic<NV>, 
                                          routing::send<NV, stereo_frame_cable<NV>>>;

template <int NV>
using frame2_block1_t = wrap::frame<2, frame2_block1_t_<NV>>;

template <int NV>
using soft_bypass_t_ = container::chain<parameter::empty, 
                                        wrap::fix<2, math::tanh<NV>>>;

template <int NV>
using soft_bypass_t = bypass::smoothed<20, soft_bypass_t_<NV>>;
using jcompressor_t = wrap::no_data<jdsp::jcompressor>;

template <int NV>
using wet_path_t = container::chain<parameter::empty, 
                                    wrap::fix<2, midichain1_t<NV>>, 
                                    frame2_block1_t<NV>, 
                                    soft_bypass_t<NV>, 
                                    jcompressor_t, 
                                    core::gain<NV>>;

namespace dry_wet1_t_parameters
{
}

template <int NV>
using dry_wet1_t = container::split<parameter::plain<resonator_impl::dry_wet_mixer_t<NV>, 0>, 
                                    wrap::fix<2, dry_path_t<NV>>, 
                                    wet_path_t<NV>>;

namespace resonator_t_parameters
{
// Parameter list for resonator_impl::resonator_t --------------------------------------------------

DECLARE_PARAMETER_RANGE_SKEW(DampingRange, 
                             20., 
                             20000., 
                             0.229905);

template <int NV>
using Damping = parameter::from0To1<filters::one_pole<NV>, 
                                    0, 
                                    DampingRange>;

DECLARE_PARAMETER_RANGE(DecayRange, 
                        0.8, 
                        1.);

template <int NV>
using Decay = parameter::from0To1<routing::receive<NV, stereo_frame_cable<NV>>, 
                                  0, 
                                  DecayRange>;

DECLARE_PARAMETER_RANGE_SKEW(TonalityRange, 
                             20., 
                             20000., 
                             0.229905);

template <int NV>
using Tonality = parameter::from0To1<fx::phase_delay<NV>, 
                                     0, 
                                     TonalityRange>;

template <int NV>
using Fuzz_0 = parameter::bypass<resonator_impl::soft_bypass_t<NV>>;

template <int NV>
using Fuzz = parameter::chain<ranges::Identity, 
                              Fuzz_0<NV>, 
                              Fuzz_0<NV>>;

template <int NV>
using Mix = parameter::plain<resonator_impl::dry_wet1_t<NV>, 
                             0>;
template <int NV>
using Glide = parameter::plain<resonator_impl::smoothed_parameter_unscaled_t<NV>, 
                               1>;
template <int NV>
using resonator_t_plist = parameter::list<Mix<NV>, 
                                          Damping<NV>, 
                                          Glide<NV>, 
                                          Decay<NV>, 
                                          Tonality<NV>, 
                                          Fuzz<NV>>;
}

template <int NV>
using resonator_t_ = container::chain<resonator_t_parameters::resonator_t_plist<NV>, 
                                      wrap::fix<2, dry_wet1_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public resonator_impl::resonator_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(resonator);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(98)
		{
			0x055C, 0x0000, 0x0000, 0x694D, 0x0078, 0x0000, 0x0000, 0x0000, 
            0x3F80, 0x0625, 0x3E81, 0x0000, 0x3F80, 0x0000, 0x0000, 0x055C, 
            0x0001, 0x0000, 0x6144, 0x706D, 0x6E69, 0x0067, 0x0000, 0x0000, 
            0x0000, 0x3F80, 0x4BC7, 0x3F37, 0x0000, 0x3F80, 0x0000, 0x0000, 
            0x025C, 0x0002, 0x0000, 0x6C47, 0x6469, 0x0065, 0xCCCD, 0x3DCC, 
            0x0000, 0x447A, 0x6667, 0x4270, 0x0000, 0x3F80, 0xCCCD, 0x3DCC, 
            0x055C, 0x0003, 0x0000, 0x6544, 0x6163, 0x0079, 0x0000, 0x0000, 
            0x0000, 0x3F80, 0x7CEE, 0x3F3F, 0x0000, 0x3F80, 0x0000, 0x0000, 
            0x055C, 0x0004, 0x0000, 0x6F54, 0x616E, 0x696C, 0x7974, 0x0000, 
            0x0000, 0x0000, 0x8000, 0x003F, 0x8000, 0x003F, 0x8000, 0x003F, 
            0x0000, 0x5C00, 0x0500, 0x0000, 0x4600, 0x7A75, 0x007A, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x3F80, 0x0000
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
		
		auto& dry_wet1 = this->getT(0);                                            // resonator_impl::dry_wet1_t<NV>
		auto& dry_path = this->getT(0).getT(0);                                    // resonator_impl::dry_path_t<NV>
		auto& dry_wet_mixer = this->getT(0).getT(0).getT(0);                       // resonator_impl::dry_wet_mixer_t<NV>
		auto& dry_gain = this->getT(0).getT(0).getT(1);                            // core::gain<NV>
		auto& wet_path = this->getT(0).getT(1);                                    // resonator_impl::wet_path_t<NV>
		auto& midichain1 = this->getT(0).getT(1).getT(0);                          // resonator_impl::midichain1_t<NV>
		auto& midi1 = this->getT(0).getT(1).getT(0).getT(0);                       // resonator_impl::midi1_t<NV>
		auto& converter2 = this->getT(0).getT(1).getT(0).getT(1);                  // resonator_impl::converter2_t<NV>
		auto& converter4 = this->getT(0).getT(1).getT(0).getT(2);                  // resonator_impl::converter4_t<NV>
		auto& frame2_block1 = this->getT(0).getT(1).getT(1);                       // resonator_impl::frame2_block1_t<NV>
		auto& receive1 = this->getT(0).getT(1).getT(1).getT(0);                    // routing::receive<NV, stereo_frame_cable<NV>>
		auto& smoothed_parameter_unscaled = this->getT(0).getT(1).getT(1).getT(1); // resonator_impl::smoothed_parameter_unscaled_t<NV>
		auto& one_pole1 = this->getT(0).getT(1).getT(1).getT(2);                   // filters::one_pole<NV>
		auto& phase_delay = this->getT(0).getT(1).getT(1).getT(3);                 // fx::phase_delay<NV>
		auto& jdelay_cubic = this->getT(0).getT(1).getT(1).getT(4);                // jdsp::jdelay_cubic<NV>
		auto& send1 = this->getT(0).getT(1).getT(1).getT(5);                       // routing::send<NV, stereo_frame_cable<NV>>
		auto& soft_bypass = this->getT(0).getT(1).getT(2);                         // resonator_impl::soft_bypass_t<NV>
		auto& tanh = this->getT(0).getT(1).getT(2).getT(0);                        // math::tanh<NV>
		auto& jcompressor = this->getT(0).getT(1).getT(3);                         // resonator_impl::jcompressor_t
		auto& wet_gain = this->getT(0).getT(1).getT(4);                            // core::gain<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		dry_wet1.getParameterT(0).connectT(0, dry_wet_mixer); // DryWet -> dry_wet_mixer::Value
		this->getParameterT(0).connectT(0, dry_wet1);         // Mix -> dry_wet1::DryWet
		
		this->getParameterT(1).connectT(0, one_pole1); // Damping -> one_pole1::Frequency
		
		this->getParameterT(2).connectT(0, smoothed_parameter_unscaled); // Glide -> smoothed_parameter_unscaled::SmoothingTime
		
		this->getParameterT(3).connectT(0, receive1); // Decay -> receive1::Feedback
		
		this->getParameterT(4).connectT(0, phase_delay); // Tonality -> phase_delay::Frequency
		
		auto& Fuzz_p = this->getParameterT(5);
		Fuzz_p.connectT(0, soft_bypass); // Fuzz -> soft_bypass::Bypassed
		Fuzz_p.connectT(1, soft_bypass); // Fuzz -> soft_bypass::Bypassed
		
		// Modulation Connections ------------------------------------------------------------------
		
		auto& dry_wet_mixer_p = dry_wet_mixer.getWrappedObject().getParameter();
		dry_wet_mixer_p.getParameterT(0).connectT(0, dry_gain);                                // dry_wet_mixer -> dry_gain::Gain
		dry_wet_mixer_p.getParameterT(1).connectT(0, wet_gain);                                // dry_wet_mixer -> wet_gain::Gain
		smoothed_parameter_unscaled.getParameter().connectT(0, jdelay_cubic);                  // smoothed_parameter_unscaled -> jdelay_cubic::DelayTime
		converter4.getWrappedObject().getParameter().connectT(0, smoothed_parameter_unscaled); // converter4 -> smoothed_parameter_unscaled::Value
		converter2.getWrappedObject().getParameter().connectT(0, converter4);                  // converter2 -> converter4::Value
		midi1.getParameter().connectT(0, converter2);                                          // midi1 -> converter2::Value
		
		// Send Connections ------------------------------------------------------------------------
		
		send1.connect(receive1);
		
		// Default Values --------------------------------------------------------------------------
		
		; // dry_wet1::DryWet is automated
		
		; // dry_wet_mixer::Value is automated
		
		;                               // dry_gain::Gain is automated
		dry_gain.setParameterT(1, 20.); // core::gain::Smoothing
		dry_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		; // converter2::Value is automated
		
		; // converter4::Value is automated
		
		; // receive1::Feedback is automated
		
		;                                                 // smoothed_parameter_unscaled::Value is automated
		;                                                 // smoothed_parameter_unscaled::SmoothingTime is automated
		smoothed_parameter_unscaled.setParameterT(2, 1.); // control::smoothed_parameter_unscaled::Enabled
		
		;                               // one_pole1::Frequency is automated
		one_pole1.setParameterT(1, 1.); // filters::one_pole::Q
		one_pole1.setParameterT(2, 0.); // filters::one_pole::Gain
		one_pole1.setParameterT(3, 0.); // filters::one_pole::Smoothing
		one_pole1.setParameterT(4, 0.); // filters::one_pole::Mode
		one_pole1.setParameterT(5, 1.); // filters::one_pole::Enabled
		
		; // phase_delay::Frequency is automated
		
		jdelay_cubic.setParameterT(0, 1000.); // jdsp::jdelay_cubic::Limit
		;                                     // jdelay_cubic::DelayTime is automated
		
		tanh.setParameterT(0, 1.); // math::tanh::Value
		
		jcompressor.setParameterT(0, -5.12069); // jdsp::jcompressor::Treshold
		jcompressor.setParameterT(1, 32.);      // jdsp::jcompressor::Ratio
		jcompressor.setParameterT(2, 12.8953);  // jdsp::jcompressor::Attack
		jcompressor.setParameterT(3, 102.479);  // jdsp::jcompressor::Release
		
		;                               // wet_gain::Gain is automated
		wet_gain.setParameterT(1, 20.); // core::gain::Smoothing
		wet_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		this->setParameterT(0, 0.252);
		this->setParameterT(1, 0.716);
		this->setParameterT(2, 60.1);
		this->setParameterT(3, 0.748);
		this->setParameterT(4, 1.);
		this->setParameterT(5, 1.);
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
		
		this->getT(0).getT(1).getT(3).setExternalData(b, index); // resonator_impl::jcompressor_t
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
using resonator = wrap::node<resonator_impl::instance<NV>>;
}


