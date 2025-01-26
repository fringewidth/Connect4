#include "MainScene.h"
#include "constants.h"
#include "LoadScreen.h"
#include <boost/asio.hpp>
#include "GameBoard.h"
#include <memory>

using namespace ax;


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

void MainScene::glowUp(int up, int right, TURN winner, const std::function<void()>& callback) {
    auto glow = getDisc(winner, true);
    auto boardCoords = getBoardCoords(right, up);
    
    if(glow){
        glow->setScale(SCALE);
        glow->setPosition(boardCoords);
        glow->setLocalZOrder(10);
        Sequence* seq = Sequence::create(Hide::create(),
                                         DelayTime::create(0.9),
                                         Blink::create(1.0, 4),
                                         CallFunc::create(callback),
                                         NULL
                                        );
        glow->runAction(seq);
        this->addChild(glow);

    }
//    Rect rect = getRect(static_cast<TURN>(), )
//    auto coinGlow = Sprite::create("glows.png"sv, )
}

void showEndScreen(Scene* scene, std::string message) {
    auto endScreen = EndScreen::create();
    endScreen->setLabelText(message);
    scene->addChild(endScreen, 10);
}

void MainScene::signalDraw() {
    showEndScreen(this, "It's a draw!");
}


void MainScene::signalGameOver() {
    askBot();
    auto winner = gameBoard.getCurrentTurn();
    auto gameOverShown = std::make_shared<bool>(false);

    for (auto coinCoords : gameBoard.getWinningCoins()) {
        glowUp(coinCoords.first, coinCoords.second, winner, [this, winner, gameOverShown]() {
            if (!(*gameOverShown)) {
                *gameOverShown = true;
                this->showGameOverScreen(winner);
            }
        });
    }
}

void MainScene::signalGameOver(bool forfeited) {
    auto winner = gameBoard.getCurrentTurn();
    auto gameOverShown = std::make_shared<bool>(false);

    this->showGameOverScreen(winner);
}


void MainScene::showGameOverScreen(TURN loser) {
    TURN winner = GameBoard::swapTurn(loser); // it just works idk bro.
    showEndScreen(this, GameBoard::stringFromTurn(winner) + " Wins!");
}

// returns 1 for failure, 0 for success.
int MainScene::placeDisc(int right){
    if(right == FORFEIT_CODE) {
        signalGameOver();
        return;
    }
    if(!gameBoard.isValidMove(right)){
        AXLOG("disc unplaced");
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
            if(gameBoard.isDraw()) signalDraw();
            else signalGameOver();
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
    AXLOG("move made: %d", columnIndex);
    if(columnIndex < 0 || columnIndex > 6){
        AXLOG("invalid out of bounds move");
        return DISC_UNPLACED;
    }
    return this->placeDisc(columnIndex);
}




// on "init" you need to initialize your instance
bool MainScene::init()
{
    AXLOG("MainScene::init() called.");
    if (!Scene::init())
    {
        AXLOG("Scene initialization failed!");
        return false;
    }

    AXLOG("Scene initialized successfully!");


    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
//        AX_CALLBACK_1(MainScene::menuCloseCallback, this));
//
//    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
//    {
//        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
//    }
//    else
//    {
//        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
//        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
//        closeItem->setPosition(Vec2(x, y));
//    }

//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu, 1);
//    
    
    auto backLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 24);
        if (backLabel)
        {
            backLabel->enableOutline(Color4B::RED, 2);
            
            backLabel->setPosition(Vec2(origin.x + backLabel->getContentSize().width / 2,
                                        origin.y + visibleSize.height - backLabel->getContentSize().height / 2) - Vec2(-10, 10));

            this->addChild(backLabel, 100);
            this->backLabel = backLabel;
        }


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

//    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
//    if (label == nullptr)
//    {
//        problemLoading("'fonts/Marker Felt.ttf'");
//    }
//    else
//    {
//        // position the label on the center of the screen
//        label->setPosition(
//            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
//
//        // add the label as a child to this layer
//        this->addChild(label, 1);
//    }
    // add "HelloWorld" splash screen"
    this->addChild(getBackground(), 0);
    
    
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
    
    if(backLabel && backLabel->getBoundingBox().containsPoint(e->getLocation())) {
        auto loadScene = utils::createInstance<LoadScreen>();
        onBackPressed();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, loadScene));
    }
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
