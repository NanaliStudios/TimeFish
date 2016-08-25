#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>


#define IS_IOS6_AWARE (__IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_5_1)

#define ITMS_PROD_VERIFY_RECEIPT_URL        @"https://buy.itunes.apple.com/verifyReceipt"
#define ITMS_SANDBOX_VERIFY_RECEIPT_URL     @"https://sandbox.itunes.apple.com/verifyReceipt"
//  Always verify your receipt for auto-renewable subscriptions first with the production URL;
//  proceed to verify with the sandbox URL if you receive a 21007 status code.
//  Following this approach ensures that you do not have to switch between URLs while your application
//  is being tested or reviewed in the sandbox or is live in the App Store.
//  when status == 21007, send receipt again to https://sandbox.itunes.apple.com/verifyReceipt. than we will get status == 0


#define KNOWN_TRANSACTIONS_KEY              @"knownIAPTransactions"
#define ITC_CONTENT_PROVIDER_SHARED_SECRET  @"10e75e84aac1424388e459515bf6a895" // generated from itunes connect In-App Purchases page : "View or generate a shared secret"
#warning "set ITC_CONTENT_PROVIDER_SHARED_SECRET generated from itunes connect In-App Purchases page"

char* base64_encode(const void* buf, size_t size);
void * base64_decode(const char* s, size_t * data_len);

typedef void (^VerifyCompletionHandler)(BOOL success, SKPaymentTransaction* transaction);

@interface VerificationController : NSObject<NSURLConnectionDelegate> {
    NSMutableDictionary *       transactionsReceiptStorageDictionary;
}

+ (VerificationController *) sharedInstance;


// Checking the results of this is not enough.
// The final verification happens in the connection:didReceiveData: callback within
// this class.  So ensure IAP feaures are unlocked from there.
- (void)verifyPurchase:(SKPaymentTransaction *)transaction completionHandler:(VerifyCompletionHandler)completionHandler_;

@end
