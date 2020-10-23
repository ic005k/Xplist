#include "itemstate.h"

ItemState::ItemState(DomItem *other)
{
    item = new DomItem();
    this->setState(other);
}

ItemState::~ItemState()
{
    delete item;
}

void ItemState::setState(DomItem *other)
{
    item = other->clone();
}

DomItem *ItemState::getState()
{
    return item;
}


