//
//  StoreKitX.h
//  Timefish
//
//  Created by midchow on 2015. 1. 29..
//
//

#ifndef Timefish_StoreKitX_h
#define Timefish_StoreKitX_h

#include "LocalizationManager.h"

enum StoreKitErrorCode : int
{
    // Dont Change order of this error codes.
    // This Codes are matched with android version storekit error codes (java)
    StoreKitErrorCode_None = -1,
    StoreKitErrorCode_Success = 0,
    StoreKitErrorCode_InvalidProductIdentifier,
    StoreKitErrorcode_TransactionFailed,
    StoreKitErrorCode_InvalidReceipt_NoReceiptOnTransaction,
    StoreKitErrorCode_NoCurrentPurchaseOnFinishPurchase,
    StoreKitErrorCode_InvalidReceipt,       // Receipt Validation Failed
    StoreKitErrorCode_IAPRestriction,   // IAP Restriction,
    StoreKitErrorCode_Max,
};

//
typedef struct
{
public:
    std::string storeItemID;
    std::string price;
    double priceValue;
    std::string currency;
    bool purchased;
} StoreItemInfo ;
typedef std::vector<StoreItemInfo> StoreItemList;

struct PurchaseItemInfo
{
public:
    std::string transactionID;
    int idx;
    bool restoredItem;
    PurchaseItemInfo(std::string tID, int i = 0, bool f = false) {
        transactionID = tID;
        idx = i;
        restoredItem = f;
    }
};
typedef PurchaseItemInfo PurchaseItemInfo;

//
class StoreKitXDelegate
{
public:
    virtual ~StoreKitXDelegate() {}

    virtual void finishPurchase(bool success) {}
    virtual void finishRestore(bool success) {}
    virtual void finishRestoreAll(bool success) {}
};

class StoreKitX {
    
public:
    static StoreKitX* getInstance();

    StoreItemList itemList;
    std::vector<PurchaseItemInfo> itemIdxListToProvide;

    int removeAdsItemIndex;
    void setRemoveAdsItemIndex() {
        removeAdsItemIndex = itemList.size() - 1;
    }

    static void purgeStoreKitX();
    
    void setDelegate(StoreKitXDelegate* delegate) { m_delegate = delegate; }
    StoreKitXDelegate* getDelegate() { return m_delegate; }
    bool initIAP();

    //
    void requestProductData();
    void beginPurchaseFreeshSkin(const char* skinName);
    void beginPurchasePackage(const char* packageID);
    void beginPurchaseRemoveAds();
    void restorePurchasesItems();

//    void onErrorPurchaseResponse(const char* storeItemID, const int lengthStoreItemID, StoreKitErrorCode errorCode, const char* errorDescription, const int lengthErrorDescription);

    PurchaseItemInfo getPurchaseItemInfoByTransantionID(std::string tId);
    StoreItemInfo getItemInfoAt(int idx);

    std::string getProductNameByIndex(int idx);
    std::string getPriceByProductName(const char* productName);
    void pushItemToProvideList(const char* transactionID, const char* productID, bool restoreItem);
    void pushItemToProvideListByIndex(const char* transactionID, int idx, bool restoreItem);
    
    void popItemIdxListToProvide(int &idx, bool &restoredItem);
    
    // for Android
    void onRequestProduct(std::string productID, std::string price, std::string currency, bool purchased);
    void onFinishPurchase(std::string transactionID, std::string productID, bool restored, bool success);

private:
    StoreKitX()
    : m_delegate(nullptr)
    {
    }

    StoreKitXDelegate* m_delegate;

    // for Android
    int productIdx;
};

#endif
