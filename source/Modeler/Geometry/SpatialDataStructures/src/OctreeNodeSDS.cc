#include "OctreeNodeSDS.hh"

static const unsigned int OCTREE_NODE_DEPTH_MAX = 40;

/// \param pDelimitation the spatial delimitation (bounding box) of the octree
/// \param pDeth the depth of the node
/// \param pParent the parent node. Null if is the top node
/// \ûparam pMaxNbAgent the maximal number of agent the node can include
OctreeNodeSDS::OctreeNodeSDS(
	BoundingBox<Point_3> pDelimitation,
	unsigned int pDeth,
	const OctreeNodeSDS* pParent,
	unsigned int pMaxNbAgt,
	double pExtensionLength):
	OctreeNode(pDelimitation, pDeth, static_cast<const OctreeNode*>(pParent), pMaxNbAgt),
	_extensionLength(pExtensionLength)
{
	assert(pExtensionLength >= 0.);
	// if extensionLength is 0 ( default value ), try to pick the one of the parent
	if(_extensionLength == 0) {
		if(pParent)
			_extensionLength = pParent->getExtensionLength();
	}

	// make sur we are not going other numeric limits for delimiation
	double xBottomExtension = pDelimitation.getBottomLeft().x();
	if(xBottomExtension != std::numeric_limits<double>::min())
		xBottomExtension -= pExtensionLength;

	double yBottomExtension = pDelimitation.getBottomLeft().y();
	if(yBottomExtension != std::numeric_limits<double>::min())
		yBottomExtension -= pExtensionLength;

	double zBottomExtension = pDelimitation.getBottomLeft().z();
	if(zBottomExtension != std::numeric_limits<double>::min())
		zBottomExtension -= pExtensionLength;

	double xTopExtension = pDelimitation.getTopRight().x();
	if(xTopExtension != std::numeric_limits<double>::max())
		xTopExtension += pExtensionLength;

	double yTopExtension = pDelimitation.getTopRight().y();
	if(yTopExtension != std::numeric_limits<double>::max())
		yTopExtension += pExtensionLength;

	double zTopExtension = pDelimitation.getTopRight().z();
	if(zTopExtension != std::numeric_limits<double>::max())
		zTopExtension += pExtensionLength;

	Point_3 extendedBottom( yBottomExtension, yBottomExtension, zBottomExtension );
	Point_3 extendedTop( xTopExtension, yTopExtension, zTopExtension );

	delimitationExtended = BoundingBox<Point_3>(extendedBottom, extendedTop);
}

bool OctreeNodeSDS::shouldBeAdd(const t_SpatialableAgent_3* pSpa) const {
	return (pSpa->isIn(&delimitation) || pSpa->cross(&delimitation) || pSpa->cross(&delimitationExtended) );
}

