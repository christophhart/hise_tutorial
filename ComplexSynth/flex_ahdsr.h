// ==================================| Third Party Node Template |==================================

#pragma once
#include <JuceHeader.h>

#include "hi_tools/hi_standalone_components/RingBuffer.h"

namespace project
{
using namespace juce;
using namespace hise;
using namespace scriptnode;

// ==========================| The node class with all required callbacks |==========================

// Use this enum to refer to the cables, eg. this->setGlobalCableValue<GlobalCables::ahdsr_flex_ui>(0.4)
enum class GlobalCables
{
	ahdsr_flex_ui = 0
};
// Subclass your node from this
using cable_manager_t = routing::global_cable_cpp_manager<SN_GLOBAL_CABLE(-889478249)>;


template <int NV> struct flex_ahdsr: public data::base,
								     public cable_manager_t,
									 public Thread

{
	// Metadata Definitions ------------------------------------------------------------------------
	
	SNEX_NODE(flex_ahdsr);
	
	struct MetadataClass
	{
		SN_NODE_ID("flex_ahdsr");
	};
	
	// set to true if you want this node to have a modulation dragger
	static constexpr bool isModNode() { return false; };
	static constexpr bool isPolyphonic() { return true; };
	// set to true if your node produces a tail
	static constexpr bool hasTail() { return true; };
	// set to true if your doesn't generate sound from silence and can be suspended when the input signal is silent
	static constexpr bool isSuspendedOnSilence() { return false; };
	// Undefine this method if you want a dynamic channel count
	static constexpr int getFixChannelAmount() { return 1; };

	static constexpr bool isProcessingHiseEvent() { return true; }
	
	// Define the amount and types of external data slots you want to use
	static constexpr int NumTables = 0;
	static constexpr int NumSliderPacks = 0;
	static constexpr int NumAudioFiles = 0;
	static constexpr int NumFilters = 0;
	static constexpr int NumDisplayBuffers = 0;

	// Scriptnode Callbacks ------------------------------------------------------------------------
	flex_ahdsr():
	  Thread("AHDSR updater")
	{
		startThread(5);
	}

	void run()
	{
		while(!threadShouldExit())
		{
			if(changed)
			{
				auto& activeVoice = state.getWithIndex(lastStartedVoiceIndex);

				JSONObject obj;
				obj[Identifier("Attack")] = activeVoice.getUIValue(State::ATTACK, ParameterType::Time);
				obj[Identifier("Hold")] = activeVoice.getUIValue(State::HOLD, ParameterType::Time);
				obj[Identifier("Decay")] = activeVoice.getUIValue(State::DECAY, ParameterType::Time);
				obj[Identifier("Sustain")] = activeVoice.getUIValue(State::SUSTAIN, ParameterType::Level);
				obj[Identifier("Release")] = activeVoice.getUIValue(State::RELEASE, ParameterType::Time);
				obj[Identifier("AttackLevel")] = activeVoice.getUIValue(State::ATTACK, ParameterType::Level);
				obj[Identifier("AttackCurve")] = activeVoice.getUIValue(State::ATTACK, ParameterType::Curve);
				obj[Identifier("DecayCurve")] = activeVoice.getUIValue(State::DECAY, ParameterType::Curve);
				obj[Identifier("ReleaseCurve")] = activeVoice.getUIValue(State::RELEASE, ParameterType::Curve);
				obj[Identifier("CurrentState")] = var((int)activeVoice.s);
				obj[Identifier("Position")] = var(activeVoice.counter / activeVoice.thisTime);

				this->sendDataToGlobalCable<0>(obj);
				changed = false;
			}
			
			Thread::wait(30);
		}
	}

	void prepare(PrepareSpecs specs)
	{
		sr = specs.sampleRate;
		polyHandler = specs.voiceIndex;
		state.prepare(specs);

		for(auto& s: state)
			s.prepare(specs.sampleRate);
	}

	PolyHandler* polyHandler = nullptr;

	void reset()
	{
		for(auto& s: state)
			s.reset();
	}
	
	void handleHiseEvent(HiseEvent& e)
	{
		if(e.isNoteOnOrOff())
		{
			if(e.isNoteOn())
				lastStartedVoiceIndex = polyHandler->getVoiceIndex();

			for(auto& s: state)
				s.gate(e.isNoteOn());
		}
	}

	template <typename T> void process(T& data)
	{
		auto& s = state.get();

		bool stopVoice = false;

		for(int i = 0; i < data.getNumSamples(); i++)
			data[0][i] = s.calculateNewValue(stopVoice);

		if(stopVoice)
			polyHandler->sendVoiceResetMessage(false);

		changed |= polyHandler->getVoiceIndex() == lastStartedVoiceIndex;
	}
	
	template <typename T> void processFrame(T& data)
	{
		bool stopVoice = false;
		data[0] = state.get().calculateNewValue(stopVoice);

		if(stopVoice)
			polyHandler->sendVoiceResetMessage(false);
	}

	SN_EMPTY_MOD;
	SN_EMPTY_SET_EXTERNAL_DATA;
	
	// Parameter Functions -------------------------------------------------------------------------

	
	struct PolyState
	{
		PolyState()
		{
			memset(data.data(), 0, sizeof(data));

			for(auto s: data)
				s.curve.set(1.0f);
		}
		template<State S, ParameterType T> void set(float newValue)
		{
			auto& v = data[(int)S];

			switch(T)
			{
			case ParameterType::Time:
				v.time = newValue;
				break;
			case ParameterType::Level:
			{
				if(S == State::ATTACK || S == State::HOLD)
				{
					originalAttackLevel = newValue;
					newValue = jmax(data[(int)State::SUSTAIN].level.targetValue, newValue);
				}
				else if(S == State::SUSTAIN)
				{
					auto atk = jmax(originalAttackLevel, newValue);

					data[(int)State::ATTACK].level.set(atk);
					data[(int)State::HOLD].level.set(atk);
				}

				v.level.set(newValue);

				break;	
			}
			
			case ParameterType::Curve:
				v.curve.set(newValue);
				break;
			}
		}

		var getUIValue(State S, ParameterType T) const
		{
			const auto& v = data[(int)S];

			switch(T)
			{
			case ParameterType::Level:
				return var(v.level.targetValue);
			case ParameterType::Curve:


				return var(std::log2(v.curve.targetValue)/8.0 + 0.5);
			case ParameterType::Time:
				return var(v.time);
			default:
				jassertfalse;
				return var();
			}
		}

		void gate(bool on)
		{
			gateActive = on;

			if(on)
			{
				bool unused = false;

				s = State::IDLE;
				prevValue = 0.0f;
				bump(unused);
			}
			else if(s < State::RELEASE)
			{
				s = State::RELEASE;
				counter = 0.0f;
				thisTime = get<ParameterType::Time>();
				prevLevel = prevValue;
			}
		}

		void reset()
		{
			prevLevel = 0.0f;
			prevValue = 0.0f;
			counter = 0.0;

			for(auto& s: data)
				s.level.reset();

			s = State::IDLE;
		}

		template <ParameterType T> float get()
		{
			auto& v = data[(int)s];

			switch(T)
			{
			case ParameterType::Time:
				return v.time;
			case ParameterType::Level:
				return v.level.advance();
			case ParameterType::Curve:
				return v.curve.advance();
			default:
				jassertfalse;
				return 0.0f;
			}
		}

		void prepare(double sampleRate)
		{
			for(int i = 0; i < data.size(); i++)
			{
				data[i].prepare(sampleRate, 20.0);
			}
		}

		bool isStaticState()
		{
			return s == State::HOLD || s == State::SUSTAIN;
		}

		float calculateNewValue(bool& stopVoice)
		{
			if(s != State::IDLE && s != State::DONE && thisTime > 0.0f)
			{
				auto value = jlimit(0.0f, 1.0f, counter / thisTime);

				auto thisLevel = get<ParameterType::Level>();

				if(isStaticState())
				{
					value = thisLevel;
				}
				else
				{
					auto c = get<ParameterType::Curve>();

					if(c > 1.0)
					{
						auto expValue = std::pow(value, c);
						value = Interpolator::interpolateLinear(prevLevel, thisLevel, expValue);
					}
					else if (c < 1.0)
					{
						c = 1.0 / c;
						value = 1.0f - value;
						auto expValue = 1.0f - std::pow(value, c);
						value = Interpolator::interpolateLinear(prevLevel, thisLevel, expValue);
					}
				}

				prevValue = value;

				if(s == State::SUSTAIN)
				{
					if(m != Mode::Note)
						bump(stopVoice);

				}
				else if (++counter > thisTime)
					bump(stopVoice);

				return value;
			}

			return 0.0f;
		}

		void bump(bool& stopVoice)
		{
			prevLevel = prevValue;

			while(true)
			{
				s = (State)((int)s + 1);

				if(s == State::DONE)
				{
					if(m == Mode::Loop && gateActive)
					{
						s = State::IDLE;
						bump(stopVoice);
						return;
					}

					stopVoice = true;
					break;
				}

				if(auto tt = get<ParameterType::Time>())
				{
					thisTime = tt;
					break;
				}
				else
				{
					data[(int)s].curve.reset();
					data[(int)s].level.reset();
					prevLevel = get<ParameterType::Level>();
				}

				if(s == State::SUSTAIN && m == Mode::Note)
					break;
			}

			counter = 0.0f;

		}

		struct Values
		{
			void prepare(double sr, double time)
			{
				level.prepare(sr, time);
				curve.prepare(sr, time);
			}

			sfloat curve;
			float time = 0.0f;
			sfloat level;
		};

		Mode m = Mode::Note;
		State s = State::IDLE;
		std::array<Values, (int)State::DONE> data;

		float counter = 0.0f;
		float thisTime = 0.0f;
		float prevValue = 0.0f;
		float prevLevel = 0.0f;

		float originalAttackLevel = 0.0f;

		bool gateActive = false;

	};

	double sr = 44100.0;
	PolyData<PolyState, NV> state;


	// x in [0, 1], curve in [0, 1]
	inline float shapeCurve(float x, float curve) {
	    // Map curve to exponent: 0 → very exponential, 0.5 → linear, 1 → inverse exponential
	    
	    return std::pow(x, exponent);
	}

	template<ParameterType T> double convert(double value)
	{
		switch(T)
		{
		case ParameterType::Level:
			return value;
		case ParameterType::Time:
			return value * 0.001 * sr;
		case ParameterType::Curve:
			return std::pow(2.0, (value -0.5) * 8.0);
		}
	}

	template <State S, ParameterType T> void setState(double newValue)
	{
		float v = convert<T>((float)newValue);

		for(auto& s: state)
			s.template set<S, T>(v);
	}

	void setMode(double newValue)
	{
		for(auto& s: state)
			s.m = (Mode)(int)newValue;
	}

	template <int P> void setParameter(double v)
	{
		changed = true;

		if (P == (int)SpecialParameters::Attack)
			setState<State::ATTACK, ParameterType::Time>(v);
		if(P == (int)SpecialParameters::Hold)
			setState<State::HOLD, ParameterType::Time>(v);
		if(P == (int)SpecialParameters::Decay)
			setState<State::DECAY, ParameterType::Time>(v);
		if(P == (int)SpecialParameters::Sustain)
		{
			setState<State::DECAY, ParameterType::Level>(v);
			setState<State::SUSTAIN, ParameterType::Level>(v);
		}
			
		if(P == (int)SpecialParameters::Release)
			setState<State::RELEASE, ParameterType::Time>(v);

		if(P == (int)SpecialParameters::Mode)
			setMode(v);

		if(P == (int)SpecialParameters::AttackLevel)
		{
			setState<State::ATTACK, ParameterType::Level>(v);
			setState<State::HOLD, ParameterType::Level>(v);
		}

		if(P == (int)SpecialParameters::AttackCurve)
			setState<State::ATTACK, ParameterType::Curve>(v);
		if(P == (int)SpecialParameters::DecayCurve)
			setState<State::DECAY, ParameterType::Curve>(v);
		if(P == (int)SpecialParameters::ReleaseCurve)
			setState<State::RELEASE, ParameterType::Curve>(v);
	}

	int lastStartedVoiceIndex = 0;
	bool changed = false;


	void createParameters(ParameterDataList& data)
	{
		{
			parameter::data p("Attack", { 0.0, 30000.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::Time;
			p.setSkewForCentre(1000.0);
			registerCallback<0>(p);
			p.setDefaultValue(5.0);
			data.add(std::move(p));
		}
		{
			parameter::data p("Hold", { 0.0, 30000.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::Time;
			p.setSkewForCentre(1000.0);
			registerCallback<1>(p);
			p.setDefaultValue(0.0);
			data.add(std::move(p));
		}
		{
			parameter::data p("Decay", { 0.0, 30000.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::Time;
			p.setSkewForCentre(100.0);
			registerCallback<2>(p);
			p.setDefaultValue(1000.0);
			data.add(std::move(p));
		}
		{
			parameter::data p("Sustain", { 0.0, 1.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::NormalizedPercentage;
			registerCallback<3>(p);
			p.setDefaultValue(0.5);
			data.add(std::move(p));
		}
		{
			parameter::data p("Release", { 0.0, 30000.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::Time;
			p.setSkewForCentre(1000.0);
			registerCallback<4>(p);
			p.setDefaultValue(5.0);
			data.add(std::move(p));
		}
		{
			parameter::data p("Mode", { 0.0, 2.0 });
			p.setParameterValueNames({ "Trigger", "Note", "Loop"});
			registerCallback<5>(p);
			p.setDefaultValue(1.0);
			data.add(std::move(p));
		}
		{
			parameter::data p("AttackLevel", { 0.0, 1.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::NormalizedPercentage;
			registerCallback<6>(p);
			p.setDefaultValue(1.0);
			data.add(std::move(p));
		}
		{
			parameter::data p("AttackCurve", { 0.0, 1.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::NormalizedPercentage;
			registerCallback<7>(p);
			p.setDefaultValue(0.5);
			data.add(std::move(p));
		}
		{
			parameter::data p("DecayCurve", { 0.0, 1.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::NormalizedPercentage;
			registerCallback<8>(p);
			p.setDefaultValue(0.5);
			data.add(std::move(p));
		}
		{
			parameter::data p("ReleaseCurve", { 0.0, 1.0 });
			p.info.textConverter = parameter::pod::TextValueConverters::NormalizedPercentage;
			registerCallback<9>(p);
			p.setDefaultValue(0.5);
			data.add(std::move(p));
		}
	}
};
}


