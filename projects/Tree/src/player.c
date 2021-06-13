#include "Tree.h"

#define GRANADE_MAX 4

unsigned int hp = 100;

static Entity usedWeapon = 0;
static Entity jetpack = 0;
static Entity granades[GRANADE_MAX];
static unsigned int granadeCount = 0;

extern Entity player;
extern vec4 cam;

extern void cameraTriggerAlarm();

static void playerDrawGUI()
{
    static color_t red = {1.0f, 0.0f, 0.0f, 0.8f};
    static color_t white = {1.0f, 1.0f, 1.0f, 1.0f};
    static float camera[] = {0.0f, 0.0f, 1.0f, 0.0f};

    char string[16];
    sprintf(string, "%u", hp);
    glUseProgram(assetsGetShader(SHADER_FONT));
    drawStringCentered(
        string, 
        assetsGetFont(FONT_1), 
        &red.r,
        -8.0f,
        (viewport.y / viewport.z) - 50.0f, 
        0.5f, 
        0.0f
    );

    if (granadeCount) {
        glUseProgram(assetsGetShader(SHADER_TEXTURE));
        texture_t* t = assetsGetTexture(TEXTURE_GRANADE);
        vec2 v = {(viewport.x / viewport.z) * 0.5f + 90.0f, (viewport.y / viewport.z) - 36.0f};
        sprintf(string, "%u", granadeCount);
        shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &camera[0]);
        drawTextureColor(*t, v, white);
        shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &cam.x);

        glUseProgram(assetsGetShader(SHADER_FONT));
        drawStringCentered(
            string, 
            assetsGetFont(FONT_1), 
            &red.r,
            (viewport.x / viewport.z) * 0.5 + 98.0f,
            (viewport.y / viewport.z) - 70.0f, 
            0.5f, 
            0.0f
        );
    }

    if (usedWeapon) {
        glUseProgram(assetsGetShader(SHADER_TEXTURE));
        rect_t r = *(rect_t*)entity_get(usedWeapon, COMPONENT_GL_RECT);
        if (r.w < 0.0f) r.w *= -1.0f;
        if (r.h < 0.0f) r.h *= -1.0f;
        rect_t v = {(viewport.x / viewport.z) * 0.5f + 90.0f, (viewport.y / viewport.z) - 24.0f, r.w, r.h};
        unsigned int ammo = ((GunType*)entity_get(usedWeapon, COMPONENT_GUN_CONTROLLER))->ammo;
        sprintf(string, "%u", ammo);
        shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &camera[0]);
        drawTextureIdColor(*(unsigned int*)entity_get(usedWeapon, COMPONENT_TEX_ID), v, white);
        shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &cam.x);

        glUseProgram(assetsGetShader(SHADER_FONT));
        drawStringCentered(
            string, 
            assetsGetFont(FONT_1), 
            &red.r,
            (viewport.x / viewport.z) * 0.5 + 98.0f,
            (viewport.y / viewport.z) - 55.0f, 
            0.5f, 
            0.0f
        );
    }

    if (jetpack) {
        glUseProgram(assetsGetShader(SHADER_TEXTURE));
        rect_t r = *(rect_t*)entity_get(jetpack, COMPONENT_GL_RECT);
        if (r.w < 0.0f) r.w *= -1.0f;
        if (r.h < 0.0f) r.h *= -1.0f;
        rect_t v = {(viewport.x / viewport.z) * 0.5f + 90.0f, (viewport.y / viewport.z) - 12.0f, r.w, r.h};
        unsigned int fuel = *(unsigned int*)entity_get(jetpack, COMPONENT_AMMO);
        sprintf(string, "%u", fuel);
        shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &camera[0]);
        drawTextureIdColor(*(unsigned int*)entity_get(jetpack, COMPONENT_TEX_ID), v, white);
        shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &cam.x);

        glUseProgram(assetsGetShader(SHADER_FONT));
        drawStringCentered(
            string, 
            assetsGetFont(FONT_1), 
            &red.r,
            (viewport.x / viewport.z) * 0.5 + 98.0f,
            (viewport.y / viewport.z) - 40.0f, 
            0.5f, 
            0.0f
        );
    }
}

static void playerDrawDead()
{
    static color_t red = {1.0f, 0.0f, 0.0f, 1.0f};
    static unsigned int sprite = SPRITE_KID_DEAD;
    vec2 center = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f};

    texture_t* t = assetsGetSprite(sprite)->textures;
    entity_set(player, COMPONENT_SPRITE_ID, &sprite);

    rect_t* rPhi = entity_get(player, COMPONENT_PHI_RECT);
    rPhi->w = t->width;
    rPhi->h = t->height;

    drawFramebuffer();
    
    glUseProgram(assetsGetShader(SHADER_FONT));
    drawStringCentered(
        "You Died", 
        assetsGetFont(FONT_1), 
        &red.r,
        center.x - 84.0f, 
        center.y - 32.0f, 
        0.75f, 
        0.0f
    );
}

