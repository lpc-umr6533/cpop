#include "Cutter_3.hh"

Cutter_3::Cutter_3(std::vector<t_SpatialableAgent_3*> pCells) {
	_uncutCells = pCells;
}

Cutter_3::Cutter_3(std::set<t_SpatialableAgent_3*> pCells) {
	_uncutCells.insert(_uncutCells.end(), pCells.begin(), pCells.end());
}

/// \brief will removed the cells not on the range requested from the toCut vector.
/// \details will move the one near the limit in the buffer and move the one put to cutted vector
/// \param dir 			The direction to process the cut
/// \param val 			The value from which to strat the cut on the given axis
/// \param bufferWidth 	The width of the buffer to create ( >= 0. )
/// \param toCut 		The cells candidates to cut
/// \param buffer 		Where to set buffered cells
/// \param cutted 		Where to set cutted cells
/// \param cutBuffer 	true if we are cutting the buffer, then we don't reinsert cell on buffer but on cutted
void Cutter_3::processCut(
	Direction dir,
	double val,
	double bufferWidth,
	std::vector<t_SpatialableAgent_3*>* toCut,
	std::vector<t_SpatialableAgent_3*>* buffer,
	std::vector<t_SpatialableAgent_3*>* cutted,
	bool cutBuffer
)
{
	auto itCell = toCut->begin();
	while(itCell != toCut->end()) {
		bool lCut = false;
		bool moveToBuffer = false;

		switch(dir) {
			case BOTTOM:
			{
				if((*itCell)->getPosition().z() < val )
				{
					if((*itCell)->getPosition().z() < (val - bufferWidth)) {
						lCut = true;
					} else {
						moveToBuffer = true;
					}
				}
				break;
			}
			case TOP:
			{
				if((*itCell)->getPosition().z() > val) {
					if( (*itCell)->getPosition().z() > (val + bufferWidth)) {
						lCut = true;
					} else {
						moveToBuffer = true;
					}
				}
				break;
			}
			case LEFT:
			{
				// if we are supposed to cut
				if((*itCell)->getPosition().x() < val )
				{
					// if not in the buffer
					if((*itCell)->getPosition().x() < (val - bufferWidth)) {
						lCut = true;
					} else {
						moveToBuffer = true;
					}
				}
				break;
			}
			case RIGHT:
			{
				if((*itCell)->getPosition().x() > val)
				{
					if( (*itCell)->getPosition().x() > (val + bufferWidth)) {
						lCut = true;
					} else {
						moveToBuffer = true;
					}
				}
				break;
			}
			case BACK:
			{
				if((*itCell)->getPosition().y() < val)
				{
					if((*itCell)->getPosition().y() < (val - bufferWidth)) {
						lCut = true;
					} else {
						moveToBuffer = true;
					}
				}
				break;
			}
			case FRONT:
			{
				if((*itCell)->getPosition().y() > val) {
					if((*itCell)->getPosition().y() > (val + bufferWidth)) {
						lCut = true;
					} else {
						moveToBuffer = true;
					}
				}
				break;
			}
		}
		// update vectors
		if(lCut) {
			if(cutted) {
				cutted->push_back(*itCell);
			}

			toCut->erase(itCell);
		} else if(moveToBuffer) {
			if(cutBuffer) {
				++itCell;
			} else {
				if(buffer) {
					buffer->push_back(*itCell);
				}
				toCut->erase(itCell);
			}
		} else {
			++itCell;
		}
	}
}

/// \param dir 			The direction where to remove cells
/// \param val 			The value from where to start the removing
/// \param bufferWidth 	The value from where the buffer start
void Cutter_3::cut(Direction dir, double val, double bufferWidth) {
	// update uncuted cells
	processCut(dir, val, bufferWidth, &_uncutCells, & _cellsOnBuffer, &_cutCells);

	// update cells on buffer
	std::vector<t_SpatialableAgent_3*> newBuffer;
	processCut(dir, val, bufferWidth, &_cellsOnBuffer, nullptr, &_cutCells, true);
	// cellsOnBuffer = newBuffer;
}

/// \param pColor The color to set to the targetted cells
void Cutter_3::setColorToCellOnBuffer(CGAL::Color pColor) {
	for(auto& itCell : _cellsOnBuffer) {
		static_cast<Agent*>(itCell)->setColor(pColor);
	}
}

/// \param pColor The color to set to the targetted cells
void Cutter_3::setColorToUncuttedCell(CGAL::Color pColor) {
	for(auto & _uncutCell : _uncutCells) {
		static_cast<Agent*>(_uncutCell)->setColor(pColor);
	}
}

/// \param pColor The color to set to the targetted cells
void Cutter_3::setColorToCuttedCell(CGAL::Color pColor) {
	for(auto & _cutCell : _cutCells) {
		static_cast<Agent*>(_cutCell)->setColor(pColor);
	}
}
