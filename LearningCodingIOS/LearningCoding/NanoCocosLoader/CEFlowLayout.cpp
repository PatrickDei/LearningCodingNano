//
//  CEFlowLayout.cpp
//  NCLCpp
//
//  Created by Bruno on 7/1/13.
//
//

#include "CEFlowLayout.h"

using namespace cocos2d;
using namespace std;

int clampi (int value, int min_inclusive, int max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        CC_SWAP (min_inclusive, max_inclusive, int);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;
}

CCNodeRGBA *CCNodeRGBACreate ()
{
    CCNodeRGBA *node = new CCNodeRGBA ();
    node->init ();
    node->autorelease ();
    return node;
}

#define SIZE_0 CCSize (0, 0)
#define POINT_0 CCPoint (0, 0)

#define ccpIsBetween(target, lower, upper)                                                                             \
    (((target.x) >= (lower.x)) && ((target.x) <= (upper.x)) && ((target.y) >= (lower.y)) && ((target.y) <= (upper.y)))

#define ccpIsBetweenf(target, l, u)                                                                                    \
    (((target.x) >= (l)) && ((target.x) <= (u)) && ((target.y) >= (l)) && ((target.y) <= (u)))

#define floatBetween(target, l, u) ((target) >= (l) && (target) <= (u))

#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? (-1) : 0))

#define ccpIsZero(point) ((point.x == 0) && (point.y == 0))

#define sizeToPoint(size) (ccp ((size).width, (size).height))

#define ccpClampf(point, minx, maxx, miny, maxy) (ccp (clampf ((point).x, minx, maxx), clampf ((point).y, miny, maxy)))

#define decPart(x) ((x)-floorf ((x)))

#define pagingDirection(x, tresh) ((x) > (tresh) ? -1 : ((x) < (-tresh) ? 1 : 0))

#define DEBUG_WITH_COLOR 0

#define MIN_ROWS 0
#define MAX_ROWS 10000
#define MIN_COLUMNS 0
#define MAX_COLUMNS 10000
#define PAGING_TRESHOLD_RATIO 0.5
#define PAGING_SPEED ccp (300, 300)
#define SCROLL_ACC ccp (50, 50)
#define SCROLL_STOP_FACTOR 0.5
#define SCROLL_SENSITIVITY 4
#define SCROLL_SMOOTHNESS 0.5
#define E_CONST 2.71828

#define D_LEFT -1
#define D_RIGHT 1
#define D_DOWN -1
#define D_UP 1

#define FLOAT_0_TRESH 0.01

#define SNAP_ANIMATION_SPEED ccp (2000.0f, 2000.0f)

#pragma mark -
#pragma mark Init and Cocos related

CEFlowLayout::CEFlowLayout ()
    : lock (false), lockFilter (vector<int> ()), isTouching (false), content (NULL), midContent (NULL),
      tagMap (map<int, CCNode *> ()), forceEndTouch (false), forceEndScroll (false), touchedMenuItemSelectDelay (0.05),
      totalSize (SIZE_0), cellSize (SIZE_0), worldStart (POINT_0), worldEnd (POINT_0), rowHeights (vector<float> ()),
      columnWidths (vector<float> ()), totalRows (MIN_ROWS), totalColumns (MIN_COLUMNS), cellIndexLeftDown (POINT_0),
      cellIndexRightUp (POINT_0), keepInvisibleNodes (false), hideInvisibleNodes (true), loadAll (false),
      touchMoved (false), touchStart (CCPoint (-1, -1)), swipeAction (kSwipeScrolling), allowTableMoveLR (true),
      allowTableMoveUD (true), touchRadiusTreshold (8), touchedMenuItem (NULL), touchedMenuItemState (false),
      activateTouchedMenuItems (true), pagingTreshold (POINT_0), pageSize (SIZE_0), pageSpeed (PAGING_SPEED),
      pageTarget (POINT_0), pagingScheduled (false), scrollSensitivity (SCROLL_SENSITIVITY),
      scrollSmoothness (SCROLL_SMOOTHNESS), scrollImpulse (POINT_0), scrollDuration (0),
      scrollStopFactor (SCROLL_STOP_FACTOR), scrollingScheduled (false), easeLeftRight (true), easeUpDown (true),
      borderOffset (CCPoint (50, 50)), boundUp (false), boundDown (false), boundLeft (false), boundRight (false),
      dataSource (NULL), delegate (NULL), shouldSnapVertically (false), shouldSnapHorizontally (false),
      snapSpeed (SNAP_ANIMATION_SPEED), CCLayerRGBA ()
{
    // Really long init list :(
    setPosition (POINT_0);
    setAnchorPoint (POINT_0);
    setTouchMode (kCCTouchesOneByOne);
    setTouchEnabled (false);
}

CEFlowLayout::~CEFlowLayout () {}

bool CEFlowLayout::init (CCSize size, CEFlowLayoutDataSource *dataSource, CEFlowLayoutDelegate *delegate, bool canTouch)
{
    bool success = CCLayer::init ();

    if (success) {
        midContent = CCNodeRGBACreate ();
        addChild (midContent);

        content = CCNodeRGBACreate ();
        midContent->addChild (content);

        this->CCNode::setContentSize (size);
        setDataSource (dataSource);
        setDelegate (delegate);

        setTouchEnabled (true);

        pageSize = size;
    }

    return success;
}

CEFlowLayout *CEFlowLayout::create (CCSize size, CEFlowLayoutDataSource *dataSource, CEFlowLayoutDelegate *delegate)
{
    CEFlowLayout *pRet = new CEFlowLayout ();
    if (pRet && pRet->init (size, dataSource, delegate)) {
        pRet->autorelease ();
        return pRet;
    }
    delete pRet;
    pRet = NULL;
    return NULL;
}

void CEFlowLayout::detectNodeAndIndex (CCPoint location, CCNode **node, CCPoint *index)
{
    location.y = totalSize.height - location.y;
    index->x = -1;
    index->y = -1;
    if (!scrollingScheduled && !pagingScheduled &&
        !(location.x < 0 || location.x > totalSize.width || location.y < 0 || location.y > totalSize.height)) {
        int column = -1;
        do {
            column++;
            location.x -= columnWidths[column];
        } while (location.x > 0 && column < totalColumns - 1);

        int row = -1;
        do {
            row++;
            location.y -= rowHeights[row];
        } while (location.y > 0 && row < totalRows - 1);

        if (location.y > 0 || location.x > 0) {
            return;
        }

        index->x = column;
        index->y = row;

        if (node) {
            int cellTag = tagForIndex (row, column);
            (*node) = tagMap[cellTag];
        }
    }
}

