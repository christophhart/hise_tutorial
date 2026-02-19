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

namespace delay_impl
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
                                        faders::linear>;

template <int NV>
using dry_path_t = container::chain<parameter::empty, 
                                    wrap::fix<2, dry_wet_mixer_t<NV>>, 
                                    core::gain<NV>>;

template <int NV>
using pma_unscaled_t = control::pma_unscaled<NV, 
                                             parameter::plain<core::fix_delay, 0>>;
template <int NV>
using tempo_sync_t = wrap::mod<parameter::plain<pma_unscaled_t<NV>, 0>, 
                               control::tempo_sync<NV>>;
namespace custom
{

struct cable_expr
{
	static double op(double input)
	{
		return input * 4.0 - 3.0;
	}
};
}

template <int NV>
using cable_expr_t = control::cable_expr<custom::cable_expr, 
                                         parameter::plain<pma_unscaled_t<NV>, 2>>;

template <int NV>
using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<2, tempo_sync_t<NV>>, 
                                  cable_expr_t<NV>, 
                                  pma_unscaled_t<NV>>;

template <int NV> using pma_unscaled1_t = pma_unscaled_t<NV>;
template <int NV>
using tempo_sync1_t = wrap::mod<parameter::plain<pma_unscaled1_t<NV>, 0>, 
                                control::tempo_sync<NV>>;
namespace custom
{

struct cable_expr1
{
	static double op(double input)
	{
		return input * -4.0 - 3.0;
	}
};
}

template <int NV>
using cable_expr1_t = control::cable_expr<custom::cable_expr1, 
                                          parameter::plain<pma_unscaled1_t<NV>, 2>>;

template <int NV>
using chain3_t = container::chain<parameter::empty, 
                                  wrap::fix<2, tempo_sync1_t<NV>>, 
                                  cable_expr1_t<NV>, 
                                  pma_unscaled1_t<NV>>;

template <int NV>
using chain4_t = container::chain<parameter::empty, 
                                  wrap::fix<2, chain1_t<NV>>, 
                                  chain3_t<NV>>;
template <int NV>
using oscillator_t = wrap::no_data<core::oscillator<NV>>;

template <int NV>
using no_midi_t_ = container::chain<parameter::empty, 
                                    wrap::fix<1, oscillator_t<NV>>>;

template <int NV>
using no_midi_t = wrap::no_midi<no_midi_t_<NV>>;

DECLARE_PARAMETER_RANGE(peak_modRange, 
                        0., 
                        6.);

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
                                     wrap::fix<1, no_midi_t<NV>>, 
                                     math::sig2mod<NV>, 
                                     peak_t<NV>, 
                                     math::mod_inv<NV>, 
                                     peak1_t<NV>>;

template <int NV>
using modchain_t = wrap::control_rate<modchain_t_<NV>>;
template <int NV>
using mono_frame_cable = cable::frame<NV, 1>;
template <int NV>
using one_pole_t = wrap::data<filters::one_pole<NV>, 
                              data::external::filter<0>>;

template <int NV>
using chain_t = container::chain<parameter::empty, 
                                 wrap::fix<1, routing::receive<NV, mono_frame_cable<NV>>>, 
                                 core::fix_delay, 
                                 jdsp::jdelay_cubic<NV>, 
                                 one_pole_t<NV>, 
                                 routing::send<NV, mono_frame_cable<NV>>>;

template <int NV>
using chain2_t = container::chain<parameter::empty, 
                                  wrap::fix<1, routing::receive<NV, mono_frame_cable<NV>>>, 
                                  core::fix_delay, 
                                  jdsp::jdelay_cubic<NV>, 
                                  filters::one_pole<NV>, 
                                  routing::send<NV, mono_frame_cable<NV>>>;

template <int NV>
using multi_t = container::multi<parameter::empty, 
                                 wrap::fix<1, chain_t<NV>>, 
                                 wrap::fix<1, chain2_t<NV>>>;

template <int NV>
using frame2_block_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, modchain_t<NV>>, 
                                         multi_t<NV>>;

template <int NV>
using frame2_block_t = wrap::frame<2, frame2_block_t_<NV>>;

