#ifndef ITEMSTATE_H
#define ITEMSTATE_H

#include "dommodel.h"

class ItemState
{
public:
    ~ItemState();
    DomItem *getState();

private:
    friend class DomModel;

    ItemState(DomItem *other);
    void setState(DomItem *other);



private:
    DomItem *item;

};

#endif // ITEMSTATE_H
