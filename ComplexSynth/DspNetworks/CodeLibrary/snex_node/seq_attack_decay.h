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