CCMenuItem *findTouchedMenuItem (CCNode *node, CCPoint point)
{
    if (!node->isVisible ()) {
        return NULL;
    }

    CCObject *item;
    CCArray *children = node->getChildren ();
    if (children) {
        for (int i = children->count () - 1; i >= 0; i--) {
            item = children->objectAtIndex (i);
            CCMenuItem *mItem = findTouchedMenuItem ((CCNode *)item, point);
            if (mItem != NULL) {
                return mItem;
            }
        }
    }

    CCMenuItem *nodeAsMenuItem = dynamic_cast<CCMenuItem *> (node);
    if (nodeAsMenuItem && nodeAsMenuItem->isVisible () && nodeAsMenuItem->isEnabled ()) {

        CCPoint cPoint = nodeAsMenuItem->convertToNodeSpace (point);
        CCRect rect = CCRectMake (0, 0, 0, 0);
        float scale = 1; // ccpLength( nodeAsMenuItem->convertToNodeSpace(CCPoint(0, 1) ));
        rect.size = nodeAsMenuItem->getContentSize () * scale;
        if (rect.containsPoint (cPoint)) {
            return nodeAsMenuItem;
        }
    }

    return NULL;
}

void CEFlowLayout::selectTouchedMenuItem ()
{
    unschedule (schedule_selector (CEFlowLayout::selectTouchedMenuItem));
    if (!touchMoved && touchedMenuItem && !forceEndTouch && !forceEndScroll && !lock) {
        if (touchedMenuItemState)
            touchedMenuItem->unselected ();
        else
            touchedMenuItem->selected ();
    }
}

void CEFlowLayout::setTouchEnabled (bool enabled)
{
    CCLayerRGBA::setTouchEnabled (enabled);

    if (isTouching && enabled == false) {
        forceTouchEnd ();
        ccTouchEnded (nullptr, nullptr);
    }
}

bool CEFlowLayout::ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_bVisible || !m_bTouchEnabled || forceEndTouch || forceEndScroll || isTouching) {
        forceEndTouch = false;
        forceEndScroll = false;
        return false;
    }
    isTouching = true;

    CCPoint touchLocation = CCDirector::sharedDirector ()->convertToGL (pTouch->getLocationInView ());
    touchMoved = false;

    bool touchValid = false;
    CCRect touchRectangle =
        CCRectMake (worldStart.x, worldStart.y, (worldEnd.x - worldStart.x), (worldEnd.y - worldStart.y));
    if (touchRectangle.containsPoint (touchLocation)) {
        touchValid = true;

        unschedulePaging ();
        unscheduleScrolling ();

        CCNode *node = NULL;
        detectNodeAndIndex (content->convertToNodeSpace (touchLocation) - getAnchorOffset (), &node, &tapIndex);
        if (node) {
            touchedMenuItem = findTouchedMenuItem (node, touchLocation);
            if (touchedMenuItem && touchedMenuItem->isEnabled ()) {
                touchedMenuItem->retain ();
                touchedMenuItemState = touchedMenuItem->isSelected ();
                if (touchedMenuItemSelectDelay > 0) {

                    scheduleOnce (schedule_selector (CEFlowLayout::selectTouchedMenuItem), touchedMenuItemSelectDelay);
                } else {
                    selectTouchedMenuItem ();
                }
            } else {
                touchedMenuItem = NULL;
            }
        }

        if (!lock /*&& (allowTableMoveLR || allowTableMoveUD)*/) {
            touchStart = touchLocation;
        }

        lastTouchChange = time (NULL);
    }
    isTouching = touchValid;
    if (touchValid && delegate) {
        CCNode *node = NULL;
        detectNodeAndIndex (content->convertToNodeSpace (touchLocation) - getAnchorOffset (), &node, &tapIndex);
        delegate->layoutTouchBegan (this, tapIndex.y, tapIndex.x, node);
    }
    return touchValid;
}

void CEFlowLayout::moveWithImpulse (cocos2d::CCPoint impulse)
{
    scrollImpulse = scrollImpulse + impulse;
    scheduleScrolling ();
}

void CEFlowLayout::ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = CCDirector::sharedDirector ()->convertToGL (pTouch->getLocationInView ());

    if (forceEndTouch || forceEndScroll) {
        if (delegate) {
            CCNode *node = NULL;
            detectNodeAndIndex (content->convertToNodeSpace (touchLocation) - getAnchorOffset (), &node, &tapIndex);
            delegate->layoutTouchMoved (this, pTouch, pEvent, node);
        }
        return;
    }

    if (!touchMoved) {
        if (ccpDistanceSQ (touchLocation, touchStart) < touchRadiusTreshold * touchRadiusTreshold) {
            return;
        } else {
            touchStart = touchLocation;
            if (touchedMenuItem) {
                if (touchedMenuItemState)
                    touchedMenuItem->selected ();
                else
                    touchedMenuItem->unselected ();
                touchedMenuItem->release ();
                touchedMenuItem = NULL;
            }
        }
    }

    touchMoved = true;

    CCPoint offset = ccpSub (touchLocation, touchStart);
    offset.x = allowTableMoveLR ? offset.x : 0;
    offset.y = allowTableMoveUD ? offset.y : 0;

    if (delegate) {
        CCNode *node = NULL;
        detectNodeAndIndex (content->convertToNodeSpace (touchLocation) - getAnchorOffset (), &node, &tapIndex);
        delegate->layoutTouchMoved (this, pTouch, pEvent, node);
    }
    if (lock || !(allowTableMoveLR || allowTableMoveUD)) {
        return;
    }

    time_t timeNow = time (NULL);
    if (swipeAction == kSwipeScrolling) {
        float diff = difftime (timeNow, lastTouchChange);

        float factor =
            ccpIsBetweenf (offset, -scrollSensitivity, scrollSensitivity) ? 0.1 : (diff < 1 ? 0.9 : 1 / diff);
        // CCLOG("mult with %.2f", factor);
        scrollImpulse = ccpAdd (ccpMult (scrollImpulse, factor), offset);

        // scrollImpulse = ccpAdd(scrollImpulse,offset);
        // scrollDuration = 0;
        moveTableWithOffset (offset);
        touchStart = touchLocation;
    } else {
        CCPoint pageChange = CCPointZero;
        pageChange.x = pagingDirection (offset.x, pagingTreshold.x);
        pageChange.y = pagingDirection (offset.y, pagingTreshold.y);
        if (!ccpIsZero (pageChange)) {
            if (ccpIsZero (pageTarget)) {
                pageForContentSize (pageChange.x, pageChange.y);
            }
            touchStart = touchLocation;
        }
    }
    lastTouchChange = timeNow;
}

bool CEFlowLayout::indexIsLockValid (CCPoint index)
{
    return !lock || find (lockFilter.begin (), lockFilter.end (), tagForIndex (index.y, index.x)) != lockFilter.end ();
}

int CEFlowLayout::tagForIndex (int row, int column) { return column * totalRows + row; }

