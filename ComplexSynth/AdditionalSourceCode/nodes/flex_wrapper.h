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

namespace flex_wrapper_impl
{
// ==============================| Node & Parameter type declarations |==============================

using flex_ahdsr_multimod = parameter::list<parameter::empty, 
                                            parameter::plain<envelope::voice_manager, 0>>;

template <int NV>
using flex_ahdsr_t = wrap::data<envelope::flex_ahdsr<NV, flex_ahdsr_multimod>, 
                                data::external::displaybuffer<0>>;

namespace flex_wrapper_t_parameters
{
// Parameter list for flex_wrapper_impl::flex_wrapper_t --------------------------------------------

template <int NV>
using Attack = parameter::plain<flex_wrapper_impl::flex_ahdsr_t<NV>, 
                                0>;
template <int NV>
using Release = parameter::plain<flex_wrapper_impl::flex_ahdsr_t<NV>, 
                                 4>;
template <int NV>
using flex_wrapper_t_plist = parameter::list<Attack<NV>, Release<NV>>;
}

template <int NV>
using flex_wrapper_t_ = container::chain<flex_wrapper_t_parameters::flex_wrapper_t_plist<NV>, 
                                         wrap::fix<1, math::add<NV>>, 
                                         flex_ahdsr_t<NV>, 
                                         envelope::voice_manager>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public flex_wrapper_impl::flex_wrapper_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 1;
		
		SNEX_METADATA_ID(flex_wrapper);
		SNEX_METADATA_NUM_CHANNELS(1);
		SNEX_METADATA_ENCODED_PARAMETERS(34)
		{
			0x025C, 0x0000, 0x0000, 0x7441, 0x6174, 0x6B63, 0x0000, 0x0000, 
            0x0000, 0xEA60, 0x0046, 0x0000, 0xA600, 0x50AF, 0x003E, 0x0000, 
            0x5C00, 0x0102, 0x0000, 0x5200, 0x6C65, 0x6165, 0x6573, 0x0000, 
            0x0000, 0x0000, 0xEA60, 0x3F46, 0x61D8, 0xA642, 0x50AF, 0x003E, 
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
		
		auto& add = this->getT(0);           // math::add<NV>
		auto& flex_ahdsr = this->getT(1);    // flex_wrapper_impl::flex_ahdsr_t<NV>
		auto& voice_manager = this->getT(2); // envelope::voice_manager
		
		// Parameter Connections -------------------------------------------------------------------
		
		this->getParameterT(0).connectT(0, flex_ahdsr); // Attack -> flex_ahdsr::Attack
		
		this->getParameterT(1).connectT(0, flex_ahdsr); // Release -> flex_ahdsr::Release
		
		// Modulation Connections ------------------------------------------------------------------
		
		auto& flex_ahdsr_p = flex_ahdsr.getWrappedObject().getParameter();
		flex_ahdsr_p.getParameterT(1).connectT(0, voice_manager); // flex_ahdsr -> voice_manager::KillVoice
		
		// Default Values --------------------------------------------------------------------------
		
		add.setParameterT(0, 1.); // math::add::Value
		
		;                                        // flex_ahdsr::Attack is automated
		flex_ahdsr.setParameterT(1, 1.7197e-77); // envelope::flex_ahdsr::Hold
		flex_ahdsr.setParameterT(2, 73.2588);    // envelope::flex_ahdsr::Decay
		flex_ahdsr.setParameterT(3, 0.244706);   // envelope::flex_ahdsr::Sustain
		;                                        // flex_ahdsr::Release is automated
		flex_ahdsr.setParameterT(5, 1.);         // envelope::flex_ahdsr::Mode
		flex_ahdsr.setParameterT(6, 0.932);      // envelope::flex_ahdsr::AttackLevel
		flex_ahdsr.setParameterT(7, 0.516);      // envelope::flex_ahdsr::AttackCurve
		flex_ahdsr.setParameterT(8, 0.548);      // envelope::flex_ahdsr::DecayCurve
		flex_ahdsr.setParameterT(9, 0.528);      // envelope::flex_ahdsr::ReleaseCurve
		
		; // voice_manager::KillVoice is automated
		
		this->setParameterT(0, 0.);
		this->setParameterT(1, 56.4612);
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
		
		this->getT(1).setExternalData(b, index); // flex_wrapper_impl::flex_ahdsr_t<NV>
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
using flex_wrapper = wrap::node<flex_wrapper_impl::instance<NV>>;
}


