#include "MainScene.h"
#include "constants.h"
#include <boost/asio.hpp>

using namespace ax;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

Vec2 MainScene::getBoardCoords(int up, int right){
    right = std::max(std::min(right, 6), 0);
    up = std::max(std::min(up, 5),0);
    
    return DISC_ORIGIN + up * DISC_UP + right * DISC_RIGHT;
}

Rect MainScene::getRect(TURN color) {
    switch(color) {
        case RED:
            return Rect(0, 0, 78, 78);
        case YELLOW:
        default:
            return Rect(78, 0, 78, 78);
    }
}

Sprite* MainScene::getDisc(TURN color, bool glowing=false) {
    Rect rect = getRect(color);
    auto fileName = glowing? "glows.png"sv : "disc.png"sv;
    return Sprite::create(fileName, rect);
}

void MainScene::glowUp(int up, int right, TURN winner) {
    auto glow = getDisc(winner, true);
    auto boardCoords = getBoardCoords(right, up);
    
    if(glow){
        glow->setScale(SCALE);
        glow->setPosition(boardCoords);
        glow->setLocalZOrder(10);
        Sequence* seq = Sequence::create(Hide::create(), DelayTime::create(0.9), Blink::create(1.0, 4), NULL);
        glow->runAction(seq);
        this->addChild(glow);

    }
//    Rect rect = getRect(static_cast<TURN>(), )
//    auto coinGlow = Sprite::create("glows.png"sv, )
}

void MainScene::signalGameOver() {
    auto winner = gameBoard.getCurrentTurn();
    for(auto coinCoords : gameBoard.getWinningCoins()){
        glowUp(coinCoords.first, coinCoords.second, winner);
    }
}

// returns 1 for failure, 0 for success.
int MainScene::placeDisc(int right){
    if(!gameBoard.isValidMove(right)){
        return DISC_UNPLACED;
    }
    int up = gameBoard.makeMoveAndGetCol(right);
    if(up < 0) return DISC_UNPLACED;
    auto disc = getDisc(gameBoard.getCurrentTurn());
    if(disc) {        
        disc->setScale(SCALE);
        disc->setPosition(getBoardCoords(INT_MAX, right) + DISC_UP);
        
        auto moveTo = MoveTo::create(0.5, getBoardCoords(up, right));
        auto jump = JumpBy::create(0.5, Vec2(0, 0), DISC_UP.y*0.4, 1);
//        auto jump_ease_out = EaseBouncOut::create(jump->clone());

//        AXLOG("%d, %d, %d", static_cast<int>(gameBoard.getCurrentTurn()), up, right);
        auto seq = Sequence::create(moveTo, jump, nullptr);
        
        disc->runAction(seq);
        this->addChild(disc, 1);
        if(gameBoard.isGameOver()) {
            signalGameOver();
        }
        return DISC_PLACED;
    }

    else {
        problemLoading("disc");
        return DISC_UNPLACED;
    }
    
}

// returns 1 for success, zero for failure
int MainScene::placeDiscAt(Vec2 sceneCoords) {
    int columnIndex = std::round((sceneCoords - DISC_ORIGIN).x/(94.4914 * SCALE)); // some issues with tuning the origin is causing this.
    if(columnIndex < 0 || columnIndex > 6) return;
    return this->placeDisc(columnIndex);
}


static void center(Sprite *&background, const Vec2 &origin, const Vec2 &visibleSize) {
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        AX_CALLBACK_1(MainScene::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // Some templates (uncomment what you  need)
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = AX_CALLBACK_2(MainScene::onTouchesBegan, this);
    touchListener->onTouchesMoved = AX_CALLBACK_2(MainScene::onTouchesMoved, this);
    touchListener->onTouchesEnded = AX_CALLBACK_2(MainScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto mouseListener             = EventListenerMouse::create();
    //mouseListener->onMouseMove   = AX_CALLBACK_1(MainScene::onMouseMove, this);
    //mouseListener->onMouseUp     = AX_CALLBACK_1(MainScene::onMouseUp, this);
    mouseListener->onMouseDown     = AX_CALLBACK_1(MainScene::onMouseDown, this);
    //mouseListener->onMouseScroll = AX_CALLBACK_1(MainScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    //auto keyboardListener           = EventListenerKeyboard::create();
    //keyboardListener->onKeyPressed  = AX_CALLBACK_2(MainScene::onKeyPressed, this);
    //keyboardListener->onKeyReleased = AX_CALLBACK_2(MainScene::onKeyReleased, this);
    //_eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 11);



    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    // add "HelloWorld" splash screen"
    auto background = Sprite::create("bg_selection.jpg"sv);
//    auto sprite = Sprite::create("disc.png"sv);
    if(background)
    {
        // position the sprite on the center of the screen
        center(background, origin, visibleSize);

        // add the sprite as a child to this layer
        this->addChild(background, 0);
//        auto drawNode = DrawNode::create();
//        drawNode->setPosition(Vec2(0, 0));
//        addChild(drawNode);
//
//        drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4B::BLUE);
    } else {
        problemLoading("background.");
    }
    
    auto board = Sprite::create("board_front.png"sv);
    if(board){
        center(board, origin, visibleSize);
        board->setScale(SCALE);
        this->addChild(board, 2);
    }
    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}


void MainScene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void MainScene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void MainScene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void MainScene::onMouseDown(Event* event) {
    EventMouse* e = static_cast<EventMouse*>(event);
    
    auto mouseWindowPosition = e->getLocationInView();
    auto mouseScenePosition = this->convertToNodeSpace(mouseWindowPosition);
    placeDiscAt(mouseScenePosition); // if you want to check for success, you can equality check with DISC_PLACED
}

void MainScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void MainScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseMove detected, X:%f  Y:%f", e->getCursorX(), e->getCursorY());
}

void MainScene::onMouseScroll(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseScroll detected, X:%f  Y:%f", e->getScrollX(), e->getScrollY());
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
//    AXLOG("onKeyPressed, keycode: %d", static_cast<int>(code));
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
//    AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void MainScene::update(float delta)
{
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {    /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {    /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {    /////////////////////////////
        // Add your codes below...like....
        // 
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    } //switch
}

void MainScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/


     // EventCustom customEndEvent("game_scene_close_event");
     //_eventDispatcher->dispatchEvent(&customEndEvent);
}
