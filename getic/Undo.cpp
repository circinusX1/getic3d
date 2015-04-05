// Undo.cpp: implementation of the CUndo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Brush.h"
#include "SceItem.h"
#include "Mmove.h"
#include "Undo.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"


static long Gdelindex = 0;

void  CUndo::Add(Brush* pB, DWORD action, int sequence)
{
    if(_fromHere || pB->_brushflags & BRSH_NEW)
        return;
    GNewUID = 0;
    Record r;
    r.e_action = action;
    r.e_type   = CLS_BRUSH;
    r.p_Obj    = new Brush(*pB);
    r.p_Obj0   = M_DELETED == action ? 0 : pB;
    r.e_sequence = sequence;
    r.e_delseq   = 0;
    if(M_DELETED==action)
    {
        ++Gdelindex;
        r.e_delseq   = Gdelindex;
        _DelPtrs(pB);
        
    }
    _records << r;
    _Resize();
    GNewUID = 1;
}

void  CUndo::Add(SceItem* pB, DWORD action, int sequence)
{
    if(_fromHere || pB->_flags & BRSH_NEW)
        return;
    Record r;
    r.e_action = action;
    r.e_type   = CLS_ITEM;
    //dispatch scene item;
    r.p_Obj    = new SceItem(*pB);
    r.p_Obj0   = ITM_DELETED==action ? 0 : pB;
    r.e_sequence = sequence;
    r.e_delseq   = 0;
    if(ITM_DELETED==action){
        ++Gdelindex;
        r.e_delseq   = Gdelindex;
        _DelPtrs(pB);
    }
    _records << r;
    _Resize();

}

void  CUndo::_UpdatePtrs(BaseEntity* pB, BaseEntity* pBnew, int delseq)
{
    FOREACH(vvector<Record>, _records, record)
    {
        if(((*record).p_Obj0 && ((*record).p_Obj0 == pB)) ||  
           ((*record).e_delseq > 0 && ((*record).e_delseq == delseq)))
        {
            (*record).p_Obj0 = pBnew;
        }
    }
}

void  CUndo::_DelPtrs(BaseEntity* pB)
{
    FOREACH(vvector<Record>, _records, record)
    {
        if((*record).p_Obj0 == pB)
        {
            (*record).e_delseq = Gdelindex;
            (*record).p_Obj0 = 0; //deleted
        }
    }
}

void  CUndo::_ClearRecord(Record& record)
{
    delete record.p_Obj;
}


void  CUndo::_Resize()
{
    if(_records.size() > 64)
    {
        Record& r = _records[0];
        delete r.p_Obj;
        _records.erase(_records.begin());
    }
}

void  CUndo::Clear()
{
    FOREACH(vvector<Record>, _records, record)
    {
        _ClearRecord(*record);
    }
    _records.clear();
}

void  CUndo::Undo(CZ_ed2Doc* pDoc,  DWORD action)
{
    _fromHere = 1;
    if(_records.size())
    {
        Record &rec = _records.back();
        _records.pop_back();
        int sequence = rec.e_sequence;
        
        do{
            if(rec.e_type == CLS_ITEM)
            {
                _UndoItem(pDoc, rec);
            }
            else
            {
                _UndoBrush(pDoc, rec);
            }//vector
            if(0==_records.size())
                break;
             
            rec = _records.back();
            _records.pop_back(); 

        }while(rec.e_sequence == sequence);
    }
    _fromHere = 0;
}

void  CUndo::_UndoBrush(CZ_ed2Doc* pDoc, const Record& rec)
{
    if(rec.p_Obj0)
        pDoc->DeleteBrush((Brush*)rec.p_Obj0);
    
    _UpdatePtrs(rec.p_Obj0, rec.p_Obj, rec.e_delseq);    
    ((Brush*)rec.p_Obj)->Dirty(1);
    pDoc->AddBrush((Brush*)rec.p_Obj);
    pDoc->SelectBrush((Brush*)rec.p_Obj);//for recut
}

void  CUndo::_UndoItem(CZ_ed2Doc* pDoc, const Record& rec)
{
    if(rec.p_Obj0)
        pDoc->DelItem((SceItem*)rec.p_Obj0);
    _UpdatePtrs(rec.p_Obj0, rec.p_Obj, 0);
    pDoc->AddItem((SceItem*)rec.p_Obj);
}

