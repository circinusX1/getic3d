#if !defined(AFX_GAMEITEM_H__3B6BEA24_0EAE_4DFD_BA81_9068ACD7F347__INCLUDED_)
#define AFX_GAMEITEM_H__3B6BEA24_0EAE_4DFD_BA81_9068ACD7F347__INCLUDED_


#include "SceItem.h"
#include "CustItemCat.h"
#include <string>
#include <map>

using namespace std;
class GameItem : public SceItem  
{
public:
    GameItem(){_item    = ITM_GAME;};
	GameItem(const char* catname);
    GameItem(const GameItem& r);
    GameItem& operator=(const GameItem& r);

    OVERWRITE void Serialize(FileWrap* pfv, const vvector<Texture>& texNames, map<int,int>& texassoc);

	void InitCommit();

    void    GlDraw2D(CZ_ed2View* pV);

	virtual ~GameItem();
	void    Commit();

    vvector<NameTypeValue>    _data;

};

#endif // !defined(AFX_GAMEITEM_H__3B6BEA24_0EAE_4DFD_BA81_9068ACD7F347__INCLUDED_)
