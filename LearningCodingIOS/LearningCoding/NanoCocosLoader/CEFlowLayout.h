//
//  CEFlowLayout.h
//  NCLCpp
//
//  Created by Bruno on 7/1/13.
//
//

#ifndef __NCLCpp__CEFlowLayout__
#define __NCLCpp__CEFlowLayout__

#include "cocos2d.h"
#include <time.h>

typedef enum{
    kSideLeft = 1,
    kSideRight = 2,
    kSideUp = 3,
    kSideDown = 4,
    kSideNone = 5
} kCEFlowLayoutSide;

std::string boundaryNameForType( kCEFlowLayoutSide side );

typedef enum{
    kSwipeScrolling = 1,
    kSwipePaging = 2
} kCEFlowLayoutSwipeAction;

class CEFlowLayout;

class CEFlowLayoutDataSource {
public:
    virtual cocos2d::CCNode* getLayoutElement( CEFlowLayout* layout, int row, int column ){ return NULL; };
    virtual float getLayoutRowHeight( CEFlowLayout* layout, int row ){ return 0; };
    virtual float getLayoutColumnWidth( CEFlowLayout* layout, int column ){ return 0; };
    virtual int getLayoutNumberOfColumns( CEFlowLayout* layout ){ return 0; };
    virtual int getLayoutNumberOfRows( CEFlowLayout* layout ){ return 0; };
};

class CEFlowLayoutDelegate
{
  public:
    virtual void layoutElementTouched (CEFlowLayout *layout, int row, int column, cocos2d::CCPoint percentage,
                                       cocos2d::CCNode *touchedNode, cocos2d::CCTouch *pTouch,
                                       cocos2d::CCEvent *pEvent){};
    virtual void layoutScrolledToBoundary (CEFlowLayout *layout, kCEFlowLayoutSide side){};
    virtual void layoutUnscrolledFromBoundary (CEFlowLayout *layout, kCEFlowLayoutSide side){};
    virtual void layoutElementRemoved (CEFlowLayout *layout, cocos2d::CCNode *element){};
    virtual void layoutTouchBegan (CEFlowLayout *layout, int row, int column, cocos2d::CCNode *node){};
    virtual void layoutTouchMoved (CEFlowLayout *layout, cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,
                                   cocos2d::CCNode *node){};
    virtual void layoutTouchEnded (CEFlowLayout *layout, cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){};
    virtual void layoutMovedWithOffset (CEFlowLayout *layout, cocos2d::CCPoint offset){};
    virtual void layoutSnapedToRow (CEFlowLayout *layout, int row){};
    virtual void layoutSnapedToCol (CEFlowLayout *layout, int col){};
};

class CEFlowLayout : public cocos2d::CCLayerRGBA/*, public cocos2d::CCTargetedTouchDelegate*/ {
    
#pragma mark -
#pragma mark Member variables
    bool lock;
    std::vector<int> lockFilter;
    
    cocos2d::CCNodeRGBA *midContent;
    cocos2d::CCNodeRGBA *content;
    std::map<int, cocos2d::CCNode*> tagMap;
    
    // Dimensions and position
    cocos2d::CCSize totalSize;        // Total size, sum of widths and heights
    cocos2d::CCSize cellSize;        // Cell size used as backup when there is no data source
    cocos2d::CCPoint worldStart;
    cocos2d::CCPoint worldEnd;
    
    std::vector<float> rowHeights;        // Per row height
    std::vector<float> columnWidths;    // Per column width
    int totalRows;            // Total flow layout rows
    int totalColumns;        // Total flow layout columns
    
    cocos2d::CCPoint cellIndexLeftDown;    // Visible table cell index for the (left,down) corner
    cocos2d::CCPoint cellIndexRightUp;    // Visible table cell index for the (right,up) corner
    
    bool keepInvisibleNodes;
    bool hideInvisibleNodes;
    bool loadAll;
    
    // Touch variables
    bool touchMoved;        // To determine a tap event
    cocos2d::CCPoint touchStart;        // Initial position of a touch, changes with moving
    kCEFlowLayoutSwipeAction swipeAction;    // Can be page or scroll
    bool allowTableMoveLR;
    bool allowTableMoveUD;
    float touchRadiusTreshold;
    cocos2d::CCMenuItem *touchedMenuItem;
    cocos2d::CCPoint tapIndex;
    bool touchedMenuItemState;
    bool activateTouchedMenuItems;
    time_t lastTouchChange;
    bool forceEndTouch;
    bool forceEndScroll;
    bool isTouching;
    float touchedMenuItemSelectDelay;
    
    // Paging variables
    cocos2d::CCPoint pagingTreshold;    // Touch drag length that will trigger a page flip, default half viewSize
    cocos2d::CCSize pageSize;        // Page movements that will be performed
    cocos2d::CCPoint pageSpeed;        // Page moving speed, default (300,300)
    cocos2d::CCPoint pageTarget;
    
