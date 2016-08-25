//
//  StoreKitX_ios.mm
//  Timefish
//
//  Created by midchow on 2015. 1. 29..
//
//

#import <StoreKit/StoreKit.h>
#import <Tapjoy/Tapjoy.h>

//#include "VerificationController.h"
#include "StoreKitX.h"

#import "FuseSDK.h"

#define RemoveAdsProductID @"TimeFish.RemoveAds"

// StoreKitDelegateBridge

@interface StoreKitDelegateBridge : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver, FuseDelegate>
{
    UIViewController* m_RootViewController;
    UIActivityIndicatorView *activityIndicator;
    
    BOOL preRequest;
};

- (BOOL) initIAP;
- (void) requestProductData;
- (void) restorePurchasesItems;

- (void) completeTransaction:(SKPaymentTransaction*)transaction;
//- (void) restoreTransaction:(SKPaymentTransaction*)transaction;
- (void) failedTransaction:(SKPaymentTransaction*)transaction;
- (BOOL) canMakePayments;
- (void) activateIndicator;
- (void) showErrorPopup:(NSInteger)errorCode productID:(NSString*)_productID;

-(void) sessionStartReceived;
-(void) sessionLoginError: (NSError*) _error;

@end

static StoreKitDelegateBridge* s_pSKDelegateBridge = nil;
static StoreKitX* s_pStoreKitX = NULL;

StoreKitX* StoreKitX::getInstance()
{
    if (s_pStoreKitX == NULL)
    {
        s_pStoreKitX = new StoreKitX();
        s_pSKDelegateBridge = [[StoreKitDelegateBridge alloc] init];

        //
        [FuseSDK startSession: @"71bb904a-a48d-4108-8a8a-ce1888bc39e4" delegate:s_pSKDelegateBridge withOptions:nil];
        [FuseSDK preloadAdForZoneID: @"71731eea"];

    }
    return s_pStoreKitX;
}

bool StoreKitX::initIAP()
{
    return [s_pSKDelegateBridge initIAP];
}

void StoreKitX::requestProductData()
{
    [s_pSKDelegateBridge requestProductData];
}

std::string StoreKitX::getProductNameByIndex(int idx)
{
    StoreItemInfo info = itemList.at(idx);
    
    // TimeFish.etc_01
    std::string productId = std::string(info.storeItemID).substr(9);

    return productId;
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

std::string StoreKitX::getPriceByProductName(const char* productName)
{
    for (std::vector<StoreItemInfo>::size_type i = 0; i != itemList.size(); i++) {
        if (itemList[i].storeItemID == std::string("TimeFish.")+std::string(productName)) {
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

//    listSize = (int)itemIdxListToProvide.size();
}

//////////////////////////////////////////////////////////////////////////////

void StoreKitX::beginPurchaseFreeshSkin(const char* skinName)
{
    //
    [s_pSKDelegateBridge activateIndicator];

    //
#ifdef ProductPurchaseCapture
    std::string storeItemID = std::string("TimeFish.Piggybank");
#else
    std::string storeItemID = std::string("TimeFish.")+std::string(skinName);
#endif
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:[NSString stringWithUTF8String:storeItemID.c_str()]]];
    productsRequest.delegate = s_pSKDelegateBridge;
    [productsRequest start];
}

void StoreKitX::beginPurchasePackage(const char* packageID)
{
    //
    [s_pSKDelegateBridge activateIndicator];

    //
    std::string storeItemID = std::string("TimeFish.")+std::string(packageID);
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:[NSString stringWithUTF8String:storeItemID.c_str()]]];
    productsRequest.delegate = s_pSKDelegateBridge;
    [productsRequest start];
}

void StoreKitX::beginPurchaseRemoveAds()
{
    //
    [s_pSKDelegateBridge activateIndicator];

    //
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:RemoveAdsProductID]];
    productsRequest.delegate = s_pSKDelegateBridge;
    [productsRequest start];
}