static void pickObject()
{
    Entity gun = checkGunCollision(player);
    Entity jet = checkJetpackCollision(player);
    Entity granade = checkGranadeCollision(player);
    bool ret = false;

    if (granade && granadeCount < GRANADE_MAX - 1) {
        granadeCollect(granade);
        granades[granadeCount] = granade;
        granadeCount ++;
        ret = true;
    }
    if (jet) {
        if (jetpack) jetpackDrop(jetpack);
        jetpackPick(jet);
        jetpack = jet;
        ret = true;
    }  
    if (gun) {
        if (usedWeapon) gunDrop(usedWeapon);
        gunPick(gun) ;
        usedWeapon = gun;
        ret = true;
    } 
    if (ret) return;

    if (usedWeapon) {
        gunDrop(usedWeapon);
        usedWeapon = 0;
    } else if (jetpack) {
        jetpackDrop(jetpack);
        jetpack = 0;
    }
}

void playerReset()
{
    static unsigned int k = SPRITE_KID_STANDING;
    static vec2 vecZero = {0.0f, 0.0f};

    sprite_t* sprite = assetsGetSprite(k);
    rect_t r = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f, sprite->textures->width, sprite->textures->height};
    entity_set(player, COMPONENT_GL_RECT, &r);
    entity_set(player, COMPONENT_PHI_RECT, &r);
    entity_set(player, COMPONENT_VEL_VEC2, &vecZero);
    entity_set(player, COMPONENT_SPRITE_ID, &k);
    hp = 100;
}

static void playerDeadStep(float deltaTime)
{
    static float deadTimer = 1.0f;
    deadTimer -= deltaTime * 0.3f;
    if (deadTimer < 0.0f) {
        deadTimer = 1.0f;
        playerDrawDead();
        playerReset();
    } else playerDrawDead();
}

