#pragma once

#include <memory>

#include "Item.h"
#include "ItemTemplate.h"

std::unique_ptr<Item> makeItem(const ItemTemplate& t);