void CEFlowLayout::ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent)
{
    if (!forceEndTouch && !forceEndScroll) {

        CCPoint touchLocation = CCDirector::sharedDirector ()->convertToGL (pTouch->getLocationInView ());
        bool didActivate = false;
        if (touchedMenuItem) {
            if (activateTouchedMenuItems && indexIsLockValid (tapIndex)) {
                touchedMenuItem->activate ();
                didActivate = true;
            }
            if (touchedMenuItemState)
                touchedMenuItem->selected ();
            else
                touchedMenuItem->unselected ();
            unschedule (schedule_selector (CEFlowLayout::selectTouchedMenuItem));
        }

        if (!touchMoved) {
            CCPoint tableOffset = getTableOffset ();
            if ((totalSize.width < m_obContentSize.width && tableOffset.x != 0) ||
                (totalSize.height < m_obContentSize.height && tableOffset.y != 0)) {
                scrollImpulse = CCPoint (0, 0);
                scheduleScrolling ();
            }
            if (!pagingScheduled && delegate && !didActivate) {
                touchLocation = content->convertToNodeSpace (touchLocation);
                CCPoint percTouch;
                CCPoint index = indexInTableSpace (touchLocation, &percTouch);
                if (index.x >= 0 && index.y >= 0) {
                    CCNode *touchedNode = getNode (index.y, index.x);
                    if (touchedNode && indexIsLockValid (index)) {
                        delegate->layoutElementTouched (this, (int)index.y, (int)index.x,
                                                        CCPoint (percTouch.y, percTouch.x), touchedNode, pTouch,
                                                        pEvent);
                    }
                }
            }
        } else {
            float fac = powf (M_E, -difftime (time (NULL), lastTouchChange));
            scrollImpulse.x *= fac;
            scrollImpulse.x *= fac;

            if (ccpIsBetweenf (scrollImpulse, -scrollSensitivity, scrollSensitivity)) {
                scrollImpulse = ccp (0, 0);
            }
            scheduleScrolling ();
        }
    }
    touchMoved = false;
    touchStart = CCPointZero;
    if (touchedMenuItem) {
        touchedMenuItem->release ();
        touchedMenuItem = NULL;
    }
    isTouching = false;

    if (delegate) {
        delegate->layoutTouchEnded (this, pTouch, pEvent);
    }
    forceEndTouch = false;
    forceEndScroll = false;
}

void CEFlowLayout::ccTouchCancelled (CCTouch *pTouch, CCEvent *pEvent)
{
    // Nothing
    forceTouchEnd ();
    ccTouchEnded (pTouch, pEvent);
}

void CEFlowLayout::forceTouchEnd ()
{
    if (!forceEndTouch && !forceEndScroll && isTouching) {
        unschedule (schedule_selector (CEFlowLayout::selectTouchedMenuItem));
        forceEndTouch = true;
        if (touchedMenuItem) {
            if (touchedMenuItemState)
                touchedMenuItem->selected ();
            else
                touchedMenuItem->unselected ();
        }
        //        if( delegate ){
        //            delegate->layoutTouchEnded(this, nullptr, nullptr);
        //        }
    }
}

void CEFlowLayout::forceUnscheduleScrolling ()
{
    if (!forceEndTouch && !forceEndScroll) {
        forceEndScroll = true;
        unscheduleScrolling ();
    }
}

void CEFlowLayout::visit ()
{
    if (!m_bVisible) {
        return;
    }

    CCPoint zeroPoint = midContent->convertToWorldSpace (CCPointZero);
    CCPoint onePoint = midContent->convertToWorldSpace (CCPoint (1, 0));
    CCPoint anchorOffset = getAnchorOffset () * ccpLength (onePoint - zeroPoint);

    glEnable (GL_SCISSOR_TEST);
    worldStart = midContent->convertToWorldSpace (CCPointZero) + anchorOffset;
    worldEnd = midContent->convertToWorldSpace (sizeToPoint (m_obContentSize)) + anchorOffset;

    glScissor (worldStart.x, worldStart.y, (worldEnd.x - worldStart.x), (worldEnd.y - worldStart.y));

    CCLayer::visit ();
    glDisable (GL_SCISSOR_TEST);
}

void CEFlowLayout::draw ()
{
    /*    ccDrawColor4B(255, 0, 0, 255);
     glLineWidth(2);

     CCPoint lowCorner = midContent->getPosition() + getAnchorOffset();
     CCPoint topCorner = lowCorner + CCPoint( m_obContentSize.width, m_obContentSize.height );

     ccDrawRect( lowCorner, topCorner );*/
}

void CEFlowLayout::setAnchorPoint (const CCPoint &anchor)
{
    CCNode::setAnchorPoint (anchor);
    setPosition (m_obPosition);
}

void CEFlowLayout::setPosition (const CCPoint &position)
{
    CCNode *parent = getParent ();
    if (parent) {
        worldStart = midContent->convertToWorldSpace (CCPointZero) + getAnchorOffset ();
        worldEnd = midContent->convertToWorldSpace (sizeToPoint (m_obContentSize)) + getAnchorOffset ();
    }
    CCNode::setPosition (position);
}

void CEFlowLayout::setParent (CCNode *parent)
{
    CCNode::setParent (parent);
    setPosition (m_obPosition);
}

void CEFlowLayout::setContentSize (const CCSize &size)
{
    if (content) {
        CCNode::setContentSize (size);
        setPageSize (size);
        setAnchorPoint (m_obAnchorPoint);
        updateContent ();
    }
}

void CEFlowLayout::setScale (float scale)
{
    CCNode::setScale (scale);
    setAnchorPoint (m_obAnchorPoint);
}

#pragma mark -
#pragma mark Private methods

CCPoint CEFlowLayout::getAnchorOffset ()
{
    return CCPoint (-m_obAnchorPoint.x * m_obContentSize.width / m_fScaleX,
                    -m_obAnchorPoint.y * m_obContentSize.height / m_fScaleY);
}

void CEFlowLayout::setDimensions (int rows, int cols)
{
    totalRows = clampi (rows, MIN_ROWS, MAX_ROWS);
    totalColumns = clampi (cols, MIN_COLUMNS, MAX_COLUMNS);
    updateDimensions ();
    updateContent ();
}

void CEFlowLayout::updateDimensions ()
{
    if (dataSource) {
        totalColumns = dataSource->getLayoutNumberOfColumns (this);
        totalRows = dataSource->getLayoutNumberOfRows (this);
    }
    if (totalColumns < 0 || totalRows < 0) {
        return;
    }

    rowHeights.clear ();
    rowHeights.resize (totalRows);

    columnWidths.clear ();
    columnWidths.resize (totalColumns);

    totalSize.height = 0;
    totalSize.width = 0;
    for (int i = 0; i < totalColumns; i++) {
        float newWidth = dataSource ? dataSource->getLayoutColumnWidth (this, i) : cellSize.width;
        setColumnWidth (i, newWidth);
        totalSize.width += newWidth;
    }
    for (int i = 0; i < totalRows; i++) {
        float newHeight = dataSource ? dataSource->getLayoutRowHeight (this, i) : cellSize.height;
        setRowHeight (i, newHeight);
        totalSize.height += newHeight;
    }

    /*    CCSize contentSize = getContentSize();
     bool updatedContentSize = false;
     if( totalSize.height < contentSize.height )
     {
     contentSize.height = totalSize.height;
     updatedContentSize = true;
     }
     if( totalSize.width < contentSize.width )
     {
     contentSize.width = totalSize.width;
     updatedContentSize = true;
     }
     if (updatedContentSize) {
     setContentSize(contentSize);
     }*/
}

