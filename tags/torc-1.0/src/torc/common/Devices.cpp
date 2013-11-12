// Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
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

/// \file
/// \brief Source for the Devices class.

#include "torc/common/Devices.hpp"


#if 0
	#include <boost/assign/list_of.hpp>
	#include <boost/assign/list_inserter.hpp>
	#include <boost/bimap/bimap.hpp>
	#include <boost/bimap/multiset_of.hpp>
	#include <boost/bimap/list_of.hpp>

	using namespace boost::bimaps;
	using namespace boost;

	// an alternate approach based on Boost bimaps, but much heavier on object code
	void populate(void);
	void populate(void) {
		typedef bimap< multiset_of< EDevice >, list_of< std::string > > bm_type;
		bm_type bm = assign::list_of< bm_type::relation >
			(eXCV50, "xcv50") (eXCV100, "xcv100") (eXCV150, "xcv150") (eXCV200, "xcv200") 
			(eXCV300, "xcv300") (eXCV400, "xcv400") (eXCV600, "xcv600") (eXCV800, "xcv800") 
			(eXCV1000, "xcv1000");
	}
#endif


namespace torc {
namespace common {

	/// \brief Helper class to initialize device map and list information.
	class DeviceFamilyHelper {
	protected:
	// friends
		/// \brief Only the Devices class has access to our internals.
		friend class Devices;
	// typedefs
		/// \brief Imported name.
		typedef std::string string;
	// members
		/// \brief The map from device name to device enumeration.
		DeviceNameMap& mDeviceMap;
		/// \brief The list of devices for a particular family.
		DeviceVector& mDeviceVector;
		/// \brief The list of devices for all families.
		DeviceVector& mCombinedDeviceVector;
		/// \brief Convenience storage for the last inserted device name.
		static string sDevice;
	// constructors
		/// \brief Protected constructor.
		DeviceFamilyHelper(DeviceNameMap& inDeviceMap, DeviceVector& inDeviceVector, 
			DeviceVector& inCombinedDeviceVector) : mDeviceMap(inDeviceMap), 
			mDeviceVector(inDeviceVector), mCombinedDeviceVector(inCombinedDeviceVector) {}
	// operators
		/// \brief Convenience operator to retain the last device name inserted.
		DeviceFamilyHelper& operator<< (const string& rhs) { sDevice = rhs; return *this; }
		/// \brief Operator to propagate an enumeration and the previously inserted device name to 
		///		the device map and lists.
		DeviceFamilyHelper& operator<< (enum EDevice rhs) {
			mDeviceMap[sDevice] = rhs;
			mDeviceVector.push_back(sDevice);
			mCombinedDeviceVector[rhs] = sDevice;
			return *this;
		}
	};

	std::string DeviceFamilyHelper::sDevice;
	Devices Devices::sDevices;

