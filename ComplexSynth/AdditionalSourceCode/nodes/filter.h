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

namespace filter_impl
{
// ===================| Node & Parameter type declarations |===================

namespace filter_t_parameters
{
}

template <int NV>
using filter_t_ = container::chain<parameter::plain<filters::svf<NV>, 0>, 
                                   wrap::fix<2, filters::svf<NV>>>;

// =======================| Root node initialiser class |=======================

template <int NV> struct instance: public filter_impl::filter_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(filter);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(20)
		{
			0x015C, 0x0000, 0x0000, 0x7246, 0x7165, 0x6575, 0x636E, 0x0079, 
            0x0000, 0x41A0, 0x4000, 0x469C, 0xF9C6, 0x4435, 0x6C1A, 0x3E6B, 
            0x0000, 0x0000, 0x0000, 0x0000
		};
		SNEX_METADATA_ENCODED_MOD_INFO(18)
		{
			0x013A, 0x0120, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
            0x0000, 0x0000
		};
	};
	
	instance()
	{
		// Node References ----------------------------------------------------
		
		auto& svf = this->getT(0); // filters::svf<NV>
		
		// Parameter Connections ----------------------------------------------
		
		this->getParameterT(0).connectT(0, svf); // Frequency -> svf::Frequency
		
		// Default Values -----------------------------------------------------
		
		;                         // svf::Frequency is automated
		svf.setParameterT(1, 1.); // filters::svf::Q
		svf.setParameterT(2, 0.); // filters::svf::Gain
		svf.setParameterT(3, 0.); // filters::svf::Smoothing
		svf.setParameterT(4, 0.); // filters::svf::Mode
		svf.setParameterT(5, 1.); // filters::svf::Enabled
		
		this->setParameterT(0, 727.903);
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
// ============================| Public Definition |============================

namespace project
{
// polyphonic template declaration

template <int NV>
using filter = wrap::node<filter_impl::instance<NV>>;
}