void CEFlowLayout::updateContent ()
{
    if (!content)
        return;

    CCPoint viewMin = ccpNeg (content->getPosition ());
    CCPoint viewMax = ccpAdd (viewMin, ccp (m_obContentSize.width, m_obContentSize.height));
    viewMin = ccpClampf (viewMin, 0, totalSize.width, 0, totalSize.height);
    viewMax = ccpClampf (viewMax, 0, totalSize.width, 0, totalSize.height);

    if (totalColumns == 0 || totalRows == 0) {
        content->removeAllChildren ();
        return;
    }

    int i = 0;
    float cumulativeWidth = columnWidths[i];
    while (i + 1 < totalColumns && (cumulativeWidth) < viewMin.x) {
        cumulativeWidth += columnWidths[i + 1];
        i++;
    };
    if (i >= totalColumns) {
        i = totalColumns - 1;
    } else {
        cumulativeWidth -= columnWidths[i];
    }

    int iCount = 0;
    float totalWidth = cumulativeWidth;
    while (i + iCount < totalColumns && totalWidth + columnWidths[i + iCount] <= viewMax.x) {
        totalWidth += columnWidths[i + iCount];
        iCount++;
    }
    if (i + iCount >= totalColumns) {
        iCount--;
    }

    int j = 0;
    float cumulativeHeight = totalSize.height;
    while (j < totalRows && (cumulativeHeight - rowHeights[j]) > viewMin.y) {
        cumulativeHeight -= rowHeights[j];
        j++;
    }
    if (j >= totalRows) {
        j = totalRows - 1;
    } else {
        cumulativeHeight -= rowHeights[j];
    }

    int jCount = 0;
    float totalHeigh = cumulativeHeight;
    while (j - jCount > 0 && (totalHeigh + rowHeights[j - jCount]) <= viewMax.y) {
        totalHeigh += rowHeights[j - jCount];
        jCount++;
    }
    if (j - jCount < 0) {
        jCount = j;
    }

    set<int> keySet = set<int> ();
    for (auto pair : tagMap) {
        keySet.insert (pair.first);
    }

    bool loadAllCells = (keepInvisibleNodes && !hideInvisibleNodes) || loadAll;
    if (loadAllCells) {
        cumulativeWidth = 0.0f;
        cumulativeHeight = 0.0f;
    }

    float initialCumulativeWidth = cumulativeWidth;
    int firstColumn = loadAllCells ? 0 : i;
    int lastColumn = loadAllCells ? totalColumns - 1 : i + iCount;
    int firstRow = loadAllCells ? 0 : j - jCount;
    int lastRow = loadAllCells ? totalRows - 1 : j;

    cellIndexLeftDown.x = firstColumn;
    cellIndexLeftDown.y = lastRow;
    cellIndexRightUp.x = lastColumn;
    cellIndexRightUp.y = firstRow;
    updateBoundaryStates ();

    // Rows go backwards (8,7,6,...)
    // Columns go forwards (1,2,3,...)
    for (int row = lastRow; row >= firstRow; row--) {
        cumulativeWidth = initialCumulativeWidth;
        for (int column = firstColumn; column <= lastColumn; column++) {
            int cellTag = tagForIndex (row, column);
            CCNode *cell = getNodeInternal (row, column, cellTag,
                                            CCPoint (cumulativeWidth, cumulativeHeight) + getAnchorOffset ());
            if (NULL != cell) {
                keySet.erase (cellTag);
                cell->setVisible (true);
                // cell->setAnchorPoint( CCPointZero );
                // cell->setPosition( CCPoint(cumulativeWidth, cumulativeHeight) + getAnchorOffset() );
            }
            cumulativeWidth += columnWidths[column];
        }
        cumulativeHeight += rowHeights[row];
    }

    for (auto key : keySet) {
        if (keepInvisibleNodes == false) {
            if (tagMap[key]) {
                if (delegate && !lock) {
                    delegate->layoutElementRemoved (this, tagMap[key]);
                }
                tagMap[key]->removeFromParentAndCleanup (true);
            }
            tagMap.erase (key);
        } else {
            if (hideInvisibleNodes && tagMap[key]) {
                tagMap[key]->setVisible (false);
            }
        }
    }
}

void CEFlowLayout::moveTableWithOffset (CCPoint newOffset)
{
    CCPoint border;
    border.x = easeLeftRight && totalSize.width >= m_obContentSize.width ? borderOffset.x : 0;
    border.y = easeUpDown && totalSize.height >= m_obContentSize.height ? borderOffset.y : 0;

    CCPoint oldPosition = content->getPosition ();
    CCPoint newPosition = ccpAdd (content->getPosition (), newOffset);
    newPosition = ccpClampf (newPosition, -totalSize.width + m_obContentSize.width - border.x, border.x,
                             -totalSize.height + m_obContentSize.height - border.y, border.y);

    content->setPosition (newPosition);
    updateContent ();

    if (delegate) {
        delegate->layoutMovedWithOffset (this, ccpSub (newPosition, oldPosition));
    }
}

CCNode *CEFlowLayout::getNodeInternal (int row, int column, int tag, CCPoint position)
{
    if (row < 0 || row >= totalRows || column < 0 || column >= totalColumns) {
        return NULL;
    }

    CCNode *cell = tagMap[tag];
    if (NULL == cell) {
        cell = dataSource ? dataSource->getLayoutElement (this, row, column) : NULL;
        if (cell) {
            tagMap[tag] = cell;
            cell->setPosition (cell->getPosition () + position);
            content->addChild (cell);
        }
    }
    return cell;
}

void CEFlowLayout::updateBoundaryStates ()
{
    CCPoint contentPosition = content->getPosition ();
    // Update bottom boundary
    if (contentPosition.y >= -5) {
        if (!boundDown) {
            boundDown = true;
            notifyDelegateOfScrollToBoundary (kSideDown);
        }
    } else {
        if (boundDown) {
            boundDown = false;
            notifyDelegateOfUnscrollFromBoundary (kSideDown);
        }
    }

    // Update top boundary
    if (contentPosition.y < (-totalSize.height + m_obContentSize.height + 5)) {
        if (!boundUp) {
            boundUp = true;
            notifyDelegateOfScrollToBoundary (kSideUp);
        }
    } else {
        if (boundUp) {
            boundUp = false;
            notifyDelegateOfUnscrollFromBoundary (kSideUp);
        }
    }

    // Update left boundary
    if (contentPosition.x >= -5) {
        if (!boundLeft) {
            boundLeft = true;
            notifyDelegateOfScrollToBoundary (kSideLeft);
        }
    } else {
        if (boundLeft) {
            boundLeft = false;
            notifyDelegateOfUnscrollFromBoundary (kSideLeft);
        }
    }
    // Update right boundary
    if (contentPosition.x < (-totalSize.width + m_obContentSize.width + 5)) {
        if (!boundRight) {
            boundRight = true;
            notifyDelegateOfScrollToBoundary (kSideRight);
        }
    } else {
        if (boundRight) {
            boundRight = false;
            notifyDelegateOfUnscrollFromBoundary (kSideRight);
        }
    }
}

