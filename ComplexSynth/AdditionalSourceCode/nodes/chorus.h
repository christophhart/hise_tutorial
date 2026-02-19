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

namespace chorus_impl
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
                                    core::fix_delay, 
                                    core::gain<NV>>;
namespace custom
{

struct cable_expr
{
	static double op(double input)
	{
		return input > 0.5 ? 2 : 0;
	}
};
}

template <int NV>
using oscillator_t = wrap::no_data<core::oscillator<NV>>;
template <int NV>
using cable_expr_t = control::cable_expr<custom::cable_expr, 
                                         parameter::plain<oscillator_t<NV>, 0>>;

template <int NV>
using smoothed_parameter_t = wrap::mod<parameter::plain<oscillator_t<NV>, 5>, 
                                       control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;

DECLARE_PARAMETER_RANGE_SKEW_INV(smoothed_parameter1_modRange, 
                                 0.2, 
                                 1., 
                                 0.612027);

template <int NV>
using smoothed_parameter1_mod = parameter::from0To1_inv<math::mul<NV>, 
                                                        0, 
                                                        smoothed_parameter1_modRange>;

template <int NV>
using smoothed_parameter1_t = wrap::mod<smoothed_parameter1_mod<NV>, 
                                        control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;

DECLARE_PARAMETER_RANGE(peak_modRange, 
                        0., 
                        10.);

template <int NV>
using peak_mod = parameter::from0To1<jdsp::jdelay_cubic<NV>, 
                                     1, 
                                     peak_modRange>;

template <int NV>
using peak_t = wrap::mod<peak_mod<NV>, 
                         wrap::no_data<core::peak>>;

template <int NV> using peak1_mod = peak_mod<NV>;

template <int NV>
using peak1_t = wrap::mod<peak1_mod<NV>, 
                          wrap::no_data<core::peak>>;

template <int NV>
using modchain_t_ = container::chain<parameter::empty, 
                                     wrap::fix<1, cable_expr_t<NV>>, 
                                     smoothed_parameter_t<NV>, 
                                     oscillator_t<NV>, 
                                     smoothed_parameter1_t<NV>, 
                                     math::mul<NV>, 
                                     math::sig2mod<NV>, 
                                     peak_t<NV>, 
                                     math::mod_inv<NV>, 
                                     peak1_t<NV>>;

template <int NV>
using modchain_t = wrap::control_rate<modchain_t_<NV>>;

template <int NV>
using no_midi_t_ = container::chain<parameter::empty, 
                                    wrap::fix<2, modchain_t<NV>>>;

template <int NV>
using no_midi_t = wrap::no_midi<no_midi_t_<NV>>;

template <int NV>
using multi_t = container::multi<parameter::empty, 
                                 wrap::fix<1, jdsp::jdelay_cubic<NV>>, 
                                 wrap::fix<1, jdsp::jdelay_cubic<NV>>>;

template <int NV>
using frame2_block_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, no_midi_t<NV>>, 
                                         multi_t<NV>>;

template <int NV>
using frame2_block_t = wrap::frame<2, frame2_block_t_<NV>>;

template <int NV>
using xfader_multimod = parameter::list<parameter::plain<math::mul<NV>, 0>, 
                                        parameter::plain<math::mul<NV>, 0>>;

template <int NV>
using xfader_t = control::xfader<xfader_multimod<NV>, faders::linear>;

template <int NV>
using chain_t = container::chain<parameter::empty, 
                                 wrap::fix<1, math::mul<NV>>>;

template <int NV> using chain3_t = chain_t<NV>;

template <int NV>
using multi1_t = container::multi<parameter::empty, 
                                  wrap::fix<1, chain_t<NV>>, 
                                  wrap::fix<1, chain3_t<NV>>>;

template <int NV>
using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<2, routing::ms_encode>, 
                                  xfader_t<NV>, 
                                  multi1_t<NV>, 
                                  math::mul<NV>, 
                                  routing::ms_decode>;

template <int NV>
using wet_path_t = container::chain<parameter::empty, 
                                    wrap::fix<2, frame2_block_t<NV>>, 
                                    chain1_t<NV>, 
                                    core::gain<NV>>;

namespace dry_wet1_t_parameters
{
}

template <int NV>
using dry_wet1_t = container::split<parameter::plain<chorus_impl::dry_wet_mixer_t<NV>, 0>, 
                                    wrap::fix<2, dry_path_t<NV>>, 
                                    wet_path_t<NV>>;

namespace chorus_t_parameters
{
// Parameter list for chorus_impl::chorus_t --------------------------------------------------------

template <int NV>
using Frequency_0 = parameter::from0To1<chorus_impl::oscillator_t<NV>, 
                                        1, 
                                        chorus_impl::peak_modRange>;

template <int NV>
using Frequency = parameter::chain<ranges::Identity, 
                                   Frequency_0<NV>, 
                                   parameter::plain<chorus_impl::smoothed_parameter1_t<NV>, 0>>;

DECLARE_PARAMETER_RANGE(Width_InputRange, 
                        0., 
                        2.);

template <int NV>
using Width = parameter::chain<Width_InputRange, 
                               parameter::plain<chorus_impl::xfader_t<NV>, 0>>;

template <int NV>
using Depth = parameter::plain<chorus_impl::smoothed_parameter_t<NV>, 
                               0>;
template <int NV>
using Mix = parameter::plain<chorus_impl::dry_wet1_t<NV>, 
                             0>;
template <int NV>
using Shape = parameter::plain<chorus_impl::cable_expr_t<NV>, 
                               0>;
template <int NV>
using chorus_t_plist = parameter::list<Frequency<NV>, 
                                       Depth<NV>, 
                                       Mix<NV>, 
                                       Shape<NV>, 
                                       Width<NV>>;
}