template <int NV>
using wet_path_t = container::chain<parameter::empty, 
                                    wrap::fix<2, chain4_t<NV>>, 
                                    frame2_block_t<NV>, 
                                    core::gain<NV>>;

namespace dry_wet1_t_parameters
{
}

template <int NV>
using dry_wet1_t = container::split<parameter::plain<delay_impl::dry_wet_mixer_t<NV>, 0>, 
                                    wrap::fix<2, dry_path_t<NV>>, 
                                    wet_path_t<NV>>;

namespace delay_t_parameters
{
// Parameter list for delay_impl::delay_t ----------------------------------------------------------

template <int NV>
using FilterType = parameter::chain<ranges::Identity, 
                                    parameter::plain<delay_impl::one_pole_t<NV>, 4>, 
                                    parameter::plain<filters::one_pole<NV>, 4>>;

template <int NV>
using FilterFrequency = parameter::chain<ranges::Identity, 
                                         parameter::plain<delay_impl::one_pole_t<NV>, 0>, 
                                         parameter::plain<filters::one_pole<NV>, 0>>;

template <int NV>
using Sync = parameter::chain<ranges::Identity, 
                              parameter::plain<delay_impl::tempo_sync_t<NV>, 2>, 
                              parameter::plain<delay_impl::tempo_sync1_t<NV>, 2>>;

template <int NV>
using Stereo = parameter::chain<ranges::Identity, 
                                parameter::plain<delay_impl::cable_expr_t<NV>, 0>, 
                                parameter::plain<delay_impl::cable_expr1_t<NV>, 0>>;

template <int NV>
using TimeLeft = parameter::plain<delay_impl::tempo_sync_t<NV>, 
                                  0>;
template <int NV>
using TimeRight = parameter::plain<delay_impl::tempo_sync1_t<NV>, 
                                   0>;
template <int NV>
using FeedbackLeft = parameter::plain<routing::receive<NV, mono_frame_cable<NV>>, 
                                      0>;
template <int NV> using FeedbackRight = FeedbackLeft<NV>;
template <int NV>
using TimeUnsyncedLeft = parameter::plain<delay_impl::tempo_sync_t<NV>, 
                                          3>;
template <int NV>
using TimeUnsyncedRight = parameter::plain<delay_impl::tempo_sync1_t<NV>, 
                                           3>;
template <int NV>
using Mix = parameter::plain<delay_impl::dry_wet1_t<NV>, 
                             0>;
template <int NV>
using ModAmount = parameter::plain<delay_impl::oscillator_t<NV>, 
                                   5>;
template <int NV>
using ModFrequency = parameter::plain<delay_impl::oscillator_t<NV>, 
                                      1>;
template <int NV>
using delay_t_plist = parameter::list<TimeLeft<NV>, 
                                      TimeRight<NV>, 
                                      FeedbackLeft<NV>, 
                                      FeedbackRight<NV>, 
                                      FilterType<NV>, 
                                      FilterFrequency<NV>, 
                                      Sync<NV>, 
                                      TimeUnsyncedLeft<NV>, 
                                      TimeUnsyncedRight<NV>, 
                                      Mix<NV>, 
                                      ModAmount<NV>, 
                                      ModFrequency<NV>, 
                                      Stereo<NV>>;
}

