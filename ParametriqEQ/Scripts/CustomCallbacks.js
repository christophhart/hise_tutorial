const var analyser = Synth.getDisplayBufferSource("Parametriq EQ");
const var dp = analyser.getDisplayBuffer(0);

dp.setRingBufferProperties({
  "BufferLength": 8192,
  "WindowType": "Blackman Harris",
  "DecibelRange": [
    -130.0,
    -80.0
  ],
  "UsePeakDecay": false,
  "UseDecibelScale": true,
  "YGamma": 1.0,
  "Decay": 0.699999988079071,
  "UseLogarithmicFreqAxis": true
});