template <int NV>
using chorus_t_ = container::chain<chorus_t_parameters::chorus_t_plist<NV>, 
                                   wrap::fix<2, dry_wet1_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public chorus_impl::chorus_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(chorus);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(82)
		{
			0x005C, 0x0000, 0x0000, 0x7246, 0x7165, 0x6575, 0x636E, 0x0079, 
            0x0000, 0x0000, 0x0000, 0x3F80, 0x57CF, 0x3D35, 0x0000, 0x3F80, 
            0x0000, 0x0000, 0x005C, 0x0001, 0x0000, 0x6544, 0x7470, 0x0068, 
            0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 
            0x0000, 0x0000, 0x005C, 0x0002, 0x0000, 0x694D, 0x0078, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x0000, 0x005C, 0x0003, 0x0000, 0x6853, 0x7061, 0x0065, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 
            0x0000, 0x055C, 0x0004, 0x0000, 0x6957, 0x7464, 0x0068, 0x0000, 
            0x0000, 0x0000, 0x4000, 0x98E1, 0x3F7F, 0x0000, 0x3F80, 0x0000, 
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
		// Node References -------------------------------------------------------------------------
		
		auto& dry_wet1 = this->getT(0);                                                    // chorus_impl::dry_wet1_t<NV>
		auto& dry_path = this->getT(0).getT(0);                                            // chorus_impl::dry_path_t<NV>
		auto& dry_wet_mixer = this->getT(0).getT(0).getT(0);                               // chorus_impl::dry_wet_mixer_t<NV>
		auto& fix_delay = this->getT(0).getT(0).getT(1);                                   // core::fix_delay
		auto& dry_gain = this->getT(0).getT(0).getT(2);                                    // core::gain<NV>
		auto& wet_path = this->getT(0).getT(1);                                            // chorus_impl::wet_path_t<NV>
		auto& frame2_block = this->getT(0).getT(1).getT(0);                                // chorus_impl::frame2_block_t<NV>
		auto& no_midi = this->getT(0).getT(1).getT(0).getT(0);                             // chorus_impl::no_midi_t<NV>
		auto& modchain = this->getT(0).getT(1).getT(0).getT(0).getT(0);                    // chorus_impl::modchain_t<NV>
		auto& cable_expr = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(0);          // chorus_impl::cable_expr_t<NV>
		auto& smoothed_parameter = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(1);  // chorus_impl::smoothed_parameter_t<NV>
		auto& oscillator = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(2);          // chorus_impl::oscillator_t<NV>
		auto& smoothed_parameter1 = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(3); // chorus_impl::smoothed_parameter1_t<NV>
		auto& mul2 = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(4);                // math::mul<NV>
		auto& sig2mod = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(5);             // math::sig2mod<NV>
		auto& peak = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(6);                // chorus_impl::peak_t<NV>
		auto& mod_inv = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(7);             // math::mod_inv<NV>
		auto& peak1 = this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(8);               // chorus_impl::peak1_t<NV>
		auto& multi = this->getT(0).getT(1).getT(0).getT(1);                               // chorus_impl::multi_t<NV>
		auto& jdelay_cubic = this->getT(0).getT(1).getT(0).getT(1).getT(0);                // jdsp::jdelay_cubic<NV>
		auto& jdelay_cubic1 = this->getT(0).getT(1).getT(0).getT(1).getT(1);               // jdsp::jdelay_cubic<NV>
		auto& chain1 = this->getT(0).getT(1).getT(1);                                      // chorus_impl::chain1_t<NV>
		auto& ms_encode = this->getT(0).getT(1).getT(1).getT(0);                           // routing::ms_encode
		auto& xfader = this->getT(0).getT(1).getT(1).getT(1);                              // chorus_impl::xfader_t<NV>
		auto& multi1 = this->getT(0).getT(1).getT(1).getT(2);                              // chorus_impl::multi1_t<NV>
		auto& chain = this->getT(0).getT(1).getT(1).getT(2).getT(0);                       // chorus_impl::chain_t<NV>
		auto& mul = this->getT(0).getT(1).getT(1).getT(2).getT(0).getT(0);                 // math::mul<NV>
		auto& chain3 = this->getT(0).getT(1).getT(1).getT(2).getT(1);                      // chorus_impl::chain3_t<NV>
		auto& mul1 = this->getT(0).getT(1).getT(1).getT(2).getT(1).getT(0);                // math::mul<NV>
		auto& mul3 = this->getT(0).getT(1).getT(1).getT(3);                                // math::mul<NV>
		auto& ms_decode = this->getT(0).getT(1).getT(1).getT(4);                           // routing::ms_decode
		auto& wet_gain = this->getT(0).getT(1).getT(2);                                    // core::gain<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		dry_wet1.getParameterT(0).connectT(0, dry_wet_mixer); // DryWet -> dry_wet_mixer::Value
		auto& Frequency_p = this->getParameterT(0);
		Frequency_p.connectT(0, oscillator);          // Frequency -> oscillator::Frequency
		Frequency_p.connectT(1, smoothed_parameter1); // Frequency -> smoothed_parameter1::Value
		
		this->getParameterT(1).connectT(0, smoothed_parameter); // Depth -> smoothed_parameter::Value
		
		this->getParameterT(2).connectT(0, dry_wet1); // Mix -> dry_wet1::DryWet
		
		this->getParameterT(3).connectT(0, cable_expr); // Shape -> cable_expr::Value
		
		this->getParameterT(4).connectT(0, xfader); // Width -> xfader::Value
		
		// Modulation Connections ------------------------------------------------------------------
		
		auto& dry_wet_mixer_p = dry_wet_mixer.getWrappedObject().getParameter();
		dry_wet_mixer_p.getParameterT(0).connectT(0, dry_gain);               // dry_wet_mixer -> dry_gain::Gain
		dry_wet_mixer_p.getParameterT(1).connectT(0, wet_gain);               // dry_wet_mixer -> wet_gain::Gain
		cable_expr.getWrappedObject().getParameter().connectT(0, oscillator); // cable_expr -> oscillator::Mode
		smoothed_parameter.getParameter().connectT(0, oscillator);            // smoothed_parameter -> oscillator::Gain
		smoothed_parameter1.getParameter().connectT(0, mul2);                 // smoothed_parameter1 -> mul2::Value
		peak.getParameter().connectT(0, jdelay_cubic);                        // peak -> jdelay_cubic::DelayTime
		peak1.getParameter().connectT(0, jdelay_cubic1);                      // peak1 -> jdelay_cubic1::DelayTime
		auto& xfader_p = xfader.getWrappedObject().getParameter();
		xfader_p.getParameterT(0).connectT(0, mul);  // xfader -> mul::Value
		xfader_p.getParameterT(1).connectT(0, mul1); // xfader -> mul1::Value
		
		// Default Values --------------------------------------------------------------------------
		
		; // dry_wet1::DryWet is automated
		
		; // dry_wet_mixer::Value is automated
		
		fix_delay.setParameterT(0, 5);    // core::fix_delay::DelayTime
		fix_delay.setParameterT(1, 512.); // core::fix_delay::FadeTime
		
		;                               // dry_gain::Gain is automated
		dry_gain.setParameterT(1, 20.); // core::gain::Smoothing
		dry_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		; // cable_expr::Value is automated
		
		;                                         // smoothed_parameter::Value is automated
		smoothed_parameter.setParameterT(1, 30.); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter.setParameterT(2, 1.);  // control::smoothed_parameter::Enabled
		
		;                                // oscillator::Mode is automated
		;                                // oscillator::Frequency is automated
		oscillator.setParameterT(2, 1.); // core::oscillator::FreqRatio
		oscillator.setParameterT(3, 1.); // core::oscillator::Gate
		oscillator.setParameterT(4, 0.); // core::oscillator::Phase
		;                                // oscillator::Gain is automated
		
		;                                          // smoothed_parameter1::Value is automated
		smoothed_parameter1.setParameterT(1, 30.); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter1.setParameterT(2, 1.);  // control::smoothed_parameter::Enabled
		
		; // mul2::Value is automated
		
		sig2mod.setParameterT(0, 0.); // math::sig2mod::Value
		
		mod_inv.setParameterT(0, 0.); // math::mod_inv::Value
		
		jdelay_cubic.setParameterT(0, 10.); // jdsp::jdelay_cubic::Limit
		;                                   // jdelay_cubic::DelayTime is automated
		
		jdelay_cubic1.setParameterT(0, 10.); // jdsp::jdelay_cubic::Limit
		;                                    // jdelay_cubic1::DelayTime is automated
		
		; // xfader::Value is automated
		
		; // mul::Value is automated
		
		; // mul1::Value is automated
		
		mul3.setParameterT(0, 2.); // math::mul::Value
		
		;                               // wet_gain::Gain is automated
		wet_gain.setParameterT(1, 20.); // core::gain::Smoothing
		wet_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		this->setParameterT(0, 0.0442732);
		this->setParameterT(1, 1.);
		this->setParameterT(2, 1.);
		this->setParameterT(3, 0.);
		this->setParameterT(4, 0.998427);
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
		
		this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(2).setExternalData(b, index); // chorus_impl::oscillator_t<NV>
		this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(6).setExternalData(b, index); // chorus_impl::peak_t<NV>
		this->getT(0).getT(1).getT(0).getT(0).getT(0).getT(8).setExternalData(b, index); // chorus_impl::peak1_t<NV>
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
using chorus = wrap::node<chorus_impl::instance<NV>>;
}


