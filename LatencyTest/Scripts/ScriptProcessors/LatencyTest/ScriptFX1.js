 function prepareToPlay(sampleRate, blockSize)
{
    // This converts the 200ms delay to samples using the samplerate
    local numSamples = Engine.getSamplesForMilliSeconds(200.0);
	
    // Propagate the latency to the host. This callback is as late as possible to
    // catch any misbehaving hosts...
	Engine.setLatencySamples(numSamples);
}
 function processBlock(channels)
{
	
}
 function onControl(number, value)
{
	
}
 