void CEFlowLayout::notifyDelegateOfScrollToBoundary (kCEFlowLayoutSide side)
{
    if (delegate && !lock) {
        delegate->layoutScrolledToBoundary (this, side);
    }
}

void CEFlowLayout::notifyDelegateOfUnscrollFromBoundary (kCEFlowLayoutSide side)
{
    if (delegate && !lock) {
        delegate->layoutUnscrolledFromBoundary (this, side);
    }
}

cocos2d::CCPoint CEFlowLayout::indexInTableSpace (cocos2d::CCPoint location, cocos2d::CCPoint *cellPercentage)
{
    if ((location.x < 0 || location.x > totalSize.width || location.y < 0 || location.y > totalSize.height)) {
        return CCPoint (-1, -1);
    }

    int column = -1;
    do {
        column++;
        location.x -= columnWidths[column];
    } while (location.x > 0 && column < totalColumns - 1);

    int row = -1;
    do {
        row++;
        location.y -= rowHeights[row];
    } while (location.y > 0 && row < totalRows - 1);

    if (cellPercentage) {
        cellPercentage->x = (location.y + rowHeights[row]) / rowHeights[row];
        cellPercentage->y = (location.x + columnWidths[column]) / columnWidths[column];
    }

    return CCPoint (column, totalRows - row - 1);
}

#pragma mark -
#pragma mark Scrolling Update

void CEFlowLayout::scheduleScrolling ()
{
    if (!scrollingScheduled && !pagingScheduled) {
        scrollingScheduled = true;
        schedule (schedule_selector (CEFlowLayout::updateForScrolling));
    }
}

void CEFlowLayout::unscheduleScrolling ()
{
    if (scrollingScheduled) {
        unschedule (schedule_selector (CEFlowLayout::updateForScrolling));
        scrollingScheduled = false;
        scrollDuration = 0;
        scrollImpulse = CCPoint (0, 0);
        // CCLOG("Unschedule scrolling");
    }
}

cocos2d::CCPoint CEFlowLayout::getTableOffset ()
{
    CCPoint cPos = content->getPosition ();
    CCSize sizeDif = CCSize (totalSize.width - m_obContentSize.width, totalSize.height - m_obContentSize.height);
    CCPoint offset;
    if (sizeDif.width < 0) {
        offset.x = -cPos.x;
    } else {
        offset.x = cPos.x > 0 ? -cPos.x : (-cPos.x > (sizeDif.width) ? -cPos.x - sizeDif.width : 0);
    }
    if (sizeDif.height < 0) {
        offset.y = -sizeDif.height - cPos.y;
    } else {
        offset.y = cPos.y > 0 ? -cPos.y : (-cPos.y > (sizeDif.height) ? -cPos.y - sizeDif.height : 0);
    }

    return offset;
}

void CEFlowLayout::updateForScrolling (float dt)
{
    scrollDuration += dt;
    // CCLOG("Scroll impulse: %.2f, %.2f", scrollImpulse.x, scrollImpulse.y);
    CCPoint currentImpulse = ccpMult (scrollImpulse, 15 * scrollSmoothness * dt);

    // Scroll impulse is reduced when the touch is released and when there is no autoscroll
    if (!touchMoved) {
        scrollImpulse = ccpSub (scrollImpulse, ccpMult (currentImpulse, scrollStopFactor));
    } else /* if( !autoscroll )*/ {
        scrollImpulse = ccpSub (scrollImpulse, currentImpulse);
    }

    // limit the impulse to prevent skipping through whole screen of cells
    currentImpulse = ccpClampf (currentImpulse, -m_obContentSize.width, m_obContentSize.width, -m_obContentSize.height,
                                m_obContentSize.height);

    if (floatBetween (scrollImpulse.x, -0.1, 0.1)) {
        currentImpulse.x += scrollImpulse.x;
        scrollImpulse.x = 0;
    }
    if (floatBetween (scrollImpulse.y, -0.1, 0.1)) {
        currentImpulse.y += scrollImpulse.y;
        scrollImpulse.y = 0;
    }

    CCPoint tableOffset = getTableOffset ();
    moveTableWithOffset (currentImpulse);
    CCPoint tableOffsetAfter = getTableOffset ();

    if (tableOffset.x != 0) {
        if (tableOffsetAfter.x == 0) {
            if (easeLeftRight && totalSize.width > m_obContentSize.width) {
                moveTableWithOffset (CCPoint (-currentImpulse.x + tableOffset.x, 0));
            }
            scrollImpulse.x = 0;
        } else {
            scrollImpulse.x = tableOffset.x;
        }
    }
    if (tableOffset.y != 0) {
        if (tableOffsetAfter.y == 0) {
            if (easeUpDown && totalSize.height > m_obContentSize.height) {
                moveTableWithOffset (CCPoint (0, -currentImpulse.y + tableOffset.y));
            }
            scrollImpulse.y = 0;
        } else {
            scrollImpulse.y = tableOffset.y;
        }
    }

    float scrollImpulseBoundary = (shouldSnapHorizontally || shouldSnapVertically) ? 2.0 : 0.000001;

    if (ccpIsBetweenf (scrollImpulse, -scrollImpulseBoundary, scrollImpulseBoundary) &&
        ccpIsBetweenf (tableOffset, -scrollImpulseBoundary, scrollImpulseBoundary)) {
        unscheduleScrolling ();
        content->setPosition (ccpAdd (content->getPosition (), tableOffset));

        if (shouldSnapVertically) {
            animateSnapVerticallyToRow (calculateRowForSnap ());
        }
        if (shouldSnapHorizontally) {
            animateSnapHorizontallyToColumn (calculateColumnForSnap ());
        }
    }
}

#pragma mark -
#pragma mark Paging Update

void CEFlowLayout::schedulePaging ()
{
    if (!scrollingScheduled && !pagingScheduled) {
        pagingScheduled = true;
        schedule (schedule_selector (CEFlowLayout::updateForPaging));
    }
}

void CEFlowLayout::unschedulePaging ()
{
    if (pagingScheduled) {
        unschedule (schedule_selector (CEFlowLayout::updateForPaging));
        pagingScheduled = false;
    }
}

void CEFlowLayout::updateForPaging (float dt)
{
    CCPoint movement = ccpMult (pageSpeed, dt);
    bool isXPositive = pageTarget.x > 0;
    movement.x *= isXPositive ? 1 : -1;
    bool isYPositive = pageTarget.y > 0;
    movement.y *= isYPositive ? 1 : -1;

    if (pageTarget.x != 0) {
        pageTarget.x -= movement.x;
        if ((isXPositive && pageTarget.x < 0) || (!isXPositive && pageTarget.x > 0)) {
            movement.x += pageTarget.x;
            pageTarget.x = 0;
        }
    } else {
        movement.x = 0;
    }

    if (pageTarget.y != 0) {
        pageTarget.y -= movement.y;
        if ((isYPositive && pageTarget.y < 0) || (!isYPositive && pageTarget.y > 0)) {
            movement.y += pageTarget.y;
            pageTarget.y = 0;
        }
    } else {
        movement.y = 0;
    }

    moveTableWithOffset (movement);
    if (ccpIsZero (pageTarget)) {
        unscheduleUpdate ();
    } else {
    }
}