	Devices::Devices(void) {
		// preinitialize the device vector size
		mDeviceNames.resize(eDeviceCount);
		// Virtex
		DeviceFamilyHelper virtexHelper(mDeviceNameMap, mVirtexDevices, mDeviceNames);
		virtexHelper
			// Virtex
			<< "xcv50" << eXCV50 << "xcv100" << eXCV100 << "xcv150" << eXCV150 
			<< "xcv200" << eXCV200 << "xcv300" << eXCV300 << "xcv400" << eXCV400 
			<< "xcv600" << eXCV600 << "xcv800" << eXCV800 << "xcv1000" << eXCV1000;
		// VirtexE
		DeviceFamilyHelper virtexEHelper(mDeviceNameMap, mVirtexEDevices, mDeviceNames);
		virtexEHelper
			// VirtexE
			<< "xcv50e" << eXCV50E << "xcv100e" << eXCV100E << "xcv200e" << eXCV200E 
			<< "xcv300e" << eXCV300E << "xcv400e" << eXCV400E << "xcv405e" << eXCV405E 
			<< "xcv600e" << eXCV600E << "xcv812e" << eXCV812E << "xcv1000e" << eXCV1000E 
			<< "xcv1600e" << eXCV1600E << "xcv2000e" << eXCV2000E << "xcv2600e" << eXCV2600E 
			<< "xcv3200e" << eXCV3200E;
		// Virtex2
		DeviceFamilyHelper virtex2Helper(mDeviceNameMap, mVirtex2Devices, mDeviceNames);
		virtex2Helper
			// Virtex2
			<< "xc2v40" << eXC2V40 << "xc2v80" << eXC2V80 << "xc2v250" << eXC2V250 
			<< "xc2v500" << eXC2V500 << "xc2v1000" << eXC2V1000 << "xc2v1500" << eXC2V1500 
			<< "xc2v2000" << eXC2V2000 << "xc2v3000" << eXC2V3000 << "xc2v4000" << eXC2V4000 
			<< "xc2v6000" << eXC2V6000 << "xc2v8000" << eXC2V8000;
		// Virtex2P
		DeviceFamilyHelper virtex2PHelper(mDeviceNameMap, mVirtex2PDevices, mDeviceNames);
		virtex2PHelper
			// Virtex2P
			<< "xc2vp2" << eXC2VP2 << "xc2vp4" << eXC2VP4 << "xc2vp7" << eXC2VP7 
			<< "xc2vp20" << eXC2VP20 << "xc2vp30" << eXC2VP30 << "xc2vp40" << eXC2VP40 
			<< "xc2vp50" << eXC2VP50 << "xc2vp70" << eXC2VP70 << "xc2vp100" << eXC2VP100 
			// Virtex2P X
			<< "xc2vpx20" << eXC2VPX20 << "xc2vpx70" << eXC2VPX70;
		// Virtex4
		DeviceFamilyHelper virtex4Helper(mDeviceNameMap, mVirtex4Devices, mDeviceNames);
		virtex4Helper
			// Virtex4 FX
			<< "xc4vfx12" << eXC4VFX12 << "xc4vfx20" << eXC4VFX20 << "xc4vfx40" << eXC4VFX40 
			<< "xc4vfx60" << eXC4VFX60 << "xc4vfx100" << eXC4VFX100 << "xc4vfx140" << eXC4VFX140 
			// Virtex4 LX
			<< "xc4vlx15" << eXC4VLX15 << "xc4vlx25" << eXC4VLX25 << "xc4vlx40" << eXC4VLX40 
			<< "xc4vlx60" << eXC4VLX60 << "xc4vlx80" << eXC4VLX80 << "xc4vlx100" << eXC4VLX100 
			<< "xc4vlx160" << eXC4VLX160 << "xc4vlx200" << eXC4VLX200
			// Virtex4 SX
			<< "xc4vsx25" << eXC4VSX25 << "xc4vsx35" << eXC4VSX35 << "xc4vsx55" << eXC4VSX55;
		// Virtex5
		DeviceFamilyHelper virtex5Helper(mDeviceNameMap, mVirtex5Devices, mDeviceNames);
		virtex5Helper
			// Virtex5 FXT
			<< "xc5vfx30t" << eXC5VFX30T << "xc5vfx70t" << eXC5VFX70T 
			<< "xc5vfx100t" << eXC5VFX100T << "xc5vfx130t" << eXC5VFX130T 
			<< "xc5vfx200t" << eXC5VFX200T
			// Virtex5 LX
			<< "xc5vlx30" << eXC5VLX30 << "xc5vlx50" << eXC5VLX50 << "xc5vlx85" << eXC5VLX85 
			<< "xc5vlx110" << eXC5VLX110 << "xc5vlx155" << eXC5VLX155 << "xc5vlx220" << eXC5VLX220 
			<< "xc5vlx330" << eXC5VLX330
			// Virtex5 LXT
			<< "xc5vlx20t" << eXC5VLX20T << "xc5vlx30t" << eXC5VLX30T << "xc5vlx50t" << eXC5VLX50T 
			<< "xc5vlx85t" << eXC5VLX85T << "xc5vlx110t" << eXC5VLX110T 
			<< "xc5vlx155t" << eXC5VLX155T << "xc5vlx220t" << eXC5VLX220T 
			<< "xc5vlx330t" << eXC5VLX330T
			// Virtex5 SXT
			<< "xc5vsx35t" << eXC5VSX35T << "xc5vsx50t" << eXC5VSX50T << "xc5vsx95t" << eXC5VSX95T 
			<< "xc5vsx240t" << eXC5VSX240T
			// Virtex5 TXT
			<< "xc5vtx150t" << eXC5VTX150T << "xc5vtx240t" << eXC5VTX240T;
		// Virtex6
		DeviceFamilyHelper virtex6Helper(mDeviceNameMap, mVirtex6Devices, mDeviceNames);
		virtex6Helper
			// Virtex6 CXT
			<< "xc6vcx75t" << eXC6VCX75T << "xc6vcx130t" << eXC6VCX130T 
			<< "xc6vcx195t" << eXC6VCX195T << "xc6vcx240t" << eXC6VCX240T
			// Virtex6 HXT
			<< "xc6vhx250t" << eXC6VHX250T << "xc6vhx255t" << eXC6VHX255T 
			<< "xc6vhx380t" << eXC6VHX380T << "xc6vhx565t" << eXC6VHX565T
			// Virtex6 LXT
			<< "xc6vlx75t" << eXC6VLX75T << "xc6vlx130t" << eXC6VLX130T 
			<< "xc6vlx195t" << eXC6VLX195T << "xc6vlx240t" << eXC6VLX240T 
			<< "xc6vlx365t" << eXC6VLX365T << "xc6vlx550t" << eXC6VLX550T 
			<< "xc6vlx760" << eXC6VLX760
			// Virtex6 SXT
			<< "xc6vsx315t" << eXC6VSX315T << "xc6vsx475t" << eXC6VSX475T;
		// Artix7
		DeviceFamilyHelper artix7Helper(mDeviceNameMap, mArtix7Devices, mDeviceNames);
		artix7Helper
			// Artix7 T
			<< "xc7a100t" << eXC7A100T << "xc7a200t" << eXC7A200T;
		// Kintex7
		DeviceFamilyHelper kintex7Helper(mDeviceNameMap, mKintex7Devices, mDeviceNames);
		kintex7Helper
			// Kintex7 T
			<< "xc7k70t" << eXC7K70T << "xc7k160t" << eXC7K160T << "xc7k325t" << eXC7K325T 
			<< "xc7k355t" << eXC7K355T << "xc7k410t" << eXC7K410T << "xc7k420t" << eXC7K420T
			<< "xc7k480t" << eXC7K480T;
		// Virtex7
		DeviceFamilyHelper virtex7Helper(mDeviceNameMap, mVirtex7Devices, mDeviceNames);
		virtex7Helper
			// Virtex7 T
			<< "xc7v585t" << eXC7V585T << "xc7v2000t" << eXC7V2000T 
			// Virtex7 HT
			<< "xc7vh580t" << eXC7VH580T << "xc7vh870t" << eXC7VH870T
			// Virtex7 XT
			<< "xc7vx330t" << eXC7VX330T << "xc7vx415t" << eXC7VX415T << "xc7vx485t" << eXC7VX485T
			<< "xc7vx550t" << eXC7VX550T << "xc7vx690t" << eXC7VX690T << "xc7vx980t" << eXC7VX980T
			<< "xc7vx1140t" << eXC7VX1140T;
		// Zynq7000
		DeviceFamilyHelper zynq7000Helper(mDeviceNameMap, mZynq7000Devices, mDeviceNames);
		zynq7000Helper
			// Zynq7000
			<< "xc7z010" << eXC7Z010 << "xc7z020" << eXC7Z020 << "xc7z030" << eXC7Z030 
			<< "xc7z045" << eXC7Z045;
		// Spartan3E
		DeviceFamilyHelper spartan3EHelper(mDeviceNameMap, mSpartan3EDevices, mDeviceNames);
		spartan3EHelper
			// Spartan3E
			<< "xc3s100e" << eXC3S100E << "xc3s250e" << eXC3S250E << "xc3s500e" << eXC3S500E 
			<< "xc3s1200e" << eXC3S1200E << "xc3s1600e" << eXC3S1600E;
		// Spartan6
		DeviceFamilyHelper spartan6Helper(mDeviceNameMap, mSpartan6Devices, mDeviceNames);
		spartan6Helper
			// Spartan6 LX
			<< "xc6slx4" << eXC6SLX4 << "xc6slx9" << eXC6SLX9 << "xc6slx16" << eXC6SLX16 
			<< "xc6slx25" << eXC6SLX25 << "xc6slx45" << eXC6SLX45 << "xc6slx75" << eXC6SLX75 
			<< "xc6slx100" << eXC6SLX100 << "xc6slx150" << eXC6SLX150
			// Spartan6 LXT
			<< "xc6slx25t" << eXC6SLX25T << "xc6slx45t" << eXC6SLX45T << "xc6slx75t" << eXC6SLX75T 
			<< "xc6slx100t" << eXC6SLX100T << "xc6slx150t" << eXC6SLX150T;

		// add all of the devices that are currently supported
		addToSupportedDevices(mVirtexDevices);
		addToSupportedDevices(mVirtexEDevices);
		addToSupportedDevices(mVirtex2Devices);
		addToSupportedDevices(mVirtex2PDevices);
		addToSupportedDevices(mVirtex4Devices);
		addToSupportedDevices(mVirtex5Devices);
		addToSupportedDevices(mVirtex6Devices);
		addToSupportedDevices(mVirtex7Devices);
		addToSupportedDevices(mKintex7Devices);
		addToSupportedDevices(mArtix7Devices);
		addToSupportedDevices(mZynq7000Devices);
		addToSupportedDevices(mSpartan3EDevices);
		addToSupportedDevices(mSpartan6Devices);

		// add a subset of devices suitable for basic unit tests
		mUnitTestDevices.push_back(mVirtexDevices.front());
		mUnitTestDevices.push_back(mVirtexEDevices.front());
		mUnitTestDevices.push_back(mVirtex2Devices.front());
		mUnitTestDevices.push_back(mVirtex2PDevices.front());
		mUnitTestDevices.push_back(mVirtex4Devices.front());
		mUnitTestDevices.push_back(mVirtex5Devices.front());
		mUnitTestDevices.push_back(mVirtex6Devices.front());
		mUnitTestDevices.push_back(mVirtex7Devices.front());
		mUnitTestDevices.push_back(mKintex7Devices.front());
		mUnitTestDevices.push_back(mArtix7Devices.front());
		mUnitTestDevices.push_back(mZynq7000Devices.front());
		mUnitTestDevices.push_back(mSpartan3EDevices.front());
		mUnitTestDevices.push_back(mSpartan6Devices.front());
	}

