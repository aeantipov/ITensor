#include "test.h"

#include "global.h"

using namespace std;
using namespace itensor;

Args
test_function(const Args& opts)
    {
    return opts;
    }


TEST_CASE("OptionTest")
{

SECTION("TestArgs")
    {
    auto& gopts = Args::Global();

    Args o1("Quiet");
    auto o2 = Args("Pinning",0.4);
    Args o3("Auto",false);
    auto o4 = Args();

    gopts += o1;
    gopts += o2;

    CHECK(gopts.getBool("Quiet") == true);

    Args opts1("Quiet",false);

    CHECK(opts1.defined("Quiet"));
    CHECK(opts1.getBool("Quiet") == false);

    Args opts2(opts1);
    opts2 += o3;
    opts2 += o4;

    CHECK(opts2.defined("Pinning"));
    CHECK(opts2.getReal("Pinning") == 0.4);

    CHECK(opts2.defined("Auto"));
    CHECK(opts2.getBool("Auto") == false);

    Args opts3 = opts1 + opts2;

    CHECK(opts3.defined("Quiet"));
    CHECK(opts3.defined("Auto"));
    }

SECTION("Operator")
    {
    Args oset1; 
    oset1 += Args("Quiet");
    oset1 += Args("Auto");
    CHECK(oset1.defined("Quiet"));
    CHECK(oset1.defined("Auto"));

    Args oset2 = Args("Pinning",1) + "Quiet" + "Auto";
    CHECK(oset1.defined("Quiet"));
    CHECK(oset1.defined("Auto"));
    CHECK(oset1.defined("Pinning"));
    }

SECTION("StringConstructor")
    {
    Args o1("Quiet,Auto,Pinning=-0.5");
    CHECK(o1.defined("Quiet"));
    CHECK(o1.defined("Auto"));
    CHECK(o1.defined("Pinning"));
    CHECK(o1.getReal("Pinning") == -0.5);

    //test_function is defined at the top of this file
    Args o2 = test_function("WriteM=500,UseSVD=false,");
    CHECK(o2.getInt("WriteM") == 500);
    CHECK(o2.getBool("UseSVD") == false);
    
    Args o3 = test_function(Args("Quiet") + "WriteM=500,UseSVD=false,");
    CHECK(o3.getBool("Quiet") == true);
    CHECK(o3.getInt("WriteM") == 500);
    CHECK(o3.getBool("UseSVD") == false);

    Args o4("Name=new,Cutoff=5E-12,Val=-4.235235");
    CHECK(o4.getString("Name") == "new");
    CHECK(fabs(o4.getReal("Cutoff")-5E-12) < 1E-14);
    CHECK(fabs(o4.getReal("Val")-(-4.235235)) < 1E-12);
    }

SECTION("VariadicConstructor")
    {
    Args o1("Quiet",true,"Auto",true,"Pinning",-0.5);
    CHECK(o1.defined("Quiet"));
    CHECK(o1.defined("Auto"));
    CHECK(o1.defined("Pinning"));
    CHECK(o1.getReal("Pinning") == -0.5);

    //test_function is defined at the top of this file
    Args o2 = test_function({"WriteM",500,"UseSVD",false});
    CHECK(o2.getInt("WriteM") == 500);
    CHECK(o2.getBool("UseSVD") == false);
    
    Args o3("Name","new",o1,"Cutoff",5E-12,"Val",-4.235235);
    CHECK(o3.getString("Name") == "new");
    CHECK(fabs(o3.getReal("Cutoff")-5E-12) < 1E-14);
    CHECK(fabs(o3.getReal("Val")-(-4.235235)) < 1E-12);
    CHECK(o3.defined("Quiet"));
    CHECK(o3.defined("Auto"));
    CHECK(o3.defined("Pinning"));
    CHECK(o3.getReal("Pinning") == -0.5);
    }

}