void CEFlowLayout::page (float width, float height)
{
    float dX = pageTarget.x + width;
    float dY = pageTarget.y + height;
    CCPoint contentPosition = content->getPosition ();

    if (width < 0) {
        if (-dX + contentPosition.x > 0) {
            dX = contentPosition.x;
        }
    } else if (width > 0) {
        if (dX - contentPosition.x + m_obContentSize.width > totalSize.width) {
            dX = totalSize.width + contentPosition.x - m_obContentSize.width;
        }
    }

    if (height < 0) {
        if (-dY + contentPosition.y > 0) {
            dY = contentPosition.y;
        }
    } else if (height > 0) {
        if (dY - contentPosition.y + m_obContentSize.height > totalSize.height) {
            dY = totalSize.height + contentPosition.y - m_obContentSize.height;
        }
    }

    pageTarget.x = -dX;
    pageTarget.y = -dY;

    if (!scrollingScheduled) {
        schedulePaging ();
    }
}

void CEFlowLayout::pageForContentSize (int horizontaly, int verticaly)
{
    page (pageSize.width * horizontaly, pageSize.height * verticaly);
}

void CEFlowLayout::page (kCEFlowLayoutSide direction)
{
    int horizontaly = 0;
    int verticaly = 0;
    switch (direction) {
        ;
        case kSideDown:
            verticaly = -1;
            break;
        case kSideLeft:
            horizontaly = -1;
            break;
        case kSideRight:
            verticaly = 1;
            break;
        case kSideUp:
            horizontaly = 1;
            break;
        default:
            break;
    }
    page (m_obContentSize.width * horizontaly, m_obContentSize.height * verticaly);
}

CCPoint CEFlowLayout::positionForRowColumn (int row, int column)
{
    CCPoint pos = CCPoint (0, totalSize.height);
    for (int x = 0; x < column; x++) {
        pos.x += columnWidths[x];
    }
    for (int y = 0; y <= row; y++) {
        pos.y -= rowHeights[y];
    }
    return pos;
}

void CEFlowLayout::updateCell (int row, int column, CCNode *newCell)
{
    if (row < 0 || row >= totalRows || column < 0 || column >= totalColumns) {
        return;
    }

    int cellTag = tagForIndex (row, column);
    CCNode *cell = tagMap[cellTag];

    if (NULL != cell) {
        if (NULL == newCell) {
            newCell = dataSource->getLayoutElement (this, row, column);
        }
        tagMap[cellTag] = newCell;
        newCell->setPosition (newCell->getPosition () + positionForRowColumn (row, column));
        content->removeChild (cell);
        content->addChild (newCell);
    }
}

void CEFlowLayout::forceUpdate () { forceUpdateRememberPosition (kSideNone); }

// void CEFlowLayout::forceUpdateAndSetTopRow(float row) {
//    forceUpdate();
//    setTopRow(row);
//}

void CEFlowLayout::forceUpdateRememberPosition (kCEFlowLayoutSide side)
{
    float oldPosition = sidePosition (side);

    unscheduleScrolling ();
    unschedulePaging ();
    content->removeAllChildrenWithCleanup (true);
    tagMap.clear ();
    updateDimensions ();
    updateContent ();

    sideMoveToPosition (side, oldPosition);
}

void CEFlowLayout::reload (kCEFlowLayoutSide side, bool stopTouches)
{
    if (content) {

        float oldPosition = sidePosition (side);

        content->removeAllChildrenWithCleanup (false);
        tagMap.clear ();
        updateDimensions ();
        updateContent ();

        sideMoveToPosition (side, oldPosition);

        if (isTouching && stopTouches) {
            forceTouchEnd ();
            isTouching = false;
        }
    }
}

#pragma mark -
#pragma mark Usefull methods

CCNode *CEFlowLayout::getNode (int row, int column)
{
    int cellTag = tagForIndex (row, column);
    return tagMap[cellTag];
}

CCNode *CEFlowLayout::getNodeByTag (int tag) { return content->getChildByTag (tag); }

void CEFlowLayout::setRowHeight (int row, float height)
{
    if (row >= 0 && row < totalRows) {
        rowHeights[row] = height;
    }
}

void CEFlowLayout::setColumnWidth (int column, float width)
{
    if (column >= 0 && column < totalColumns) {
        columnWidths[column] = width;
    }
}

void CEFlowLayout::moveBottomRowTo (float row)
{
    if (totalRows <= 0)
        return;

    if (isContentsHeightSmall ()) {
        CCPoint oldPos = content->getPosition ();
        content->setPosition (CCPoint (oldPos.x, m_obContentSize.height - totalSize.height));
        updateContent ();
        return;
    }

    row = clampf (row, 0, totalRows - 1);
    float height = totalSize.height;
    float fractionMove = row - floorf (row);
    row -= fractionMove;

    int rowIndex = 0;
    while (row >= 0) {
        height -= rowHeights[rowIndex];
        row -= 1.0;
        rowIndex++;
    }

    if (fractionMove > 0 && rowIndex < totalRows) {
        height -= rowHeights[rowIndex] * fractionMove;
    }

    content->setPosition (ccp (content->getPosition ().x, -height));
    updateContent ();
}

void CEFlowLayout::moveTopRowTo (float row)
{
    if (totalRows <= 0)
        return;

    if (isContentsHeightSmall ()) {
        CCPoint oldPos = content->getPosition ();
        content->setPosition (CCPoint (oldPos.x, m_obContentSize.height - totalSize.height));
        updateContent ();
        return;
    }

    row = clampf (row, 0, totalRows - 1);
    float height = -totalSize.height + m_obContentSize.height;
    float fractionMove = row - floorf (row);
    row -= fractionMove;

    int rowIndex = (int)row;
    if (fractionMove > 0) {
        height += rowHeights[rowIndex] * (fractionMove);
    }

    while (rowIndex > 0) {
        rowIndex--;
        height += rowHeights[rowIndex];
    }

    content->setPosition (ccp (content->getPosition ().x, MIN (height, 0)));
    updateContent ();
}

void CEFlowLayout::moveLeftColumnTo (float column)
{
    if (totalColumns <= 0)
        return;

    if (isContentsWidthSmall ()) {
        CCPoint oldPos = content->getPosition ();
        content->setPosition (CCPoint (0, oldPos.y));
        updateContent ();
        return;
    }

    column = clampf (column, 0, totalColumns - 1);
    float width = 0;
    float fractionMove = column - floorf (column);
    column -= fractionMove;

    int columnIndex = 0;
    while (column > 0) {
        width += columnWidths[columnIndex];
        column -= 1.0;
        columnIndex++;
    }

    if (fractionMove > 0 && columnIndex < totalColumns) {
        width += columnWidths[columnIndex] * fractionMove;
    }

    content->setPosition (ccp (MAX (-width, m_obContentSize.width - totalSize.width), content->getPosition ().y));
    updateContent ();
}

