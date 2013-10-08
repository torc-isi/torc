// Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
// $HeadURL$
// $Id$

// This program is free software: you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation, either version 3 of the 
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program.  If 
// not, see <http://www.gnu.org/licenses/>.

#include <fstream>
#include "torc/common/DirectoryTree.hpp"
#include "torc/Physical.hpp"
#include "torc/physical/XdlImporter.hpp"
#include "torc/physical/XdlExporter.hpp"
#include "torc/packer/XdlUnpack.hpp"
#include "torc/packer/CombinationalPath.hpp"
#include "torc/packer/RcFactory.hpp"

using namespace std;
int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <input xdlfile>" << std::endl;
		return 1;
	}

	ifstream inp(argv[1]);
	cout<<"Reading xdl file ....................."<<endl;
	torc::physical::XdlImporter importer;
	importer(inp, "");
	torc::physical::DesignSharedPtr designPtr = importer.getDesignPtr();
	
	torc::physical::XdlUnpack unpacker;
	cout<<"Unpacking ............................"<<endl;
	torc::physical::DesignSharedPtr dp = unpacker(designPtr);
	string unpackName = argv[1];
	unpackName += "_unpack";
	ofstream outp(unpackName.c_str());
	torc::physical::XdlExporter fileExporter(outp);
	fileExporter(dp);
	cout<<"Extracting combinational paths ......."<<endl;
	// test combinational paths
	torc::physical::CombinationalPathSharedPtr combPtr = torc::physical::RcFactory::newCombinationalPathPtr();
	combPtr->setPatchCounts(dp);
	torc::physical::CombinationalPath::RoutingNetSharedPtrConstIterator rnb = combPtr->routingNetsBegin();
	torc::physical::CombinationalPath::RoutingNetSharedPtrConstIterator rne = combPtr->routingNetsEnd();
	cout<<"Done ................................."<<endl;
	/*while(rnb<rne){
		cout<<(*rnb)->getName()<<":"<<endl;
		size_t n=(*rnb)->getSuperNet()->getSourceCount() + (*rnb)->getSuperNet()->getSinkCount();
		for(size_t i=0; i<n; ++i)
			cout<<"\n"<<(*rnb)->getPathCount(i)<<endl;
		++rnb;
	}*/
	return 0;
}
