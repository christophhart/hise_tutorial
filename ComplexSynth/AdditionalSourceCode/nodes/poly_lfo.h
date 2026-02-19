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

namespace poly_lfo_impl
{
// ==============================| Node & Parameter type declarations |==============================

namespace custom
{

struct cable_expr
{
	static double op(double input)
	{
		return input < 0.9 ? 0.0 : 1.0;
	}
};
}

template <int NV>
using ramp_t = wrap::no_data<core::ramp<NV, false>>;
template <int NV>
using tempo_sync_t = wrap::mod<parameter::plain<ramp_t<NV>, 0>, 
                               control::tempo_sync<NV>>;
template <int NV>
using cable_expr_t = control::cable_expr<custom::cable_expr, 
                                         parameter::plain<tempo_sync_t<NV>, 2>>;
namespace custom
{

struct cable_expr1
{
	static double op(double input)
	{
		return input < 1.5 ? 0.0 : 1.0;
	}
};
}

template <int NV>
using pma_unscaled_t = control::pma_unscaled<NV, 
                                             parameter::plain<tempo_sync_t<NV>, 3>>;
template <int NV>
using converter_t = control::converter<parameter::plain<pma_unscaled_t<NV>, 0>, 
                                       conversion_logic::freq2ms>;

template <int NV>
using chain_t = container::chain<parameter::empty, 
                                 wrap::fix<1, converter_t<NV>>, 
                                 pma_unscaled_t<NV>, 
                                 tempo_sync_t<NV>, 
                                 ramp_t<NV>>;
template <int NV>
using clock_ramp_t = wrap::no_data<core::clock_ramp<NV, false>>;

template <int NV>
using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<1, clock_ramp_t<NV>>>;
template <int NV>
using branch_t = container::branch<parameter::empty, 
                                   wrap::fix<1, chain_t<NV>>, 
                                   chain1_t<NV>>;
template <int NV>
using cable_expr1_t = control::cable_expr<custom::cable_expr1, 
                                          parameter::plain<branch_t<NV>, 0>>;

// Calculates the modes: ---------------------------------------------------------------------------

template <int NV>
using chain2_t = container::chain<parameter::empty, 
                                  wrap::fix<1, cable_expr_t<NV>>, 
                                  cable_expr1_t<NV>>;

using chain3_t = container::chain<parameter::empty, 
                                  wrap::fix<1, core::empty>>;

template <int NV>
using chain4_t = container::chain<parameter::empty, 
                                  wrap::fix<1, math::mul<NV>>, 
                                  math::mod_inv<NV>, 
                                  math::abs<NV>, 
                                  math::mod_inv<NV>>;

template <int NV>
using chain5_t = container::chain<parameter::empty, 
                                  wrap::fix<1, math::mod_inv<NV>>>;
template <int NV>
using branch1_t = container::branch<parameter::empty, 
                                    wrap::fix<1, chain3_t>, 
                                    chain4_t<NV>, 
                                    chain5_t<NV>>;

template <int NV>
using chain8_t = container::chain<parameter::empty, 
                                  wrap::fix<1, math::add<NV>>, 
                                  math::fmod<NV>>;
using table_t = wrap::data<math::table, 
                           data::external::table<0>>;

using chain6_t = container::chain<parameter::empty, wrap::fix<1, table_t>>;
namespace custom
{

struct cable_expr2
{
	static double op(double input)
	{
		return input < 0.5 || input > 1.5 ? 1.0 : 0.0;
	}
};
}

template <int NV> struct seq_attack_decay: public data::base
{
	SNEX_NODE(seq_attack_decay);
	// Initialise the processing specs here
	void prepare(PrepareSpecs ps)
	{
		attackLevel.prepare(ps.sampleRate, 20.0);
		sustainLevel.prepare(ps.sampleRate, 20.0);
		curve.prepare(ps.sampleRate, 20.0);
	}
	// Reset the processing pipeline here
	void reset()
	{
		attackLevel.reset();
		sustainLevel.reset();
		curve.reset();
	}
	using IndexType = index::clamped<0, false>;
	// Process the signal here
	template <typename ProcessDataType> void process(ProcessDataType& data)
	{
		double displayIndex = 0.0;
		for(auto& s: data[0])
		{
			float scaled = (s * lengthMultiplier);
			displayIndex = (double)scaled;
			float alpha = Math.fmod(scaled, 1.0f);
			auto a = attackLevel.advance();
			auto sl = sustainLevel.advance();
			auto c = curve.advance();
			idx = (int)scaled;
			float v = sequenceData[idx];
			float upper = sl * v;
			float lower = a * v;
			if(c > 1.02f)
			{
				v = lower + Math.pow(alpha, c) * (upper - lower);
			}
			else if(c < 0.98f)
			{
				c = 1.0f / c;
				alpha = 1.0f - alpha;
				alpha = 1.0f - Math.pow(alpha, c);
				v = lower + alpha * (upper - lower);
			}
			else
			{
				v = lower + alpha * (upper - lower);
			}
			s = v;
		}
		e.setDisplayedValue(displayIndex);
	}
	// Process the signal as frame here
	template <int C> void processFrame(span<float, C>& data)
	{
	}
	// Process the MIDI events here
	void handleHiseEvent(HiseEvent& e)
	{
	}
	block sequenceData;
	float lengthMultiplier = 1.0f;
	IndexType idx;
	ExternalData e;
	// Use this function to setup the external data
	void setExternalData(const ExternalData& d, int index)
	{
		e = d;
		d.referBlockTo(sequenceData, 0);
		lengthMultiplier = (float)sequenceData.size();
	}
	sfloat attackLevel;
	sfloat sustainLevel;
	sfloat curve;
	// Set the parameters here
	template <int P> void setParameter(double v)
	{
		if(P == 0)
			attackLevel.set(Math.range((float)v, 0.0f, 1.0f));
		if(P == 1)
			sustainLevel.set(Math.range((float)v, 0.0f, 1.0f));
		if(P == 2)
			curve.set((float)Math.pow(2.0, (v -0.5) * 8.0));
	}
};

template <int NV>
using snex_node_t = wrap::data<seq_attack_decay<NV>, 
                               data::external::sliderpack<0>>;
template <int NV>
using cable_expr2_t = control::cable_expr<custom::cable_expr2, 
                                          parameter::plain<snex_node_t<NV>, 0>>;
namespace custom
{

struct cable_expr3
{
	static double op(double input)
	{
		return input > 0.5 ? 1.0 : 0.0;
	}
};
}

template <int NV>
using cable_expr3_t = control::cable_expr<custom::cable_expr3, 
                                          parameter::plain<snex_node_t<NV>, 1>>;

template <int NV>
using offline_t_ = container::chain<parameter::empty, 
                                    wrap::fix<1, cable_expr2_t<NV>>, 
                                    cable_expr3_t<NV>>;

template <int NV>
using offline_t = wrap::offline<offline_t_<NV>>;
using pack_resizer_t = wrap::data<control::pack_resizer, 
                                  data::external::sliderpack<0>>;

template <int NV>
using chain7_t = container::chain<parameter::empty, 
                                  wrap::fix<1, offline_t<NV>>, 
                                  pack_resizer_t, 
                                  snex_node_t<NV>>;
template <int NV>
using branch2_t = container::branch<parameter::empty, 
                                    wrap::fix<1, chain6_t>, 
                                    chain7_t<NV>>;

namespace poly_lfo_t_parameters
{
// Parameter list for poly_lfo_impl::poly_lfo_t ----------------------------------------------------

template <int NV>
using Tempo = parameter::chain<ranges::Identity, 
                               parameter::plain<poly_lfo_impl::tempo_sync_t<NV>, 0>, 
                               parameter::plain<poly_lfo_impl::clock_ramp_t<NV>, 0>>;

DECLARE_PARAMETER_RANGE_STEP(Multiplier_InputRange, 
                             1., 
                             16., 
                             1.);
DECLARE_PARAMETER_RANGE_STEP(Multiplier_0Range, 
                             1., 
                             16., 
                             1.);

template <int NV>
using Multiplier_0 = parameter::from0To1<poly_lfo_impl::tempo_sync_t<NV>, 
                                         1, 
                                         Multiplier_0Range>;

template <int NV>
using Multiplier_1 = parameter::from0To1<poly_lfo_impl::clock_ramp_t<NV>, 
                                         1, 
                                         Multiplier_0Range>;

DECLARE_PARAMETER_RANGE_SKEW(Multiplier_2Range, 
                             1., 
                             16., 
                             0.909475);

template <int NV>
using Multiplier_2 = parameter::from0To1<poly_lfo_impl::pma_unscaled_t<NV>, 
                                         1, 
                                         Multiplier_2Range>;

template <int NV>
using Multiplier = parameter::chain<Multiplier_InputRange, 
                                    Multiplier_0<NV>, 
                                    Multiplier_1<NV>, 
                                    Multiplier_2<NV>>;

template <int NV>
using Mode = parameter::chain<ranges::Identity, 
                              parameter::plain<poly_lfo_impl::cable_expr_t<NV>, 0>, 
                              parameter::plain<poly_lfo_impl::cable_expr1_t<NV>, 0>>;

template <int NV>
using SeqMode = parameter::chain<ranges::Identity, 
                                 parameter::plain<poly_lfo_impl::cable_expr2_t<NV>, 0>, 
                                 parameter::plain<poly_lfo_impl::cable_expr3_t<NV>, 0>>;

template <int NV>
using Frequency = parameter::plain<poly_lfo_impl::converter_t<NV>, 
                                   0>;
template <int NV>
using Direction = parameter::plain<poly_lfo_impl::branch1_t<NV>, 
                                   0>;
template <int NV>
using Phase = parameter::plain<math::add<NV>, 0>;
template <int NV>
using Sequencer = parameter::plain<poly_lfo_impl::branch2_t<NV>, 
                                   0>;
using NumSteps = parameter::plain<poly_lfo_impl::pack_resizer_t, 
                                  0>;
template <int NV>
using SeqCurve = parameter::plain<poly_lfo_impl::snex_node_t<NV>, 
                                  2>;
template <int NV>
using poly_lfo_t_plist = parameter::list<Tempo<NV>, 
                                         Frequency<NV>, 
                                         Multiplier<NV>, 
                                         Mode<NV>, 
                                         Direction<NV>, 
                                         Phase<NV>, 
                                         Sequencer<NV>, 
                                         NumSteps, 
                                         SeqMode<NV>, 
                                         SeqCurve<NV>>;
}

template <int NV>
using poly_lfo_t_ = container::chain<poly_lfo_t_parameters::poly_lfo_t_plist<NV>, 
                                     wrap::fix<1, chain2_t<NV>>, 
                                     branch_t<NV>, 
                                     branch1_t<NV>, 
                                     chain8_t<NV>, 
                                     branch2_t<NV>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public poly_lfo_impl::poly_lfo_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 1;
		static const int NumSliderPacks = 1;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(poly_lfo);
		SNEX_METADATA_NUM_CHANNELS(1);
		SNEX_METADATA_ENCODED_PARAMETERS(174)
		{
			0x035C, 0x0000, 0x0000, 0x6554, 0x706D, 0x006F, 0x0000, 0x0000, 
            0x0000, 0x4190, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 
            0x015C, 0x0001, 0x0000, 0x7246, 0x7165, 0x6575, 0x636E, 0x0079, 
            0xCCCD, 0x3DCC, 0x0000, 0x42C8, 0xB9DF, 0x3EAF, 0x6A72, 0x3E4A, 
            0x0000, 0x0000, 0x005C, 0x0002, 0x0000, 0x754D, 0x746C, 0x7069, 
            0x696C, 0x7265, 0x0000, 0x8000, 0x003F, 0x8000, 0x0041, 0x8000, 
            0x0040, 0x8000, 0x003F, 0x8000, 0x5C3F, 0x0300, 0x0000, 0x4D00, 
            0x646F, 0x0065, 0x0000, 0x0000, 0x0000, 0x4000, 0x0000, 0x4000, 
            0x0000, 0x3F80, 0x0000, 0x3F80, 0x005C, 0x0004, 0x0000, 0x6944, 
            0x6572, 0x7463, 0x6F69, 0x006E, 0x0000, 0x0000, 0x0000, 0x4000, 
            0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x005C, 0x0005, 
            0x0000, 0x6850, 0x7361, 0x0065, 0x0000, 0x0000, 0x0000, 0x3F80, 
            0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x0000, 0x005C, 0x0006, 
            0x0000, 0x6553, 0x7571, 0x6E65, 0x6563, 0x0072, 0x0000, 0x0000, 
            0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 
            0x005C, 0x0007, 0x0000, 0x754E, 0x536D, 0x6574, 0x7370, 0x0000, 
            0x0000, 0x0040, 0x0000, 0x0042, 0x0000, 0x0042, 0x8000, 0x003F, 
            0x8000, 0x5C3F, 0x0800, 0x0000, 0x5300, 0x7165, 0x6F4D, 0x6564, 
            0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x8000, 0x003F, 0x8000, 
            0x003F, 0x8000, 0x5C3F, 0x0900, 0x0000, 0x5300, 0x7165, 0x7543, 
            0x7672, 0x0065, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 
            0x0000, 0x3F80, 0x0000, 0x0000, 0x0000, 0x0000
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
		
		auto& chain2 = this->getT(0);                              // poly_lfo_impl::chain2_t<NV>
		auto& cable_expr = this->getT(0).getT(0);                  // poly_lfo_impl::cable_expr_t<NV>
		auto& cable_expr1 = this->getT(0).getT(1);                 // poly_lfo_impl::cable_expr1_t<NV>
		auto& branch = this->getT(1);                              // poly_lfo_impl::branch_t<NV>
		auto& chain = this->getT(1).getT(0);                       // poly_lfo_impl::chain_t<NV>
		auto& converter = this->getT(1).getT(0).getT(0);           // poly_lfo_impl::converter_t<NV>
		auto& pma_unscaled = this->getT(1).getT(0).getT(1);        // poly_lfo_impl::pma_unscaled_t<NV>
		auto& tempo_sync = this->getT(1).getT(0).getT(2);          // poly_lfo_impl::tempo_sync_t<NV>
		auto& ramp = this->getT(1).getT(0).getT(3);                // poly_lfo_impl::ramp_t<NV>
		auto& chain1 = this->getT(1).getT(1);                      // poly_lfo_impl::chain1_t<NV>
		auto& clock_ramp = this->getT(1).getT(1).getT(0);          // poly_lfo_impl::clock_ramp_t<NV>
		auto& branch1 = this->getT(2);                             // poly_lfo_impl::branch1_t<NV>
		auto& chain3 = this->getT(2).getT(0);                      // poly_lfo_impl::chain3_t
		auto& chain4 = this->getT(2).getT(1);                      // poly_lfo_impl::chain4_t<NV>
		auto& mul = this->getT(2).getT(1).getT(0);                 // math::mul<NV>
		auto& mod_inv = this->getT(2).getT(1).getT(1);             // math::mod_inv<NV>
		auto& abs = this->getT(2).getT(1).getT(2);                 // math::abs<NV>
		auto& mod_inv1 = this->getT(2).getT(1).getT(3);            // math::mod_inv<NV>
		auto& chain5 = this->getT(2).getT(2);                      // poly_lfo_impl::chain5_t<NV>
		auto& mod_inv2 = this->getT(2).getT(2).getT(0);            // math::mod_inv<NV>
		auto& chain8 = this->getT(3);                              // poly_lfo_impl::chain8_t<NV>
		auto& add = this->getT(3).getT(0);                         // math::add<NV>
		auto& fmod = this->getT(3).getT(1);                        // math::fmod<NV>
		auto& branch2 = this->getT(4);                             // poly_lfo_impl::branch2_t<NV>
		auto& chain6 = this->getT(4).getT(0);                      // poly_lfo_impl::chain6_t
		auto& table = this->getT(4).getT(0).getT(0);               // poly_lfo_impl::table_t
		auto& chain7 = this->getT(4).getT(1);                      // poly_lfo_impl::chain7_t<NV>
		auto& offline = this->getT(4).getT(1).getT(0);             // poly_lfo_impl::offline_t<NV>
		auto& cable_expr2 = this->getT(4).getT(1).getT(0).getT(0); // poly_lfo_impl::cable_expr2_t<NV>
		auto& cable_expr3 = this->getT(4).getT(1).getT(0).getT(1); // poly_lfo_impl::cable_expr3_t<NV>
		auto& pack_resizer = this->getT(4).getT(1).getT(1);        // poly_lfo_impl::pack_resizer_t
		auto& snex_node = this->getT(4).getT(1).getT(2);           // poly_lfo_impl::snex_node_t<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		auto& Tempo_p = this->getParameterT(0);
		Tempo_p.connectT(0, tempo_sync); // Tempo -> tempo_sync::Tempo
		Tempo_p.connectT(1, clock_ramp); // Tempo -> clock_ramp::Tempo
		
		this->getParameterT(1).connectT(0, converter); // Frequency -> converter::Value
		
		auto& Multiplier_p = this->getParameterT(2);
		Multiplier_p.connectT(0, tempo_sync);   // Multiplier -> tempo_sync::Multiplier
		Multiplier_p.connectT(1, clock_ramp);   // Multiplier -> clock_ramp::Multiplier
		Multiplier_p.connectT(2, pma_unscaled); // Multiplier -> pma_unscaled::Multiply
		
		auto& Mode_p = this->getParameterT(3);
		Mode_p.connectT(0, cable_expr);  // Mode -> cable_expr::Value
		Mode_p.connectT(1, cable_expr1); // Mode -> cable_expr1::Value
		
		this->getParameterT(4).connectT(0, branch1); // Direction -> branch1::Index
		
		this->getParameterT(5).connectT(0, add); // Phase -> add::Value
		
		this->getParameterT(6).connectT(0, branch2); // Sequencer -> branch2::Index
		
		this->getParameterT(7).connectT(0, pack_resizer); // NumSteps -> pack_resizer::NumSliders
		
		auto& SeqMode_p = this->getParameterT(8);
		SeqMode_p.connectT(0, cable_expr2); // SeqMode -> cable_expr2::Value
		SeqMode_p.connectT(1, cable_expr3); // SeqMode -> cable_expr3::Value
		
		this->getParameterT(9).connectT(0, snex_node); // SeqCurve -> snex_node::Curve
		
		// Modulation Connections ------------------------------------------------------------------
		
		tempo_sync.getParameter().connectT(0, ramp);                            // tempo_sync -> ramp::PeriodTime
		cable_expr.getWrappedObject().getParameter().connectT(0, tempo_sync);   // cable_expr -> tempo_sync::Enabled
		pma_unscaled.getWrappedObject().getParameter().connectT(0, tempo_sync); // pma_unscaled -> tempo_sync::UnsyncedTime
		converter.getWrappedObject().getParameter().connectT(0, pma_unscaled);  // converter -> pma_unscaled::Value
		cable_expr1.getWrappedObject().getParameter().connectT(0, branch);      // cable_expr1 -> branch::Index
		cable_expr2.getWrappedObject().getParameter().connectT(0, snex_node);   // cable_expr2 -> snex_node::AttackLevel
		cable_expr3.getWrappedObject().getParameter().connectT(0, snex_node);   // cable_expr3 -> snex_node::DecayLevel
		
		// Default Values --------------------------------------------------------------------------
		
		; // cable_expr::Value is automated
		
		; // cable_expr1::Value is automated
		
		; // branch::Index is automated
		
		; // converter::Value is automated
		
		;                                  // pma_unscaled::Value is automated
		;                                  // pma_unscaled::Multiply is automated
		pma_unscaled.setParameterT(2, 0.); // control::pma_unscaled::Add
		
		; // tempo_sync::Tempo is automated
		; // tempo_sync::Multiplier is automated
		; // tempo_sync::Enabled is automated
		; // tempo_sync::UnsyncedTime is automated
		
		;                          // ramp::PeriodTime is automated
		ramp.setParameterT(1, 0.); // core::ramp::LoopStart
		ramp.setParameterT(2, 1.); // core::ramp::Gate
		
		;                                // clock_ramp::Tempo is automated
		;                                // clock_ramp::Multiplier is automated
		clock_ramp.setParameterT(2, 1.); // core::clock_ramp::AddToSignal
		clock_ramp.setParameterT(3, 1.); // core::clock_ramp::UpdateMode
		clock_ramp.setParameterT(4, 0.); // core::clock_ramp::Inactive
		
		; // branch1::Index is automated
		
		mul.setParameterT(0, 2.); // math::mul::Value
		
		mod_inv.setParameterT(0, 0.); // math::mod_inv::Value
		
		abs.setParameterT(0, 0.); // math::abs::Value
		
		mod_inv1.setParameterT(0, 0.); // math::mod_inv::Value
		
		mod_inv2.setParameterT(0, 0.); // math::mod_inv::Value
		
		; // add::Value is automated
		
		fmod.setParameterT(0, 1.); // math::fmod::Value
		
		; // branch2::Index is automated
		
		; // cable_expr2::Value is automated
		
		; // cable_expr3::Value is automated
		
		; // pack_resizer::NumSliders is automated
		
		; // snex_node::AttackLevel is automated
		; // snex_node::DecayLevel is automated
		; // snex_node::Curve is automated
		
		this->setParameterT(0, 0.);
		this->setParameterT(1, 0.343215);
		this->setParameterT(2, 4.);
		this->setParameterT(3, 2.);
		this->setParameterT(4, 0.);
		this->setParameterT(5, 0.);
		this->setParameterT(6, 1.);
		this->setParameterT(7, 32.);
		this->setParameterT(8, 1.);
		this->setParameterT(9, 1.);
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
		
		this->getT(1).getT(0).getT(3).setExternalData(b, index); // poly_lfo_impl::ramp_t<NV>
		this->getT(1).getT(1).getT(0).setExternalData(b, index); // poly_lfo_impl::clock_ramp_t<NV>
		this->getT(4).getT(0).getT(0).setExternalData(b, index); // poly_lfo_impl::table_t
		this->getT(4).getT(1).getT(1).setExternalData(b, index); // poly_lfo_impl::pack_resizer_t
		this->getT(4).getT(1).getT(2).setExternalData(b, index); // poly_lfo_impl::snex_node_t<NV>
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
using poly_lfo = wrap::node<poly_lfo_impl::instance<NV>>;
}


