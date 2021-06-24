/** Pitch Shifter SNEX version

	This is the coded version of the pitch shifting algorithm

	It's more or less the same signal path as the scriptnode patch
	but it allows a more fine grained control over a few parameters
	(eg. the length of the delay line and various crossfade versions).
*/

// Change this to 1 in order to write the oscillator's
// output to the audio stream.
#define DEBUG_OSCILLATORS 0

// Change this to use another square-wave algorithm
#define SATURATED_TRIANGLE 0

// This class will contain a few compile-time constants
// that define the processing specifications...
struct config
{
	// We're operating with stereo signals. If you want 
	// to use this in a mono context, set it to 1.
	// (the processing will be bypassed if the channels
	// do not match the current channel amount)...
	static const int NumChannels = 2;

	// This constant defines the channel amount that is not used
	// (so if NumChannels is 2, it will be 1 and vice versa)...
	static const int UnusedChannels = 3 - NumChannels;

	// The length of the delay line modulation in ms
	// You can experiment with different values (anything between
	// 30ms and 200ms might work)
	static const int DelayLength = 80;
	
	// The fixed upper limit for the delay buffers
	static const int MaxSize = 8192;
	
	static const int XFadeTime = 3;
};

namespace types
{
// This defines a stereo frame (two float numbers)
using Frame = span<float, config::NumChannels>;

// This is the delay buffer. The data layout is interleaved
// so that we can use the arithmetic operators on the frames
using BufferType = span<Frame, config::MaxSize>;

// This is a index type that wraps around the max size
// (if MaxSize is a power of two it can use logical operators
// instead of the slow modulo function)
using WriteIndex = index::wrapped<config::MaxSize>;

// This is an index type that performs a linear interpolation
// when you use it in an []-operator
using ReadIndex  = index::lerp<index::unscaled<double, 
							   WriteIndex>>;

}


/** This namespace contains the building blocks that we need.
In scriptnode we can use the ready made nodes, but here we have
to assemble them ourselves
*/
namespace tools
{
	
/** A varying delay line with linear interpolation. */
struct DelayLine
{
	// This function is called whenever the samplerate changes
	// or the delay time is changed.
	void updateIndexes()
	{
		delaySamples = delayTimeMs * sampleRate * 0.001;
	}
	
	// Changes the delay time. The function signature is
	// following the SNEX parameter prototype with a 
	// templated integer constant to determine the parameter
	// (here we only have a single parameter, but it's 
	// a good idea to get acquainted to the style
	template <int P> void setParameter(double v)
	{
		if(P == 0)
		{
			delayTimeMs = v;
			updateIndexes();
		}
	}
	
	// Clears the delay buffer
	void reset()
	{
		for(auto& fr: buffer)
		{
			for(auto& s: fr)
				s = 0.0f;
		}
	}
	
	// Initialises the processing
	void prepare(PrepareSpecs ps)
	{
		sampleRate = ps.sampleRate;
		updateIndexes();
		reset();
	}	
	
	// This will write a frame into the delay buffer
	// and read a delayed value using linear interpolation
	void processFrame(types::Frame& d)
	{
		// write the frame to the current write position
		buffer[writeIndex] = d;
	
		// we convert the write index to a double and subtract
		// the delay time to get the read index (in the past).
		// Note how we don't need to care about wrapping
		// around the limits and handling edge cases as this
		// is all done by the index types defined above.
		auto readIndex = (double)(int)(writeIndex) - delaySamples;
	
		// assign that value to the read index type
		types::ReadIndex r(readIndex);
		 
		// This single line performs a bounds check, then
		// creates a frame from the interpolated position and
		// writes it back to the input frame
		d = buffer[r];
		
		// bump the write index. Again, we don't need to care
		// about wrapping the value around the buffer size.
		writeIndex++;
	}
	
private:
	
	double delaySamples = 0.0;
	double delayTimeMs = 0.0;
	double sampleRate = 0.0;
	
	types::BufferType  buffer;
	types::WriteIndex writeIndex = 0;
};

// A simple oscillator with a naive saw and a square function
// It is tailored exactly for our use cases:
// 1. Driving the delay line modulation (Saw)
// 2. Crossfading between two sources (Square)
struct SimpleOscillator
{
	
	// Produces a new saw waveform sample.
	// The intensity will apply a bipolar gain value to
	// the output (so if it is 0 then the output is 0.5)
	double tickSaw(double intensity, double phase)
	{
		// create a ramp using the float modulo function
		auto v = Math.fmod(uptime + phase, 1.0);
	
		// Apply the bipolar intensity
		v -= 0.5;
		v *= intensity;
		v += 0.5;
	
		// bump the uptime
		uptime += uptimeDelta;
		
		return v;
	}
	
	// This creates a square wave function that
	// is smoothed a little bit for the crossfading
	double tickSquare()
	{
		
	#if SATURATED_TRIANGLE
		auto v = tickSaw(1.0, 0.5);
		auto thisValue = Math.abs(v - 0.5) * 2.0;
		
		thisValue = (thisValue - 0.5);
		thisValue *= 28.0;
		thisValue += 0.5;
		
		thisValue = Math.range(thisValue, 0.0, 1.0);
		
		thisValue *= thisValue;
		
		return thisValue;//squareRamp.advance();
	#else
	
		// We just use the saw waveform as starting point
		auto v = tickSaw(1.0, 0.5);
		
		// now we transform the saw to a square
		auto thisValue = v > 0.5 ? 1.0 : 0.0;
		
		// set the smoothing target to the square wave
		squareRamp.set(thisValue);
		
		// return the smoothed square wave
		return squareRamp.advance();
	#endif
		
	}
	