template <int NV>
using delay_t_ = container::chain<delay_t_parameters::delay_t_plist<NV>, 
                                  wrap::fix<2, dry_wet1_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public delay_impl::delay_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 1;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(delay);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(244)
		{
			0x035C, 0x0000, 0x0000, 0x6954, 0x656D, 0x654C, 0x7466, 0x0000, 
            0x0000, 0x0000, 0x9000, 0x0041, 0xA000, 0x0040, 0x8000, 0x003F, 
            0x8000, 0x5C3F, 0x0103, 0x0000, 0x5400, 0x6D69, 0x5265, 0x6769, 
            0x7468, 0x0000, 0x0000, 0x0000, 0x9000, 0x0041, 0xC000, 0x0040, 
            0x8000, 0x003F, 0x8000, 0x5C3F, 0x0205, 0x0000, 0x4600, 0x6565, 
            0x6264, 0x6361, 0x4C6B, 0x6665, 0x0074, 0x0000, 0x0000, 0x0000, 
            0x3F80, 0xCAA0, 0x3EFD, 0x0000, 0x3F80, 0x0000, 0x0000, 0x055C, 
            0x0003, 0x0000, 0x6546, 0x6465, 0x6162, 0x6B63, 0x6952, 0x6867, 
            0x0074, 0x0000, 0x0000, 0x0000, 0x3F80, 0xB38D, 0x3F00, 0x0000, 
            0x3F80, 0x0000, 0x0000, 0x005C, 0x0004, 0x0000, 0x6946, 0x746C, 
            0x7265, 0x7954, 0x6570, 0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 
            0x0000, 0x0000, 0x8000, 0x003F, 0x8000, 0x5C3F, 0x0501, 0x0000, 
            0x4600, 0x6C69, 0x6574, 0x4672, 0x6572, 0x7571, 0x6E65, 0x7963, 
            0x0000, 0xA000, 0x0041, 0x9C40, 0xB846, 0x758E, 0x1A45, 0x6B6C, 
            0x003E, 0x0000, 0x5C00, 0x0600, 0x0000, 0x5300, 0x6E79, 0x0063, 
            0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 
            0x0000, 0x3F80, 0x025C, 0x0007, 0x0000, 0x6954, 0x656D, 0x6E55, 
            0x7973, 0x636E, 0x6465, 0x654C, 0x7466, 0x0000, 0x0000, 0x0000, 
            0x7A00, 0x3344, 0x9D73, 0x0043, 0x8000, 0xCD3F, 0xCCCC, 0x5C3D, 
            0x0802, 0x0000, 0x5400, 0x6D69, 0x5565, 0x736E, 0x6E79, 0x6563, 
            0x5264, 0x6769, 0x7468, 0x0000, 0x0000, 0x0000, 0x7A00, 0x9A44, 
            0x5819, 0x0043, 0x8000, 0xCD3F, 0xCCCC, 0x5C3D, 0x0905, 0x0000, 
            0x4D00, 0x7869, 0x0000, 0x0000, 0x0000, 0x8000, 0x173F, 0xFF79, 
            0x003E, 0x8000, 0x003F, 0x0000, 0x5C00, 0x0A05, 0x0000, 0x4D00, 
            0x646F, 0x6D41, 0x756F, 0x746E, 0x0000, 0x8000, 0x00BF, 0x8000, 
            0x0A3F, 0x23D7, 0x003D, 0x8000, 0x003F, 0x0000, 0x5C00, 0x0B01, 
            0x0000, 0x4D00, 0x646F, 0x7246, 0x7165, 0x6575, 0x636E, 0x0079, 
            0x0000, 0x0000, 0x0000, 0x4120, 0x44D7, 0x3F1C, 0x209B, 0x3E9A, 
            0x0000, 0x0000, 0x055C, 0x000C, 0x0000, 0x7453, 0x7265, 0x6F65, 
            0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 0x0000, 0x8000, 
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
		
		auto& dry_wet1 = this->getT(0);                                              // delay_impl::dry_wet1_t<NV>
		auto& dry_path = this->getT(0).getT(0);                                      // delay_impl::dry_path_t<NV>
		auto& dry_wet_mixer = this->getT(0).getT(0).getT(0);                         // delay_impl::dry_wet_mixer_t<NV>
		auto& dry_gain = this->getT(0).getT(0).getT(1);                              // core::gain<NV>
		auto& wet_path = this->getT(0).getT(1);                                      // delay_impl::wet_path_t<NV>
		auto& chain4 = this->getT(0).getT(1).getT(0);                                // delay_impl::chain4_t<NV>
		auto& chain1 = this->getT(0).getT(1).getT(0).getT(0);                        // delay_impl::chain1_t<NV>
		auto& tempo_sync = this->getT(0).getT(1).getT(0).getT(0).getT(0);            // delay_impl::tempo_sync_t<NV>
		auto& cable_expr = this->getT(0).getT(1).getT(0).getT(0).getT(1);            // delay_impl::cable_expr_t<NV>
		auto& pma_unscaled = this->getT(0).getT(1).getT(0).getT(0).getT(2);          // delay_impl::pma_unscaled_t<NV>
		auto& chain3 = this->getT(0).getT(1).getT(0).getT(1);                        // delay_impl::chain3_t<NV>
		auto& tempo_sync1 = this->getT(0).getT(1).getT(0).getT(1).getT(0);           // delay_impl::tempo_sync1_t<NV>
		auto& cable_expr1 = this->getT(0).getT(1).getT(0).getT(1).getT(1);           // delay_impl::cable_expr1_t<NV>
		auto& pma_unscaled1 = this->getT(0).getT(1).getT(0).getT(1).getT(2);         // delay_impl::pma_unscaled1_t<NV>
		auto& frame2_block = this->getT(0).getT(1).getT(1);                          // delay_impl::frame2_block_t<NV>
		auto& modchain = this->getT(0).getT(1).getT(1).getT(0);                      // delay_impl::modchain_t<NV>
		auto& no_midi = this->getT(0).getT(1).getT(1).getT(0).getT(0);               // delay_impl::no_midi_t<NV>
		auto& oscillator = this->getT(0).getT(1).getT(1).getT(0).getT(0).getT(0);    // delay_impl::oscillator_t<NV>
		auto& sig2mod = this->getT(0).getT(1).getT(1).getT(0).getT(1);               // math::sig2mod<NV>
		auto& peak = this->getT(0).getT(1).getT(1).getT(0).getT(2);                  // delay_impl::peak_t<NV>
		auto& mod_inv = this->getT(0).getT(1).getT(1).getT(0).getT(3);               // math::mod_inv<NV>
		auto& peak1 = this->getT(0).getT(1).getT(1).getT(0).getT(4);                 // delay_impl::peak1_t<NV>
		auto& multi = this->getT(0).getT(1).getT(1).getT(1);                         // delay_impl::multi_t<NV>
		auto& chain = this->getT(0).getT(1).getT(1).getT(1).getT(0);                 // delay_impl::chain_t<NV>
		auto& receive = this->getT(0).getT(1).getT(1).getT(1).getT(0).getT(0);       // routing::receive<NV, mono_frame_cable<NV>>
		auto& fix_delay = this->getT(0).getT(1).getT(1).getT(1).getT(0).getT(1);     // core::fix_delay
		auto& jdelay_cubic = this->getT(0).getT(1).getT(1).getT(1).getT(0).getT(2);  // jdsp::jdelay_cubic<NV>
		auto& one_pole = this->getT(0).getT(1).getT(1).getT(1).getT(0).getT(3);      // delay_impl::one_pole_t<NV>
		auto& send = this->getT(0).getT(1).getT(1).getT(1).getT(0).getT(4);          // routing::send<NV, mono_frame_cable<NV>>
		auto& chain2 = this->getT(0).getT(1).getT(1).getT(1).getT(1);                // delay_impl::chain2_t<NV>
		auto& receive2 = this->getT(0).getT(1).getT(1).getT(1).getT(1).getT(0);      // routing::receive<NV, mono_frame_cable<NV>>
		auto& fix_delay2 = this->getT(0).getT(1).getT(1).getT(1).getT(1).getT(1);    // core::fix_delay
		auto& jdelay_cubic1 = this->getT(0).getT(1).getT(1).getT(1).getT(1).getT(2); // jdsp::jdelay_cubic<NV>
		auto& one_pole2 = this->getT(0).getT(1).getT(1).getT(1).getT(1).getT(3);     // filters::one_pole<NV>
		auto& send2 = this->getT(0).getT(1).getT(1).getT(1).getT(1).getT(4);         // routing::send<NV, mono_frame_cable<NV>>
		auto& wet_gain = this->getT(0).getT(1).getT(2);                              // core::gain<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		dry_wet1.getParameterT(0).connectT(0, dry_wet_mixer); // DryWet -> dry_wet_mixer::Value
		this->getParameterT(0).connectT(0, tempo_sync);       // TimeLeft -> tempo_sync::Tempo
		
		this->getParameterT(1).connectT(0, tempo_sync1); // TimeRight -> tempo_sync1::Tempo
		
		this->getParameterT(2).connectT(0, receive); // FeedbackLeft -> receive::Feedback
		
		this->getParameterT(3).connectT(0, receive2); // FeedbackRight -> receive2::Feedback
		
		auto& FilterType_p = this->getParameterT(4);
		FilterType_p.connectT(0, one_pole);  // FilterType -> one_pole::Mode
		FilterType_p.connectT(1, one_pole2); // FilterType -> one_pole2::Mode
		
		auto& FilterFrequency_p = this->getParameterT(5);
		FilterFrequency_p.connectT(0, one_pole);  // FilterFrequency -> one_pole::Frequency
		FilterFrequency_p.connectT(1, one_pole2); // FilterFrequency -> one_pole2::Frequency
		
		auto& Sync_p = this->getParameterT(6);
		Sync_p.connectT(0, tempo_sync);  // Sync -> tempo_sync::Enabled
		Sync_p.connectT(1, tempo_sync1); // Sync -> tempo_sync1::Enabled
		
		this->getParameterT(7).connectT(0, tempo_sync); // TimeUnsyncedLeft -> tempo_sync::UnsyncedTime
		
		this->getParameterT(8).connectT(0, tempo_sync1); // TimeUnsyncedRight -> tempo_sync1::UnsyncedTime
		
		this->getParameterT(9).connectT(0, dry_wet1); // Mix -> dry_wet1::DryWet
		
		this->getParameterT(10).connectT(0, oscillator); // ModAmount -> oscillator::Gain
		
		this->getParameterT(11).connectT(0, oscillator); // ModFrequency -> oscillator::Frequency
		
		auto& Stereo_p = this->getParameterT(12);
		Stereo_p.connectT(0, cable_expr);  // Stereo -> cable_expr::Value
		Stereo_p.connectT(1, cable_expr1); // Stereo -> cable_expr1::Value
		
		// Modulation Connections ------------------------------------------------------------------
		
		auto& dry_wet_mixer_p = dry_wet_mixer.getWrappedObject().getParameter();
		dry_wet_mixer_p.getParameterT(0).connectT(0, dry_gain);                   // dry_wet_mixer -> dry_gain::Gain
		dry_wet_mixer_p.getParameterT(1).connectT(0, wet_gain);                   // dry_wet_mixer -> wet_gain::Gain
		pma_unscaled.getWrappedObject().getParameter().connectT(0, fix_delay);    // pma_unscaled -> fix_delay::DelayTime
		tempo_sync.getParameter().connectT(0, pma_unscaled);                      // tempo_sync -> pma_unscaled::Value
		cable_expr.getWrappedObject().getParameter().connectT(0, pma_unscaled);   // cable_expr -> pma_unscaled::Add
		pma_unscaled1.getWrappedObject().getParameter().connectT(0, fix_delay2);  // pma_unscaled1 -> fix_delay2::DelayTime
		tempo_sync1.getParameter().connectT(0, pma_unscaled1);                    // tempo_sync1 -> pma_unscaled1::Value
		cable_expr1.getWrappedObject().getParameter().connectT(0, pma_unscaled1); // cable_expr1 -> pma_unscaled1::Add
		peak.getParameter().connectT(0, jdelay_cubic);                            // peak -> jdelay_cubic::DelayTime
		peak1.getParameter().connectT(0, jdelay_cubic1);                          // peak1 -> jdelay_cubic1::DelayTime
		
		// Send Connections ------------------------------------------------------------------------
		
		send.connect(receive);
		send2.connect(receive2);
		
		// Default Values --------------------------------------------------------------------------
		
		; // dry_wet1::DryWet is automated
		
		; // dry_wet_mixer::Value is automated
		
		;                               // dry_gain::Gain is automated
		dry_gain.setParameterT(1, 20.); // core::gain::Smoothing
		dry_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                                // tempo_sync::Tempo is automated
		tempo_sync.setParameterT(1, 1.); // control::tempo_sync::Multiplier
		;                                // tempo_sync::Enabled is automated
		;                                // tempo_sync::UnsyncedTime is automated
		
		; // cable_expr::Value is automated
		
		;                                  // pma_unscaled::Value is automated
		pma_unscaled.setParameterT(1, 1.); // control::pma_unscaled::Multiply
		;                                  // pma_unscaled::Add is automated
		
		;                                 // tempo_sync1::Tempo is automated
		tempo_sync1.setParameterT(1, 1.); // control::tempo_sync::Multiplier
		;                                 // tempo_sync1::Enabled is automated
		;                                 // tempo_sync1::UnsyncedTime is automated
		
		; // cable_expr1::Value is automated
		
		;                                   // pma_unscaled1::Value is automated
		pma_unscaled1.setParameterT(1, 1.); // control::pma_unscaled::Multiply
		;                                   // pma_unscaled1::Add is automated
		
		oscillator.setParameterT(0, 2.); // core::oscillator::Mode
		;                                // oscillator::Frequency is automated
		oscillator.setParameterT(2, 1.); // core::oscillator::FreqRatio
		oscillator.setParameterT(3, 1.); // core::oscillator::Gate
		oscillator.setParameterT(4, 0.); // core::oscillator::Phase
		;                                // oscillator::Gain is automated
		
		sig2mod.setParameterT(0, 0.); // math::sig2mod::Value
		
		mod_inv.setParameterT(0, 0.); // math::mod_inv::Value
		
		; // receive::Feedback is automated
		
		;                                 // fix_delay::DelayTime is automated
		fix_delay.setParameterT(1, 512.); // core::fix_delay::FadeTime
		
		jdelay_cubic.setParameterT(0, 20.); // jdsp::jdelay_cubic::Limit
		;                                   // jdelay_cubic::DelayTime is automated
		
		;                              // one_pole::Frequency is automated
		one_pole.setParameterT(1, 1.); // filters::one_pole::Q
		one_pole.setParameterT(2, 0.); // filters::one_pole::Gain
		one_pole.setParameterT(3, 0.); // filters::one_pole::Smoothing
		;                              // one_pole::Mode is automated
		one_pole.setParameterT(5, 1.); // filters::one_pole::Enabled
		
		; // receive2::Feedback is automated
		
		;                                  // fix_delay2::DelayTime is automated
		fix_delay2.setParameterT(1, 512.); // core::fix_delay::FadeTime
		
		jdelay_cubic1.setParameterT(0, 20.); // jdsp::jdelay_cubic::Limit
		;                                    // jdelay_cubic1::DelayTime is automated
		
		;                               // one_pole2::Frequency is automated
		one_pole2.setParameterT(1, 1.); // filters::one_pole::Q
		one_pole2.setParameterT(2, 0.); // filters::one_pole::Gain
		one_pole2.setParameterT(3, 0.); // filters::one_pole::Smoothing
		;                               // one_pole2::Mode is automated
		one_pole2.setParameterT(5, 1.); // filters::one_pole::Enabled
		
		;                               // wet_gain::Gain is automated
		wet_gain.setParameterT(1, 20.); // core::gain::Smoothing
		wet_gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		this->setParameterT(0, 5.);
		this->setParameterT(1, 6.);
		this->setParameterT(2, 0.495687);
		this->setParameterT(3, 0.50274);
		this->setParameterT(4, 0.);
		this->setParameterT(5, 3928.92);
		this->setParameterT(6, 1.);
		this->setParameterT(7, 314.9);
		this->setParameterT(8, 216.1);
		this->setParameterT(9, 0.498971);
		this->setParameterT(10, 0.04);
		this->setParameterT(11, 0.610425);
		this->setParameterT(12, 0.);
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
	
	static constexpr bool isSuspendedOnSilence() { return true; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(0).getT(1).getT(1).getT(0).getT(0).getT(0).setExternalData(b, index); // delay_impl::oscillator_t<NV>
		this->getT(0).getT(1).getT(1).getT(0).getT(2).setExternalData(b, index);         // delay_impl::peak_t<NV>
		this->getT(0).getT(1).getT(1).getT(0).getT(4).setExternalData(b, index);         // delay_impl::peak1_t<NV>
		this->getT(0).getT(1).getT(1).getT(1).getT(0).getT(3).setExternalData(b, index); // delay_impl::one_pole_t<NV>
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
using delay = wrap::node<delay_impl::instance<NV>>;
}


