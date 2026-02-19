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

namespace routing_test_impl
{
// ==============================| Node & Parameter type declarations |==============================

DECLARE_PARAMETER_RANGE(midi_modRange, 
                        10., 
                        250.);

template <int NV>
using midi_mod = parameter::from0To1<jdsp::jdelay<NV>, 
                                     1, 
                                     midi_modRange>;

template <int NV>
using midi_t = wrap::mod<midi_mod<NV>, 
                         control::midi<midi_logic::velocity<NV>>>;

using ahdsr_multimod = parameter::list<parameter::empty, parameter::empty>;

template <int NV>
using ahdsr_t = wrap::no_data<envelope::ahdsr<NV, ahdsr_multimod>>;

using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<2, core::empty>>;
template <int NV>
using stereo_frame_cable = cable::frame<NV, 2>;

template <int NV>
using chain_t = container::chain<parameter::empty, 
                                 wrap::fix<2, routing::receive<NV, stereo_frame_cable<NV>>>, 
                                 jdsp::jdelay<NV>, 
                                 routing::send<NV, stereo_frame_cable<NV>>>;

template <int NV>
using split_t = container::split<parameter::empty, 
                                 wrap::fix<2, chain1_t>, 
                                 chain_t<NV>>;

template <int NV>
using frame2_block_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, split_t<NV>>>;

template <int NV>
using frame2_block_t = wrap::frame<2, frame2_block_t_<NV>>;

template <int NV>
using routing_test_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, midi_t<NV>>, 
                                         ahdsr_t<NV>, 
                                         frame2_block_t<NV>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public routing_test_impl::routing_test_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(routing_test);
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
		// Node References -------------------------------------------------------------------------
		
		auto& midi = this->getT(0);                            // routing_test_impl::midi_t<NV>
		auto& ahdsr = this->getT(1);                           // routing_test_impl::ahdsr_t<NV>
		auto& frame2_block = this->getT(2);                    // routing_test_impl::frame2_block_t<NV>
		auto& split = this->getT(2).getT(0);                   // routing_test_impl::split_t<NV>
		auto& chain1 = this->getT(2).getT(0).getT(0);          // routing_test_impl::chain1_t
		auto& chain = this->getT(2).getT(0).getT(1);           // routing_test_impl::chain_t<NV>
		auto& receive = this->getT(2).getT(0).getT(1).getT(0); // routing::receive<NV, stereo_frame_cable<NV>>
		auto& jdelay = this->getT(2).getT(0).getT(1).getT(1);  // jdsp::jdelay<NV>
		auto& send = this->getT(2).getT(0).getT(1).getT(2);    // routing::send<NV, stereo_frame_cable<NV>>
		
		// Modulation Connections ------------------------------------------------------------------
		
		midi.getParameter().connectT(0, jdelay); // midi -> jdelay::DelayTime
		auto& ahdsr_p = ahdsr.getWrappedObject().getParameter();
		
		// Send Connections ------------------------------------------------------------------------
		
		send.connect(receive);
		
		// Default Values --------------------------------------------------------------------------
		
		ahdsr.setParameterT(0, 0.);   // envelope::ahdsr::Attack
		ahdsr.setParameterT(1, 1.);   // envelope::ahdsr::AttackLevel
		ahdsr.setParameterT(2, 20.);  // envelope::ahdsr::Hold
		ahdsr.setParameterT(3, 40.2); // envelope::ahdsr::Decay
		ahdsr.setParameterT(4, 0.);   // envelope::ahdsr::Sustain
		ahdsr.setParameterT(5, 20.);  // envelope::ahdsr::Release
		ahdsr.setParameterT(6, 0.5);  // envelope::ahdsr::AttackCurve
		ahdsr.setParameterT(7, 0.);   // envelope::ahdsr::Retrigger
		ahdsr.setParameterT(8, 0.);   // envelope::ahdsr::Gate
		
		receive.setParameterT(0, 0.388); // routing::receive::Feedback
		
		jdelay.setParameterT(0, 203.738); // jdsp::jdelay::Limit
		;                                 // jdelay::DelayTime is automated
		
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
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(1).setExternalData(b, index); // routing_test_impl::ahdsr_t<NV>
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
using routing_test = wrap::node<routing_test_impl::instance<NV>>;
}