void CEFlowLayout::moveRightColumnTo (float column)
{
    if (totalColumns <= 0)
        return;

    if (isContentsWidthSmall ()) {
        CCPoint oldPos = content->getPosition ();
        content->setPosition (CCPoint (0, oldPos.y));
        updateContent ();
        return;
    }

    column = clampf (column, 0, totalColumns - 1);
    float width = 0;
    float fractionMove = column - floorf (column);
    column -= fractionMove;

    int columnIndex = 0;
    while (column > 0) {
        width += columnWidths[columnIndex];
        column -= 1.0;
        columnIndex++;
    }

    if (fractionMove > 0 && columnIndex < totalColumns) {
        width += columnWidths[columnIndex] * fractionMove;
    }

    content->setPosition (ccp (-width + m_obContentSize.width - columnWidths[columnIndex], content->getPosition ().y));
    updateContent ();
}

float CEFlowLayout::topRowPosition ()
{
    if (totalRows <= 0 || isContentsHeightSmall ())
        return 0;

    float height = content->getPosition ().y;

    if (height + totalSize.height < m_obContentSize.height)
        return 0;
    if (height > m_obContentSize.height)
        return totalRows - 1;

    int heightIndex = 0;
    height += totalSize.height;
    while (heightIndex < totalRows && height - rowHeights[heightIndex] > m_obContentSize.height) {
        height -= rowHeights[heightIndex];
        heightIndex++;
    }

    return heightIndex + (height - m_obContentSize.height) / rowHeights[heightIndex];
}

float CEFlowLayout::leftColumnPosition ()
{
    if (totalColumns <= 0 || isContentsWidthSmall ())
        return 0;

    float width = content->getPosition ().x;

    if (width > 0)
        return 0;
    /*if( width < m_obContentSize.width-totalSize.width )
     return totalColumns-1;
     */
    int widthIndex = 0;
    while (widthIndex < totalColumns && width + columnWidths[widthIndex] < 0) {
        width += columnWidths[widthIndex];
        widthIndex++;
    }

    return widthIndex + -width / columnWidths[widthIndex];
}

float CEFlowLayout::sidePosition (kCEFlowLayoutSide side)
{
    float pos = 0;
    switch (side) {
        case kSideNone:
            break;

        case kSideLeft:
            pos = leftColumnPosition ();
            break;
        case kSideUp:
            pos = topRowPosition ();
            break;
        case kSideRight:
            break;
        case kSideDown:
            break;

        default:
            break;
    }
    return pos;
}

void CEFlowLayout::sideMoveToPosition (kCEFlowLayoutSide side, float position)
{
    switch (side) {
        case kSideNone:
            break;

        case kSideLeft:
            moveLeftColumnTo (position);
            break;
        case kSideUp:
            moveTopRowTo (position);
            break;
        case kSideRight:
            moveRightColumnTo (position);
            break;
        case kSideDown:
            moveBottomRowTo (position);
            break;

        default:
            break;
    }
}

#pragma mark - Snap

int CEFlowLayout::calculateRowForSnap ()
{
    double rowHeight = rowHeights[0];
    int row = (fabs (content->getPosition ().y) + snapOffset.y + rowHeight / 2.0) / rowHeight;
    return (totalRows - row - 1);
}

int CEFlowLayout::calculateColumnForSnap ()
{
    double colwWidth = columnWidths[0];
    int column = (fabs (content->getPosition ().x) + snapOffset.x + colwWidth / 2.0) / colwWidth;
    return column;
}

void CEFlowLayout::snapVerticallyToRow (int row)
{
    double rowHeight = rowHeights[0];
    content->setPosition (ccpAdd (ccp (content->getPosition ().x, -(totalRows - row - 1) * rowHeight), snapOffset));

    if (delegate) {
        delegate->layoutSnapedToRow (this, row);
    }

    updateContent ();
}

void CEFlowLayout::animateSnapVerticallyToRow (int row)
{
    double rowHeight = rowHeights[0];
    snapFinalPosition = ccpAdd (ccp (content->getPosition ().x, -(totalRows - row - 1) * rowHeight), snapOffset);
    snapRow = row;
    snapDirection.y = content->getPosition ().y < snapFinalPosition.y ? 1 : -1;
    schedule (schedule_selector (CEFlowLayout::updateForSnap));
}

void CEFlowLayout::snapHorizontallyToColumn (int column)
{
    double columnWidth = columnWidths[0];
    content->setPosition (ccpAdd (ccp (-column * columnWidth, content->getPosition ().y), snapOffset));

    if (delegate) {
        delegate->layoutSnapedToCol (this, column);
    }

    updateContent ();
}

void CEFlowLayout::animateSnapHorizontallyToColumn (int column)
{
    double columnWidth = columnWidths[0];
    snapFinalPosition = ccpAdd (ccp (-column * columnWidth, content->getPosition ().y), snapOffset);
    snapColumn = column;
    snapDirection.x = content->getPosition ().x < snapFinalPosition.x ? 1 : -1;
    schedule (schedule_selector (CEFlowLayout::updateForSnap));
}

void CEFlowLayout::updateForSnap (float dt)
{
    CCPoint contentPosition = content->getPosition ();
    double dx = 0.f, dy = 0.f;

    if (shouldSnapVertically) {
        int snapDirection = contentPosition.y > snapFinalPosition.y ? -1 : 1;
        dy = snapDirection * dt * snapSpeed.y;
    }

    if (shouldSnapHorizontally) {
        int snapDirection = contentPosition.x > snapFinalPosition.x ? -1 : 1;
        dx = snapDirection * dt * snapSpeed.x;
    }

    float newPosX = contentPosition.x + dx;
    if ((snapDirection.x > 0 && newPosX > snapFinalPosition.x) ||
        (snapDirection.x < 0 && newPosX < snapFinalPosition.x)) {
        newPosX = snapFinalPosition.x;
        dx = snapDirection.x * fabs(newPosX - contentPosition.x);
    }
    float newPosY = contentPosition.y + dy;
    if ((snapDirection.y > 0 && newPosY > snapFinalPosition.y) ||
        (snapDirection.y < 0 && newPosY < snapFinalPosition.y)) {
        newPosY = snapFinalPosition.y;
        dy = snapDirection.y * fabs(newPosY - contentPosition.y);
    }

    moveTableWithOffset (ccp(dx,dy));

    if (newPosX == snapFinalPosition.x && newPosY == snapFinalPosition.y) {
        unschedule (schedule_selector (CEFlowLayout::updateForSnap));
        if (delegate) {
            if (shouldSnapVertically) {
                delegate->layoutSnapedToRow (this, snapRow);
            } else {
                delegate->layoutSnapedToCol (this, snapColumn);
            }
        }
    }
}

#pragma mark -
#pragma mark Getters and Setters

void CEFlowLayout::setLock (bool lock) { this->lock = lock; }

bool CEFlowLayout::getLock () { return lock; }

void CEFlowLayout::setLockEnabledElements (std::vector<std::pair<int, int>> enabledElements)
{
    lockFilter.clear ();
    for (auto pair : enabledElements) {
        addLockEnabledElement (pair.first, pair.second);
    }
}

