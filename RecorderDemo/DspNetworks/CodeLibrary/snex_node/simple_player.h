/** A class that will just play the given buffer without any resampling
    or interpolation.
    
    This is a very simple buffer player that assumes that the samplerate
    of the buffer to play is equal to the audio sample rate.
*/
template <int NV> struct simple_player
{
	SNEX_NODE(simple_player);
	
	// This tells the SNEX compiler to create an audio file slot
	static const int NumAudioFiles = 1;

	void processFrame(span<float, 2>& data)
	{
		// No interpolation necessary, just copy the
		// recorded buffer
		data[0] = stereoData[0][currentIndex];
		data[1] = stereoData[1][currentIndex];

		currentIndex++;
							
		if(currentIndex >= ed.numSamples)
			currentIndex = 0;
	}
	
	template <typename ProcessDataType> void process(ProcessDataType& data)
	{
		// We need to make sure that the audio render thread
		// isn't processing this function while the external data slot 
		// is modified, so we use a DataReadLock with a try read access
		DataReadLock sl(ed, true);
		
		// check if the data lock was grabbed successfully, the playback
		// is enabled and the data slot is not empty
		if(sl.isLocked() && isPlaying && ed.numSamples > 0)
		{
			auto fd = data.toFrameData();
										
			while(fd.next())
				processFrame(fd.toSpan());
				
			// set the display position
			ed.setDisplayedValue((double)currentIndex);
		}
	}
	
	// Use this function to setup the external data
	void setExternalData(ExternalData& d, int index)
	{
		ed = d;
		
		if(ed.numSamples > 0)
		{
			// refer the stereo data object to the audio buffer
			ed.referBlockTo(stereoData[0], 0);		
			ed.referBlockTo(stereoData[1], Math.min(d.numChannels-1, 1));
		}
		else
		{
			// route it to a dummy buffer if the slot is empty
			// (that's not required but otherwise the stereo data
			//  might point to a dangling buffer)
			stereoData[0].referTo(dummy, 8, 0);
			stereoData[1].referTo(dummy, 8, 0);
		}
	}
	
	// Set the parameters here
	template <int P> void setParameter(double v)
	{
		auto shouldBePlaying = v > 0.5;
		
		if(isPlaying != shouldBePlaying)
		{
			isPlaying = shouldBePlaying;
			currentIndex = 0;
		}
	}
	
	// Unused callbacks
	
	// We don't need to use the samplerate because
	// the recorded buffer will always match the samplerate
	void prepare(PrepareSpecs ps) {}
	void handleHiseEvent(HiseEvent& e){}
	void reset() {}
	void processFrame(span<float, 1>& data) {}
	
private:
	
	span<float, 8> dummy = { 0.0f };
	
	ExternalData ed;
		
	// this is an array of two dynamic float buffers that will 
	// hold the recorded data
	span<dyn<float>, 2> stereoData;
	
	int currentIndex = 0;
	int isPlaying = 0;
};
