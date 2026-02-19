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

namespace flanger_impl
{
// ==============================| Node & Parameter type declarations |==============================

template <int NV>
using smoothed_parameter_unscaled_t = wrap::mod<parameter::plain<jdsp::jdelay_cubic<NV>, 1>, 
                                                control::smoothed_parameter_unscaled<NV, smoothers::linear_ramp<NV>>>;
template <int NV>
using pma_unscaled_t = control::pma_unscaled<NV, 
                                             parameter::plain<smoothed_parameter_unscaled_t<NV>, 0>>;

template <int NV>
using smoothed_parameter_unscaled1_t = smoothed_parameter_unscaled_t<NV>;
template <int NV>
using pma_unscaled1_t = control::pma_unscaled<NV, 
                                              parameter::plain<smoothed_parameter_unscaled1_t<NV>, 0>>;

template <int NV>
using converter1_mod = parameter::chain<ranges::Identity, 
                                        parameter::plain<pma_unscaled_t<NV>, 0>, 
                                        parameter::plain<pma_unscaled1_t<NV>, 0>>;

template <int NV>
using converter1_t = control::converter<converter1_mod<NV>, 
                                        conversion_logic::freq2ms>;
template <int NV>
using converter_t = control::converter<parameter::plain<converter1_t<NV>, 0>, 
                                       conversion_logic::midi2freq>;
template <int NV>
using normaliser_t = control::normaliser<parameter::plain<converter_t<NV>, 0>>;
template <int NV>
using mono_frame_cable = cable::frame<NV, 1>;
namespace custom
{

struct cable_expr
{
	static double op(double input)
	{
		return input * 0.2;
	}
};
}

template <int NV>
using cable_expr_t = control::cable_expr<custom::cable_expr, 
                                         parameter::plain<pma_unscaled_t<NV>, 2>>;

template <int NV>
using chain_t = container::chain<parameter::empty, 
                                 wrap::fix<1, routing::receive<NV, mono_frame_cable<NV>>>, 
                                 cable_expr_t<NV>, 
                                 pma_unscaled_t<NV>, 
                                 smoothed_parameter_unscaled_t<NV>, 
                                 filters::one_pole<NV>, 
                                 jdsp::jdelay_cubic<NV>, 
                                 routing::send<NV, mono_frame_cable<NV>>>;
namespace custom
{

struct cable_expr1
{
	static double op(double input)
	{
		return input * -0.2;
	}
};
}

template <int NV>
using cable_expr1_t = control::cable_expr<custom::cable_expr1, 
                                          parameter::plain<pma_unscaled1_t<NV>, 2>>;

template <int NV>
using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<1, routing::receive<NV, mono_frame_cable<NV>>>, 
                                  cable_expr1_t<NV>, 
                                  pma_unscaled1_t<NV>, 
                                  smoothed_parameter_unscaled1_t<NV>, 
                                  filters::one_pole<NV>, 
                                  jdsp::jdelay_cubic<NV>, 
                                  routing::send<NV, mono_frame_cable<NV>>>;

template <int NV>
using multi_t = container::multi<parameter::empty, 
                                 wrap::fix<1, chain_t<NV>>, 
                                 wrap::fix<1, chain1_t<NV>>>;

template <int NV>
using frame2_block_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, normaliser_t<NV>>, 
                                         converter_t<NV>, 
                                         converter1_t<NV>, 
                                         multi_t<NV>>;

template <int NV>
using frame2_block_t = wrap::frame<2, frame2_block_t_<NV>>;

namespace flanger_t_parameters
{
// Parameter list for flanger_impl::flanger_t ------------------------------------------------------

DECLARE_PARAMETER_RANGE_STEP(Note_InputRange, 
                             0., 
                             127., 
                             1.);

template <int NV>
using Note = parameter::chain<Note_InputRange, 
                              parameter::plain<flanger_impl::normaliser_t<NV>, 0>>;

template <int NV>
using Stereo = parameter::chain<ranges::Identity, 
                                parameter::plain<flanger_impl::cable_expr_t<NV>, 0>, 
                                parameter::plain<flanger_impl::cable_expr1_t<NV>, 0>>;

DECLARE_PARAMETER_RANGE(Damping_0Range, 
                        1000., 
                        20000.);

template <int NV>
using Damping_0 = parameter::from0To1<filters::one_pole<NV>, 
                                      0, 
                                      Damping_0Range>;

template <int NV> using Damping_1 = Damping_0<NV>;

template <int NV>
using Damping = parameter::chain<ranges::Identity, 
                                 Damping_0<NV>, 
                                 Damping_1<NV>>;

template <int NV>
using Feedback = parameter::chain<ranges::Identity, 
                                  parameter::plain<routing::receive<NV, mono_frame_cable<NV>>, 0>, 
                                  parameter::plain<routing::receive<NV, mono_frame_cable<NV>>, 0>>;

template <int NV>
using flanger_t_plist = parameter::list<Note<NV>, 
                                        Stereo<NV>, 
                                        Damping<NV>, 
                                        Feedback<NV>>;
}

template <int NV>
using flanger_t_ = container::chain<flanger_t_parameters::flanger_t_plist<NV>, 
                                    wrap::fix<2, frame2_block_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public flanger_impl::flanger_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(flanger);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(68)
		{
			0x005C, 0x0000, 0x0000, 0x6F4E, 0x6574, 0x0000, 0x0000, 0x0000, 
            0xFE00, 0x0042, 0x4000, 0x0042, 0x8000, 0x003F, 0x8000, 0x5C3F, 
            0x0100, 0x0000, 0x5300, 0x6574, 0x6572, 0x006F, 0x0000, 0x0000, 
            0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x0000, 
            0x005C, 0x0002, 0x0000, 0x6144, 0x706D, 0x6E69, 0x0067, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x2439, 0x3E63, 0x0000, 0x3F80, 0x0000, 
            0x0000, 0x005C, 0x0003, 0x0000, 0x6546, 0x6465, 0x6162, 0x6B63, 
            0x0000, 0x0000, 0x0000, 0x8000, 0x0E3F, 0x5F6B, 0x003F, 0x8000, 
            0x003F, 0x0000, 0x0000, 0x0000
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
		
		auto& frame2_block = this->getT(0);                                         // flanger_impl::frame2_block_t<NV>
		auto& normaliser = this->getT(0).getT(0);                                   // flanger_impl::normaliser_t<NV>
		auto& converter = this->getT(0).getT(1);                                    // flanger_impl::converter_t<NV>
		auto& converter1 = this->getT(0).getT(2);                                   // flanger_impl::converter1_t<NV>
		auto& multi = this->getT(0).getT(3);                                        // flanger_impl::multi_t<NV>
		auto& chain = this->getT(0).getT(3).getT(0);                                // flanger_impl::chain_t<NV>
		auto& receive = this->getT(0).getT(3).getT(0).getT(0);                      // routing::receive<NV, mono_frame_cable<NV>>
		auto& cable_expr = this->getT(0).getT(3).getT(0).getT(1);                   // flanger_impl::cable_expr_t<NV>
		auto& pma_unscaled = this->getT(0).getT(3).getT(0).getT(2);                 // flanger_impl::pma_unscaled_t<NV>
		auto& smoothed_parameter_unscaled = this->getT(0).getT(3).getT(0).getT(3);  // flanger_impl::smoothed_parameter_unscaled_t<NV>
		auto& one_pole = this->getT(0).getT(3).getT(0).getT(4);                     // filters::one_pole<NV>
		auto& jdelay_cubic = this->getT(0).getT(3).getT(0).getT(5);                 // jdsp::jdelay_cubic<NV>
		auto& send = this->getT(0).getT(3).getT(0).getT(6);                         // routing::send<NV, mono_frame_cable<NV>>
		auto& chain1 = this->getT(0).getT(3).getT(1);                               // flanger_impl::chain1_t<NV>
		auto& receive1 = this->getT(0).getT(3).getT(1).getT(0);                     // routing::receive<NV, mono_frame_cable<NV>>
		auto& cable_expr1 = this->getT(0).getT(3).getT(1).getT(1);                  // flanger_impl::cable_expr1_t<NV>
		auto& pma_unscaled1 = this->getT(0).getT(3).getT(1).getT(2);                // flanger_impl::pma_unscaled1_t<NV>
		auto& smoothed_parameter_unscaled1 = this->getT(0).getT(3).getT(1).getT(3); // flanger_impl::smoothed_parameter_unscaled1_t<NV>
		auto& one_pole1 = this->getT(0).getT(3).getT(1).getT(4);                    // filters::one_pole<NV>
		auto& jdelay_cubic1 = this->getT(0).getT(3).getT(1).getT(5);                // jdsp::jdelay_cubic<NV>
		auto& send1 = this->getT(0).getT(3).getT(1).getT(6);                        // routing::send<NV, mono_frame_cable<NV>>
		
		// Parameter Connections -------------------------------------------------------------------
		
		this->getParameterT(0).connectT(0, normaliser); // Note -> normaliser::Value
		
		auto& Stereo_p = this->getParameterT(1);
		Stereo_p.connectT(0, cable_expr);  // Stereo -> cable_expr::Value
		Stereo_p.connectT(1, cable_expr1); // Stereo -> cable_expr1::Value
		
		auto& Damping_p = this->getParameterT(2);
		Damping_p.connectT(0, one_pole);  // Damping -> one_pole::Frequency
		Damping_p.connectT(1, one_pole1); // Damping -> one_pole1::Frequency
		
		auto& Feedback_p = this->getParameterT(3);
		Feedback_p.connectT(0, receive);  // Feedback -> receive::Feedback
		Feedback_p.connectT(1, receive1); // Feedback -> receive1::Feedback
		
		// Modulation Connections ------------------------------------------------------------------
		
		smoothed_parameter_unscaled.getParameter().connectT(0, jdelay_cubic);                      // smoothed_parameter_unscaled -> jdelay_cubic::DelayTime
		pma_unscaled.getWrappedObject().getParameter().connectT(0, smoothed_parameter_unscaled);   // pma_unscaled -> smoothed_parameter_unscaled::Value
		smoothed_parameter_unscaled1.getParameter().connectT(0, jdelay_cubic1);                    // smoothed_parameter_unscaled1 -> jdelay_cubic1::DelayTime
		pma_unscaled1.getWrappedObject().getParameter().connectT(0, smoothed_parameter_unscaled1); // pma_unscaled1 -> smoothed_parameter_unscaled1::Value
		converter1.getWrappedObject().getParameter().connectT(0, pma_unscaled);                    // converter1 -> pma_unscaled::Value
		converter1.getWrappedObject().getParameter().connectT(1, pma_unscaled1);                   // converter1 -> pma_unscaled1::Value
		converter.getWrappedObject().getParameter().connectT(0, converter1);                       // converter -> converter1::Value
		normaliser.getWrappedObject().getParameter().connectT(0, converter);                       // normaliser -> converter::Value
		cable_expr.getWrappedObject().getParameter().connectT(0, pma_unscaled);                    // cable_expr -> pma_unscaled::Add
		cable_expr1.getWrappedObject().getParameter().connectT(0, pma_unscaled1);                  // cable_expr1 -> pma_unscaled1::Add
		
		// Send Connections ------------------------------------------------------------------------
		
		send.connect(receive);
		send1.connect(receive1);
		
		// Default Values --------------------------------------------------------------------------
		
		; // normaliser::Value is automated
		
		; // converter::Value is automated
		
		; // converter1::Value is automated
		
		; // receive::Feedback is automated
		
		; // cable_expr::Value is automated
		
		;                                  // pma_unscaled::Value is automated
		pma_unscaled.setParameterT(1, 1.); // control::pma_unscaled::Multiply
		;                                  // pma_unscaled::Add is automated
		
		;                                                   // smoothed_parameter_unscaled::Value is automated
		smoothed_parameter_unscaled.setParameterT(1, 100.); // control::smoothed_parameter_unscaled::SmoothingTime
		smoothed_parameter_unscaled.setParameterT(2, 1.);   // control::smoothed_parameter_unscaled::Enabled
		
		;                                // one_pole::Frequency is automated
		one_pole.setParameterT(1, 1.);   // filters::one_pole::Q
		one_pole.setParameterT(2, 0.);   // filters::one_pole::Gain
		one_pole.setParameterT(3, 0.01); // filters::one_pole::Smoothing
		one_pole.setParameterT(4, 0.);   // filters::one_pole::Mode
		one_pole.setParameterT(5, 1.);   // filters::one_pole::Enabled
		
		jdelay_cubic.setParameterT(0, 50.); // jdsp::jdelay_cubic::Limit
		;                                   // jdelay_cubic::DelayTime is automated
		
		; // receive1::Feedback is automated
		
		; // cable_expr1::Value is automated
		
		;                                   // pma_unscaled1::Value is automated
		pma_unscaled1.setParameterT(1, 1.); // control::pma_unscaled::Multiply
		;                                   // pma_unscaled1::Add is automated
		
		;                                                    // smoothed_parameter_unscaled1::Value is automated
		smoothed_parameter_unscaled1.setParameterT(1, 100.); // control::smoothed_parameter_unscaled::SmoothingTime
		smoothed_parameter_unscaled1.setParameterT(2, 1.);   // control::smoothed_parameter_unscaled::Enabled
		
		;                                 // one_pole1::Frequency is automated
		one_pole1.setParameterT(1, 1.);   // filters::one_pole::Q
		one_pole1.setParameterT(2, 0.);   // filters::one_pole::Gain
		one_pole1.setParameterT(3, 0.01); // filters::one_pole::Smoothing
		one_pole1.setParameterT(4, 0.);   // filters::one_pole::Mode
		one_pole1.setParameterT(5, 1.);   // filters::one_pole::Enabled
		
		jdelay_cubic1.setParameterT(0, 50.); // jdsp::jdelay_cubic::Limit
		;                                    // jdelay_cubic1::DelayTime is automated
		
		this->setParameterT(0, 48.);
		this->setParameterT(1, 1.);
		this->setParameterT(2, 0.221818);
		this->setParameterT(3, 0.872727);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool hasTail() { return true; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
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
using flanger = wrap::node<flanger_impl::instance<NV>>;
}