	/*
	The list of 7-Series devices supported by ISE has changed frequently from 13.4 to 14.5.
	Reconstruct the output of partgen by pasting the following hex data as input to:

		xxd -r -g 4 -c 24 | tar xvz

	The following files will be generated:

		artix-13.4 artix-14.1 artix-14.2 artix-14.3 artix-14.4 artix-14.5
		kintex-13.4 kintex-14.1 kintex-14.2 kintex-14.3 kintex-14.4 kintex-14.5
		virtex-13.4 virtex-14.1 virtex-14.2 virtex-14.3 virtex-14.4 virtex-14.5
		zynq-13.4 zynq-14.1 zynq-14.2 zynq-14.3 zynq-14.4 zynq-14.5

	0000000: 1f8b0800 82b7b351 0003ed9d 5d6fe246 1486731b 7ec55cee  .......Q....]o.F..s.~.\.
	0000018: 4a05e67b a0aa2a45 bbe96ad5 561b3552 55f566c5 12036e28  J..{..*E..j.V.5RU.f...n(
	0000030: a4c69bb2 f9f5b53d fe2480ed c28ce3f8 bc178189 ed33c6f8  .......=.$...........3..
	0000048: 3ce73073 6c4f3cdf ddf6091b f00b63c2 8124e7c1 2b518230  <.0slO<.......c..$..+Q.0
	0000060: fd4a55f4 ff404452 71412811 5c312624 bdc08461 cc2f1036  .JU..@DRqA(.\1&$...a./.6
	0000078: b74b99be 6efc8987 d0c56ae3 3beecaf1 0ead57b6 bca5facd  .K..n.....j.;.....W.....
	0000090: 593a938d 83c23300 f5d1cdc4 f33f382b f4693052 db3bf466  Y:....3......?8+.i0R.;.f
	00000a8: e9ae247f db7bb77e f8e6b9f3 858fde4c df22321e 8b3ec584  ..$..{.~.......L."2..>..
	00000c0: a03fdc60 85ed77e8 e36a3a40 e86ab944 d15a1be4 391bc77b  .?.`..w..j:@.j.D.Z..9..{
	00000d8: 74ee06bd 9b5f6f6f aedfa17e 1f7d7a74 3ccfbd73 57f3783b  t...._oo...~.}zt<..sW.x;
	00000f0: 3473974e ef87a1b3 7d587bfe d05faf97 9be1365a 340cf767  4s.N....}X{.._....6Z4..g
	0000108: f8f1f6fa f3fbdb61 f83a9c84 27aa1ade 4dfc49fc 7e3099de  .......a.:..'...M.I.~0..
	0000120: fd88fe75 fd055aae a7936562 cefd3b32 f7e0adff 72a6fe66  ...u..Z...eb..;2....r..f
	0000138: f8d3cd87 abcfdab8 fe1b74f0 75bbafa3 7dfdf4e2 8e7a57d1  ..........t.u...}....zW.
	0000150: 82de76aa 26a3cbcb e0435dbf bffdfef2 b2cf50a0 3e8dfe12  ..v.&....C].......P.>...
	0000168: fdfe971e 8a35ddcc 19e559f3 614e994c 9b337f4e 858c4c12  .....5....Y.aN.L.3.N..L.
	0000180: 717e9b0c fbffd7e8 6c3ee723 becfa830 6194e0f3 599dcfa5  q~......l>.#...0a...Y...
	0000198: da7b3868 9d4e665f 8a56bf14 adcee684 2447b9ce 11a966d6  .{8h.Nf_.V......$G....f.
	00001b0: a2ff4f34 fff98098 eba38cff 84b290ff 5c704e14 0bf94f95  ..O4............\pN...O.
	00001c8: 94c07f1b 4af91f9c 0139fedf 0c88d8ce 4af84f4d f23fd81f  ....J....9......J.OM.?..
	00001e0: 3bfccf75 5495ff6d c555d2de e8b5db4f 2fd0a94a f94fcdf5  ;..uT..m.U.....O/..J.O..
	00001f8: 518bff4a e9fc1f03 ff6d28c7 7f5ae03f 1d95e6ff 86f94f6d  Q..J.....m(..Z.?......Om
	0000210: f19f02ff 81ff1d55 ca7f66ae 8f12fe63 a578c27f 8a198bf2  .......U..f....c.x......
	0000228: 7f05f9bf 15e5f8cf 0afce7b8 69fe335b fc671de5 7ff8fda7  ............i.3[.g......
	0000240: fedfdcf8 6fe0ff69 fe4719d6 bfff9500 ffb7a19c fff3a2ff  ....o..i.G..............
	0000258: 8f1b767f 4bc3bfbc fef0ef2b 72ffccff 85b973ac 96ff4bfd  ..v.K......+r.....s...K.
	0000270: fb8f10f0 7f1bcaf9 bf28f8bf 18353cfc 276cf9bf e8b2ffdf  .........(...5<.'l......
	0000288: bb2bdf31 3c015c36 fe13f87d 3afe13ce 0507fe4f 0403ffb7  .+.1<.\6...}:......O....
	00002a0: a1d6ccff ea1335f6 ccb86166 0678b7a7 5eda55ef 67bd2874  ......5...af.x..^.U.g.(t
	00002b8: d07b55cf 3f0b0e99 8ebfdc13 798299d9 21ab8c8a 53766e8c  .{U.?.......y...!...Svn.
	00002d0: f1c14e66 d1d2a813 51a79370 3b126dc7 c94947ae dace715a  ..Nf....Q..p;.m..IG...qZ
	00002e8: a713bd73 b96632bb 7bcf47a7 db69dac7 8f29e1bf c909e03a  ...s.f2.{.G..i...).....:
	0000300: fc579c6a fe43fe67 45ad99ff 35c8ff9d 1960e03f f0bf83fc  .W.j.C.gE...5....`.?....
	0000318: 3737015c 83ffc1af 1e16f19f 03ffada8 35f3bf46 f94f81ff  77.\............5..F.O..
	0000330: c0ff8ef3 dfdc0470 2dfe73ac f90fe33f 56d49af9 5fa3fc67  .......p-.s....?V..._..g
	0000348: c07fe07f c7f9dfe4 f87f7efe 5fc1f88f 45b565fe df28fe61  ..........~._...E.e..(.a
	0000360: f81ff0df 4dfce7f8 6fae00a4 0eff3989 e77f29f0 df86da52  ....M...o.....9...)....R
	0000378: ff6194ff 02f80ffc ef26ff1f 5dafe9fa 1f4a83f7 69fd0f93  .a.......&..]....J..i...
	0000390: 11ff158c ff58516b ea7ff489 1a63396e 98a9ffd9 ed291700  .....XQk.....c9n.....)..
	00003a8: 7ed78b42 343c6e59 9dfb2b68 18a8425b 49a22d05 49cf6996  ~..B4<nY..+h..B[I.-.I.i.
	00003c0: 881815da 631aef23 1f9dd972 b8cfbb3d 15da0ceb 9e45ad0b  ....c..#...r...=.....E..
	00003d8: cb8e7c06 71ea4748 0fb31c9f f885951f 0c59ede0 8c77c2ca  ..|.q.GH.........Y...w..
	00003f0: 917d786e 73b4df26 21fc88d1 0fcf6c2f f5b68576 603bb445  .}xns..&!.....l/...v`;.E
	0000408: 04ae6b2a 39c814d7 de744c45 d65e6853 4d63b133 4ae27f83  ..k*9....tLE.^hSMc.3J...
	0000420: f55f54e6 afffe67a fc4fc1fd 9faca835 f55f06e3 ff4efd57  ._T....z.O.....5._...N.W
	0000438: c5f8bfa0 e363a44b 91b69886 814340ce 50e8a9d0 3e7bceb0  .....c.K.....C@.P...>{..
	0000450: 10c702ec f3ef26df 1e33b2d3 a6af350f 598c54e5 c374e8b0  ......&..3....5.Y.T..t..
	0000468: 9c3b9729 b6d95973 9be272b6 b35c7631 f7c9e27f 63f57f54  .;.)..Ys..r..\v1....c..T
	0000480: c86cfc17 135dff21 e1fe2f56 d49afa3f a3f19f42 fc87f88f  .l...].!../V...?...B....
	0000498: 20febf80 f86f2cde 7733be97 298bff8d d57f528e f3f59f5c   ....o,.w3..).....R....\
	00004b0: c77f98ff b5a2d6d4 7f1a8dff 0cc6ff2b 5a86580e b1fcc5c4  ...............+Z.X.....
	00004c8: f2d71890 2d2b8bff cdcdff73 4ab2fa5f a2afff93 50ff6b45  ....-+.....sJ.._....P.kE
	00004e0: 6da9ff35 1afe61fa bfaae517 17fe774d 411650f7 684132a0  m..5..a.......wMA.P.hA2.
	00004f8: dbdd4c06 b2f8df58 fd7721fe 73aaebff a482f86f 436da9ff  ..L....X.w!.s......oCm..
	0000510: 361aff05 c4ff8a96 21feeffd 0621fe43 fc6fa79e beadfe31  6.......!....!.C.o.....1
	0000528: fcf8c7d2 fbff8637 fb49ebff 7918ffa9 e470ff6f 2b325eff  .......7.I..y....p.o+2^.
	0000540: dff40704 1d95f67f a38fff2b 7ffe13ce f27f4171 e4ff0cea  ...........+......Aq....
	0000558: 7faca835 f5bfe189 aa73f2f0 9d99cadf dd3e7a61 d2ff67f0  ...5.....s.......>za..g.
	0000570: 364c309e 30c10773 bc348d98 2ee73c77 8562d8d4 17653e61  6L0.0..s.4....<w.b...e>a
	0000588: 7adae6ac c2e655af 107dc2fc f0f366ab 5ad3575b 367d0283  z.....U..}....f.Z.W[6}..
	00005a0: 4e52c27f 838fff2b e53f9634 e33f8ef3 3fb8fed3 8a5a53ff  NR.....+.?.4.?..?....ZS.
	00005b8: 6988ffd4 0cff01f8 a05628e1 bfc1c7ff d5caffb9 223aff87  i........V(.........":..
	00005d0: f17f2b6a 4dfd9f21 fe33e3f9 3fa502c2 01e8852a e17f83e3  ..+jM..!.3..?......*....
	00005e8: bf01ff71 56ff25a8 e63f8cff 5a515bea bf0ce19f 03fe01ff  ...qV.%..?..ZQ[.........
	0000600: 5d56c27f 838fff2c e7bfcac6 7fb87efe b7e490ff 5b515bea  ]V.....,......~.....[Q[.
	0000618: 7f0cf15f 00ff4fe3 7f682dc8 dff6d45d ecae996b befcdb22  ..._..O..h-....]...k..."
	0000630: 82402010 08040281 4020d0ab d37f4e1a 67e600a0 0000      .@ .....@ ....N.g.....
	*/

} // namespace common
} // namespace torc