    bool pagingScheduled;   // When an updatePaging is scheduled, used to prevent schedule assert failures
    
    // Scrolling variables
    float scrollSensitivity;        // Higher sensitivity means more rigid scrolling
    float scrollSmoothness;            // [0,1] how fast will the scrolling be
    cocos2d::CCPoint scrollImpulse;            // Used to animate scrolling
    float scrollDuration;            // Updated with dt while touch is not moving
    float scrollStopFactor;            // While no autoscrolling, determines the stop duration
    bool scrollingScheduled;// When an updateScrolling is scheduled, used to prevent schedule assert failures
    
    // Easing and snapping
    bool easeLeftRight;        // When YES, user can drag the table a bit out of its borders
    bool easeUpDown;        // When NO, table movement is rigid on the edges (eg. useful for row/column only tables)
    cocos2d::CCPoint borderOffset;
    
    // Bounds
    bool boundUp;            // States that determine which side is "touched" in the table (eg. (0,0) is Left and Down)
    bool boundDown;            // On update of theese states delegate methods are called (didScroll and did Unscroll)
    bool boundLeft;
    bool boundRight;
    
    bool shouldSnapVertically;
    bool shouldSnapHorizontally;
    cocos2d::CCPoint snapOffset;
    cocos2d::CCPoint snapFinalPosition;
    cocos2d::CCPoint snapDirection;
    int snapRow;
    int snapColumn;
    cocos2d::CCPoint snapSpeed;     // Snap moving speed
    
    CEFlowLayoutDataSource* dataSource;        // Source of table elements
    CEFlowLayoutDelegate* delegate;    // Delegate that gets notified of some key events
    
#pragma mark -
#pragma mark Private methods
    cocos2d::CCPoint getAnchorOffset();
    void setDimensions( int rows, int cols );
    void updateDimensions();
    void updateContent();
    void moveTableWithOffset( cocos2d::CCPoint newOffset );
    cocos2d::CCNode* getNodeInternal( int row, int column, int tag, cocos2d::CCPoint position );
    void updateBoundaryStates();
    
    void notifyDelegateOfScrollToBoundary(kCEFlowLayoutSide side);
    void notifyDelegateOfUnscrollFromBoundary(kCEFlowLayoutSide side);
    
    void scheduleScrolling();
    void unscheduleScrolling();
    void updateForScrolling(float delta);
    
    void schedulePaging();
    void unschedulePaging();
    void updateForPaging(float delta);
    
    cocos2d::CCPoint getTableOffset();
    // Returns index as (column, row)
    cocos2d::CCPoint indexInTableSpace( cocos2d::CCPoint location, cocos2d::CCPoint *cellPercentage = NULL );
    void detectNodeAndIndex( cocos2d::CCPoint location, cocos2d::CCNode** node, cocos2d::CCPoint* index );
    
    cocos2d::CCPoint positionForRowColumn( int row, int column );
    // Index is (column, row)
    bool indexIsLockValid( cocos2d::CCPoint index );
    inline int tagForIndex( int row, int column );
    
public:
    
#pragma mark -
#pragma mark Init and Cocos related
    
    CEFlowLayout();
    ~CEFlowLayout();
    
    virtual bool init( cocos2d::CCSize size, CEFlowLayoutDataSource* dataSource, CEFlowLayoutDelegate* delegate, bool canTouch = true );
    
    static CEFlowLayout* create( cocos2d::CCSize size, CEFlowLayoutDataSource* dataSource = NULL, CEFlowLayoutDelegate* delegate = NULL );
    
    virtual void setTouchEnabled(bool value);
    
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void forceTouchEnd();
    void forceUnscheduleScrolling();
    
    void visit();
    virtual void draw();
    //void onEnter();
    //void onExit();
    
    void setAnchorPoint( const cocos2d::CCPoint& anchor );
    void setPosition( const cocos2d::CCPoint& position );
    void setParent( cocos2d::CCNode *parent );
    void setContentSize( const cocos2d::CCSize& size );
    void setScale(float scale);
    
    void selectTouchedMenuItem();
    
    void forceUpdateRememberPosition(kCEFlowLayoutSide side);
    void forceUpdate();
//    void forceUpdateAndSetTopRow(float row);
    
#pragma mark -
#pragma mark Usefull methods
    
    cocos2d::CCNode* getContent() { return content; };
    
    cocos2d::CCNode* getNode( int row, int column );
    cocos2d::CCNode* getNodeByTag( int tag );
    
    void setRowHeight( int row, float width );
    void setColumnWidth( int column, float width );
    
    void moveBottomRowTo( float row );
    void moveTopRowTo( float row );
    void moveLeftColumnTo( float row );
    void moveRightColumnTo( float row );
    void moveWithImpulse( cocos2d::CCPoint impulse );
    
