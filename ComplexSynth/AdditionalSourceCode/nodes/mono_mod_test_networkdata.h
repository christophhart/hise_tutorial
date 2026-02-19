namespace project
{

struct mono_mod_test_networkdata: public scriptnode::dll::InterpretedNetworkData
{
	String getId() const override
	{
		return "mono_mod_test";
	}
	bool isModNode() const override
	{
		return false;
	}
	String getNetworkData() const override
	{
		return "303.nT6K8CF5AzRB.XEzzLBLIaS.rMt1UmBWaHNIIiEJblAQdbAYrt1Ov.aAjbCKsmj9AnB.q.PJ.D2GSuN9rGzjmgZGY9kZoV5g7tlAebjGgbTBTGIoccjhH+BFL4S30JDYGsKtvMon1.lTwu2VD5FEUfCYRNZc7kBPdHFBWoIE2w1JOpG6Xaz0O5.4qDvkswWpkp4YKsMhi0PyqQNZe6iemDZS9KLjiQlG93ISrnXZaRP1hL+2X7e3TjAcbzwPCvIgZfg5FpABZqiBCBDxO86ghPatdXobVfAQhiP9RUfiZH.Hf.DQj4z7ADPe+VSvuO00Wg2Dbhw0GVub2h5.yMXO.IiCXOsGycGancvozMPXSYjqiz2tMztN6Cb.o+AhaSul6ByMtBrA7nYwN7BWPzwn";
	}
};
}

