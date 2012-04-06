#pragma once


#include "FunctionList.h"
#include "../lib/ResourceSet.h"
#include "../lib/GameConstants.h"
#include "UIFramework/CIntObject.h"
#include "UIFramework/CIntObjectClasses.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

/*
 * GUIClasses.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

struct ArtifactLocation;
class IArtifactSetBase;
class CStackBasicDescriptor;
class CBonusSystemNode;
class CArtifact;
class CDefEssential;
class CAdventureMapButton;
class CHighlightableButtonsGroup;
class CDefHandler;
struct HeroMoveDetails;
class CDefEssential;
class CGHeroInstance;
class CAdvMapInt;
class CCastleInterface;
class CBattleInterface;
class CStack;
class CComponent;
class CCreature;
struct SDL_Surface;
struct CPath;
class CCreatureAnim;
class CSelectableComponent;
class CCreatureSet;
class CGObjectInstance;
class CGDwelling;
class CSlider;
struct UpgradeInfo;
template <typename T> struct CondSh;
class CInGameConsole;
class CGarrisonInt;
class CInGameConsole;
struct Component;
class CArmedInstance;
class CGTownInstance;
class StackState;
class CPlayerInterface;
class CHeroWindow;
class CArtifact;
class CArtifactsOfHero;
class CCreatureArtifactSet;
class CResDataBar;
struct SPuzzleInfo;
class CGGarrison;
class CStackInstance;
class IMarket;
class CTextBox;
class CArtifactInstance;
class IBonusBearer;
class CArtPlace;
class CAnimImage;

/// text + comp. + ok button
class CInfoWindow : public CSimpleWindow
{ //window able to delete its components when closed
	bool delComps; //whether comps will be deleted

public:
	typedef std::vector<std::pair<std::string,CFunctionList<void()> > > TButtonsInfo;
	typedef std::vector<CComponent*> TCompsInfo;
	int ID; //for identification
	CTextBox *text;
	std::vector<CAdventureMapButton *> buttons;
	std::vector<CComponent*> components;
	CSlider *slider;

	void setDelComps(bool DelComps);
	virtual void close();

	void show(SDL_Surface * to);
	void showAll(SDL_Surface * to);
	void sliderMoved(int to);

	CInfoWindow(std::string Text, int player, const TCompsInfo &comps = TCompsInfo(), const TButtonsInfo &Buttons = TButtonsInfo(), bool delComps = true); //c-tor
	CInfoWindow(); //c-tor
	~CInfoWindow(); //d-tor

	static void showYesNoDialog( const std::string & text, const std::vector<CComponent*> *components, const CFunctionList<void( ) > &onYes, const CFunctionList<void()> &onNo, bool DelComps = true, int player = 1); //use only before the game starts! (showYesNoDialog in LOCPLINT must be used then)
	static CInfoWindow *create(const std::string &text, int playerID = 1, const std::vector<CComponent*> *components = NULL, bool DelComps = false);
};

/// component selection window
class CSelWindow : public CInfoWindow
{ //warning - this window deletes its components by closing!
public:
	void selectionChange(unsigned to);
	void madeChoice(); //looks for selected component and calls callback
	CSelWindow(const std::string& text, int player, int charperline ,const std::vector<CSelectableComponent*> &comps, const std::vector<std::pair<std::string,CFunctionList<void()> > > &Buttons, int askID); //c-tor
	CSelWindow(){}; //c-tor
	//notification - this class inherits important destructor from CInfoWindow
};

/// popup displayed on R-click
class CRClickPopup : public CIntObject
{
public:
	virtual void activate();
	virtual void deactivate();
	virtual void close();
	void clickRight(tribool down, bool previousState);

	CRClickPopup();
	virtual ~CRClickPopup(); //d-tor

	static void createAndPush(const std::string &txt, const CInfoWindow::TCompsInfo &comps = CInfoWindow::TCompsInfo());
	static void createAndPush(const CGObjectInstance *obj, const Point &p, EAlignment alignment = BOTTOMRIGHT);
};

/// popup displayed on R-click
class CRClickPopupInt : public CRClickPopup
{
public:
	IShowActivatable *inner;
	bool delInner;

	void show(SDL_Surface * to);
	void showAll(SDL_Surface * to);
	CRClickPopupInt(IShowActivatable *our, bool deleteInt); //c-tor
	virtual ~CRClickPopupInt(); //d-tor
};

class CInfoPopup : public CRClickPopup
{
public:
	bool free; //TODO: comment me
	SDL_Surface * bitmap; //popup background
	void close();
	void show(SDL_Surface * to);
	CInfoPopup(SDL_Surface * Bitmap, int x, int y, bool Free=false); //c-tor
	CInfoPopup(SDL_Surface * Bitmap, const Point &p, EAlignment alignment, bool Free=false); //c-tor
	CInfoPopup(SDL_Surface *Bitmap = NULL, bool Free = false); //default c-tor

	void init(int x, int y);
	~CInfoPopup(); //d-tor
};

/// common popup window component
class CComponent : public virtual CIntObject
{
public:
	enum Etype
	{
		primskill, secskill, resource, creature, artifact, experience, secskill44, spell, morale, luck, building, hero, flag
	} type; //component type
	int subtype; //TODO: comment me
	int val; //TODO: comment me

	std::string description; //r-click
	std::string subtitle; //TODO: comment me

	SDL_Surface *img; //our image
	bool free; //should surface be freed on delete

	SDL_Surface * setSurface(std::string defName, int imgPos);

	void init(Etype Type, int Subtype, int Val);
	CComponent(Etype Type, int Subtype, int Val, SDL_Surface *sur=NULL, bool freeSur=false); //c-tor
	CComponent(const Component &c); //c-tor
	CComponent();; //c-tor
	virtual ~CComponent(); //d-tor

	void clickRight(tribool down, bool previousState); //call-in
	SDL_Surface * getImg() const;
	virtual void show(SDL_Surface * to);
	virtual void activate();
	virtual void deactivate();
};

class CSelectableComponent : public CComponent, public CKeyShortcut
{
public:
	bool selected; //if true, this component is selected
	boost::function<void()> onSelect; //function called on selection change

	void clickLeft(tribool down, bool previousState); //call-in
	void init();
	CSelectableComponent(Etype Type, int Sub, int Val, boost::function<void()> OnSelect = 0); //c-tor
	CSelectableComponent(const Component &c, boost::function<void()> OnSelect = 0); //c-tor
	~CSelectableComponent(); //d-tor
	virtual void show(SDL_Surface * to);
	void activate();
	void deactivate();
	void select(bool on);
};

////////////////////////////////////////////////////////////////////////////////


class CGarrisonInt;

/// A single garrison slot which holds one creature of a specific amount
class CGarrisonSlot : public CIntObject
{
public:
	int ID; //for identification
	CGarrisonInt *owner;
	const CStackInstance *myStack; //NULL if slot is empty
	const CCreature *creature;
	int count; //number of creatures
	int upg; //0 - up garrison, 1 - down garrison
	bool active; //TODO: comment me
	bool highlight;

	virtual void hover (bool on); //call-in
	const CArmedInstance * getObj();
	bool our();
	void clickRight(tribool down, bool previousState);
	void clickLeft(tribool down, bool previousState);
	void activate();
	void deactivate();
	void showAll(SDL_Surface * to);
	CGarrisonSlot(CGarrisonInt *Owner, int x, int y, int IID, int Upg=0, const CStackInstance * Creature=NULL);
	~CGarrisonSlot(); //d-tor
};

/// Class which manages slots of upper and lower garrison, splitting of units
class CGarrisonInt :public CIntObject
{
public:
	int interx; //space between slots
	Point garOffset; //offset between garrisons (not used if only one hero)
	CGarrisonSlot *highlighted; //chosen slot
	std::vector<CAdventureMapButton *> splitButtons; //may be empty if no buttons

	int p2, //TODO: comment me
	    shiftPos;//1st slot of the second row, set shiftPoint for effect
	bool splitting, pb,
	     smallIcons, //true - 32x32 imgs, false - 58x64
	     removableUnits,//player can remove units from up
	     twoRows,//slots will be placed in 2 rows
		 owned[2];//player owns up or down army [0] upper, [1] lower

// 	const CCreatureSet *set1; //top set of creatures
// 	const CCreatureSet *set2; //bottom set of creatures

	std::vector<CGarrisonSlot*> slotsUp, slotsDown; //slots of upper and lower garrison
	const CArmedInstance *armedObjs[2]; //[0] is upper, [1] is down
	//const CArmedInstance *oup, *odown; //upper and lower garrisons (heroes or towns)

	void setArmy(const CArmedInstance *army, bool bottomGarrison);
	void addSplitBtn(CAdventureMapButton * button);
	void createSet(std::vector<CGarrisonSlot*> &ret, const CCreatureSet * set, int posX, int distance, int posY, int Upg );

	void activate();
	void createSlots();
	void deleteSlots();
	void recreateSlots();

	void splitClick(); //handles click on split button
	void splitStacks(int am2); //TODO: comment me
	//x, y - position;
	//inx - distance between slots;
	//pomsur, SurOffset - UNUSED
	//s1, s2 - top and bottom armies;
	//removableUnits - you can take units from top;
	//smallImgs - units images size 64x58 or 32x32;
	//twoRows - display slots in 2 row (1st row = 4, 2nd = 3)
	CGarrisonInt(int x, int y, int inx, const Point &garsOffset, SDL_Surface *pomsur, const Point &SurOffset, const CArmedInstance *s1, const CArmedInstance *s2=NULL, bool _removableUnits = true, bool smallImgs = false, bool _twoRows=false); //c-tor
	~CGarrisonInt(); //d-tor
};

/// List of heroes which is shown at the right of the adventure map screen
class CHeroList
	: public CList
{
public:
	CDefHandler *mobile, *mana; //mana and movement indicators
	int posmobx, posporx, posmanx, posmoby, pospory, posmany;

	CHeroList(int Size); //c-tor
	int getPosOfHero(const CGHeroInstance* h); //hero's position on list
	void genList();
	void select(int which); //call-in
	void mouseMoved (const SDL_MouseMotionEvent & sEvent); //call-in
	void clickLeft(tribool down, bool previousState); //call-in
	void clickRight(tribool down, bool previousState); //call-in
	void hover (bool on); //call-in
	void keyPressed (const SDL_KeyboardEvent & key); //call-in
	void updateHList(const CGHeroInstance *toRemove=NULL); //removes specific hero from the list or recreates it
	void updateMove(const CGHeroInstance* which); //draws move points bar
	void draw(SDL_Surface * to);
	void show(SDL_Surface * to);
	void showAll(SDL_Surface * to);
	void init();
	int size(); //how many elements do we have
};

/// List of towns which is shown at the right of the adventure map screen
class CTownList
	: public CList
{
public:
	boost::function<void()> fun; //function called on selection change
	int posporx,pospory;

	CTownList(int Size, int x, int y, std::string arrupg, std::string arrdog); //c-tor
	~CTownList(); //d-tor
	void genList();
	void select(int which); //call-in
	void mouseMoved (const SDL_MouseMotionEvent & sEvent);  //call-in
	void clickLeft(tribool down, bool previousState);  //call-in
	void clickRight(tribool down, bool previousState); //call-in
	void hover (bool on);  //call-in
	void keyPressed (const SDL_KeyboardEvent & key);  //call-in
	void draw(SDL_Surface * to);
	void show(SDL_Surface * to);
	void showAll(SDL_Surface * to);
	int size(); //how many elements do we have
};

/// draws picture with creature on background, use Animated=true to get animation
class CCreaturePic : public CIntObject
{
private:
	CPicture *bg; //background
	CCreatureAnim *anim; //displayed animation

public:
	CCreaturePic(int x, int y, const CCreature *cre, bool Big=true, bool Animated=true); //c-tor
	~CCreaturePic(); //d-tor
};

/// Recruitment window where you can recruit creatures
class CRecruitmentWindow : public CIntObject
{
public:
	static const int SPACE_BETWEEN = 18;
	static const int CREATURE_WIDTH = 102;
	static const int TOTAL_CREATURE_WIDTH = SPACE_BETWEEN + CREATURE_WIDTH;

	struct creinfo
	{
		SDL_Rect pos;
		CCreaturePic *pic; //creature's animation
		int ID, amount; //creature ID and available amount
		std::vector<std::pair<int,int> > res; //res_id - cost_per_unit
	};
	std::vector<int> amounts; //how many creatures we can afford
	std::vector<creinfo> creatures; //recruitable creatures
	boost::function<void(int,int)> recruit; //void (int ID, int amount) <-- call to recruit creatures
	CSlider *slider; //for selecting amount
	CAdventureMapButton *max, *buy, *cancel;
	CPicture *bitmap; //background
	CGStatusBar *bar;
	int which; //which creature is active

	const CGDwelling *dwelling;
	int level;
	const CArmedInstance *dst;

	void close();
	void Max();
	void Buy();
	void Cancel();
	void sliderMoved(int to);
	void clickLeft(tribool down, bool previousState);
	void clickRight(tribool down, bool previousState);
	void showAll(SDL_Surface * to);
	void initCres();
	CRecruitmentWindow(const CGDwelling *Dwelling, int Level, const CArmedInstance *Dst, const boost::function<void(int,int)> & Recruit, int y_offset = 0); //creatures - pairs<creature_ID,amount> //c-tor
	~CRecruitmentWindow(); //d-tor
};

/// Split window where creatures can be splitted up into two single unit stacks
class CSplitWindow : public CIntObject
{
public:
	CGarrisonInt *gar;
	CSlider *slider;
	CCreaturePic *animLeft, *animRight; //creature's animation
	CAdventureMapButton *ok, *cancel;
	SDL_Surface *bitmap; //background
	int a1, a2, c; //TODO: comment me
	bool which; //which creature is selected
	int last; //0/1/2 - at least one creature must be in the src/dst/both stacks; -1 - no restrictions

	CSplitWindow(int cid, int max, CGarrisonInt *Owner, int Last = -1, int val=0); //c-tor; val - initial amount of second stack
	~CSplitWindow(); //d-tor
	void activate();
	void split();
	void close();
	void deactivate();
	void show(SDL_Surface * to);
	void clickLeft(tribool down, bool previousState); //call-in
	void keyPressed (const SDL_KeyboardEvent & key); //call-in
	void sliderMoved(int to);
};

/// Raised up level windowe where you can select one out of two skills
class CLevelWindow : public CIntObject
{
public:
	int heroPortrait;
	SDL_Surface *bitmap; //background
	std::vector<CSelectableComponent *> comps; //skills to select
	CAdventureMapButton *ok;
	boost::function<void(ui32)> cb;

	void close();
	CLevelWindow(const CGHeroInstance *hero, int pskill, std::vector<ui16> &skills, boost::function<void(ui32)> &callback); //c-tor
	~CLevelWindow(); //d-tor
	void activate();
	void deactivate();
	void selectionChanged(unsigned to);
	void show(SDL_Surface * to);
};

/// Resource bar like that at the bottom of the adventure map screen
class CMinorResDataBar : public CIntObject
{
public:
	SDL_Surface *bg; //background bitmap
	void show(SDL_Surface * to);
	void showAll(SDL_Surface * to);
	CMinorResDataBar(); //c-tor
	~CMinorResDataBar(); //d-tor
};

/// Town portal, castle gate window
class CObjectListWindow : public CIntObject
{
	class CItem : public CIntObject
	{
		CObjectListWindow *parent;
		CLabel *text;
		CPicture *border;
	public:
		const size_t index;
		CItem(CObjectListWindow *parent, size_t id, std::string text);

		void select(bool on);
		void clickLeft(tribool down, bool previousState);
	};

	boost::function<void(int)> onSelect;//called when OK button is pressed, returns id of selected item.
	CLabel * title;
	CLabel * descr;

	CListBox *list;
	CPicture *bg; //background
	CPicture *titleImage;//title image (castle gate\town portal picture)
	CAdventureMapButton *ok, *exit;

	std::vector< std::pair<int, std::string> > items;//all items present in list

	void init(CPicture * titlePic, std::string _title, std::string _descr);
public:
	size_t selected;//index of currently selected item
	/// Callback will be called when OK button is pressed, returns id of selected item. initState = initially selected item
	/// Image can be NULL
	///item names will be taken from map objects
	CObjectListWindow(const std::vector<int> &_items, CPicture * titlePic, std::string _title, std::string _descr,
                      boost::function<void(int)> Callback);
	CObjectListWindow(const std::vector<std::string> &_items, CPicture * titlePic, std::string _title, std::string _descr,
                      boost::function<void(int)> Callback);

	CIntObject *genItem(size_t index);
	void elementSelected();//call callback and close this window
	void changeSelection(size_t which);
	void keyPressed (const SDL_KeyboardEvent & key);
};

class CArtifactHolder : public virtual CIntObject
{
public:
	CArtifactHolder();

	virtual void artifactRemoved(const ArtifactLocation &artLoc)=0;
	virtual void artifactMoved(const ArtifactLocation &artLoc, const ArtifactLocation &destLoc)=0;
	virtual void artifactDisassembled(const ArtifactLocation &artLoc)=0;
	virtual void artifactAssembled(const ArtifactLocation &artLoc)=0;
};

class CWindowWithArtifacts : public CArtifactHolder
{
public:
	std::vector<CArtifactsOfHero *> artSets;

	void artifactRemoved(const ArtifactLocation &artLoc);
	void artifactMoved(const ArtifactLocation &artLoc, const ArtifactLocation &destLoc);
	void artifactDisassembled(const ArtifactLocation &artLoc);
	void artifactAssembled(const ArtifactLocation &artLoc);
};

class CTradeWindow : public CWindowWithArtifacts //base for markets and altar of sacrifice
{
public:
	enum EType
	{
		RESOURCE, PLAYER, ARTIFACT_TYPE, CREATURE, CREATURE_PLACEHOLDER, ARTIFACT_PLACEHOLDER, ARTIFACT_INSTANCE
	};
	class CTradeableItem : public CIntObject
	{
		const CArtifactInstance *hlp; //holds ptr to artifact instance id type artifact
	public:
		EType type;
		int id;
		int serial;
		bool left;
		std::string subtitle; //empty if default

		const CArtifactInstance *getArtInstance() const;
		void setArtInstance(const CArtifactInstance *art);
// 		const CArtifact *getArt() const;
// 		void setArt(const CArtifact *artT) const;

		CFunctionList<void()> callback;
		bool downSelection;

		void showAllAt(const Point &dstPos, const std::string &customSub, SDL_Surface * to);

		void clickRight(tribool down, bool previousState);
		void hover (bool on);
		void showAll(SDL_Surface * to);
		void clickLeft(tribool down, bool previousState);
		SDL_Surface *getSurface();
		std::string getName(int number = -1) const;
		CTradeableItem(EType Type, int ID, bool Left, int Serial);
	};

	const IMarket *market;
	const CGHeroInstance *hero;
	CPicture *bg; //background

	CArtifactsOfHero *arts;
	//all indexes: 1 = left, 0 = right
	std::vector<CTradeableItem*> items[2];
	CTradeableItem *hLeft, *hRight; //highlighted items (NULL if no highlight)
	EType itemsType[2];

	EMarketMode::EMarketMode mode;//0 - res<->res; 1 - res<->plauer; 2 - buy artifact; 3 - sell artifact
	CAdventureMapButton *ok, *max, *deal;
	CSlider *slider; //for choosing amount to be exchanged
	bool readyToTrade;

	CTradeWindow(const IMarket *Market, const CGHeroInstance *Hero, EMarketMode::EMarketMode Mode); //c

	void showAll(SDL_Surface * to);

	void initSubs(bool Left);
	void initTypes();
	void initItems(bool Left);
	std::vector<int> *getItemsIds(bool Left); //NULL if default
	void getPositionsFor(std::vector<Rect> &poss, bool Left, EType type) const;
	void removeItems(const std::set<CTradeableItem *> &toRemove);
	void removeItem(CTradeableItem * t);
	void getEmptySlots(std::set<CTradeableItem *> &toRemove);
	void setMode(EMarketMode::EMarketMode Mode); //mode setter

	void artifactSelected(CArtPlace *slot); //used when selling artifacts -> called when user clicked on artifact slot

	virtual void getBaseForPositions(EType type, int &dx, int &dy, int &x, int &y, int &h, int &w, bool Right, int &leftToRightOffset) const = 0;
	virtual void selectionChanged(bool side) = 0; //true == left
	virtual Point selectionOffset(bool Left) const = 0;
	virtual std::string selectionSubtitle(bool Left) const = 0;
	virtual void garrisonChanged() = 0;
	virtual void artifactsChanged(bool left) = 0;
};

class CMarketplaceWindow : public CTradeWindow
{
	bool printButtonFor(EMarketMode::EMarketMode M) const;
public:
	int r1, r2; //suggested amounts of traded resources
	bool madeTransaction; //if player made at least one transaction
	CTextBox *traderText;

	void setMax();
	void sliderMoved(int to);
	void makeDeal();
	void selectionChanged(bool side); //true == left
	CMarketplaceWindow(const IMarket *Market, const CGHeroInstance *Hero = NULL, EMarketMode::EMarketMode Mode = EMarketMode::RESOURCE_RESOURCE); //c-tor
	~CMarketplaceWindow(); //d-tor

	Point selectionOffset(bool Left) const;
	std::string selectionSubtitle(bool Left) const;


	void garrisonChanged(); //removes creatures with count 0 from the list (apparently whole stack has been sold)
	void artifactsChanged(bool left);
	void resourceChanged(int type, int val);

	void getBaseForPositions(EType type, int &dx, int &dy, int &x, int &y, int &h, int &w, bool Right, int &leftToRightOffset) const;
	void updateTraderText();
};

class CAltarWindow : public CTradeWindow
{
public:
	CAltarWindow(const IMarket *Market, const CGHeroInstance *Hero, EMarketMode::EMarketMode Mode); //c-tor

	void getExpValues();
	~CAltarWindow(); //d-tor

	std::vector<int> sacrificedUnits, //[slot_nr] -> how many creatures from that slot will be sacrificed
		expPerUnit;

	CAdventureMapButton *sacrificeAll, *sacrificeBackpack;
	CLabel *expToLevel, *expOnAltar;


	void selectionChanged(bool side); //true == left
	void SacrificeAll();
	void SacrificeBackpack();

	void putOnAltar(int backpackIndex);
	bool putOnAltar(CTradeableItem* altarSlot, const CArtifactInstance *art);
	void makeDeal();
	void showAll(SDL_Surface * to);

	void blockTrade();
	void sliderMoved(int to);
	void getBaseForPositions(EType type, int &dx, int &dy, int &x, int &y, int &h, int &w, bool Right, int &leftToRightOffset) const;
	void mimicCres();

	Point selectionOffset(bool Left) const;
	std::string selectionSubtitle(bool Left) const;
	void garrisonChanged();
	void artifactsChanged(bool left);
	void calcTotalExp();
	void setExpToLevel();
	void updateRight(CTradeableItem *toUpdate);

	void artifactPicked();
	int firstFreeSlot();
	void moveFromSlotToAltar(int slotID, CTradeableItem* altarSlot, const CArtifactInstance *art);
};

class CSystemOptionsWindow : public CIntObject
{
private:
	CLabel *title;
	CLabelGroup *leftGroup;
	CLabelGroup *rightGroup;
	CPicture * bg; //background of window
	CAdventureMapButton *load, *save, *restart, *mainMenu, *quitGame, *backToMap; //load and restart are not used yet
	CHighlightableButtonsGroup * heroMoveSpeed;
	CHighlightableButtonsGroup * mapScrollSpeed;
	CHighlightableButtonsGroup * musicVolume, * effectsVolume;

	//CHighlightableButton * showPath;
	CHighlightableButton * showReminder;
	//CHighlightableButton * quickCombat;
	//CHighlightableButton * videoSubs;
	CHighlightableButton * newCreatureWin;
	CHighlightableButton * fullscreen;

	CAdventureMapButton *gameResButton;

	void setMusicVolume( int newVolume );
	void setSoundVolume( int newVolume );
	void setHeroMoveSpeed( int newSpeed );
	void setMapScrollingSpeed( int newSpeed );

	//functions bound to buttons
	void bsavef(); //save game
	void bquitf(); //quit game
	void breturnf(); //return to game
	void bmainmenuf(); //return to main menu

	//functions for checkboxes
	void toggleReminder(bool on);
	void toggleCreatureWin(bool on);
	void toggleFullscreen(bool on);

	void selectGameRes(bool pregame);
	void setGameRes(bool pregame, int index);

	void pushSDLEvent(int type, int usercode);

public:
	CSystemOptionsWindow(const SDL_Rect & pos, CPlayerInterface * owner); //c-tor
};

class CTavernWindow : public CIntObject
{
public:
	class HeroPortrait : public CIntObject
	{
	public:
		std::string hoverName;
		const CGHeroInstance *h;
		char descr[100];		// "XXX is a level Y ZZZ with N artifacts"

		void clickLeft(tribool down, bool previousState);
		void clickRight(tribool down, bool previousState);
		void hover (bool on);
		HeroPortrait(int &sel, int id, int x, int y, const CGHeroInstance *H);
		void show(SDL_Surface * to);

	private:
		int *_sel;
		const int _id;

	} *h1, *h2; //recruitable heroes

	CPicture *bg; //background
	CGStatusBar *bar; //tavern's internal status bar
	int selected;//0 (left) or 1 (right)
	int oldSelected;//0 (left) or 1 (right)

	CAdventureMapButton *thiefGuild, *cancel, *recruit;
	const CGObjectInstance *tavernObj;

	CTavernWindow(const CGObjectInstance *TavernObj); //c-tor
	~CTavernWindow(); //d-tor

	void recruitb();
	void close();
	void thievesguildb();
	void show(SDL_Surface * to);
};

class CInGameConsole : public CIntObject
{
private:
	std::list< std::pair< std::string, int > > texts; //<text to show, time of add>
	boost::mutex texts_mx;		// protects texts
	std::vector< std::string > previouslyEntered; //previously entered texts, for up/down arrows to work
	int prevEntDisp; //displayed entry from previouslyEntered - if none it's -1
	int defaultTimeout; //timeout for new texts (in ms)
	int maxDisplayedTexts; //hiw many texts can be displayed simultaneously
public:
	std::string enteredText;
	void activate();
	void deactivate();
	void show(SDL_Surface * to);
	void print(const std::string &txt);
	void keyPressed (const SDL_KeyboardEvent & key); //call-in

	void startEnteringText();
	void endEnteringText(bool printEnteredText);
	void refreshEnteredText();

	CInGameConsole(); //c-tor
};

/// Can interact on left and right mouse clicks
class LRClickableAreaWTextComp: public LRClickableAreaWText
{
public:
	int baseType;
	int bonusValue, type;
	virtual void clickLeft(tribool down, bool previousState);
	virtual void clickRight(tribool down, bool previousState);

	LRClickableAreaWTextComp(const Rect &Pos = Rect(0,0,0,0), int BaseType = -1);
	CComponent * createComponent() const;
};

class MoraleLuckBox : public LRClickableAreaWTextComp
{
public:
	bool morale; //true if morale, false if luck
	bool small;

	void set(const IBonusBearer *node);
	void showAll(SDL_Surface * to);

	MoraleLuckBox(bool Morale, const Rect &r, bool Small=false);
	~MoraleLuckBox();
};

/// Opens hero window by left-clicking on it
class CHeroArea: public CIntObject
{
public:
	const CGHeroInstance * hero;

	CHeroArea(int x, int y, const CGHeroInstance * _hero);

	void clickLeft(tribool down, bool previousState);
	void clickRight(tribool down, bool previousState);
	void hover(bool on);
	void showAll(SDL_Surface * to);
};

/// Opens town screen by left-clicking on it
class LRClickableAreaOpenTown: public LRClickableAreaWTextComp
{
public:
	const CGTownInstance * town;
	void clickLeft(tribool down, bool previousState);
	void clickRight(tribool down, bool previousState);
	LRClickableAreaOpenTown();
};

/// Artifacts can be placed there. Gets shown at the hero window
class CArtPlace: public LRClickableAreaWTextComp
{
public:
	int slotID; //Arts::EPOS enum + backpack starting from Arts::BACKPACK_START

	bool picked;
	bool marked;
	bool locked;
	CArtifactsOfHero * ourOwner;
	const CArtifactInstance * ourArt;

	CArtPlace(const CArtifactInstance * Art); //c-tor
	CArtPlace(Point position, const CArtifactInstance * Art = NULL); //c-tor
	void clickLeft(tribool down, bool previousState);
	void clickRight(tribool down, bool previousState);
	void select ();
	void deselect ();
	void activate();
	void deactivate();
	void showAll(SDL_Surface * to);
	bool fitsHere (const CArtifactInstance * art) const; //returns true if given artifact can be placed here

	void setMeAsDest(bool backpackAsVoid = true);
	void setArtifact(const CArtifactInstance *art);

	~CArtPlace(); //d-tor
};

/// Contains artifacts of hero. Distincts which artifacts are worn or backpacked
class CArtifactsOfHero : public CIntObject
{
	const CGHeroInstance * curHero;

	std::vector<CArtPlace *> artWorn; // 0 - head; 1 - shoulders; 2 - neck; 3 - right hand; 4 - left hand; 5 - torso; 6 - right ring; 7 - left ring; 8 - feet; 9 - misc1; 10 - misc2; 11 - misc3; 12 - misc4; 13 - mach1; 14 - mach2; 15 - mach3; 16 - mach4; 17 - spellbook; 18 - misc5
	std::vector<CArtPlace *> backpack; //hero's visible backpack (only 5 elements!)
	int backpackPos; //number of first art visible in backpack (in hero's vector)

public:
	struct SCommonPart
	{
		struct Artpos
		{
			int slotID;
			const CArtifactsOfHero *AOH;
			const CCreatureArtifactSet *CAS;
			const CArtifactInstance *art;

			Artpos();
			void clear();
			void setTo(const CArtPlace *place, bool dontTakeBackpack);
			IArtifactSetBase * getArtHolder(); // returns AOH or CAS
			bool valid();
			bool operator==(const ArtifactLocation &al) const;
		} src, dst;

		std::set<CArtifactsOfHero *> participants; // Needed to mark slots.

		void reset();
	} * commonInfo; //when we have more than one CArtifactsOfHero in one window with exchange possibility, we use this (eg. in exchange window); to be provided externally

	bool updateState; // Whether the commonInfo should be updated on setHero or not.

	CAdventureMapButton * leftArtRoll, * rightArtRoll;
	bool allowedAssembling;
	std::multiset<const CArtifactInstance*> artifactsOnAltar; //artifacts id that are technically present in backpack but in GUI are moved to the altar - they'll be omitted in backpack slots
	boost::function<void(CArtPlace*)> highlightModeCallback; //if set, clicking on art place doesn't pick artifact but highlights the slot and calls this function

	void realizeCurrentTransaction(); //calls callback with parameters stored in commonInfo
	void artifactMoved(const ArtifactLocation &src, const ArtifactLocation &dst);
	void artifactRemoved(const ArtifactLocation &al);
	void artifactAssembled(const ArtifactLocation &al);
	void artifactDisassembled(const ArtifactLocation &al);
	CArtPlace *getArtPlace(int slot);

	void setHero(const CGHeroInstance * hero);
	const CGHeroInstance *getHero() const;
	void dispose(); //free resources not needed after closing windows and reset state
	void scrollBackpack(int dir); //dir==-1 => to left; dir==1 => to right

	void safeRedraw();
	void markPossibleSlots(const CArtifactInstance* art);
	void unmarkSlots(bool withRedraw = true); //unmarks slots in all visible AOHs
	void unmarkLocalSlots(bool withRedraw = true); //unmarks slots in that particular AOH
	void setSlotData (CArtPlace* artPlace, int slotID);
	void updateWornSlots (bool redrawParent = true);

	void updateSlot(int i);
	void eraseSlotData (CArtPlace* artPlace, int slotID);

	CArtifactsOfHero(const Point& position, bool createCommonPart = false);
	//Alternative constructor, used if custom artifacts positioning required (Kingdom interface)
	CArtifactsOfHero(std::vector<CArtPlace *> ArtWorn, std::vector<CArtPlace *> Backpack,
		CAdventureMapButton *leftScroll, CAdventureMapButton *rightScroll, bool createCommonPart = false);
	~CArtifactsOfHero(); //d-tor
	void updateParentWindow();
	friend class CArtPlace;
};

class CGarrisonHolder : public virtual CIntObject
{
public:
	CGarrisonHolder();
	virtual void updateGarrisons(){};
};

class CWindowWithGarrison : public CGarrisonHolder
{
public:
	CGarrisonInt *garr;
	virtual void updateGarrisons();
};

/// Garrison window where you can take creatures out of the hero to place it on the garrison
class CGarrisonWindow : public CWindowWithGarrison
{
public:
	CPicture *bg; //background surface
	CLabel *title;
	CAdventureMapButton *quit;

	void close();
	void showAll(SDL_Surface * to);
	CGarrisonWindow(const CArmedInstance *up, const CGHeroInstance *down, bool removableUnits); //c-tor
	~CGarrisonWindow(); //d-tor
};

class CExchangeWindow : public CWindowWithGarrison, public CWindowWithArtifacts
{
	CPicture *background;
	CGStatusBar * ourBar; //internal statusbar

	CAdventureMapButton * quit, * questlogButton[2];

	std::vector<LRClickableAreaWTextComp *> secSkillAreas[2], primSkillAreas;

	MoraleLuckBox *morale[2], *luck[2];

	LRClickableAreaWText *speciality[2];
	LRClickableAreaWText *experience[2];
	LRClickableAreaWText *spellPoints[2];
	CHeroArea *portrait[2];

public:

	const CGHeroInstance* heroInst[2];
	CArtifactsOfHero * artifs[2];

	void close();
	void showAll(SDL_Surface * to);

	void questlog(int whichHero); //questlog button callback; whichHero: 0 - left, 1 - right

	void prepareBackground(); //prepares or redraws bg

	CExchangeWindow(si32 hero1, si32 hero2); //c-tor
	~CExchangeWindow(); //d-tor
};

/// Here you can buy ships
class CShipyardWindow : public CIntObject
{
public:
	CGStatusBar *bar;
	CPicture *bg; //background
	CPicture *bgWater;

	CLabel *title;
	CLabel *costLabel;

	CAnimImage *woodPic, *goldPic;
	CLabel *woodCost, *goldCost;

	CAnimImage *bgShip;
	CAdventureMapButton *build, *quit;

	CGStatusBar * statusBar;

	CShipyardWindow(const std::vector<si32> &cost, int state, int boatType, const boost::function<void()> &onBuy);
};

/// Puzzle screen which gets uncovered when you visit obilisks
class CPuzzleWindow : public CIntObject
{
private:
	SDL_Surface * background;
	CAdventureMapButton * quitb;
	CResDataBar * resdatabar;

	std::vector<std::pair<SDL_Surface *, const SPuzzleInfo *> > puzzlesToPullBack;
	ui8 animCount;

public:
	void activate();
	void deactivate();
	void show(SDL_Surface * to);

	CPuzzleWindow(const int3 &grailPos, double discoveredRatio);
	~CPuzzleWindow();
};

/// Creature transformer window
class CTransformerWindow : public CIntObject
{
public:
	class CItem : public CIntObject
	{
	public:
		int id;//position of creature in hero army
		bool left;//position of the item
		int size; //size of creature stack
		CTransformerWindow * parent;

		void move();
		void showAll(SDL_Surface * to);
		void clickLeft(tribool down, bool previousState);
		CItem(CTransformerWindow * _parent, int _size, int _id);
		~CItem();
	};

	const CArmedInstance *army;//object with army for transforming (hero or town)
	const CGHeroInstance *hero;//only if we have hero in town
	const CGTownInstance *town;//market, town garrison is used if hero == NULL
	CPicture *bg; //background
	std::vector<CItem*> items;

	CAdventureMapButton *all, *convert, *cancel;
	CGStatusBar *bar;
	void showAll(SDL_Surface * to);
	void makeDeal();
	void addAll();
	CTransformerWindow(const CGHeroInstance * _hero, const CGTownInstance * _town); //c-tor
	~CTransformerWindow(); //d-tor
};

class CUniversityWindow : public CIntObject
{
	class CItem : public CPicture
	{
	public:
		int ID;//id of selected skill
		CUniversityWindow * parent;

		void showAll(SDL_Surface * to);
		void clickLeft(tribool down, bool previousState);
		void clickRight(tribool down, bool previousState);
		void hover(bool on);
		int state();//0=can't learn, 1=learned, 2=can learn
		CItem(CUniversityWindow * _parent, int _ID, int X, int Y);
	};

public:
	const CGHeroInstance *hero;
	const IMarket * market;

	CPicture * green, * yellow, * red;//colored bars near skills
	CPicture *bg; //background
	std::vector<CItem*> items;

	CAdventureMapButton *cancel;
	CGStatusBar *bar;

	CUniversityWindow(const CGHeroInstance * _hero, const IMarket * _market); //c-tor
	~CUniversityWindow(); //d-tor
};

/// Confirmation window for University
class CUnivConfirmWindow : public CIntObject
{
public:
	CUniversityWindow * parent;
	CPicture * bg;
	CGStatusBar *bar;
	CAdventureMapButton *confirm, *cancel;

	CUnivConfirmWindow(CUniversityWindow * PARENT, int SKILL, bool available); //c-tor
	void makeDeal(int skill);
};

/// Hill fort is the building where you can upgrade units
class CHillFortWindow : public CWindowWithGarrison
{
public:

	int slotsCount;//=7;
	CGStatusBar * bar;
	CDefEssential *resources;
	CPicture *bg; //background surface
	CHeroArea *heroPic;//clickable hero image
	CAdventureMapButton *quit,//closes window
	                   *upgradeAll,//upgrade all creatures
	                   *upgrade[7];//upgrade single creature

	const CGObjectInstance * fort;
	const CGHeroInstance * hero;
	std::vector<int> currState;//current state of slot - to avoid calls to getState or updating buttons
	std::vector<TResources> costs;// costs [slot ID] [resource ID] = resource count for upgrade
	TResources totalSumm; // totalSum[resource ID] = value

	CHillFortWindow(const CGHeroInstance *visitor, const CGObjectInstance *object); //c-tor
	~CHillFortWindow(); //d-tor

	void activate();
	void showAll (SDL_Surface *to);
	std::string getDefForSlot(int slot);//return def name for this slot
	std::string getTextForSlot(int slot);//return hover text for this slot
	void makeDeal(int slot);//-1 for upgrading all creatures
	int getState(int slot); //-1 = no creature 0=can't upgrade, 1=upgraded, 2=can upgrade
	void updateGarrisons();//update buttons after garrison changes
};

class CThievesGuildWindow : public CIntObject
{
	const CGObjectInstance * owner;

	CGStatusBar * statusBar;
	CAdventureMapButton * exitb;
	SDL_Surface * background;
	CMinorResDataBar * resdatabar;

public:
	void activate();
	void show(SDL_Surface * to);

	void bexitf();

	CThievesGuildWindow(const CGObjectInstance * _owner);
	~CThievesGuildWindow();
};