void CEFlowLayout::addLockEnabledElement (int row, int column)
{
    int enabledTag = tagForIndex (row, column);
    lockFilter.push_back (enabledTag);
}

void CEFlowLayout::clearLockEnabledElements () { lockFilter.clear (); }

void CEFlowLayout::setTotalRows (int rows) { setDimensions (rows, totalColumns); }

int CEFlowLayout::getTotalRows () { return totalRows; }

void CEFlowLayout::setTotalColumns (int columns) { setDimensions (totalRows, columns); }

int CEFlowLayout::getTotalColumns () { return totalColumns; }
void CEFlowLayout::setCellSize (CCSize size)
{
    cellSize = size;
    updateDimensions ();
    updateContent ();
}

CCSize CEFlowLayout::geCellSize () { return cellSize; }

CCSize CEFlowLayout::getTotalSize () { return totalSize; }

bool CEFlowLayout::getKeepInvisibleNodes () { return keepInvisibleNodes; }

void CEFlowLayout::setKeepInvisibleNodes (bool toggle) { keepInvisibleNodes = toggle; }

bool CEFlowLayout::getHideInvisibleNodes () { return hideInvisibleNodes; }

void CEFlowLayout::setHideInvisibleNodes (bool toggle) { hideInvisibleNodes = toggle; }

bool CEFlowLayout::getLoadAll () { return loadAll; }

void CEFlowLayout::setLoadAll (bool toggle) { loadAll = toggle; }

bool CEFlowLayout::getAllowMoveLR () { return allowTableMoveLR; }

void CEFlowLayout::setAllowMoveLR (bool allow) { allowTableMoveLR = allow; }

bool CEFlowLayout::getAllowMoveUD () { return allowTableMoveUD; }

void CEFlowLayout::setAllowMoveUD (bool allow) { allowTableMoveUD = allow; }

bool CEFlowLayout::getActivateTouchedMenuItems () { return activateTouchedMenuItems; }

void CEFlowLayout::setActivateTouchedMenuItems (bool doActivate) { activateTouchedMenuItems = doActivate; }

float CEFlowLayout::getTouchedMenuItemSelectDelay () { return touchedMenuItemSelectDelay; }

void CEFlowLayout::setTouchedMenuItemSelectDelay (float value) { touchedMenuItemSelectDelay = value; }

float CEFlowLayout::getToucRadiusTreshold () { return touchRadiusTreshold; }

void CEFlowLayout::setToucRadiusTreshold (float r) { touchRadiusTreshold = r; }

void CEFlowLayout::setBorderOffset (CCPoint offset) { borderOffset = offset; }

CCPoint CEFlowLayout::getBorderOffset () { return borderOffset; }

void CEFlowLayout::setEaseLeftRight (bool ease) { easeLeftRight = ease; }

bool CEFlowLayout::getEaseLeftRight () { return easeLeftRight; }

void CEFlowLayout::setEaseUpDown (bool ease) { easeUpDown = ease; }

bool CEFlowLayout::getEaseUpDown () { return easeUpDown; }

void CEFlowLayout::setSwipeAction (kCEFlowLayoutSwipeAction action) { swipeAction = action; }

kCEFlowLayoutSwipeAction CEFlowLayout::getSwipeAction () { return swipeAction; }

void CEFlowLayout::setPagingTreshold (CCPoint tresh) { pagingTreshold = tresh; }

CCPoint CEFlowLayout::getPagingTreshold () { return pagingTreshold; }

void CEFlowLayout::setPageSpeed (CCPoint speed) { pageSpeed = speed; }

CCPoint CEFlowLayout::getPageSpeed () { return pageSpeed; }

void CEFlowLayout::setPageSize (CCSize newPageSize) { pageSize = newPageSize; }

CCSize CEFlowLayout::getPageSize () { return pageSize; }

void CEFlowLayout::setScrollSensitivity (float sensitivity) { scrollSensitivity = clampf (sensitivity, 1, 100); }

float CEFlowLayout::getScrollSensitivity () { return scrollSensitivity; }

void CEFlowLayout::setScrollSmoothness (float sensitivity) { scrollSmoothness = clampf (sensitivity, 0.001, 1); }

float CEFlowLayout::getScrollSmoothness () { return scrollSmoothness; }

/*
 void CEFlowLayout::setAutoscroll( bool autoscroll )
 {
 this->autoscroll = autoscroll;
 }

 bool CEFlowLayout::getAutoscroll()
 {
 return autoscroll;
 }*/

void CEFlowLayout::setScrollStopFactor (float factor)
{
    scrollStopFactor = clampf (factor, 0.001, 1);
    ;
}

float CEFlowLayout::getScrollStopFactor () { return scrollStopFactor; }

bool CEFlowLayout::getBoundTouchedUp () { return boundUp; }

bool CEFlowLayout::getBoundTouchedDown () { return boundDown; }

bool CEFlowLayout::getBoundTouchedLeft () { return boundLeft; }

bool CEFlowLayout::getBoundTouchedRight () { return boundRight; }

void CEFlowLayout::setDataSource (CEFlowLayoutDataSource *source)
{
    dataSource = source;
    if (dataSource) {
        updateDimensions ();
        updateContent ();

        if (shouldSnapHorizontally) {
            snapHorizontallyToColumn (calculateColumnForSnap ());
        }
        if (shouldSnapVertically) {
            snapVerticallyToRow (calculateRowForSnap ());
        }
    }
}

void CEFlowLayout::setDelegate (CEFlowLayoutDelegate *delegate) { this->delegate = delegate; }

int CEFlowLayout::getCellIndexLeft () { return (int)cellIndexLeftDown.x; }

int CEFlowLayout::getCellIndexRight () { return (int)cellIndexLeftDown.y; }

int CEFlowLayout::getCellIndexUp () { return (int)cellIndexRightUp.x; }

int CEFlowLayout::getCellIndexDown () { return (int)cellIndexRightUp.y; }

bool CEFlowLayout::isContentsHeightSmall () { return totalSize.height < m_obContentSize.height; }

bool CEFlowLayout::isContentsWidthSmall () { return totalSize.width < m_obContentSize.width; }

CCSize CEFlowLayout::getCellSizeForCell (int row, int column)
{
    CCSize size;
    size.width = totalColumns != 0 ? columnWidths[column] : 0;
    size.height = totalRows != 0 ? rowHeights[row] : 0;
    return size;
}

void CEFlowLayout::setShouldSnapVertically (bool value) { shouldSnapVertically = value; }

bool CEFlowLayout::getShouldSnapVertically () { return shouldSnapVertically; }

void CEFlowLayout::setShouldSnapHorizontally (bool value) { shouldSnapHorizontally = value; }

bool CEFlowLayout::getShouldSnapHorizontally () { return shouldSnapHorizontally; }

void CEFlowLayout::setSnapOffset (cocos2d::CCPoint offset) { snapOffset = offset; }

cocos2d::CCPoint CEFlowLayout::getSnapOffset () { return snapOffset; }

void CEFlowLayout::setSnapSpeed (CCPoint speed) { snapSpeed = speed; }

CCPoint CEFlowLayout::getSnapSpeed () { return snapSpeed; }
