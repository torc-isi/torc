
#ifndef TORC_PLACER_DEVICESITEPIN_HPP
#define TORC_PLACER_DEVICESITEPIN_HPP

#include "torc/architecture/Tilewire.hpp"
#include <string>

namespace torc {
namespace placer {
	class DeviceSitePin
	{
	// types
		typedef architecture::Tilewire Tilewire;
	protected:
		std::string& mName;
		
		Tilewire mTilewire;
		int mRow; // for the sitepin, we have actual row and column locations for distance calculation
		int mCol;
		//int mTileOffset; // these offsets tell you where the pin actually lives relative to the tile index of the site
		
	public:
		DeviceSitePin(std::string& inName, int inRow, int inCol) : 
			mName(inName), mRow(inRow), mCol(inCol) {}
		~DeviceSitePin() {}
		std::string& getName() {
			return mName;
		}
		int getRow() {
			return mRow;
		}
		int getCol() {
			return mCol;
		}
		Tilewire getTilewire() {
			return mTilewire;
		}
		
	}; //class DeviceSitePin
} // namespace placer
} // namespace torc
#endif // TORC_PLACER_DEVICESITEPIN_HPP