void StoreKitX::restorePurchasesItems()
{
    //
    [s_pSKDelegateBridge activateIndicator];
    
    //
    [s_pSKDelegateBridge restorePurchasesItems];
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
@implementation StoreKitDelegateBridge
- (BOOL) initIAP
{
    // Check if available
    if( [self canMakePayments] == NO )
        return NO;

    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];

    return true;
}

-(void) activateIndicator
{
    activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0, 0, 32, 32)];
    
    m_RootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    
    [activityIndicator setCenter:m_RootViewController.view.center];
    [activityIndicator setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhiteLarge];
    [m_RootViewController.view addSubview : activityIndicator];
    [activityIndicator startAnimating];
    
//    [self requestProductData];
}

- (void) restorePurchasesItems
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void) requestProductData
{
    //
    preRequest = YES;

    //
    // use product IDs:
    //
    NSSet *products = [NSSet setWithObjects:
                                RemoveAdsProductID,
                                @"TimeFish.snapback_01",
                                @"TimeFish.snapback_02",
                                @"TimeFish.snapback_03",
                                @"TimeFish.snapback_04",
                                @"TimeFish.snapback_05",
                                @"TimeFish.snapback_06",
                                @"TimeFish.snapback_07",
                                @"TimeFish.snapback_08",
                                @"TimeFish.snapback_09",
                                @"TimeFish.snapback_10",
                                @"TimeFish.job_01",
                                @"TimeFish.job_02",
                                @"TimeFish.job_03",
                                @"TimeFish.job_04",
                                @"TimeFish.job_05",
                                @"TimeFish.job_06",
                                @"TimeFish.job_07",
                                @"TimeFish.job_08",
                                @"TimeFish.job_09",
                                @"TimeFish.job_10",
                                @"TimeFish.retro_01",
                                @"TimeFish.retro_02",
                                @"TimeFish.retro_04",
                                @"TimeFish.retro_05",
                                @"TimeFish.retro_06",
                                @"TimeFish.retro_07",
                                @"TimeFish.retro_08",
                                @"TimeFish.retro_09",
                                @"TimeFish.retro_10",
                                @"TimeFish.history_02",
                                @"TimeFish.history_03",
                                @"TimeFish.history_04",
                                @"TimeFish.history_05",
                                @"TimeFish.history_06",
                                @"TimeFish.history_07",
                                @"TimeFish.history_08",
                                @"TimeFish.history_09",
                                @"TimeFish.history_10",
                                @"TimeFish.etc_01",
                                @"TimeFish.etc_02",
                                @"TimeFish.etc_03",
                                @"TimeFish.etc_04",
                                @"TimeFish.etc_05",
                                @"TimeFish.etc_06",
                                @"TimeFish.etc_07",
                                @"TimeFish.parody_01",
                                @"TimeFish.parody_02",
                                @"TimeFish.parody_03",
                                @"TimeFish.parody_05",
                                @"TimeFish.parody_07",
                                @"TimeFish.parody_08",
                                @"TimeFish.special_02",
                                @"TimeFish.Goldfish",
                                @"TimeFish.Package01",
                                @"TimeFish.Package02",
                                @"TimeFish.Package03",
                                @"TimeFish.Package04",
                                @"TimeFish.Package05",
                                @"TimeFish.Package06",
                                @"TimeFish.Package07",
                                @"TimeFish.Package08",
                                @"TimeFish.Package09",
                                @"TimeFish.Package10",
                                @"TimeFish.Piggybank",
                                nil];
    
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:products];
    productsRequest.delegate = s_pSKDelegateBridge;
    [productsRequest start];
}

- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse*)response
{
    if (!preRequest) {
        for( SKProduct* product in response.products ) {
            if( product != nil ) {
//                NSLog(@"InAppPurchase Product title: %@", product.localizedTitle);
//                NSLog(@"InAppPurchase Product description: %@", product.localizedDescription);
//                NSLog(@"InAppPurchase Product price: %@", product.price);
//                NSLog(@"InAppPurchase Product id: %@", product.productIdentifier);
//                
//                if (preRequest) {
//                    // Open IAP popup
//                    UIAlertView *message = [[UIAlertView alloc] initWithTitle:product.localizedTitle
//                                                                      message:product.localizedDescription
//                                                                     delegate:self
//                                                            cancelButtonTitle:nil
//                                                            otherButtonTitles:nil];
//                    
//                    [message addButtonWithTitle:@"Make Purchase"];
//                    [message addButtonWithTitle:@"Restore"];
//                    [message addButtonWithTitle:@"Cancel"];
//                    
//                    [message show];
//                    [message release];
//
//                    preRequest = NO;
//                }
//                else {
//                    //
//                    SKPayment* payment = [SKPayment paymentWithProduct:product];
//                    [[SKPaymentQueue defaultQueue] addPayment:payment];
//                }
                
                
                //
                SKPayment* payment = [SKPayment paymentWithProduct:product];
                [[SKPaymentQueue defaultQueue] addPayment:payment];
            }
        }
    }
    
    [request release];

    for (NSString *invalidProductId in response.invalidProductIdentifiers)
    {
//        NSLog(@"InAppPurchase Invalid product id: %@", invalidProductId);
        [self showErrorPopup:StoreKitErrorCode_InvalidProductIdentifier productID:invalidProductId];
    }
    
    ///
    //
    //
    if (preRequest) {
        NSArray *products = response.products;
//        NSLog(@"Product count: %d", (int)[products count]);
        
        //
        // FusePowered : Fetching Product List
        //
        [FuseSDK registerInAppPurchaseList:response];
        
        for (SKProduct *product in products) {
            
            NSNumberFormatter* numberFormatter = [[NSNumberFormatter alloc] init];
            [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
            [numberFormatter setLocale:product.priceLocale];
            NSString* formattedPrice = [numberFormatter stringFromNumber:product.price];
            
            StoreItemInfo item;
            item.storeItemID = [[product productIdentifier] UTF8String];
            item.price = [formattedPrice UTF8String];
            item.priceValue = [product.price doubleValue];
            item.currency = [[numberFormatter currencyCode] UTF8String];
            
            s_pStoreKitX->itemList.push_back(item);

            if (item.storeItemID == "TimeFish.RemoveAds") {
                s_pStoreKitX->setRemoveAdsItemIndex();
            }
            
            //
//            NSLog(@"Product: %s %@ %s %.2f,%s", [[product productIdentifier] UTF8String],
//                  product.localizedTitle,
//                  [formattedPrice UTF8String],
//                  [product.price doubleValue],
//                  [[numberFormatter currencyCode] UTF8String]
//                  );
        }

        //
        preRequest = NO;
    }
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchasing:
//                NSLog(@"InAppPurchase SKPaymentTransactionStatePurchasing");
                break;
            case SKPaymentTransactionStatePurchased:
            {
                [s_pSKDelegateBridge completeTransaction:transaction];
//                //
//                // receipt verification!
//                //
//                VerificationController* controller = [VerificationController sharedInstance];
//                [controller verifyPurchase:transaction completionHandler:^(BOOL success, SKPaymentTransaction* transaction)
//                 {
//                     NSLog(@"SKPaymentTransactionStatePurchased: %d", success);
//                     if (success) {
//                         [s_pSKDelegateBridge completeTransaction:transaction];
//                     }
//                     else {
//                         [s_pSKDelegateBridge showErrorPopup:StoreKitErrorCode_InvalidReceipt];
//                         [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//                         [s_pSKDelegateBridge finishPurchase:NO];
//                     }
//                 }];
            }
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
            {
                //
                //
                //
//                NSLog(@"SKPaymentTransactionStateRestored %@", transaction.payment.productIdentifier);
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];

//                [s_pSKDelegateBridge completeTransaction:transaction];

//                VerificationController* controller = [VerificationController sharedInstance];
//                [controller verifyPurchase:transaction completionHandler:^(BOOL success, SKPaymentTransaction* transaction)
//                 {
//                     NSLog(@"SKPaymentTransactionStateRestored: %d", success);
//                     if (success) {
//                         [s_pSKDelegateBridge restoreTransaction:transaction];
//                     }
//                     else {
//                         [s_pSKDelegateBridge showErrorPopup:StoreKitErrorCode_InvalidReceipt];
//                         [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//                         [s_pSKDelegateBridge finishPurchase:NO];
//                     }
//                 }];
            }
                break;
            default:
            {
//                NSLog(@"Invalid Payment TransactionState");
            }
                break;
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
//    NSLog(@"paymentQueue removedTransactions");
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
//    NSLog(@"paymentQueue restoreCompletedTransactionsFailedWithError: %@", error.code);
    [self showErrorPopup:error.code productID:nil];
    [self finishPurchase:NO];
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
//    NSLog(@"paymentQueueRestoreCompletedTransactionsFinished");

    if (queue.transactions.count == 0) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Restore Failed"
                                                        message:@"Purchase could not be verified. To restore the purchase, make a purchase instead. It will NOT be charged again." delegate:nil
                                              cancelButtonTitle:nil
                                              otherButtonTitles:@"OK", nil];
        [alert show];
        [alert release];
        
        [self finishPurchase:NO];

    }
    else {
        for (SKPaymentTransaction *transaction in queue.transactions) {
            switch ([transaction transactionState])
            {
                case SKPaymentTransactionStateRestored:
                {
                    NSString *productID = transaction.payment.productIdentifier;
                    
//                    NSLog (@"product id is %@" , productID);
                    
                    s_pStoreKitX->pushItemToProvideList([transaction.transactionIdentifier UTF8String], [productID UTF8String], true);

                }
                    break;
                default:
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    break;
            }
        }

        //
        [self finishRestoreAll:YES];
    }
}