    float topRowPosition();
    float leftColumnPosition();
    
    float sidePosition( kCEFlowLayoutSide side );
    void sideMoveToPosition( kCEFlowLayoutSide side, float position );
    
    void page( float width, float height );
    void pageForContentSize( int horizontaly, int verticaly );
    void page( kCEFlowLayoutSide direction );
    
    void updateCell( int row, int column, cocos2d::CCNode *newCell = NULL );
    void reload( kCEFlowLayoutSide side = kSideNone, bool stopTouches = false );
    
    bool isCellOnTable( cocos2d::CCNode* node ){
        cocos2d::CCArray* contNodes = content->getChildren();
        cocos2d::CCObject* tmp;
        CCARRAY_FOREACH(contNodes, tmp){
            if( tmp == node )
                return true;
        }
        return false;
    }
    
#pragma mark Snap
    int calculateRowForSnap ();
    int calculateColumnForSnap ();
    void updateForSnap (float dt);
    void snapVerticallyToRow (int row);
    void animateSnapVerticallyToRow (int row);
    void snapHorizontallyToColumn (int column);
    void animateSnapHorizontallyToColumn (int column);

#pragma mark -
#pragma mark Getters and Setters
    
    void setLock( bool lock );
    bool getLock();
    
    // List of (row,column) pairs
    void setLockEnabledElements( std::vector<std::pair<int, int> > );
    void addLockEnabledElement( int row, int column );
    void clearLockEnabledElements();
    
    void setTotalRows( int rows );
    int getTotalRows();
    
    void setTotalColumns( int columns );
    int getTotalColumns();
    
    void setCellSize( cocos2d::CCSize size );
    cocos2d::CCSize geCellSize();
    
    cocos2d::CCSize getTotalSize();
    
    bool getKeepInvisibleNodes();
    void setKeepInvisibleNodes(bool toggle);
    
    bool getHideInvisibleNodes();
    void setHideInvisibleNodes(bool toggle);
    
    bool getLoadAll();
    void setLoadAll(bool toggle);
    
    bool getAllowMoveLR();
    void setAllowMoveLR(bool);
    
    bool getAllowMoveUD();
    void setAllowMoveUD(bool);
    
    bool getActivateTouchedMenuItems();
    void setActivateTouchedMenuItems(bool);
    
    float getTouchedMenuItemSelectDelay();
    void setTouchedMenuItemSelectDelay( float value );
    
    float getToucRadiusTreshold();
    void setToucRadiusTreshold(float);
    
    void setBorderOffset( cocos2d::CCPoint offset );
    cocos2d::CCPoint getBorderOffset();
    
    void setEaseLeftRight( bool ease );
    bool getEaseLeftRight();
    
    void setEaseUpDown( bool ease );
    bool getEaseUpDown();
    
    void setSwipeAction( kCEFlowLayoutSwipeAction action );
    kCEFlowLayoutSwipeAction getSwipeAction();
    
    void setPagingTreshold( cocos2d::CCPoint tresh );
    cocos2d::CCPoint getPagingTreshold();
    
    void setPageSpeed( cocos2d::CCPoint tresh );
    cocos2d::CCPoint getPageSpeed();
    
    void setPageSize( cocos2d::CCSize );
    cocos2d::CCSize getPageSize();
    
    void setScrollSensitivity( float sensitivity );
    float getScrollSensitivity();
    
    void setScrollSmoothness( float sensitivity );
    float getScrollSmoothness();
    
    /*void setAutoscroll( bool autoscroll );
     bool getAutoscroll();*/
    
    void setScrollStopFactor( float duration );
    float getScrollStopFactor();
    
    bool getBoundTouchedUp();
    bool getBoundTouchedDown();
    bool getBoundTouchedLeft();
    bool getBoundTouchedRight();
    
    void setDataSource( CEFlowLayoutDataSource* source );
    void setDelegate( CEFlowLayoutDelegate* delegate );
    
    int getCellIndexLeft();
    int getCellIndexRight();
    int getCellIndexUp();
    int getCellIndexDown();
    
    bool isContentsWidthSmall();
    bool isContentsHeightSmall();
    
    cocos2d::CCSize getCellSizeForCell( int row, int column );
    
    void setShouldSnapVertically ( bool shouldSnapVertically );
    bool getShouldSnapVertically ();
    
    void setShouldSnapHorizontally ( bool shouldSnapHorizontally );
    bool getShouldSnapHorizontally ();
    
    void setSnapOffset ( cocos2d::CCPoint snapOffset );
    cocos2d::CCPoint getSnapOffset ();
    
    void setSnapSpeed( cocos2d::CCPoint snapSpeed );
    cocos2d::CCPoint getSnapSpeed();
};




#endif /* defined(__NCLCpp__CEFlowLayout__) */
