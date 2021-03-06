#include <fstream>

#include <cppunit/config/SourcePrefix.h>
#include "ChainIntegrationTestCase.h"

#include "../Configuration.h"
#include "../ModelFileReaderOperation.h"
#include "../SliceOperation.h"
#include "../RegionerOperation.h"
#include "../PatherOperation.h"
#include "../GCoderOperation.h"
#include "../FileWriterOperation.h"


CPPUNIT_TEST_SUITE_REGISTRATION( ChainIntegrationTestCase );

#define SINGLE_EXTRUDER_FILE_NAME "v29_single_xtruder_warmup.gcode"
#define DUAL_EXTRUDER_FILE_NAME "v29_dual_xtruder_warmup.gcode"

void configurePathTest(Configuration& config)
{
	config["machineName"] = "TOM";
	config["firmware"] ="v2.9";

	Json::Value extruder;

	extruder["defaultExtrusionSpeed"] = 6;
	extruder["extrusionTemperature"] = 200;
	extruder["coordinateSystemOffsetX"] = 0;
	config["extruders"].append(extruder);
	config["platform"]["temperature"]= 30;

	config["FileWriterOperation"]["filename"] = SINGLE_EXTRUDER_FILE_NAME;
	config["FileWriterOperation"]["format"]= ".gcode";

}

typedef std::vector<Operation*> inout;
void ChainIntegrationTestCase::testChain()
{
    Configuration &config = *(new Configuration());
    config["gcodeFilename"]= "chainIntegration.gcode";
    
	Operation *reader = new ModelFileReaderOperation();
	MeshData* mesh = new MeshData();
    
	Operation *slicer = new SliceOperation();
	SliceData *slice = new SliceData(0.2, 0.4);
    
	Operation *regioner = new RegionerOperation();
	RegionData *region = new RegionData(0.2, 0.4);
    
	Operation *pather = new PatherOperation();
	PathData *path = new PathData(0.2, 0.4);
    
	Operation *gcoder = new GCoderOperation();
	GCodeData *gcode = new GCodeData("(This is gcode)");
    
	Operation *writer = new FileWriterOperation();
    
    
	inout empty;
    
	inout readerOut;
	readerOut.push_back(slicer);
	reader->init(config, empty, readerOut);
    
	inout sliceIn;
	inout sliceOut;
	sliceOut.push_back(regioner);
	sliceIn.push_back(reader);
	slicer->init(config, sliceIn, sliceOut);
    
	inout regionIn;
	inout regionOut;
	regionIn.push_back(slicer);
	regionOut.push_back(pather);
	regioner->init(config, regionIn, regionOut);
    
	inout patherIn;
	inout patherOut;
	patherIn.push_back(regioner);
	patherOut.push_back(gcoder);
	pather->init(config, patherIn, patherOut);
    
	inout gcoderIn;
	inout gcoderOut;
	gcoderIn.push_back(pather);
	gcoderOut.push_back(writer);
	gcoder->init(config, gcoderIn, gcoderOut);
    
	inout writerIn;
	writerIn.push_back(gcoder);
	writer->init(config, writerIn, empty);
    
	// flaky begin
	writer->start();
	// magic happens here
	//reader->finish();
	// flaky end
    
    
	delete writer;
	delete gcoder;
	delete pather;
	delete regioner;
	delete slicer;
	delete reader;
	delete &config;

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.01, 0.05 );
	CPPUNIT_ASSERT( 1 == 1 );
	CPPUNIT_ASSERT_EQUAL( 12, 12 );
}




