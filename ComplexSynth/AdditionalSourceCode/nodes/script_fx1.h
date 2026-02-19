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

namespace script_fx1_impl
{
// ==============================| Node & Parameter type declarations |==============================

using extra_mod_t_index = runtime_target::indexers::fix_hash<5000>;
using extra_mod_t_config = modulation::config::constant<false, 
                                                        modulation::TargetMode::Raw>;

template <int NV>
using xfader_multimod = parameter::list<parameter::plain<math::mul<NV>, 0>, 
                                        parameter::plain<math::mul<NV>, 0>>;

template <int NV>
using xfader_t = control::xfader<xfader_multimod<NV>, faders::linear>;
template <int NV>
using extra_mod_t = wrap::mod<parameter::plain<xfader_t<NV>, 0>, 
                              wrap::no_data<core::extra_mod<NV, extra_mod_t_index, extra_mod_t_config>>>;

template <int NV>
using multi_t = container::multi<parameter::empty, 
                                 wrap::fix<1, math::mul<NV>>, 
                                 wrap::fix<1, math::mul<NV>>>;

template <int NV>
using frame2_block_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, extra_mod_t<NV>>, 
                                         xfader_t<NV>, 
                                         multi_t<NV>, 
                                         filters::svf<NV>>;

template <int NV>
using frame2_block_t = wrap::frame<2, frame2_block_t_<NV>>;

template <int NV>
using midichain_t_ = container::chain<parameter::empty, 
                                      wrap::fix<2, frame2_block_t<NV>>>;

template <int NV>
using midichain_t = wrap::event<midichain_t_<NV>>;

namespace script_fx1_t_parameters
{
// Parameter list for script_fx1_impl::script_fx1_t ------------------------------------------------

using Gain = parameter::empty;
using Balance = Gain;
template <int NV>
using Freq = parameter::plain<filters::svf<NV>, 0>;
template <int NV>
using script_fx1_t_plist = parameter::list<Gain, 
                                           Balance, 
                                           Freq<NV>>;
}

template <int NV>
using script_fx1_t_ = container::chain<script_fx1_t_parameters::script_fx1_t_plist<NV>, 
                                       wrap::fix<2, midichain_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public script_fx1_impl::script_fx1_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(script_fx1);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(50)
		{
			0x005C, 0x0000, 0x0000, 0x6147, 0x6E69, 0x0000, 0x0000, 0x0000, 
            0x8000, 0x003F, 0x8000, 0x003F, 0x8000, 0x003F, 0x0000, 0x5C00, 
            0x0104, 0x0000, 0x4200, 0x6C61, 0x6E61, 0x6563, 0x0000, 0xC800, 
            0x00C2, 0xC800, 0x8842, 0x3A03, 0x00C2, 0x8000, 0x003F, 0x0000, 
            0x5C00, 0x0201, 0x0000, 0x4600, 0x6572, 0x0071, 0x0000, 0x41A0, 
            0x4000, 0x469C, 0xBB11, 0x4645, 0x6C1A, 0x3E6B, 0x0000, 0x0000, 
            0x0000, 0x0000
		};
		SNEX_METADATA_ENCODED_MOD_INFO(17)
		{
			0x003A, 0x0100, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000
		};
	};
	
	instance()
	{
		// Node References -------------------------------------------------------------------------
		
		auto& midichain = this->getT(0);                    // script_fx1_impl::midichain_t<NV>
		auto& frame2_block = this->getT(0).getT(0);         // script_fx1_impl::frame2_block_t<NV>
		auto& extra_mod = this->getT(0).getT(0).getT(0);    // script_fx1_impl::extra_mod_t<NV>
		auto& xfader = this->getT(0).getT(0).getT(1);       // script_fx1_impl::xfader_t<NV>
		auto& multi = this->getT(0).getT(0).getT(2);        // script_fx1_impl::multi_t<NV>
		auto& mul = this->getT(0).getT(0).getT(2).getT(0);  // math::mul<NV>
		auto& mul2 = this->getT(0).getT(0).getT(2).getT(1); // math::mul<NV>
		auto& svf = this->getT(0).getT(0).getT(3);          // filters::svf<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		this->getParameterT(2).connectT(0, svf); // Freq -> svf::Frequency
		
		// Modulation Connections ------------------------------------------------------------------
		
		auto& xfader_p = xfader.getWrappedObject().getParameter();
		xfader_p.getParameterT(0).connectT(0, mul);   // xfader -> mul::Value
		xfader_p.getParameterT(1).connectT(0, mul2);  // xfader -> mul2::Value
		extra_mod.getParameter().connectT(0, xfader); // extra_mod -> xfader::Value
		
		// Default Values --------------------------------------------------------------------------
		
		extra_mod.setParameterT(0, 0.); // core::extra_mod::Index
		extra_mod.setParameterT(1, 0.); // core::extra_mod::ProcessSignal
		
		; // xfader::Value is automated
		
		; // mul::Value is automated
		
		; // mul2::Value is automated
		
		;                         // svf::Frequency is automated
		svf.setParameterT(1, 1.); // filters::svf::Q
		svf.setParameterT(2, 0.); // filters::svf::Gain
		svf.setParameterT(3, 0.); // filters::svf::Smoothing
		svf.setParameterT(4, 0.); // filters::svf::Mode
		svf.setParameterT(5, 1.); // filters::svf::Enabled
		
		this->setParameterT(0, 1.);
		this->setParameterT(1, -46.);
		this->setParameterT(2, 12654.8);
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
		
		this->getT(0).getT(0).getT(0).connectToRuntimeTarget(addConnection, c); // script_fx1_impl::extra_mod_t<NV>
	}
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(0).getT(0).getT(0).setExternalData(b, index); // script_fx1_impl::extra_mod_t<NV>
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
using script_fx1 = wrap::node<script_fx1_impl::instance<NV>>;
}


