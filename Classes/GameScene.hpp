//
//  GameScene.hpp
//  Santra
//
//  Created by Anıl Gülgör on 16/12/15.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "cocos2d.h"
#include "AppManager.hpp"
#include "Footballer.hpp"
#include "Kart.h"
#include "Player.hpp"
#include "Opponent.hpp"
#include "MenuScene.hpp"
#include "appwarp.h"
#include "ImlecNode.hpp"
#include "Explosion.hpp"
#include "App42API.h"
#include "picojson.h"
#include "PluginAdMob/PluginAdMob.h"
#include "SimpleAudioEngine.h"

#include <cocos/ui/UILoadingBar.h>

#define APPWARP_APP_KEY     "26d0bbabc7da3fb8710dab08f04edbfd4e6e1938b9fc2202bc46e627003b26a2"
#define APPWARP_SECRET_KEY  "4f9c64daa2dec09603758f664a516844b39a60118d2da90ea6f02dba965ed303"
#define GAME_NAME           "onlinegametest"
#define MAX_PLAYER          2


#define cardFlipTime .7

USING_NS_CC;

using namespace cocos2d;

using namespace cocos2d::ui;

bool sortByKartPower(const Kart *lhs, const Kart *rhs); // sort


class GameScene : public cocos2d::Layer, public AppWarp::ConnectionRequestListener, public AppWarp::NotificationListener, public AppWarp::RoomRequestListener, public AppWarp::ZoneRequestListener, public App42CallBack, public sdkbox::AdMobListener {
    
  
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    
    void onEnterTransitionDidFinish();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);
    
    
    void controlForConnection(float dt);
    
    
    //appwarp
    void connectToAppwarp();
    void onConnectDone(int res, int reasonCode);
    
    void noConnectionWarning(std::string st1, std::string st2, std::string st3);
    
    void removeNoConnectionWarning();
    
    void goBackMainForError(float dt);
    
    void scheduleRecover();
    void unscheduleRecover();
    
    void recover(float dt);


    void updateUserInformation(const char *username, double gold, double xp, double level, double avatar, double games, double won, double lost, double point);
    
    void updateUserProfile(const char *jsonString);

    void updateLeaderboardScore(double point, std::string gameName);
    
    void getLastScoreByUsername(std::string username, std::string gamename);
    
    void onStorageRequestCompleted(void *response);

    void onScoreBoardRequestCompleted(void *response);
    
    void onLastScoreRequestCompleted(void *response);
    
    
    void openWarningPanel(std::string warning);
    
    void closeWarningSchedule(float dt);

    void addLoader();
    
    void deleteAndGone(float dt);
    
    void updateMyStatusAndChangeScene(std::string tName);
    
    //exit android
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
    
    
    //admob
    
    void adViewDidReceiveAd(const std::string &name) {}
    void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {}
    void adViewWillPresentScreen(const std::string &name) {}
    void adViewDidDismissScreen(const std::string &name) {}
    void adViewWillDismissScreen(const std::string &name) {}
    void adViewWillLeaveApplication(const std::string &name) {}
    
    
    void showInsterstitial();
    
    CREATE_FUNC(GameScene);
    
    
    
    
