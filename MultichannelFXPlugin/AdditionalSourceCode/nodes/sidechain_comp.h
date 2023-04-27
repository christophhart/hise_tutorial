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

namespace sidechain_comp_impl
{
// ============================| Node & Parameter type declarations |============================

using comp_t = wrap::data<dynamics::comp, 
                          data::external::displaybuffer<0>>;

namespace sidechain_comp_t_parameters
{
// Parameter list for sidechain_comp_impl::sidechain_comp_t ------------------------------------

DECLARE_PARAMETER_RANGE_STEP(SidechainRange, 
                             1., 
                             2., 
                             1.);

using Sidechain = parameter::from0To1<sidechain_comp_impl::comp_t, 
                                      4, 
                                      SidechainRange>;

using Threshold = parameter::plain<sidechain_comp_impl::comp_t, 
                                   0>;
using sidechain_comp_t_plist = parameter::list<Threshold, Sidechain>;
}

using sidechain_comp_t_ = container::chain<sidechain_comp_t_parameters::sidechain_comp_t_plist, 
                                           wrap::fix<4, comp_t>>;

// ===============================| Root node initialiser class |===============================

struct instance: public sidechain_comp_impl::sidechain_comp_t_
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 1;
		
		SNEX_METADATA_ID(sidechain_comp);
		SNEX_METADATA_NUM_CHANNELS(4);
		SNEX_METADATA_ENCODED_PARAMETERS(36)
		{
			0x005B, 0x0000, 0x5400, 0x7268, 0x7365, 0x6F68, 0x646C, 0x0000, 
            0xC800, 0x00C2, 0x0000, 0x9800, 0x3999, 0x3EC1, 0xAD83, 0xCD40, 
            0xCCCC, 0x5B3D, 0x0001, 0x0000, 0x6953, 0x6564, 0x6863, 0x6961, 
            0x006E, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x3F80, 0x0000, 0x3F80, 0x0000
		};
	};
	
	instance()
	{
		// Node References ---------------------------------------------------------------------
		
		auto& comp = this->getT(0); // sidechain_comp_impl::comp_t
		
		// Parameter Connections ---------------------------------------------------------------
		
		this->getParameterT(0).connectT(0, comp); // Threshold -> comp::Threshhold
		
		this->getParameterT(1).connectT(0, comp); // Sidechain -> comp::Sidechain
		
		// Default Values ----------------------------------------------------------------------
		
		;                            // comp::Threshhold is automated
		comp.setParameterT(1, 0.);   // dynamics::comp::Attack
		comp.setParameterT(2, 14.8); // dynamics::comp::Release
		comp.setParameterT(3, 32.);  // dynamics::comp::Ratio
		;                            // comp::Sidechain is automated
		
		this->setParameterT(0, -11.);
		this->setParameterT(1, 1.);
		this->setExternalData({}, -1);
	}
	
	static constexpr bool hasTail() { return false; };
	
	static constexpr bool isSuspendedOnSilence() { return true; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections -----------------------------------------------------------
		
		this->getT(0).setExternalData(b, index); // sidechain_comp_impl::comp_t
	}
};
}

#undef getT
#undef connectT
#undef setParameterT
#undef setParameterWT
#undef getParameterT
// ====================================| Public Definition |====================================

namespace project
{
using sidechain_comp = wrap::node<sidechain_comp_impl::instance>;
}


