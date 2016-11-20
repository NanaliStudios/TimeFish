//
//  StoreKitX_android.cpp
//  Timefish
//
//  Created by midchow on 2015. 2. 4..
//
//

#include "C2DXStoreJni.h"
#include "StoreKitX.h"

//static StoreKitDelegateBridge* s_pSKDelegateBridge = nil;
static StoreKitX* s_pStoreKitX = nullptr;

StoreKitX* StoreKitX::getInstance()
{
    if (s_pStoreKitX == nullptr)
    {
        s_pStoreKitX = new StoreKitX();
//        s_pSKDelegateBridge = [[StoreKitDelegateBridge alloc] init];
    }
    return s_pStoreKitX;
}

bool StoreKitX::initIAP()
{
    //
    // With Android, it looks like there're no 'canMakePayments' options
    //
    return true;
}

void StoreKitX::requestProductData()
{
    //
    // With Android platfor, we request data with initialization.
    // So, do nothing here.
    //
}

void StoreKitX::onRequestProduct(std::string productID, std::string price, std::string currency, bool purchased)
{
    StoreItemInfo item;
    item.storeItemID = productID;
    item.price = price;
    item.priceValue = 0.0; // not using
    item.currency = currency;
    item.purchased = purchased;
    
    itemList.push_back(item);
    
    if (item.storeItemID == "timefish.remove_ads") {
        s_pStoreKitX->setRemoveAdsItemIndex();
    }
}

void StoreKitX::onFinishPurchase(std::string transactionID, std::string productID, bool restored, bool success)
{
    if (success) {
        //
        // TODO: Do we need to update item purchased information in the itemList?
        //
        if (restored) {
            //
            // if purchasing is turned into restoring, there's no
            //
            pushItemToProvideListByIndex(transactionID.c_str(), productIdx, true);
            //
            if (m_delegate) {
                m_delegate->finishRestore(true);
            }
        }
        else {
            pushItemToProvideList(transactionID.c_str(), productID.c_str(), false);
            //
            if (m_delegate) {
                m_delegate->finishPurchase(true);
            }
        }
    }
    else {
        //
        if (m_delegate) {
            m_delegate->finishPurchase(false);
        }
    }
}

void StoreKitX::beginPurchaseFreeshSkin(const char* skinName)
{
    //
    // set productIdx for possible future restore
    //
    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].storeItemID == std::string("timefish.")+std::string(skinName)) {
            productIdx = (int)i;
            break;
        }
        else {
            productIdx = -1;
        }
    }

    //
//    beginPurchaseFreeshSkinJNI(skinName);
}

void StoreKitX::beginPurchasePackage(const char* packageID)
{
    //
    // set productIdx for possible future restore
    //
    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].storeItemID == std::string("timefish.")+std::string(packageID)) {
            productIdx = (int)i;
            break;
        }
        else {
            productIdx = -1;
        }
    }
    
    //
//    beginPurchasePackageJNI(packageID);
}

void StoreKitX::beginPurchaseRemoveAds()
{
    //
    // set productIdx for possible future restore
    //
    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].storeItemID == std::string("timefish.")+std::string("remove_ads")) {
            productIdx = (int)i;
            break;
        }
        else {
            productIdx = -1;
        }
    }

    //
//    beginPurchaseRemoveAdsJNI("remove_ads");
}

void StoreKitX::restorePurchasesItems()
{
    //
    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].purchased == true) {
            pushItemToProvideList("", itemList[i].storeItemID.c_str(), true);
        }
    }

    //
//    showRestoreAllPopupJNI(LocalizationManager::getInstance()->getLocalizationString("RestoreUI1").c_str(),
//                           LocalizationManager::getInstance()->getLocalizationString("RestoreUI2").c_str(),
//                           LocalizationManager::getInstance()->getLocalizationString("RestoreUI3").c_str());

    //
    if (m_delegate) {
        m_delegate->finishRestoreAll(true);
    }
}

PurchaseItemInfo StoreKitX::getPurchaseItemInfoByTransantionID(std::string tId)
{
    int listSize = (int)itemIdxListToProvide.size();
    
    for (int i=0; i < listSize; i++) {
        if (itemIdxListToProvide.at(i).transactionID == tId) {
            return itemIdxListToProvide.at(i);
        }
    }
    
    return PurchaseItemInfo("0",-1,0);
}

StoreItemInfo StoreKitX::getItemInfoAt(int idx)
{
    return itemList[idx];
}

std::string StoreKitX::getProductNameByIndex(int idx)
{
    StoreItemInfo info = itemList.at(idx);
    
    // TimeFish.etc_01
    std::string productId = std::string(info.storeItemID).substr(9);
    
    return productId;
}

std::string StoreKitX::getPriceByProductName(const char* productName)
{
    std::string productStr = std::string(productName);
    std::transform(productStr.begin(), productStr.end(), productStr.begin(), ::tolower);

    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].storeItemID == std::string("timefish.")+productStr) {
            return itemList[i].price;
        }
    }
    return "0";
}

void StoreKitX::pushItemToProvideList(const char* transactionID, const char* productID, bool restoreItem)
{
    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].storeItemID == std::string(productID)) {
//            CCLOG("%s", transactionID);
//            CCLOG("%s", productID);
//            CCLOG("%d", restoreItem);
            itemIdxListToProvide.push_back(PurchaseItemInfo(transactionID, (int)i, restoreItem));

            return;
        }
    }
}

void StoreKitX::pushItemToProvideListByIndex(const char* transactionID, int idx, bool restoreItem)
{
    itemIdxListToProvide.push_back(PurchaseItemInfo(transactionID, idx, restoreItem));
}

void StoreKitX::popItemIdxListToProvide(int &idx, bool &restoredItem)
{
    int listSize = (int)itemIdxListToProvide.size();
    
    idx = itemIdxListToProvide.at(listSize - 1).idx;
    restoredItem = itemIdxListToProvide.at(listSize - 1).restoredItem;
    itemIdxListToProvide.pop_back();
    
    productIdx = -1; // re-init
    
    listSize = (int)itemIdxListToProvide.size();
}