private:
    
    AppManager *appManager;
    
    int actOfGame;
    
    bool deathMatchStart, desteDagitiliyor;
    
    float HEIGHT, WIDTH;
    
    void setGameTypeWithIdentifier(std::string kindOfGame);
    void setOlumMaci();
    void setMac();
    void createTutorial(std::string tutorialString);
    void createDrawPanel(std::string drawString);
    
    Sprite *blackBg, *loaderBack, *loader;

    Label *connectingLabel;
    
    bool winState;
    
    ImlecNode *imlec;
    
    Explosion *exp;
    
    
    Sprite *ayarlarPerde;
    
    Node *errorPanel, *tutorialNode;
    
    void removeTutorial();
    void removeDraw();
    
    //Kart
    
    Kart *kart;
    Vector<Kart*>deckVec;
    Vector<Kart *> tempKarts;
    Vector<Kart *> myPlayedKarts;
    Vector<Kart *> playerKarts;
    Vector<Kart *> opponentKarts;
    
    Vector<Kart *>playerAllWonCards;
    Vector<Kart *>opponentAllWonCards;
    
    int counter = 5;
    
    //olummacidesteanimkart
    Vector<Sprite *>olumMaciDesteCards;
    
    bool isMyTurn = false;
    
    bool ithrowed = false;
    
    void desteDagit(float dt);
    
    //multiplayer
    
    void onChatReceived(AppWarp::chat chatevent);
    void onUserLeftRoom (AppWarp::room event , std::string username);  
    void updateOpponentStatus(Kart *kart ,int sira);
    void onDeleteRoomDone(AppWarp::room event);
       
    void updateOpponentPenaltyKartStatus(Kart *kart);
    
    
    bool waitForRecover = false;

    //avatars
    
    void setAvatars(std::string playerName, std::string opponentName);
    void setAvatarsOlumMaci(std::string playerName, std::string opponentName);
    
    Label *playerNameLabel ,*opponentNameLabel;
    
    void setSure();
    
    void levelAnim();
    
    Label *sureLabel;
    
    Sprite *surePanel;
    
    ProgressTimer *progressTimer;
    
    //throwsure
    
    Sprite *sureDolu;
    Sprite *sureBos;
    
    void startThrowCounter();
    
    bool canThrow;
    
    bool letMeThrow = false;
    
    int getLowestKartWithOrder();
    
    //Player
    
    Player *player;
    
    void createPlayer();
    void createImlec(bool isMyTurn);
    void showPlayerCardsOnScreen(Player *player);
    void showOpponentCardsOnScreen(Opponent *opponent);
    
    void createDeck(int kartNum);
    
    void playKart(Kart *playingKart);
    
    void sendMeKart(Vec2 pos);
    
    void giveMeKart(cocos2d::Point pos);
    
    void sendDataToOpponent(Kart *playedKart, int i);
    
    void finishTheGame();

    void listAllPlayerCards(float dt);
    
    void listAllOpponentCards(float dt);
    
    void removePlayedCardsFromScene(float dt);
    
    void controlOpponentCard(Kart *oppKart);
    
    Vector<Sprite *> controlSigns;
    
    Vector<Kart *>sahnePlayerKarts;
    Vector<Kart *>sahneOpponentKarts;
    
    void finishAndGetPanel(float dt);
    
    void invitePlayAgain();
    
    void rakipTekrarBekle();
    
    bool backPanelIsActive = false;
    
    Node *tekrarBeklePanel;
    
    Sprite *tekrarBekleBg, *evetButton, *hayirButton;
    
    Node *quitPanel;
    
    Button *yesButton, *noButton;
    
    Sprite *quitBG;
    
    void callQuitKeyAction();
    
    Label *tekrarBekleLabel;
    
    void getInvitePlayAgain();
    
    //scores of game
    
    float playerFinishScore, opponentFinishScore;
    
    float playerCurrentScore, opponentCurrentScore;
    
    Label *playerScoreLabel, *opponentScoreLabel;
    
    void updateScoreLabels(float player, float opponent);
    
    //Game control card
    
    Kart *controlCard;

    //control opponent card and add signals
    
    void addToSignalsOnCards(Kart *opponentCard);
    void removeSignalsOnCards();
    Vector<Sprite*>signalsOnCards;
    
    //won lose
    
    void playerWon();
    void playerLose();
    
    void wonPerThrow(float dt);
    void losePerThrow(float dt);
    void penalties(float dt);
    void chooseCardForPenalty(float dt);
    
    bool isOnPenalty;
    
    void controlScoreForPenalties(float dt);
    
    void sendDataToOpponentForPenalty(Vector<Kart*>penaltyKarts);
    
    Vector<Kart*>penaltyPool;
    Vector<Kart*>opponentPenaltyPool;
    
    //ölüm maçı
    
    void createPlayerForDeathMatch();
    void showPlayerCardsForDeathMatch();
    void showOpponentCardsForDeathMatch();
    
    void createDesteForOnline();
    
    Vector<Sprite *>desteCardOnline;
    
    void startCounter(int order);
    
    bool finishDeathMatch(int order);
    
    void decideDeathMatchResult(float dt);
    
    void openOthers();
    
    int glb_order,glb_random;
    
    Vector<Node *>deathMatchCardArray;
    Vector<Sprite *>deathMathchOpponentArray;

    //online control card array
    Vector<Kart *>cardPool;
    Vector<Kart *>tempPool;
    
    //opponent properties
    Opponent *opponent;
    void createOpponent();
    
    //game time
    int timeOfGame;
    
    //footballers
    
    Footballer *footballer;
    Vector<Footballer*>footballerVector;
    
    void setAllFootballers();
    
    //game end menu
    
    void gameEnd();
    
    void createEndPanel(bool winstate);
    
    //Sprites
    
    Sprite *backGround, *avatarPlayer, *avatarOpponent;
    
    //panel
    Sprite *panel, *panelAvatarPlayer, *panelAvatarOpponent,*isimButtonPlayer, *isimButtonOpponent, *odulBut1, *odulBut2, *odulBut3, *geriBut, *yenidenOynaBut, *gameTitle;
    
    Sprite *isimPlayerLabel , *isimOpponentLabel, *kramponPanel, *altinPanel, *xpPanel;
    
    Node *panelNode;
    
    
    
    //left panel game
    
    void setAyarlarButton();
    void setMyInventory();
    
    Node *ayarlarNode;
    Node *inventoryNode;
    
    Sprite *quitGameButton, *soundButton, *musicButton, *ayarlarBut, *ayarlarPanel;
    Sprite *inventoryButton, *inventoryPanel;
    
    Sprite *perdeForPenalty;
    
    
    void openAyarlarMenu(bool isOpen);
    bool isAyarlarOpen;
    
};


#endif /* GameScene_hpp */