	// reset the state
	void reset()
	{
		uptime = 0.0;
		squareRamp.reset();
	}
	
	// calculate the frequency and the smoothing time
	void prepare(PrepareSpecs ps)
	{
		// the frequency of the oscillators must be synced
		// to the delay time
		auto delayFreq = 1.0 / (0.001 * (double)config::DelayLength);
		
		// calculate the pitch factor
		uptimeDelta = delayFreq / ps.sampleRate;
		
		// set the smoothing time
		squareRamp.prepare(ps.sampleRate, (double)config::XFadeTime);
	}
	
	// the oscillator's uptime
	double uptime = 0.0;
	
	// the value that the uptime is increased each sample
	// (aka pitch factor)
	double uptimeDelta = 0.0;
	
	// sdouble is a native type that offers low-pass smoothing
	sdouble squareRamp;
};

// A simple tool class that mixes two signals using
// the square wave oscillator as input modulation source
struct Mixer
{
	// just pass it on to the oscillator
	void prepare(PrepareSpecs ps)
	{
		xfader.prepare(ps);
	}
	
	// just pass it on to the oscillator
	void reset()
	{
		xfader.reset();
	}
	
	// Writes a mix of left and right to dst based on the crossfade
	// oscillator's value
	void process(types::Frame& dst, types::Frame& left, types::Frame& right)
	{
		// the left gain is the crossfade value
		auto lGain = (float)xfader.tickSquare();
		// the right gain is the inverted value
		auto rGain = 1.0f - lGain;
		
		// multiply the frames with the gain factors
		left *= lGain;
		right *= rGain;
		
		// copy the frames to the destination
		dst = left;
		dst += right;
	}

	SimpleOscillator xfader;
};

// This class implements the modulation of the delay line.
struct DelayChannel
{
	// forward to the inner modules
	void prepare(PrepareSpecs ps)
	{
		ramper.prepare(ps);
		dl.prepare(ps);
	}
	
	void reset()
	{
		ramper.reset();
		dl.reset();
	}
	
	void processFrame(types::Frame& d)
	{
		// calculates the delay modulation value (from 0...1).
		auto delayValue = ramper.tickSaw(shiftAmount, phase);
		
		// set the delay time
		dl.setParameter<0>(delayValue * (double)config::DelayLength);
		
		// send the frame to the delay line
		dl.processFrame(d);
	}

	SimpleOscillator ramper;
	DelayLine dl;
	double shiftAmount = 1.0;
	double phase = 0.0;
};	
}

// Now this is the actual pitch shifting class
// The heavy lifting is done by the classes defined above,
// so this class is just assembling the building blocks.
template <int NV> struct PitchShifter
{
	SNEX_NODE(PitchShifter);
	
	// pass this callback to all members
	void prepare(PrepareSpecs ps)
	{
		d1.prepare(ps);
		d2.prepare(ps);
		xfader.prepare(ps);
		shiftAmount.prepare(ps.sampleRate, 50.0);
	}
	
	// pass this callback to all members
	void reset()
	{
		d1.reset();
		d2.reset();
		xfader.reset();
		shiftAmount.reset();
	}
	
	void processFrame(span<float, config::NumChannels>& data)
	{
		// calculate the next smoothed shift amount
		double sa = shiftAmount.advance();
	
		// set the shiftAmount to both delay channels
		d1.shiftAmount = sa;
		d2.shiftAmount = sa;
		
	#if DEBUG_OSCILLATORS
		// this will write the output of the saw and
		// square wave to the audio channels
		// (so you can inspect them using the test panel).
		data[0] = d1.ramper.tickSaw(1.0, 0.0);
		data[1] = d1.ramper.tickSquare();
	#else
	
		// the second oscillator needs to have a phase
		// shift of 180 degrees
		d2.phase = 0.5;
	
		// Copy the input to two frames
		types::Frame l = data;
		types::Frame r = data;
		
		// process each frame with a delay channel
		d1.processFrame(l);
		d2.processFrame(r);
		
		// mix the two frames back to the input
		xfader.process(data, l, r);
		
	#endif
	}
	
	// This function will be called for the deactivated channel amount
	void processFrame(span<float, config::UnusedChannels>& d)
	{
		
	}
	
	template <typename ProcessDataType> void process(ProcessDataType& data)
	{
		// this creates a FrameProcessor object which
		// will allow frame-based processing
		// (similar to the container.frame2_block).
		auto fd = data.toFrameData();
		
		// Loop through the buffer and call the processFrame
		// function for each interleaved frame
		while(fd.next())
			processFrame(fd.toSpan());
	}
	
	// unused
	void handleHiseEvent(HiseEvent& e)
	{
		
	}
	
	// unused
	void setExternalData(const ExternalData& d, int index)
	{
		
	}
	
	// this sets the shift amount
	template <int P> void setParameter(double v)
	{
		// convert the 0.5...2.0 input range to
		// the range the oscillator expects...
		v = 2.0 - v;
		v *= 0.5;
		v -= 0.5;
		
		// set the target value for the low pass
		// smoother
		shiftAmount.set(v);	
	}
	
	// this determines the shift amount and needs
	// to be smoothed in order to avoid clicks when
	// changing the pitch
	sdouble shiftAmount;
	
	tools::DelayChannel d1;
	tools::DelayChannel d2;
	tools::Mixer xfader;	
};
