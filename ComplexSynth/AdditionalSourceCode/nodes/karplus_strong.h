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

namespace karplus_strong_impl
{
// ==============================| Node & Parameter type declarations |==============================

template <int NV>
using oscillator_t = wrap::no_data<core::oscillator<NV>>;

using ahdsr_multimod = parameter::list<parameter::empty, parameter::empty>;

template <int NV>
using ahdsr_t = wrap::no_data<envelope::ahdsr<NV, ahdsr_multimod>>;

template <int NV>
using converter1_t = control::converter<parameter::plain<jdsp::jdelay_cubic<NV>, 1>, 
                                        conversion_logic::freq2ms>;
template <int NV>
using converter_t = control::converter<parameter::plain<converter1_t<NV>, 0>, 
                                       conversion_logic::midi2freq>;
template <int NV>
using midi_t = wrap::mod<parameter::plain<converter_t<NV>, 0>, 
                         control::midi<midi_logic::notenumber<NV>>>;

template <int NV>
using stereo_frame_cable = cable::frame<NV, 2>;
DECLARE_PARAMETER_RANGE(xfader_c0Range, 
                        0.587687, 
                        1.);

template <int NV>
using xfader_c0 = parameter::from0To1<routing::receive<NV, stereo_frame_cable<NV>>, 
                                      0, 
                                      xfader_c0Range>;

DECLARE_PARAMETER_RANGE(xfader_c1Range, 
                        1.32979, 
                        2.9);

template <int NV>
using xfader_c1 = parameter::from0To1<math::tanh<NV>, 
                                      0, 
                                      xfader_c1Range>;

template <int NV>
using xfader_multimod = parameter::list<xfader_c0<NV>, xfader_c1<NV>>;

template <int NV>
using xfader_t = control::xfader<xfader_multimod<NV>, faders::linear>;
using extra_mod_t_index = runtime_target::indexers::fix_hash<5000>;
using extra_mod_t_config = modulation::config::constant<false, 
                                                        modulation::TargetMode::Raw>;

DECLARE_PARAMETER_RANGE_SKEW(extra_mod_modRange, 
                             1000., 
                             20000., 
                             0.229905);

template <int NV>
using extra_mod_mod = parameter::from0To1<filters::one_pole<NV>, 
                                          0, 
                                          extra_mod_modRange>;

template <int NV>
using extra_mod_t = wrap::mod<extra_mod_mod<NV>, 
                              wrap::no_data<core::extra_mod<NV, extra_mod_t_index, extra_mod_t_config>>>;

template <int NV>
using frame2_block_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, extra_mod_t<NV>>, 
                                         routing::receive<NV, stereo_frame_cable<NV>>, 
                                         math::tanh<NV>, 
                                         filters::one_pole<NV>, 
                                         jdsp::jdelay_cubic<NV>, 
                                         routing::send<NV, stereo_frame_cable<NV>>>;

template <int NV>
using frame2_block_t = wrap::frame<2, frame2_block_t_<NV>>;

namespace karplus_strong_t_parameters
{
// Parameter list for karplus_strong_impl::karplus_strong_t ----------------------------------------

using Damping = parameter::empty;
template <int NV>
using Distortion = parameter::plain<karplus_strong_impl::xfader_t<NV>, 
                                    0>;
template <int NV>
using karplus_strong_t_plist = parameter::list<Damping, Distortion<NV>>;
}

template <int NV>
using karplus_strong_t_ = container::chain<karplus_strong_t_parameters::karplus_strong_t_plist<NV>, 
                                           wrap::fix<2, oscillator_t<NV>>, 
                                           ahdsr_t<NV>, 
                                           midi_t<NV>, 
                                           converter_t<NV>, 
                                           converter1_t<NV>, 
                                           xfader_t<NV>, 
                                           frame2_block_t<NV>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public karplus_strong_impl::karplus_strong_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(karplus_strong);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(36)
		{
			0x005C, 0x0000, 0x0000, 0x6144, 0x706D, 0x6E69, 0x0067, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x0000, 0x005C, 0x0001, 0x0000, 0x6944, 0x7473, 0x726F, 0x6974, 
            0x6E6F, 0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 0x0000, 
            0x8000, 0x003F, 0x0000, 0x0000
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
		
		auto& oscillator = this->getT(0);           // karplus_strong_impl::oscillator_t<NV>
		auto& ahdsr = this->getT(1);                // karplus_strong_impl::ahdsr_t<NV>
		auto& midi = this->getT(2);                 // karplus_strong_impl::midi_t<NV>
		auto& converter = this->getT(3);            // karplus_strong_impl::converter_t<NV>
		auto& converter1 = this->getT(4);           // karplus_strong_impl::converter1_t<NV>
		auto& xfader = this->getT(5);               // karplus_strong_impl::xfader_t<NV>
		auto& frame2_block = this->getT(6);         // karplus_strong_impl::frame2_block_t<NV>
		auto& extra_mod = this->getT(6).getT(0);    // karplus_strong_impl::extra_mod_t<NV>
		auto& receive = this->getT(6).getT(1);      // routing::receive<NV, stereo_frame_cable<NV>>
		auto& tanh = this->getT(6).getT(2);         // math::tanh<NV>
		auto& one_pole = this->getT(6).getT(3);     // filters::one_pole<NV>
		auto& jdelay_cubic = this->getT(6).getT(4); // jdsp::jdelay_cubic<NV>
		auto& send = this->getT(6).getT(5);         // routing::send<NV, stereo_frame_cable<NV>>
		
		// Parameter Connections -------------------------------------------------------------------
		
		this->getParameterT(1).connectT(0, xfader); // Distortion -> xfader::Value
		
		// Modulation Connections ------------------------------------------------------------------
		
		auto& ahdsr_p = ahdsr.getWrappedObject().getParameter();
		converter1.getWrappedObject().getParameter().connectT(0, jdelay_cubic); // converter1 -> jdelay_cubic::DelayTime
		converter.getWrappedObject().getParameter().connectT(0, converter1);    // converter -> converter1::Value
		midi.getParameter().connectT(0, converter);                             // midi -> converter::Value
		auto& xfader_p = xfader.getWrappedObject().getParameter();
		xfader_p.getParameterT(0).connectT(0, receive); // xfader -> receive::Feedback
		xfader_p.getParameterT(1).connectT(0, tanh);    // xfader -> tanh::Value
		extra_mod.getParameter().connectT(0, one_pole); // extra_mod -> one_pole::Frequency
		
		// Send Connections ------------------------------------------------------------------------
		
		send.connect(receive);
		
		// Default Values --------------------------------------------------------------------------
		
		oscillator.setParameterT(0, 4.);   // core::oscillator::Mode
		oscillator.setParameterT(1, 220.); // core::oscillator::Frequency
		oscillator.setParameterT(2, 1.);   // core::oscillator::FreqRatio
		oscillator.setParameterT(3, 1.);   // core::oscillator::Gate
		oscillator.setParameterT(4, 0.);   // core::oscillator::Phase
		oscillator.setParameterT(5, 1.);   // core::oscillator::Gain
		
		ahdsr.setParameterT(0, 0.);    // envelope::ahdsr::Attack
		ahdsr.setParameterT(1, 1.);    // envelope::ahdsr::AttackLevel
		ahdsr.setParameterT(2, 0.);    // envelope::ahdsr::Hold
		ahdsr.setParameterT(3, 459.3); // envelope::ahdsr::Decay
		ahdsr.setParameterT(4, 0.);    // envelope::ahdsr::Sustain
		ahdsr.setParameterT(5, 20.);   // envelope::ahdsr::Release
		ahdsr.setParameterT(6, 0.692); // envelope::ahdsr::AttackCurve
		ahdsr.setParameterT(7, 0.);    // envelope::ahdsr::Retrigger
		ahdsr.setParameterT(8, 0.);    // envelope::ahdsr::Gate
		
		; // converter::Value is automated
		
		; // converter1::Value is automated
		
		; // xfader::Value is automated
		
		extra_mod.setParameterT(0, 0.); // core::extra_mod::Index
		extra_mod.setParameterT(1, 0.); // core::extra_mod::ProcessSignal
		
		; // receive::Feedback is automated
		
		; // tanh::Value is automated
		
		;                               // one_pole::Frequency is automated
		one_pole.setParameterT(1, 4.2); // filters::one_pole::Q
		one_pole.setParameterT(2, 0.);  // filters::one_pole::Gain
		one_pole.setParameterT(3, 0.);  // filters::one_pole::Smoothing
		one_pole.setParameterT(4, 0.);  // filters::one_pole::Mode
		one_pole.setParameterT(5, 1.);  // filters::one_pole::Enabled
		
		jdelay_cubic.setParameterT(0, 30.); // jdsp::jdelay_cubic::Limit
		;                                   // jdelay_cubic::DelayTime is automated
		
		this->setParameterT(0, 1.);
		this->setParameterT(1, 0.);
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
	
	void connectToRuntimeTarget(bool addConnection, const runtime_target::connection& c)
	{
		// Runtime target Connections --------------------------------------------------------------
		
		this->getT(6).getT(0).connectToRuntimeTarget(addConnection, c); // karplus_strong_impl::extra_mod_t<NV>
	}
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(0).setExternalData(b, index);         // karplus_strong_impl::oscillator_t<NV>
		this->getT(1).setExternalData(b, index);         // karplus_strong_impl::ahdsr_t<NV>
		this->getT(6).getT(0).setExternalData(b, index); // karplus_strong_impl::extra_mod_t<NV>
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
using karplus_strong = wrap::node<karplus_strong_impl::instance<NV>>;
}


