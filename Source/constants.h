
//  constants.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 07/01/25.
//

using namespace ax;


#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

auto const DISC_ORIGIN = Vec2(101.75,324.5);
auto const SCALE = 0.5f;
auto const DISC_UP = Vec2(0, 45.75);
auto const DISC_RIGHT = Vec2(45.9, 0);

auto const DISC_PLACED = 0;
auto const DISC_UNPLACED = 1;
auto const FONT_SIZE = 40;


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

static void center(Sprite *&background, const Vec2 &origin, const Vec2 &visibleSize) {
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
}

static Sprite* getBackground() {
    auto background = Sprite::create("bg_selection.jpg"sv);
//    auto sprite = Sprite::create("disc.png"sv);
    if(background)
    {
        // position the sprite on the center of the screen
        center(background, Director::getInstance()->getVisibleOrigin(), Director::getInstance()->getVisibleSize());
        background->setScale(1.2, 1.2);
        
    }    // add the sprite as a child to this layer
    return background;
}



#endif
