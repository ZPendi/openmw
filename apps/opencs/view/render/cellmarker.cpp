#include "cellmarker.hpp"

#include <boost/lexical_cast.hpp>

#include <osg/AutoTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osgText/Text>

void CSVRender::CellMarker::buildMarker()
{
    const int characterSize = 20;

    // Set up marker text containing cell's coordinates.
    osg::ref_ptr<osgText::Text> markerText (new osgText::Text);
    markerText->setBackdropType(osgText::Text::OUTLINE);
    markerText->setLayout(osgText::Text::LEFT_TO_RIGHT);
    markerText->setCharacterSize(characterSize);
    std::string coordinatesText =
        "#" + boost::lexical_cast<std::string>(mCoordinates.getX()) +
        " " + boost::lexical_cast<std::string>(mCoordinates.getY());
    markerText->setText(coordinatesText);

    // Add text to marker node.
    osg::ref_ptr<osg::Geode> geode (new osg::Geode);
    geode->addDrawable(markerText);
    mMarkerNode->addChild(geode);
}

void CSVRender::CellMarker::positionMarker()
{
    const int cellSize = 8192;
    const int markerHeight = 0;

    // Move marker to center of cell.
    int x = (mCoordinates.getX() * cellSize) + (cellSize / 2);
    int y = (mCoordinates.getY() * cellSize) + (cellSize / 2);
    mMarkerNode->setPosition(osg::Vec3f(x, y, markerHeight));
}

CSVRender::CellMarker::CellMarker(
    osg::Group *cellNode,
    const CSMWorld::CellCoordinates& coordinates
) : mCellNode(cellNode),
    mCoordinates(coordinates)
{
    // Set up node for cell marker.
    mMarkerNode = new osg::AutoTransform();
    mMarkerNode->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
    mMarkerNode->setAutoScaleToScreen(true);
    mMarkerNode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

    mCellNode->addChild(mMarkerNode);

    buildMarker();
    positionMarker();
}

CSVRender::CellMarker::~CellMarker()
{
    mCellNode->removeChild(mMarkerNode);
}
