#ifndef WARRIOR_C
#define WARRIOR_C

#include "warrior.h"
#include "raylib.h"
#include "zombies.h"
#include "mainPlayer.h"
#include "basic.h"
#include <stdlib.h>
#include <stdio.h>


Vector2 normalAnimationCenter;
Texture2D normalAttackTexture;

void initWarriorAttack(){
    normalAttackTexture = LoadTexture("resources/WarriorNormalAttack.png");
}

void warriorNormalAttack(){
    zombie *currZombie = getZombies();
    enum MAINPLAYERFACING facing = getMainPlayerFacing();
    Rectangle attackRange;

    // Generate Attack Range
    if(facing == NORTH){
        attackRange = (Rectangle){getMainPlayerCenter().x - NORMAL_ATTACK_WIDTH/2, getMainPlayerCenter().y - NORMAL_ATTACK_HEIGHT, NORMAL_ATTACK_WIDTH, NORMAL_ATTACK_HEIGHT};
    }
    else if(facing == WEST){
        attackRange = (Rectangle){getMainPlayerCenter().x - NORMAL_ATTACK_WIDTH, getMainPlayerCenter().y - NORMAL_ATTACK_HEIGHT/2, NORMAL_ATTACK_HEIGHT, NORMAL_ATTACK_WIDTH};
    }
    else if(facing == EAST){
        attackRange = (Rectangle){getMainPlayerCenter().x, getMainPlayerCenter().y - NORMAL_ATTACK_HEIGHT/2, NORMAL_ATTACK_HEIGHT, NORMAL_ATTACK_WIDTH};
    }
    else if(facing == SOUTH){
        attackRange = (Rectangle){getMainPlayerCenter().x - NORMAL_ATTACK_WIDTH/2, getMainPlayerCenter().y, NORMAL_ATTACK_WIDTH, NORMAL_ATTACK_HEIGHT};
    }

    // For Attack Animation Position
    normalAnimationCenter = (Vector2){attackRange.x + attackRange.width/2, attackRange.y + attackRange.height/2};
    drawWarriorNormalAnimation(true);

    if(DEBUG == 1)
        DrawRectangleLines((int)attackRange.x, (int)attackRange.y, (int)attackRange.width, (int)attackRange.height, RED);
    
    // Check if Zombie is in Attack Range
    while(currZombie != NULL){
        if(currZombie->zombieType != FIRSTNODEZOMBIE){
            if(CheckCollisionRecs(currZombie->hitbox, attackRange)){
                int damage = getNormalAttackDamage();
                currZombie->health -= damage;
                // Draw Damage Value (with critical damage calculate)
                if((rand()%100+1) <= getCriticalHitPercentage()*100){
                    // Critical Hit
                    addZombieDamageAnimation(currZombie, damage*2, CRITICAL);
                }else{
                    // Normal Hit
                    addZombieDamageAnimation(currZombie, damage, NORMAL);
                }
            }
        }
        currZombie = currZombie->nextZombie;
    }

    return;
}

void drawWarriorNormalAnimation(bool newAttack){
    static float currTime = 0.0;

    currTime += GetFrameTime();
    if(currTime >= ANIMATION_TIME) currTime = ANIMATION_TIME;

    if(newAttack) currTime = 0.0; // if there is a new attack reset rimer

    if(currTime < ANIMATION_TIME){
        DrawCircleLines(normalAnimationCenter.x, normalAnimationCenter.y, 30.0, RED);   
        DrawTexture(normalAttackTexture, normalAnimationCenter.x - normalAttackTexture.width/2, normalAnimationCenter.y - normalAttackTexture.height/2, WHITE );
    }

    return;
}

#endif