void playerGameStep(float deltaTime)
{
    if (hp == 0) {
        playerDeadStep(deltaTime);
        return;
    }

    static const float speed = 100.0f;
    static const float jump = 200.0f;
    static const float wallSlideSpeed = -40.0f;
    static const float wallJumpDash = 300.0f;
    static const float wallJumpForce = 170.0f;
    static const float doubleJumpForce = 150.0f;
    static const float dashSpeed = 500.0f;
    static const float doubleClick = 0.2f;
    static const float dashWait = 0.15f;

    static bool doubleJump = true;
    static bool canDash = true; 
    static float isDashing = 0.0f;

    static float doubleKeyTimer = 0.2f;
    static int doubleKeyDir = 0;

    unsigned int sprite = SPRITE_KID_STANDING;
    rect_t* const playerPhi = (rect_t*)entity_get(player, COMPONENT_PHI_RECT);
    rect_t* const playerTex = (rect_t*)entity_get(player, COMPONENT_GL_RECT);
    vec2* const vel = (vec2*)entity_get(player, COMPONENT_VEL_VEC2);

    bool keyGranade = keyboard_pressed(GLFW_KEY_N);
    bool keySpace = keyboard_pressed(GLFW_KEY_SPACE);
    bool keyShift = keyboard_down(GLFW_KEY_LEFT_SHIFT);
    bool keyUp = (keyboard_pressed(GLFW_KEY_UP) || keyboard_pressed(GLFW_KEY_W));
    bool keyRight = (keyboard_down(GLFW_KEY_RIGHT) || keyboard_down(GLFW_KEY_D));
    bool keyLeft = (keyboard_down(GLFW_KEY_LEFT) || keyboard_down(GLFW_KEY_A));
    bool keyRightPressed = (keyboard_pressed(GLFW_KEY_RIGHT) || keyboard_pressed(GLFW_KEY_D));
    bool keyLeftPressed = (keyboard_pressed(GLFW_KEY_LEFT) || keyboard_pressed(GLFW_KEY_A));
    int keyDir = 0;

    bool isGrounded = checkRigidCollision(player, vec2_new(0.0f, -1.0f));
    bool camAlarm = false;
    bool wallSliding = false;

    //Input Logic
    if (keyRight) keyDir++;
    if (keyLeft) keyDir--;
    if (keyRightPressed) {
        doubleKeyDir++;
        doubleKeyTimer = doubleClick;
    }
    if (keyLeftPressed) {
        doubleKeyDir--;
        doubleKeyTimer = doubleClick;
    }
    if (doubleKeyTimer < 0.0f) doubleKeyDir = 0;
    else doubleKeyTimer -= deltaTime;

    if (keySpace) pickObject();
    if (keyShift && jetpack) jetpackUse(jetpack);
    if (keyGranade && granadeCount) granadeThrow(granades[--granadeCount], vec2_new(playerPhi->x, playerPhi->y));

    Entity explotion = checkPhiScaledCollision(player, COMPONENT_EXPLOTION);
    if (explotion) {
        float scale = *(float*)entity_get(explotion, COMPONENT_SCALE);
        float exp = *(float*)entity_get(explotion, COMPONENT_EXPLOTION);
        unsigned int dammage = (unsigned int)(exp / scale);
        hp = (hp - dammage) * (dammage <= hp);
    }

    //Lateral Movement
    if (keyDir) {
        if (!checkRigidCollision(player, vec2_new((float)keyDir * speed * deltaTime, 1.0f))) {
            playerPhi->x += (float)keyDir * deltaTime * speed;
            playerTex->x = playerPhi->x;
            sprite = SPRITE_KID_RUNNING;
            playerTex->w = (float)keyDir;
            camAlarm = true;
            if (isGrounded) {
                smokeEmit(vec2_new(playerPhi->x, playerPhi->y - 12.0f), TEXTURE_SMOKE);
            }
        } 
        //WallSlide
        else if (!isGrounded) {
            wallSliding = true;
            if (vel->y < wallSlideSpeed) {
                vel->y = wallSlideSpeed;
                smokeEmit(vec2_new(playerPhi->x + (float)keyDir * 8.0f, playerPhi->y - 12.0f), TEXTURE_SMOKE);
            }
            if (keyUp && !checkRigidCollision(player, vec2_new(-(float)keyDir * wallJumpDash * deltaTime, wallJumpForce * deltaTime))) {
                vel->y = wallJumpForce;
                vel->x = -(float)keyDir * wallJumpDash;
                doubleJump = true;
                smokeEmit(vec2_new(playerPhi->x + (float)keyDir * 8.0f, playerPhi->y - 12.0f), TEXTURE_SMOKE);
            }
        }
    }

    //Dashing
    if (doubleKeyDir && doubleKeyDir % 2 == 0 && canDash) {
        float dir = (float)(doubleKeyDir / 2);
        if (!checkRigidCollision(player, vec2_new(dir * dashSpeed * deltaTime, 1.0f))) {
            vel->x = dir * dashSpeed;
            canDash = false;
            isDashing = dashWait;
        }
        doubleKeyDir = 0;
    }
    if (isDashing > 0.0f) {
        vel->y = 0.0f;
        sprite = SPRITE_KID_JUMPING;
        camAlarm = true;
        isDashing -= deltaTime;
        shadowEmit(vec2_new(playerPhi->x, playerPhi->y), vel->x);
    }
    else if (isGrounded || wallSliding) canDash = true;
    
    //Vertical Phi
    if (isGrounded) {
        doubleJump = true;
        if (keyUp && !checkRigidCollision(player, vec2_new(0.0f, jump * deltaTime))) {
            vel->y = jump;
            camAlarm = true;
            smokeEmit(vec2_new(playerPhi->x, playerPhi->y - 12.0f), TEXTURE_SMOKE);
        }
    } else {
        camAlarm = true;
        if (keyUp && !wallSliding && doubleJump &&
            !checkRigidCollision(player, vec2_new(0.0f, doubleJumpForce * deltaTime))) {
            vel->y = doubleJumpForce;
            doubleJump = false;
            smokeEmit(vec2_new(playerPhi->x, playerPhi->y - 12.0f), TEXTURE_SMOKE);
        }
        if (vel->y > 0.0f) sprite = SPRITE_KID_JUMPING;
        else sprite = SPRITE_KID_FALLING;
    } 

    //Horizontal Phi
    if (vel->x > 0.1f || vel->x < -0.1f) {
        float lerp = lerpf(vel->x, 0.0f, 0.1f);
        if (!checkRigidCollision(player, vec2_new(lerp * deltaTime, 1.0f))) {
            vel->x = lerp;
        } else vel->x = -lerp * 0.5f;
    } else vel->x = 0.0f;

    //Set calculated values
    if (playerPhi->y < -600.0f) playerReset();
    entity_set(player, COMPONENT_SPRITE_ID, &sprite);
    sprite_frame_update(assetsGetSprite(sprite));
    if (camAlarm) cameraTriggerAlarm();
    playerDrawGUI();
}