#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)

- (void) completeTransaction:(SKPaymentTransaction *)transaction
{
    //
    //
    //
    if (transaction.originalTransaction == nil) {
        //
        // New Purchase!
        //
        [FuseSDK registerInAppPurchase:transaction];

        //
        // Push First, Verify Next.
        //
        s_pStoreKitX->pushItemToProvideList([transaction.transactionIdentifier UTF8String],
                                            [transaction.payment.productIdentifier UTF8String],
                                            false);
    }
    else {
        //
        // Purchase is turned into Restore!
        // NOTE: With iOS 9, it looks like FusePowered using current transaction for
        // receipt validation.
        // Other than iOS 9, using originalTransaction!!!
        //
        if (SYSTEM_VERSION_LESS_THAN(@"9.0")) {
            [FuseSDK registerInAppPurchase:transaction.originalTransaction];

            // Push First, Verify Next.
            s_pStoreKitX->pushItemToProvideList([transaction.originalTransaction.transactionIdentifier UTF8String],
                                                [transaction.originalTransaction.payment.productIdentifier UTF8String],
                                                true);
        }
        else {
            [FuseSDK registerInAppPurchase:transaction];

            // Push First, Verify Next.
            s_pStoreKitX->pushItemToProvideList([transaction.transactionIdentifier UTF8String],
                                                [transaction.payment.productIdentifier UTF8String],
                                                true);
        }
    }

    //
    // Remove the transaction from the payment queue.
    //
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

//- (void) restoreTransaction:(SKPaymentTransaction *)transaction
//{
//    NSLog(@"restoreTransaction");
//    //
//    //
//    //
//    if (transaction.originalTransaction == nil) {
//        [FuseSDK registerInAppPurchase:transaction];
//
//        //
//        NSLog(@"InAppPurchase completeTransaction");
//        NSLog(@"InAppPurchase Transaction Identifier : %@", transaction.transactionIdentifier );
//        NSLog(@"InAppPurchase Transaction Data : %@", transaction.transactionDate );
//        NSLog(@"InAppPurchase Transaction : %@", transaction.payment.productIdentifier);
//    }
//    else {
//        [FuseSDK registerInAppPurchase:transaction.originalTransaction];
//
//        //
//        NSLog(@"----------------------");
//        NSLog(@"InAppPurchase Transaction : %@", transaction.originalTransaction.transactionIdentifier);
//        NSLog(@"InAppPurchase Transaction : %@", transaction.originalTransaction.transactionDate);
//        NSLog(@"InAppPurchase Transaction : %@", transaction.originalTransaction.payment.productIdentifier);
//    }
//
//    //
//    // Remove the transaction from the payment queue.
//    //
//    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//}

- (void) failedTransaction:(SKPaymentTransaction *)transaction
{
//    CCLOG( "InAppPurchase failedTransaction." );

    if( transaction.error.code != SKErrorPaymentCancelled )
    {
//        CCLOG("InAppPurchase failedTransaction SKErrorDomain - %ld", (long)transaction.error.code );
    }
    else
    {
//        CCLOG("InAppPurchase failedTransaction SKErrorPaymentCancelled");
    }
    [self showErrorPopup:transaction.error.code productID:transaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    [self finishPurchase:NO];
}


-(void) purchaseVerification:(NSNumber*)_verified TransactionID:(NSString*)_tx_id OriginalTransactionID:(NSString*)_o_tx_id;
{
    /*
     1: The purchase is suspected to be valid.
     0: The purchase is suspected to be invalid.
     -1: We did not attempt verification. This can be due to a network error or lack of connectivity.
     */
//    NSLog(@"purchaseVerification: %@, %@, %@", _verified, _tx_id, _o_tx_id);
    if ([_verified intValue] == 1) {

        //
        // Tapjoy iap tracking
        //
        PurchaseItemInfo purchaseInfo = s_pStoreKitX->getPurchaseItemInfoByTransantionID([_tx_id UTF8String]);
        
        if (!purchaseInfo.restoredItem) {
            //
            // if New Purchase...
            //
            StoreItemInfo itemInfo = s_pStoreKitX->getItemInfoAt(purchaseInfo.idx);

//            NSLog(@"> %@", [NSString stringWithFormat:@"%s", itemInfo.storeItemID.c_str()]);
//            NSLog(@"> %@", [NSString stringWithFormat:@"%s", itemInfo.currency.c_str()]);
//            NSLog(@"> %@", [NSString stringWithFormat:@"%s", itemInfo.price.c_str()]);
//            NSLog(@"> %.2f", itemInfo.priceValue);
            
            [Tapjoy trackPurchase:[NSString stringWithFormat:@"%s", itemInfo.storeItemID.c_str()]
                    currencyCode:[NSString stringWithFormat:@"%s", itemInfo.currency.c_str()]
                    price:itemInfo.priceValue campaignId:nil transactionId:nil];
            
            [self finishPurchase:YES];
//            
//            NSString *msg = [NSString stringWithFormat:@"finishPurchase:YES"];
//            UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//            [alertView show];
//            [alertView release];
        }
        else {
            [self finishRestore:YES];
//            
//            NSString *msg = [NSString stringWithFormat:@"finishRestore:YES"];
//            UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//            [alertView show];
//            [alertView release];
        }
    }
    else {
//        [s_pSKDelegateBridge showErrorPopup:StoreKitErrorCode_InvalidReceipt];
        
//        NSLog(@"s_pSKDelegateBridge showErrorPopup:StoreKitErrorCode_InvalidReceipt");

        [self finishPurchase:NO];
//        
//        NSString *msg = [NSString stringWithFormat:@"finishPurchase:NO"];
//        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//        [alertView show];
//        [alertView release];
    }

    [activityIndicator stopAnimating];
}

-(void) finishPurchase:(BOOL) success
{
    if (s_pStoreKitX->getDelegate()) {
        s_pStoreKitX->getDelegate()->finishPurchase(success);
        [activityIndicator stopAnimating];
    }
}

-(void) finishRestore:(BOOL) success
{
    if (s_pStoreKitX->getDelegate()) {
        s_pStoreKitX->getDelegate()->finishRestore(success);
        [activityIndicator stopAnimating];
    }
}

-(void) finishRestoreAll:(BOOL) success
{
    NSString *title = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("RestoreUI1").c_str()];
    NSString *msg = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("RestoreUI2").c_str()];
    NSString *btn = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("RestoreUI3").c_str()];

    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:title message:msg delegate:nil cancelButtonTitle:btn otherButtonTitles:nil];
    [alertView show];
    [alertView release];

    //
    if (s_pStoreKitX->getDelegate()) {
        s_pStoreKitX->getDelegate()->finishRestoreAll(success);
        [activityIndicator stopAnimating];
    }
}

//- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
//{
//    NSString *title = [alertView buttonTitleAtIndex:buttonIndex];
//
//    if([title isEqualToString:@"Make Purchase"])
//    {
//        [self requestProductData];
//    }
//    else if([title isEqualToString:@"Restore"])
//    {
//        [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
//    }
//    else if([title isEqualToString:@"Cancel"])
//    {
//        [self finishPurchase:NO];
//        [alertView dismissWithClickedButtonIndex:buttonIndex animated:YES];
//    }
//}

- (BOOL) canMakePayments
{
    if ([SKPaymentQueue canMakePayments]) {
        return YES;
    }

    [self showErrorPopup:StoreKitErrorCode_IAPRestriction productID:nil];

    return NO;
}

-(void) showErrorPopup:(NSInteger)errorCode productID:(NSString*)_productID
{
    if( errorCode == SKErrorUnknown ||
       errorCode == StoreKitErrorCode_InvalidProductIdentifier )
    {
        NSString *msg = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI6").c_str()];
        NSString *btn = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI3").c_str()];
        
        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:btn otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    }
    else if( errorCode == StoreKitErrorcode_TransactionFailed )
    {
        NSString *msg = nil;
        if ([_productID isEqualToString:[NSString stringWithFormat:RemoveAdsProductID]]) {
            msg = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI6").c_str()];
        }
        else {
            msg = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI2").c_str()];
        }

        NSString *btn = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI3").c_str()];
        
        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:btn otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    }
    else if(errorCode == StoreKitErrorCode_IAPRestriction)
    {
        NSString *msg = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI5").c_str()];
        NSString *btn = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI3").c_str()];

        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:btn otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    }
    else if (errorCode == StoreKitErrorCode_InvalidReceipt) {
        NSString *msg = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI7").c_str()];
        NSString *btn = [NSString stringWithUTF8String:LocalizationManager::getInstance()->getLocalizationString("PurchaseUI3").c_str()];

        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil message:msg delegate:nil cancelButtonTitle:btn otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    }
}


-(void) sessionStartReceived
{
    
}

-(void) sessionLoginError: (NSError*) _error
{
    NSLog(@"sessionLoginError");
}

-(void) adAvailabilityResponse:(NSNumber*)_available Error:(NSError*)_error
{
    BOOL isAvailable = [_available boolValue];
    
    int error = (int) [_error code];
    
    if (error != FUSE_ERROR_NO_ERROR)
    {
        // An error has occurred checking for the ad
    }
    else
    {
        if (isAvailable)
        {
            // An ad is available
//            NSLog(@"An ad is available");
        }
        else
        {
            // An ad is not available
//            NSLog(@"An ad is not available");
        }
    }
    
}
-(void) adWillClose
{
    
}

@end
